#include <stdio.h>
#include <string.h>
#include <dos.h>

#define sym_tab_size 500
#define ntokens 20         /* max tokens per line */
#define tokensep "\t :+-,[]\n"

#define true 1
#define false 0
#define mybuffsize 4096

/* this program can be compiled in small model */

/* Schema for converting Turbo C assembly output to ROMable code:

 Pass 1: build a table of symbols in _DATA segment

  (omit "_TEXT" and "_BSS" segment symbols)

 Pass 2:
     a. Change the "DGROUP" group command to just "_BSS"
     b. Add a "CGROUP" with _TEXT,_DATA
     c. Change the assume to "assume cs:CGROUP,ds:DGROUP"

     d: change group reference for any symbol in "_DATA" segment
           from "DGROUP" to "CGROUP"
           add a "CGROUP:" reference if none exists

     e: change associated ds reference to cs
         1. a "mov" before
         2. a "push  ds" before or after

1/16/89  Updated by the author for Turbo C 2.0

   */

  struct sym_table_entry {
    char symbol[32];
  };

  struct sym_table_entry sym_table[sym_tab_size];

  int no_symbols;

  char ibuff[mybuffsize];
  char obuff[mybuffsize];

  FILE *asmsource;
  FILE *output;


  char prev_line[255];
  char next_line[255];
  char curr_line[255];

  /* insert text *c at *r in a string *p
     assumes there is enough room to do the insertion */

void strins(char *p,char *r,char *c)
{
  int i,m,n;
  char work[255];
  m = r - p;           /* # chars in 1st part of *p */
  strcpy(work,p);      /* work = all of *p */
  work[m] = 0;         /* trunc to length up to *r */
  strcat(work,c);      /* append *c */
  strcat(work,r);      /* append rest of *p */
  strcpy(p,work);      /* move back to p *p */
  /* printf("strins --%s",p); */
}

struct sym_table_entry *search_sym(char *sym)
{
  struct sym_table_entry *p;
  int i;

  p = sym_table; i = 0;
  while ((i < no_symbols) && strcmp((char*)p,sym)) {
    p++;
    i++;
  }
  if (i == no_symbols) return (NULL);
  else return (p);
}

  /* search the line for an occurrence of any symbol in table */

char *sym_in_line(char *theline)
{
  int i;
  char *p;
  i = 0;
  while ((i < no_symbols) && (!(p = strstr(theline,sym_table[i].symbol))))
    i++;
  if (i < no_symbols) return (p);
  else return (NULL);
}

void list_symbols(void)
{
  int i;
  for (i = 0; i < no_symbols; i++)
    printf("%s\n",sym_table[i].symbol);
}

void add_symbol(char *sym)
{
  /* printf("%s symbol to add\n",sym); */
  strcpy(sym_table[no_symbols++].symbol,sym);
}

/* Pass1 -- Read through the text and accumulate a table of
  symbols in the _DATA segment -- returns # lines input */

int pass1(void)
{
  int i;
  int linect;
  char *s[ntokens];
  char dataseg;   /* boolean == in _DATA segment */
  char cursegname[20];

  no_symbols = 0;
  linect = 0;

  cursegname[0] = 0;  /* no current segment */
  dataseg = false;

  while (!feof(asmsource)) {
    fgets(curr_line,sizeof(curr_line),asmsource);
    linect++;

    /* printf("%s",curr_line); */
    i = 0;
    s[i] = strtok(curr_line,tokensep);
    while (s[i]) {
      /* printf("%s||",s[i]); */
      s[++i] = strtok(NULL,tokensep);
    }
    if (!i) continue;   /* 0 - length line */
    if (s[0] == ";") continue;  /* comment, ignore */
    /* printf(" %d tokens\n",i); */
    if (i > 1) {
      if (!strcmp(s[1],"segment")) {
        if (!strcmp(s[0],"_DATA")) {
          dataseg = true;
          /* printf("Begin _DATA segment \n"); */
        }
        strcpy(cursegname,s[0]);
        continue;
      }
    }
    if (!strcmp(s[1],"ends") && !strcmp(s[0],"_DATA")) {
      dataseg = false;
      /* printf("End _DATA segment \n"); */
      cursegname[0] = 0;   /* no current segment */
      continue;
    }
    if (dataseg && !strcmp(s[1],"label")) {
      add_symbol(s[0]);
      continue;
    }
      /* extrn not in any segment */
    if (!strcmp(s[0],"extrn") && !cursegname[0]) {
      add_symbol(s[1]);
      continue;
    }

  }  /* while not eof */
  list_symbols();
  return (linect);
}

/* Pass 2 processing - see above -- returns # lines output */

int pass2(void)
{
  int   i,j, linect;
  struct sym_table_entry *s;
  char *t[ntokens];
  char *tt[ntokens];
  char *r,*u;

  char linebuff[255];
  char linebuff2[255];

  prev_line[0] = 0;   /* clear prev_line */

  /* Fill the pipeline */
  fgets(prev_line,sizeof(prev_line),asmsource);
  fgets(curr_line,sizeof(curr_line),asmsource);
  fgets(next_line,sizeof(next_line),asmsource);
  linect = 0;

  do {
    /* printf("%s",curr_line); */
    i = 0;  /* get tokens */
    strcpy(linebuff,curr_line);
    t[i] = strtok(linebuff,tokensep);
    while (t[i]) {
      /* printf("%t||",t[i]); */
      t[++i] = strtok(NULL,tokensep);
    }
    if (!strcmp(t[0],"assume")) {
      /* substutite new assume statement */
      strcpy(curr_line,"\tassume\tcs:CGROUP,ds:DGROUP\n");

      /* printf("assume  cs:CGROUP,ds:DGROUP\n"); */
    }
    else if (!strcmp(t[1],"group")) {
      strcpy(curr_line,"DGROUP\tgroup\t_BSS\n");
     /* insert a CGROUP line */
      fputs(prev_line,output);
      linect++;
      strcpy(prev_line,curr_line);
      strcpy(curr_line,"CGROUP\tgroup\t_TEXT,_DATA\n");
      linect++;

      /* printf("DGROUP\tgroup\t_BSS\n");
      printf("CGROUP\tgroup\t_TEXT,_DATA\n"); */

    }
    else if (!strcmp(t[0],"extrn"));
    else if (!strcmp(t[0],"public"));
    else if (!strcmp(t[1],"label"));

    else if (r = sym_in_line(curr_line)) {  /* symbol found in line */
      if (u = strstr(curr_line,"DGROUP:")) {   /* "DGROUP" found */
        *u = 'C';   /* change to CGROUP: */

/* following three lines added for v 2.0.  A "dd" statement has
   two "DGROUP" references in it */

        if (!strcmp(t[0],"dd") && (u = strstr(curr_line,"DGROUP:"))) {
          *u = 'C';   /* change to CGROUP: */
        }
      }
      else {   /* no group found, insert "CGROUP: in line before symbol */
        strins(curr_line,r,"CGROUP:");
      }

          /* now check for previous line with ds reference */
          /* tokenize previous line */

      strcpy(linebuff,prev_line);
      i = 0;
      tt[i] = strtok(linebuff,tokensep);
      while (tt[i]) tt[++i] = strtok(NULL,tokensep);
      /*  see if "ds" present */
      i = 0;
      while (tt[i] && strcmp(tt[i],"ds")) i++;
      if (tt[i]) {  /* "ds" found, change to "cs" */
        *(prev_line + (tt[i] - linebuff)) = 'c';
        /* printf("Previous line %s",prev_line); */
      }
      else {  /* not on previous line, check next line */
        strcpy(linebuff,next_line);
        i = 0;
        tt[i] = strtok(linebuff,tokensep);
        while (tt[i]) tt[++i] = strtok(NULL,tokensep);
        i = 0;
        while (tt[i] && strcmp(tt[i],"ds")) i++;
        if (tt[i]) {  /* ds found in next line */
          *(next_line + (tt[i] - linebuff)) = 'c';
          /* printf("Following line %s",next_line); */
        }

      }

    } /* if symbol found in line */

    fputs(prev_line,output);
    linect++;
    strcpy(prev_line,curr_line);
    strcpy(curr_line,next_line);
    fgets(next_line,sizeof(next_line),asmsource);
  } while (!feof(asmsource));
  /* empty the pipeline */
  fputs(prev_line,output);
  fputs(curr_line,output);
  linect += 2;
  return (linect);
}


int main(int argc, char *argv[])
{
  int l;

  if (argc < 3) {
    printf("Usage:  romize input.asm output.asm \n");
    return (1);
  }

  asmsource = fopen(argv[1],"rt");
  if (!asmsource) {
    printf("Trouble opening input file\n");
    return(1);
  }
  setvbuf(asmsource,ibuff,_IOFBF,mybuffsize);
  l = pass1();
  fclose(asmsource);
  printf("%d Lines input\n",l);

  asmsource = fopen(argv[1],"rt");
  setvbuf(asmsource,ibuff,_IOFBF,mybuffsize);
  output = fopen(argv[2],"wt");
  if (!output) {
    printf("Trouble opening output\n");
    fclose(asmsource);
    return(1);
  }
  setvbuf(output,obuff,_IOFBF,mybuffsize);

  l = pass2();
  fclose(asmsource);

  fclose(output);
  printf("%d Lines output\n",l);
  return(0);
}
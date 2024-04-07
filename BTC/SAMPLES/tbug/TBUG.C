/**************************************************
*
*   SOURCE LEVEL DEBUG MODULE FOR TURBO C
*   by: Gary L. Mellor          Sept 1987
*
**************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <alloc.h>
#include <dos.h>

/*     IDENTIFIER ATTRIBUTE CODES */

#define  INT        1
#define  CHAR       2
#define  FLOAT      3
#define  DOUBLE     4
#define  POINTER    0x1000
#define  ADDRESS    0x2000

/*     COMMAND CODES    */

#define  HELP        1
#define  MAP         2
#define  SYMS        3
#define  LIST        4
#define  PRINT       5
#define  BREAK       6
#define  NOBREAK     7
#define  STEP        8
#define  CONTINUE    9
#define  TRACE       10
#define  REGISTERS   11
#define  QUIT        12


typedef struct          /* INTERRUPT REGISTERS */
{
  unsigned int bp,di,si,ds,es,dx,cx,bx,ax,ip,cs,flags;
}INTREGS;


typedef struct         /* LINE NUMBER TABLE ENTRY */
{
  int line_number;
  int address;
  unsigned char flag;
  unsigned char opcode;
  void far *link;
}LINE;


typedef struct         /* SYMBOL TABLE ENTRY */
{
  char name[32];
  int  offset;
  void far *link;
}SYMBOL;

typedef struct        /* DECODE LIST ENTRY */
{
  int key;
  char *string;
}DECODELIST;

/* COMMAND DECODE LIST IS ALSO USED TO DISPLAY HELP  */

DECODELIST cmdlist[]=
{
  HELP,     "HELP\n\nCOMMAND PARAM(s) <>=optional     FUNCTION\n",
  MAP,      "MAP                         lists the line numbers",
  SYMS,     "SYMS                        lists the public symbols",
  LIST,     "LIST    <line_number>       lists the source file",
  PRINT,    "PRINT   <type> identifier   prints the value",
  BREAK,    "BREAK   <line_number>       sets/lists the breakpoint(s)",
  NOBREAK,  "NOBREAK line_number         removes a breakpoint",
  STEP,     "STEP                        steps to the next source line",
  CONTINUE, "CONTINUE                    executes to next breakpoint",
  REGISTERS,"REGISTERS                   displays the cpu registers",
  QUIT,     "QUIT                        terminates the program",
  0,0
};

/* IDENTIFIER ATTRIBUTE DECODE LIST FOR PRINT COMMAND */
DECODELIST keylist[]=
{
  INT,"INT",
  CHAR,"CHAR",
  FLOAT,"FLOAT",
  DOUBLE,"DOUBLE",
  0,0
};

LINE far *linetable;
SYMBOL far *symtable;
void interrupt (*vect1)();
void interrupt (*vect3)();

/*******************************
*  DEBUG INITIALIZATION
*******************************/
int debug_init(name)
char *name;
{
  void interrupt brk_handler();
  void interrupt trace_handler();
  void debug_exit();

  FILE *mapfile;
  char mapname[40];
  char sname[40];
  char buffer[80];
  char *ptr;
  int i;

  i = 0;
  while (*name && (*name != '.'))
    mapname[i++] = *name++;
  mapname[i] = 0;
  strcat(mapname,".MAP");
  mapfile = fopen(mapname,"r");
  if (mapfile == 0)
  {
    printf("MAPFILE NOT FOUND: %s\n",mapname);
    return(-1);
  }
  while(fgets(buffer,80,mapfile))
  {
                                  /* locate public symbols */
    if (strstr(buffer,"Publics by Val"))
    {
                                  /* allocate root entry */
      symtable = farmalloc(sizeof(SYMBOL));
      symtable->link = 0;
      fgets(buffer,80,mapfile);   /* dump blank line */
      do                          /* build symbol table */
      {
        fgets(buffer,80,mapfile);
        i = addsym(symtable,buffer);
        if (i < 0)
          break;
      }
      while(buffer[0] == ' ');
      break;
    }
  }
  while(fgets(buffer,80,mapfile))
  {
                                  /* locate the line numbers */
    if (buffer[0] == 'L')
    {
                                  /* allocate root entry */
      linetable =  farmalloc(sizeof(LINE));
      linetable->link = 0;
                                  /* extract source file name */
      ptr = strchr(buffer,'(');
      ptr++;
      i = 0;
      while (*ptr != ')')
        sname[i++] = *ptr++;
      sname[i] = 0;
      fgets(buffer,80,mapfile);   /* dump blank line */
      do                          /* build the line number table */

      {
        fgets(buffer,80,mapfile);
        i = addlines(linetable,buffer);
      }while(i > 0);
      if (i < 0)
      {
        puts("NOT ENOUGH MEMORY FOR DEBUGGER");
        return(-1);
      }
      break;
    }
  }
  fclose(mapfile);
  vect1 = getvect(1);            /* save old vectors */
  vect3 = getvect(3);
  if (cmdline(0,sname) != 0)     /* bkpts have been set */
  {
    setvect(1,&trace_handler);   /* so init our vectors */
    setvect(3,&brk_handler);
    atexit(debug_exit);          /* and exit handler */

  }
  return(0);
}
/*******************************
*  DEBUG EXIT HANDLER
*******************************/
void debug_exit()
{
  printf("\nPROGRAM TERMINATION\n");
  setvect(1,vect1);
  setvect(3,vect3);

}
/*******************************
* COMMAND LINE INPUT HANDLER
********************************/
/*
   prompts for operator input 
   and handles commands
*/
int cmdline(regs,file)
INTREGS *regs;
char *file;
{
  LINE far *findline();
  SYMBOL far *findsym();
  char *fstrg();
  char *next();

  char temp[132];
  char *tptr;
  int  brkline;
  int  listline;
  int  address;
  int  index;
  LINE far *line;
  SYMBOL far *symbol;

  static int cmd;
  static FILE *fp;
  static char name[15];
  int i;
  int mode;
  int flag;

  if (regs == 0)          /* if called from debug_init  */
  {
    fp = fopen(file,"r"); /* open the source file       */
    strcpy(name,file);
  }
  listline = 1;
  for(;;)
  {
    printf("TBUG?? ");    /* prompt for and     */
    gets(temp);           /* input command line */
    tptr = temp;
    while (*tptr == ' ')  /* skip leading spaces */
      tptr++;
    if (*tptr != 0)       /* if more than RETURN then  */
                          /* decode new command  code  */
      cmd = decode(tptr,cmdlist);
    switch(cmd)           /* process command code */
    {
      case HELP:
                i = 0;
                while(cmdlist[i].key != 0)
                  puts(cmdlist[i++].string);
                break;
      case MAP:
              line = linetable;
              i = 0;
              printf("LINE NUMBERS FOR %s\n",name);
              while(line->link != 0)
              {
                if ((i++ % 4) == 0)
                  putchar('\n');
                 printf("%4d  %04X     ",
                        line->line_number,line->address);
                 line = line->link;
              }
              putchar('\n');
              break;
      case SYMS:
              symbol = symtable;
              while(symbol->link != 0)
              {
                if ((i++ % 2) == 0)
                  putchar('\n');
                printf("%32s %04X  ",
                        fstrg(symbol->name),symbol->offset);
                symbol = symbol->link;
              }
              putchar('\n');
              break;
      case PRINT:
              tptr = next(tptr);
              if (*tptr != 0)
              {
                mode = 0;
                do
                {
                  i = decode(tptr,keylist);
                  if (i != 0)
                    tptr = next(tptr);
                  mode = mode | i;
                }while (i != 0);
                i = 0;
                if (*tptr == '*')
                {
                  mode = mode | POINTER;
                  tptr++;
                }
                while ((*tptr != 0) && (strchr(" +-[",*tptr) == 0))
                  temp[i++] = *tptr++;
                temp[i] = 0;
                while(*tptr == ' ')
                  tptr++;
                switch(*tptr++)
                {
                  case '+':
                  case '[':
                            while (*tptr == ' ')
                              tptr++;
                            index = getnum(tptr);
                            break;
                  case '-':
                            while (*tptr == ' ')
                              tptr++;
                            index = -(getnum(tptr));
                            break;
                   default:
                            index = 0;
                }
                if (isalpha(temp[0]))
                {
                  symbol = findsym(temp,symtable);
                  if (symbol == 0)
                  {
                    puts("SYMBOL NOT FOUND");
                    break;
                  }
                  address = symbol->offset;
                }
                else
                {
                  if (strcmp(temp,"_BP") == 0)
                  {
                    address = regs->bp + index;
                    index = 0;
                  }
                  else
                    address = getnum(temp);
                }
                printloc(mode,address,index);
              }
              else
                puts("VARIABLE MUST BE SPECIFIED");
              break;
      case LIST:
              if (fp == 0)
                puts("SOURCE NOT AVAILABLE");
              else
              {
                tptr = next(tptr);
                if (*tptr != 0)
                {
                  rewind(fp);
                  listline = atoi(tptr);
                  i = listline - 1;
                  if ( i < 0)
                    i = 0;
                }
                else
                  i = 0;
                while(i-- > 0)
                  fgets(temp,132,fp);
                for (i = 0; i < 20; i++)
                {
                  if (fgets(temp,132,fp))
                    printf ("%03d:  %s",listline++,temp);
                  else
                  {
                    puts("*****END OF FILE*****");
                    break;
                  }
                }
              }
              break;
      case BREAK:
              tptr = next(tptr);
              if (*tptr != 0)
              {
                brkline = atoi(tptr);
                line = findline(brkline,linetable);
                if (line == 0)
                  puts("LINE NOT FOUND");
                else
                {
                  if (line->flag != BREAK)
                  {
                    line->flag = BREAK;
                    pokeb(_CS,line->address,0xCC);
                  }
                  else
                    puts("BREAK ALREADY SET");
                }
              }
              else
              {
                for(line = linetable; line->link != 0; line = line->link)
                {
                  if (line->flag == BREAK)
                  {
                    printf("%d  %04X\n",line->line_number,
                           line->address);
                  }
                }
              }
              break;
      case NOBREAK:
              tptr = next(tptr);
              brkline = atoi(tptr);
              line = findline(brkline,linetable);
              if (line == 0)
                puts("LINE NOT FOUND");
              else
              {
                if (line->flag == BREAK)
                {
                  line->flag = NOBREAK;
                  pokeb(_CS,line->address,line->opcode);
                }
                else
                  puts("BREAK NOT SET");
              }
              break;
      case REGISTERS:
              printf("AX = %04X  BX = %04X  CX = %04X  DX = %04X\n",
                     regs->ax,regs->bx,regs->cx,regs->dx);
              printf("BP = %04X  SI = %04X  DI = %04X  IP = %04X\n",
                     regs->bp,regs->si,regs->di,regs->ip);
              printf("DS = %04X  ES = %04X  CS = %04X  FLAGS  %02X\n",
                     regs->ds,regs->es,regs->cs,regs->flags&0xFF);
              break;
      case CONTINUE:
              line = linetable;
              i = 0;
              flag = 0;
              while(line->link != 0)
              {
                if (line->flag != BREAK) /* restore proper opcode */
                {
                  line->flag = NOBREAK;
                  pokeb(_CS,line->address,line->opcode);
                }
                else
                  flag = 1;             /* we have at least one bkpt */
                line = line->link;
              }
              if (flag ==  0)
              {
                setvect(1,vect1);
                setvect(3,vect3);
              }
              return(flag);             /* tell caller if have bkpts */
      case STEP:
              line = linetable;
              while(line->link != 0)
              {
                if (line->flag != BREAK)
                {
                  line->flag = STEP;
                  pokeb(_CS,line->address,0xCC);
                }
                line = line->link;
              }
              return(1);                /* we definately have bkpts */
      case QUIT:
              exit(0);
      case -1:
              puts("COMMAND SPELLING NOT UNIQUE");
              break;
      default:
              puts("INVALID COMMAND");
    }
  }
}
/*******************************
*   STRING HANDELING ROUTINES
*******************************/
/*
    decode compares a command string
    to a command list and returns a
    command key. More than one match
    means enough chars in command spelling
    to decode the command.
*/
int decode(s,list)
char s[];
DECODELIST list[];
{
  int i;
  int idx;

  i = 0;
  idx = 0;
  while(list[i].key)        /* while not end of list */
  {
     if (match(s,list[i].string))
     {
       if (idx == 0)        /* first match OK */
         idx = list[i].key;
       else                 
         return(-1);        /* multiple match NOT OK */
     }
     i++;
  }
  return(idx);
}
/*
  string comparison is a match
  if all chars of string s1 match s2,
  even if we are not at end of s2
*/
int match(s1,s2)
char *s1;
char *s2;
{
  while (*s1 == ' ')    /* skip leading spaces */
    s1++;
  while(*s1 > ' ')      /* while valid char */
  {
    if (toupper(*s1++) != *s2++)
      return(0);        /* we didnt match */
  }
  return(1);
}
/*
  next skips the current token
  and trailing spaces to return
  a pointer to the next token
*/
char *next(s)
char *s;
{
  while(*s)        /* skip over current token */
  {
    if (*s <= ' ')
      break;
    else
      s++;
  }
  while (*s)       /* skip over spaces */
  {
    if (*s > ' ')
      break;
    else
      s++;
  }
  return(s);
}
/*
  moves a far string into local data space
  and returns a pointer to it.
*/
char *fstrg(fs)
char far *fs;
{
  static char temp[80];
  int i;

  i = 0;

  while (*fs != 0)
    temp[i++] = *fs++;
  temp[i] = 0;
  return(&temp[0]);
}
/*******************************
* LINE NUMBER TABLE ROUTINES
*******************************/
/*
    adds line numbers/offsets to table
*/
int addlines(table,buf)
LINE far *table;
char *buf;
{
  int count;

  while(table->link != 0)
    table  = table->link;
  count = 0;
  while (*buf != 0)
  {
    if (isdigit(*buf))
    {
      table->line_number = atoi(buf);
      while(*buf++ != ':');
      table->address = xatoi(buf);
      table->flag = 0;
      table->opcode = peekb(_CS,table->address);
      while(isxdigit(*buf++));
      table->link = farmalloc(sizeof(LINE));
      table = table->link;
      table->link = 0;
      if (table == 0)
        return(-1);
      count++;
    }
    else
      buf++;
  }
  return(count);
}
/*
  finds a line number in the table
*/
LINE far *findline(lnum,table)
int lnum;
LINE far *table;
{
  int i;

  while(table->link != 0)
  {
    if (table->line_number == lnum)
      return(table);
    table = table->link;
   }
   return(0);
}
/*
  finds an address in the table
*/
LINE far *findaddress(adr,table)
int adr;
LINE far *table;
{

  while(table->link != 0)
  {
    if (table->address == adr)
      return(table);
    table = table->link;
   }
   return(0);
}
/*******************************
* SYMBOL TABLE ROUTINES
I*******************************/
/*
  adds symbol info to the table
*/
int addsym(table,buf)
SYMBOL far *table;
char *buf;
{
  char *fstrg();
  int i;
  int n;
  char *tptr;

  tptr = buf;
  while (*tptr == ' ')
    tptr++;
  i = xatoi(tptr);
  if (i != (_DS - _CS))
    return(0);
  while(table->link != 0)
    table  = table->link;
  while (*tptr != ':')
    tptr++;
  tptr++;
  n = xatoi(tptr);
  table->offset = n;
  while (*tptr != '_')
    tptr++;
  tptr++;
  i = 0;
  while(*tptr > ' ')
    table->name[i++] = *tptr++;
  table->name[i] = 0;
  table->link = farmalloc(sizeof(SYMBOL));
  table = table->link;
  if (table == 0)                  /* out of memory */
    return(-1);
  table->link = 0;
  return(1);
}
/*
  finds a symbol in the table
*/
SYMBOL far *findsym(s,table)
char *s;
SYMBOL far *table;
{
  char *fstrg();
  int i;

  while(table->link != 0)
  {
    if (strcmp(s,fstrg(table->name)) == 0)
      return(table);
    table = table->link;
   }
   return(0);
}
/*******************************
*  FORMATTED DATA PRINT
*******************************/
/*
  uses printf to display the data
  int the specified format
*/
int printloc(mode,loc,idx)
int mode;
void *loc;
int idx;
{
  int *iptr;
  char *cptr;
  float *fptr;
  double *dptr;

  switch(mode & 7)
  {
    case 0:
    case INT:
           iptr = (int *)loc + idx;
           if (mode & POINTER)
             printf("%04X = %d   %#X\n",*iptr,*((int *)*iptr),*(int *)*iptr);
           else
             printf("%04X  = %d   %#X\n",iptr,*iptr,*iptr);
           break;
    case CHAR:
           cptr = (char *)loc + idx;
           if (mode & POINTER)
             printf("%04X  = %s\n",cptr,cptr);
           else
             printf("%04X  = %c   %#X\n",cptr,*cptr,*cptr);
           break;
    case FLOAT:
           fptr = (float *)loc + idx;
           if (mode & POINTER)
             printf("%04X  = %g\n",*fptr,*(float *)fptr);
           else
             printf("%04X  = %g\n",fptr,*fptr);
           break;
    case DOUBLE:
           dptr = (double *)loc + idx;
           if (mode & POINTER)
             printf("%04X  = %lg\n",*dptr,*(double *)dptr);
           else
             printf("%04X  = %lg\n",dptr,*dptr);
           break;
    default:
           puts("MODE NOT SUPPORTED");
  }
  return(0);
}

/*******************************
* ASCII NUMBER CONVERSIONS
*******************************/
/*
    converts a number in the various
    C formats to an integer
*/
int getnum(s)
char *s;
{
  int val;

  if (*s == '0')
  {
    s++;
    if (toupper(*s) == 'X')
    {
      s++;
      val = xatoi(s);
    }
    else
      val = oatoi(s);
  }
  else
    val = atoi(s);
  return(val);
}
/*
    octal ascii to integer conversion
*/
int oatoi(s)
char *s;
{
  int num;
  num = 0;
  while((*s >= '0') && (*s <= '7'))
  {
    num = num << 3;
    num = num + (*s++ - '0');
  }
  return(num);
}
/*
    hexadecimal ascii to integer conversion
*/
int xatoi(s)
char *s;
{
  int num;
  num = 0;
  while(isxdigit(*s))
  {
    num = num << 4;
    if (*s <= '9')
      num = num + (*s++ - '0');
    else
      num = num + (toupper(*s++) - 'A' + 10);
  }
  return(num);
}

/*******************************
* BREAKPOINT and TRACE HANDLERS
*******************************/
LINE far *brkline;
void interrupt brk_handler(regs)
INTREGS regs;
{
  LINE far *findaddress();

  regs.ip--;
  brkline = findaddress(regs.ip,linetable);
  if (brkline == 0)
    printf("INTERRUPT 3 AT ADDRESS = %02X\n",regs.ip++);
  else
  {
    switch(brkline->flag)
    {
      case BREAK:
                 printf("BREAKPOINT");
                 break;
      case STEP:
                 printf("STEP");
                 break;
      default:
                 printf("INTERRUPT 3");
                 regs.ip++;
    }
    printf(" AT LINE %d = %04X\n",brkline->line_number,brkline->address);
  }
  enable();                /* enable other interrupts */
  if (cmdline(&regs) != 0) /* we need trace to resotre bkpts */
    regs.flags = regs.flags | 0x100;
  pokeb(_CS,brkline->address,brkline->opcode);
}

void interrupt trace_handler(regs)
INTREGS regs;
{
  if (brkline == 0)
  {
    printf("TRACE AT %04X\n",regs.ip);
    cmdline(&regs);
  }
  else
  {
    if (brkline->flag != NOBREAK)
      pokeb(_CS,brkline->address,0xCC);
  }
  regs.flags = regs.flags & 0xFF;
}

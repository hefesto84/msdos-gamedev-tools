/*****************************************************************************
*
*	Library Demo
*
*	By Kevin Spencer, DigiServ		(c) 1991
*
*	Edit History
*	------------
*
*****************************************************************************/

/*
*	Defines
*/

#define title	"Library Demo Program"
#define version "1.1"
#define verdate "14-Oct-91"

#define SHOWTIME 200

/*
*	System includes
*/

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>
#include <dos.h>

/*
*	Project includes
*/

#include "stdinc.h"
#include "misc.h"
#include "wn.h"
#include "mn.h"
#include "kb.h"
#include "arg.h"
#include "cf.h"
#include "pr.h"
#include "sc.h"
#include "db.h"

/*
*	Structures
*/

struct rec_st {
	char	sn[21];
	char	fn[21];
	char	addr1[31];
	char	addr2[31];
	char	phon[11];
	char	paid;
};

typedef struct rec_st rec_st;

/*
*	Function prototypes
*/

void	main(void);
void	initsystem(void);
int		seekfunc(rec_st *,rec_st *);
int		findfunc(rec_st *,rec_st *);
void	search(void);
int		checksave(void);
void	save(void);
int		scandata(rec_st *);
int		selectrec(int);
void	message(char *,...);
void	help(void);
void	popup(char *);
int		getans(char *);
void	scan(void);
void	pack(void);
void	add(void);
void	delete(void);
void	back(void);
void	next(void);
void	clear(void);
int		reports(void);
void	rbrief(void);
void	prerr(char *);
void	rfull(void);
void	rpaid(void);
void	header(char *);

/*
*	Configuration parameters
*/

struct conf_st {
	char	datafile[65];
	char	indexfile[65];
	char	helpfile[65];
	char	tempfile[65];
	char	tempidx[65];
	uint	norm;
	uint	high;
}	conf = {
	"patient.dat",
	"patient.idx",
	"prs.hlp",
	"temp.dat",
	"temp.idx",
	WHITE,
	BLACK|BKGR(RED),
};

CFdata	config[] = {
	'S',"datafile",&conf.datafile,
	'S',"indexfile",&conf.indexfile,
	'S',"helpfile",&conf.helpfile,
	'S',"tempfile",&conf.tempfile,
	'S',"tempidx",&conf.tempidx,
	'C',"norm",&conf.norm,
	'C',"high",&conf.high,
	NULL,NULL,NULL
};

/*
*	Global variables
*/

static	rec_st	rec;
static	rec_st	frec;
static	int		recn[20];
static	char	recs[20][81];

static	SCdata	screen[] = {
	'S',20,0,17, 1,0,0,&rec.sn,NULL,
	'S',20,0,17, 2,0,0,&rec.fn,NULL,
	'S',30,0,17, 4,0,0,&rec.addr1,NULL,
	'S',30,0,17, 5,0,0,&rec.addr2,NULL,
	'S',10,0,17, 7,0,0,&rec.phon,NULL,
	'L', 1,0,17, 9,0,0,&rec.paid,NULL,
	'0',0,0,0,0,0,0,NULL,NULL
};

MNdata	reportmenu =	{	4,40,0,0,CENTRE,
							" Report Menu ",
							"Brief Listing",NULL,rbrief,
							"Full Listing",NULL,rfull,
							"Unpaid patients",NULL,rpaid,
							NULL,NULL,NULL
						};

MNdata	cmdmenu =		{	4,40,0,0,CENTRE,
							" Command Menu ",
							"Help",NULL,help,
							"Search for patient",NULL,search,
							"Add patient",NULL,add,
							"Save patient",NULL,save,
							"Delete patient",NULL,delete,
							"List patients",NULL,scan,
							"Report menu",&reportmenu,NULL,
							"Pack database",NULL,pack,
							"Previous patient",NULL,back,
							"Next patient",NULL,next,
							"Clear screen",NULL,clear,
							NULL,NULL,NULL
						};

char	confarg[81];
DBfile	*curr;			/* Pointer to current database file					*/
char	pline[] = "--------------------------------------------------------------------------------";
int		cf;

/*
*	Start of code
*/

void
main()
{
	int		exitloop;

	initsystem();

	if ((curr = DBopen(conf.datafile,sizeof(rec_st))) == NULL) {
		WNdisplay(20,10,"Database inaccessible - Press any key : ");
		KBgetch();
		exit(1);
	}

	if (DBindex(conf.indexfile,findfunc) < 0) {
		WNdisplay(20,10,"Index file inaccessible - Press any key : ");
		KBgetch();
		exit(1);
	}

	WNdisplay(2,1,"Surname      :");
	WNdisplay(2,2,"First name   :");
	WNdisplay(2,4,"Address 1    :");
	WNdisplay(2,5,"Address 2    :");
	WNdisplay(2,7,"Phone number :");
	WNdisplay(2,9,"Paid :");

	DBgo(0);
	SCclear(screen);
	cf = 0;
	exitloop = FALSE;
	while (!exitloop) {
		switch(SCgetscreen(screen,&cf)) {
			case ESCAPE :
				checksave();
				exitloop = TRUE;
				break;
			case F1 :
				help();
				break;
			case F2 :
				search();
				break;
			case F3 :
				add();
				break;
			case F4 :
				save();
				break;
			case F5 :
				delete();
				break;
			case F6 :
				scan();
				break;
			case F7 :
				MNmenu(&reportmenu);
				break;
			case F8 :
				pack();
				break;
			case F9 :
				MNmenu(&cmdmenu);
				break;
			case F0 :
				clear();
				break;
			case CPGUP :
				back();
				break;
			case CPGDN :
				next();
				break;
		}
	}
}

void
initsystem()
{
	getopt("c:",NULL,confarg);
	CFconf(confarg,config);
	MNcolor(conf.norm,conf.high);

	WNopen(1,1,80,3,conf.norm,SINGLE,NULL);
	WNgotoxy(39-(strlen(title)+strlen(version)+4)/2,1);
	WNprintf("%s (v%s)",title,version);
	WNopen(1,4,80,25,conf.norm,DOUBLE,NULL);
}

void
pack()
{
	int		r;
	DBfile	*temp;

	if (!getans("Packing database - Are you sure (Y/N) ? "))
		return;

	if ((temp = DBopen(conf.tempfile,sizeof(rec_st))) == NULL) {
		message("Can't open temporary file - Press any key");
		return;
	}
	if (DBindex(conf.tempidx,findfunc) < 0) {
		message("Can't open temporary index - Press any key");
		DBclose();
		remove(conf.tempfile);
		return;
	}

	WNopen(1,1,80,3,conf.norm,DOUBLE|POPUP,NULL);
	r = 1;
	DBselect(curr);
	DBgo(DBtop());
	while (DBrecno() != 0) {
		DBread(&rec);
		WNprintf("\r\nCopying record %d (%s)",r,rec.sn);
		DBselect(temp);
		DBadd(&rec);
		DBselect(curr);
		DBskip(1);
		r++;
	}

	DBselect(temp);
	DBclose();		/* Temp database */
	DBclose();		/* Real database */
	remove(conf.datafile);
	remove(conf.indexfile);
	rename(conf.tempfile,conf.datafile);
	rename(conf.tempidx,conf.indexfile);
	curr = DBopen(conf.datafile,sizeof(rec_st));
	DBindex(conf.indexfile,findfunc);
	WNclose();
	DBgo(0);
	SCclear(screen);
}

void
add()
{
	if ((DBfindrec(&rec) == 0) ||
		(getans("Are you sure you want to add this patient (Y/N) ? "))) {
		popup("Adding");
		DBadd(&rec);
		WNclose();
	}
}

void
delete()
{
	if (DBrecno() == 0) {
		message("No patient selected - Press any key");
	}
	else {
		if (getans("Are you sure you want to delete this patient (Y/N) ? ")) {
			popup("Deleting");
			DBdelete(&rec);
			WNclose();
		}
	}
}

void
back()
{
	if (DBrecno() != DBtop()) {
		checksave();
		DBskip(-1);
		DBread(&rec);
		SCshow(screen);
	}
}

void
next()
{
	if (DBrecno() != DBbot()) {
		checksave();
		DBskip(1);
		DBread(&rec);
		SCshow(screen);
	}
}

void
clear()
{
	checksave();
	DBgo(0);
	SCclear(screen);
	cf = 0;
}

void
scan()
{
	int		exitloop;
	rec_st	t;
	int		i;
	int		orn;
	int		tmprn;

	WNopen(1,4,80,25,conf.norm,DOUBLE|POPUP,NULL);
	tmprn = DBrecno();
	orn = -1;
	exitloop = FALSE;
	while (!exitloop) {
		if (DBrecno() == 0)
			DBgo(DBtop());
		if (DBrecno() != orn) {
			orn = DBrecno();
			WNclear();
			for (i=1; (i<=20) && (DBrecno() != 0); i++) {
				DBread(&t);
				WNgotoxy(2,i);
				WNprintf("%-20s %-20s %-34.34s",t.sn,t.fn,t.addr1);
				DBskip(1);
			}
			DBskip(-20);
		}
		switch(KBgetch()) {
			case ESCAPE :
				exitloop = TRUE;
				break;
			case PGUP :
				DBskip(-20);
				if (DBrecno() == 0)
					DBgo(DBtop());
				break;
			case PGDN :
				DBskip(40);
				DBskip(-20);
				break;
		}
	}
	WNclose();
	DBgo(tmprn);
}

void
save()
{
	if (DBrecno() == 0)
		return;

	DBread(&frec);
	if (findfunc(&rec,&frec)) {
		if (!getans("Patient surname changed - Add new patient (Y/N) ? ")) {
			popup("Saving");
			DBdelete(&rec);
			DBadd(&rec);
			WNclose();
		}
		else {
			popup("Adding");
			DBadd(&rec);
			WNclose();
		}
	}
	else {
		popup("Saving");
		DBwrite(&rec);
		WNclose();
	}
}

int
seekfunc(r1,r2)
	rec_st	*r1;
	rec_st	*r2;
{
	return(r1->sn[0] == '\0' ? 0 : strnicmp(r1->sn,r2->sn,strlen(r1->sn)));
}

int
findfunc(r1,r2)
	rec_st	*r1;
	rec_st	*r2;
{
	return(stricmp(r1->sn,r2->sn));
}

void
help()
{
	FILE	*fp;
	char	s[81];

	WNopen(1,4,80,25,conf.norm,DOUBLE|POPUP,NULL);
	if ((fp = fopen(conf.helpfile,"rt")) == NULL) {
		WNdisplay(20,10,"Help not available - Press any key : ");
	}
	else {
		while (fgets(s,80,fp)) {
			WNputs(s);
			WNputc('\r');
		}
		WNdisplay(1,20,"Press any key : ");
	}
	KBgetch();
	fclose(fp);
	WNclose();
}

int
checksave()
{
	if (DBrecno() == 0)
		return(TRUE);

	DBread(&frec);
	if (!DBcmp(&rec,&frec))
		return(TRUE);

	if (getans("Current patient has been updated - Save (Y/N) ? "))
		save();

	return(TRUE);
}

void
search()
{
	int		n;
	int		r;

	n = scandata(&rec);
	if (n == 1) {
		checksave();
		DBreadn(recn[0],&rec);
		SCshow(screen);
	}
	else if (n == 0) {
		if (getans("Patient not found - Add (Y/N) ? ")) {
			popup("Adding");
			DBadd(&rec);
			WNclose();
		}
	}
	else {
		if ((r = selectrec(n)) > 0) {
			checksave();
			DBreadn(r,&rec);
			SCshow(screen);
		}
	}
}

int
scandata(r)
	rec_st	*r;
{
	int		n = 0;
	int		rn;
	rec_st	t;
	int		currn;

	currn = DBrecno();
	for (rn = DBseekfirst(r,seekfunc); (n<20) && (rn>0);
		rn = DBseeknext(r,seekfunc)) {
		DBreadn(rn,&t);
		sprintf(recs[n]," %-20s %-20s %-32.32s ",t.sn,t.fn,t.addr1);
		recn[n++] = rn;
	}
	DBgo(currn);

	return(n);
}

int
selectrec(n)
	int		n;
{
	int		i;
	int		l;
	int		ol;
	int		exitloop;
	int		retval;

	WNopen(1,4,80,25,conf.norm,DOUBLE|POPUP,NULL);
	for (i=0; i<n; i++) {
		WNdisplay(2,i+1,recs[i]);
	}

	l = 0;
	ol = -1;
	retval = 0;
	exitloop = FALSE;
	while (!exitloop) {
		if (l != ol) {
			WNcolor(conf.norm);
			if (ol >= 0)
				WNdisplay(2,ol+1,recs[ol]);
			WNcolor(conf.high);
			WNdisplay(2,l+1,recs[l]);
			ol = l;
		}
		switch (KBgetch()) {
			case ESCAPE :
				exitloop = TRUE;
				break;
			case RETURN :
				exitloop = TRUE;
				retval = recn[l];
				break;
			case CURUP :
				l = (l == 0 ? n-1 : l-1);
				break;
			case CURDN :
				l = (l == n-1 ? 0 : l+1);
				break;
		}
	}

	WNcolor(conf.norm);
	WNclose();
	return(retval);
}

void
message(s)
	char	*s;
{
	va_list	ap;
	char	t[81];

	WNopen(2,2,79,2,conf.norm,POPUP,NULL);
	va_start(ap,s);
	vsprintf(t,s,ap);
	va_end(ap);
	strcat(t," : ");
	WNputs(t);
	KBgetch();
	WNclose();
}

void
popup(s)
	char	*s;
{
	int		l;

	l = 39-(strlen(s)/2);
	WNopen(l,4,l+strlen(s)+1,4,conf.high,POPUP,NULL);
	WNprintf(" %s",s);
	delay(SHOWTIME);
}

int
getans(s)
	char	*s;
{
	uchar	ch;

	WNopen(2,2,79,2,conf.norm,POPUP,NULL);
	WNputc(' ');
	WNputs(s);
	ch = toupper(KBgetch());
	WNclose();

	return(ch == 'Y' ? TRUE : FALSE);
}

void
rbrief()
{
	int		tmprn;
	rec_st	t;
	int		l;

	tmprn = DBrecno();
	PRseterr(prerr);
	l = 0;
	DBgo(DBtop());
	while (DBrecno() != 0) {
		if (l++ > 59) {
			PRputs("\014");
			l = 1;
		}
		if (l == 1)
			header("Brief Listing");
		DBread(&t);
		PRprintf("%-20s %-20s %-36.36s\r\n",t.sn,t.fn,t.addr1);
		DBskip(1);
	}
	DBgo(tmprn);
}

void
prerr(s)
	char	*s;
{
	message(s);
	DBgo(DBbot());
}

void
rfull()
{
	int		tmprn;
	rec_st	t;
	int		l;

	tmprn = DBrecno();
	PRseterr(prerr);
	l = 0;
	DBgo(DBtop());
	while (DBrecno() != 0) {
		if (l++ > 9) {
			PRputs("\014");
			l = 1;
		}
		if (l == 1)
			header("Full Listing");
		DBread(&t);
		PRprintf("%-20s %-20s %s\r\n%-60s %s\r\n",t.sn,t.fn,
			(t.paid == 'Y' ? "Paid" : "Not Paid"),t.addr1,t.phon);
		PRputs(pline);
		DBskip(1);
	}
	DBgo(tmprn);
}

void
header(s)
	char	*s;
{
	PRprintf("\016%6sLibrary Demo System (v%s)\r\n","",version);
	PRprintf("%*s%s\r\n",40-(strlen(s)/2),"",s);
	PRputs(pline);
}

void
rpaid()
{
	int		tmprn;
	rec_st	t;
	int		l;

	tmprn = DBrecno();
	PRseterr(prerr);
	l = 0;
	DBgo(DBtop());
	while (DBrecno() != 0) {
		DBread(&t);
		if (t.paid != 'Y') {
			if (l++ > 59) {
				PRputs("\014");
				l = 1;
			}
			if (l == 1)
				header("Unpaid Report");
			PRprintf("%-20s %-20s %s\r\n",t.sn,t.fn,t.addr1);
			DBskip(1);
		}
	}
	DBgo(tmprn);
}

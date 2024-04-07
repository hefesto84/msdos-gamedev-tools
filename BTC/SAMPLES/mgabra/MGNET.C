/*****************************************
      NETWORK FUNCTION LIBRARY
*****************************************/

/*

The following library was built over several years of hacking with the
Novell OS.  Not all the code is guaranteed to exemplify good programming.
(In fact, some of it is darnright embarrassing). The nastiest part of
dealing with the Novell OS is the fact that integers (2 bytes) and
especially troublesome long integers (4 bytes) are swapped in their
byte order. You don't have to worry about that because these functions
handle that. However be careful. I haven't always written everything
clearly enough to show which  swapping return values or parameters
should have.

Not all the functions available are here by far. These are just the
ones I found necessary while writing Utilities.

I recently rewrote some of the library to use dynamic (on stack) buffers
for the packets. Before I was using statics which consumed data space in
an unnecessarily wasteful manner. I haven't been able to test ALL the
rewritten functions for bugs. If you do have trouble it helps to have the:

APPLICATION PROGRAMMER'S GUIDE TO NETWARE AND FUNCTION CALL REFERENCE

Which is available from Novell.

Walt Howard
1-31-90
*/

#include <dos.h>
#include <string.h>
#include <stdio.h>
#include <dir.h>

#define WILD 0xFFFF
#define UNKNOWN_TYPE 0
#define NET_USER 1
#define USER_GROUP 2
#define PRINT_SERVER 3
#define FILE_SERVER 4
#define SENDFUNC 0xE1
#define LACKOFSPACE 0xFE
#define DOSSERVICE 0x21
#define MAXBROAD 60
#define LOGFUNC 0xE3
#define DIRFUNC 0xE2
#define TSR 1
#define GETMAP 0x01
#define MAXPATHLENGTH 64
#define GETFILEINFO 0x0f
#define ALLOCATEBASE 0x12

#define GETOBJECT 0x16
#define SCANBIND 0x37
#define READPROP 0x3D
#define SEARCH_PATTERN_LENGTH 20
#define BINDERY 0xE3
#define BUZZ 0x00
#define CASTOFF 0x02
#define CASTON 0x03
#define PREF 0xF0
#define FILEINFO 0x0F
#define GETIDNAME 0x36

#define MAXSTATIONS 100
#define MAXSEND 60

#define WILD 0xFFFF
#define UNKNOWN_TYPE 0
#define NET_USER 1
#define USER_GROUP 2
#define PRINT_SERVER 3
#define FILE_SERVER 4
#define GETOBJECT 0x16
#define SCANBIND 0x37
#define SEARCH_PATTERN_LENGTH 20
#define BINDERY 0xE3
#define MAXSTATIONS 100
#define	COMMFUNC  0xE1
#define OPENPIPE 0x06
#define GETMSG 0x05
#define SENDMSG 0x04
#define PIPESTAT 0x08
#define CLOSEPIPE 0x07
#define MAXMSG 126

#define PIPEGOOD 0x00
#define PIPEHALF 0xFE
#define NOSTATION 0xFF

typedef int NATIVE;
typedef int WORD;
typedef long LONG;
typedef char BYTE;

BYTE spare;
typedef unsigned int REGISTER;

union trans {
   char byte[4];
   int intval;
   long longval;
} transform;

/*****************************************
                 NETCALL
*****************************************/

int netcall(BYTE funcfamily, void far *req, void far *rep)
{
   int rval;

   #ifdef TSR
      swapno();
   #endif
   _DI = FP_OFF(rep);
   _SI = FP_OFF(req);
   _ES = FP_SEG(rep);
   _DS = FP_SEG(req);
   _AL = 0;
   _AH = funcfamily;

   geninterrupt(0x21);

   rval = (int) _AL;

   #ifdef TSR
      swapyes();
   #endif

   return(rval);

}

/*****************************************
     BYTE ORDER SWAPPING FUNCTIONS
*****************************************/

long swaplong(long longint)
{

   transform.longval = longint;

   spare = transform.byte[0];
   transform.byte[0] = transform.byte[3];
   transform.byte[3] = spare;

   spare = transform.byte[1];
   transform.byte[1] = transform.byte[2];
   transform.byte[2] = spare;

   return(transform.longval);

}

/*****************************************
                SWAPINT
*****************************************/

int swapint(int val)
{

   transform.intval = val;
   spare = transform.byte[0];
   transform.byte[0] = transform.byte[1];
   transform.byte[1] = spare;
   return(transform.intval);

}

#define PROPERTY_SET 0x02
#define GETOBJECTID 0x35
#define ENVIRONMENT 0xF3
#define ADDTRUSTEE 0x0d

#define CREATEOBJECT 0x32
#define ADDPROPERTY 0x39
#define MAXOBJECTNAMELENGTH 128
#define MAXPROPERTYNAMELENGTH 128
#define ADDVALUE 0x41
#define MAXMEMBERNAMELENGTH 128
#define ADDITEMVALUE 0x3e
#define SET 0x02
#define ITEM 0x00

/* SECURITY DEFINES */

#define READ_ANYONE 0xf0
#define READ_ANYONE_LOGGED 0xf1
#define READ_OBJECT_OR_SUPERVISOR 0xf2
#define READ_SUPERVISOR 0xf3
#define READ_BINDERY 0xf4

#define WRITE_ANYONE 0x0f
#define WRITE_ANYONE_LOGGED 0x1f
#define WRITE_OBJECT_OR_SUPERVISOR 0x2f
#define WRITE_SUPERVISOR 0x3f
#define WRITE_BINDERY 0x4f

/*****************************************
         NOVELL PIPE FUNCTIONS
*****************************************/

#define COMMFUNC 0xE1
#define OPENPIPE 0x06
#define GETMSG 0x05
#define SENDMSG 0x04
#define PIPESTAT 0x08
#define CLOSEPIPE 0x07
#define MAXMSG 126
#define PIPEGOOD 0x00
#define PIPEHALF 0xFE
#define NOSTATION 0xFF

/*****************************************
             BINDFINDFIRST
*****************************************/

static LONG lastobject;

int bindfindfirst(WORD patterntype, char *pattern)
{
   int rval;

   struct {
      NATIVE packetlength;
      BYTE function;
      LONG lastobjectseen;
      WORD patterntype;
      BYTE patternlength;
      BYTE searchpattern[SEARCH_PATTERN_LENGTH];
   } send;

   struct {
      NATIVE packetlength;
      LONG uniqueobjectid;
      WORD objecttype;
      BYTE objectname[48];
      BYTE objectflags;
      BYTE objectsecurity;
      BYTE propertiesexist;
   } recv;

   lastobject = -1;
   send.lastobjectseen = swaplong(lastobject);
   lastobject = send.lastobjectseen;
   send.packetlength = sizeof(send) - sizeof(send.packetlength);
   send.function = SCANBIND;
   send.patterntype = swapint(patterntype);
   stpcpy(send.searchpattern, pattern);
   send.patternlength = strlen(send.searchpattern);
   recv.packetlength = sizeof(recv);
   rval = (int) netcall(BINDERY,  &send,  &recv);
   lastobject = recv.uniqueobjectid;
   return(rval);
}

/*****************************************
              BINDFINDNEXT
*****************************************/

long bindfindnext(void)
{
   int rval;

   struct {
      NATIVE packetlength;
      BYTE function;
      LONG lastobjectseen;
      WORD patterntype;
      BYTE patternlength;
      char searchpattern[SEARCH_PATTERN_LENGTH];
   } send;

   struct {
      NATIVE packetlength;
      LONG uniqueobjectid;
      WORD objecttype;
      BYTE objectname[48];
      BYTE objectflags;
      BYTE objectsecurity;
      BYTE propertiesexist;
   } recv;

   send.packetlength = sizeof(send) - sizeof(send.packetlength);
   send.function = SCANBIND;
   send.lastobjectseen = lastobject;
   recv.packetlength = sizeof(recv);
   rval = netcall(BINDERY,  &send,  &recv);
   lastobject = recv.uniqueobjectid;
   return(rval);

}

/*****************************************
              GETOBJECTID
*****************************************/

long getobjectid(char *name, WORD objecttype)
{
   int rval;
   long temp;

   struct {
      NATIVE packetlength;
      BYTE function;
      WORD objecttype;
      BYTE objectnamelength;
      BYTE objectname[MAXOBJECTNAMELENGTH];
   } send;

   struct {
      NATIVE packetlength;
      LONG objectid;
      WORD objecttype;
      BYTE objectname[MAXOBJECTNAMELENGTH];
   } recv;

   send.function = GETOBJECTID;
   send.objecttype = swapint(objecttype);
   send.objectnamelength = (char) strlen(name);
   strcpy(send.objectname, name);
   send.packetlength = send.objectnamelength + 4;
   recv.packetlength = 54;
   recv.objectid = 0L;
   rval = netcall(BINDERY,  &send,  &recv);
   temp = swaplong(recv.objectid);
   if (rval == 0) return(temp);
   return(0L);
   }

/****************************************
             READ PROPERTY
****************************************/

char *getproperty(char *user, char *property)
{
   int rval;
   int temp;

   char send[256];

   struct {
      NATIVE packetlen;
      BYTE data[128];
      BYTE more;
      BYTE propertyflags;
   } recv;

   send[2] = READPROP;
   send[3] = NET_USER >> 8;
   send[4] = NET_USER & 255;
   send[5] = strlen(user);
   strcpy(&send[6], user);
   temp = 6 + send[5];
   send[temp++] = 1;
   send[temp++] = strlen(property);
   strcpy(&send[temp], property);
   temp = temp + send[temp];
   send[0] = temp & 255;
   send[1] = temp >> 8;
   recv.packetlen = sizeof(recv) - sizeof(recv.packetlen);
   rval = netcall(BINDERY,  send,  &recv);
   if (rval != 0) return(0);
   recv.more = 0;
   return(recv.data);

}

/*****************************************
             SCAN PROPERTY
*****************************************/

LONG lastinstance;

char *scanproperty(WORD objtype, char *objname, LONG instance, char *property)
{
   int rval;
   int temp;

   char send[256];

   struct {
      NATIVE packetlen;
      BYTE propertyname[16];
      BYTE propertyflags;
      BYTE propertysec;
      LONG searchinstance;
      BYTE valueavailable;
      BYTE morecvperties;
   } recv;

   if (instance == -1L) lastinstance = swaplong(-1L);
   if (instance != -1L) lastinstance = recv.searchinstance;
   send[2] = 60;
   send[3] = objtype >> 8;
   send[4] = objtype & 255;
   send[5] = strlen(objname);
   strcpy(&send[6], objname);
   temp = 6 + send[5];
   *((long *) &send[temp]) = lastinstance;
   temp += 4;
   send[temp++] = (char) strlen(property);
   strcpy(&send[temp], property);
   temp = temp + send[temp];
   send[0] = temp & 255;
   send[1] = temp >> 8;
   recv.packetlen = sizeof(recv) - sizeof(recv.packetlen);
   rval = netcall(BINDERY,  send,  &recv);
   if (rval != 0) return(0);
   return(recv.propertyname);
}

/*****************************************
  WHOUSER - RETURNS USER OF CONNECTION
*****************************************/

char objectname[48];

char *whouser(int connection)
{
   int rval;

   struct {
      NATIVE packetlen;
      BYTE function;
      BYTE connection;
   } send;

   struct {
      NATIVE packetlen;
      LONG uniqueid;
      WORD type;
      BYTE objectname[48];
      BYTE logtime[8];
   } recv;

   send.function = GETOBJECT;
   send.connection = (char) connection;
   send.packetlen = sizeof(send) - sizeof(send.packetlen);
   recv.packetlen = sizeof(recv) - sizeof(recv.packetlen);
   rval = netcall(BINDERY,  &send,  &recv);
   if(!rval) strcpy(objectname, recv.objectname); else return(0);
   objectname[10] = 0;
   return(objectname);
}


/*****************************************
                OPENPIPE
*****************************************/

int openpipe(int station)
{
   int rval;

   struct {
      NATIVE packetlen;
      BYTE function;
      BYTE numstations;
      BYTE station;
   } send;

   struct {
      NATIVE packetlen;
      BYTE numstations;
      BYTE pipestat;
   } recv;

   send.packetlen = sizeof(send) - sizeof(send.packetlen);
   recv.packetlen = sizeof(recv) - sizeof(recv.packetlen);
   send.function = OPENPIPE;
   send.numstations = 1;
   send.station = (BYTE) station;
   rval = netcall(COMMFUNC,  &send,  &recv);
   if (rval == LACKOFSPACE) perror("NETWORK LACK OF PIPE SPACE");
   return((int) recv.pipestat);

}

/*****************************************
               PIPE STATUS
*****************************************/

int pipestat(int station)
{
   int rval;

   struct {
      NATIVE packetlen;
      BYTE function;
      BYTE numstations;
      BYTE station;
   } send;

   struct {
      NATIVE packetlen;
      BYTE numstations;
      BYTE status;
   } recv;

   send.packetlen = sizeof(send) - sizeof(send.packetlen);
   recv.packetlen = sizeof(recv) - sizeof(recv.packetlen);
   send.function = PIPESTAT;
   send.numstations = 1;
   send.station = (BYTE) station;
   rval = netcall(COMMFUNC,  &send,  &recv);
   if (rval == LACKOFSPACE) perror("NETWORK LACK OF PIPE SPACE");
   return((int) recv.status);
}

/*****************************************
                GETMESSAGE
*****************************************/

char *getmessage()
{
   int rval;

   struct {
      NATIVE packetlen;
      BYTE function;
   } send;

   struct {
      NATIVE packetlen;
      BYTE source;
      BYTE msglen;
      BYTE message[126];
   } recv;

   send.packetlen = sizeof(send) - sizeof(send.packetlen);
   recv.packetlen = sizeof(recv) - sizeof(recv.packetlen);
   send.function = GETMSG;
   rval = netcall(COMMFUNC,  &send,  &recv);
   if (rval == LACKOFSPACE) perror("NETWORK LACK OF PIPE SPACE");
   recv.message[recv.msglen] = 0;
   if(!recv.msglen && !recv.source) return(0); else return(&recv.source);
}

/************************************************
SENDMESSAGE - SEND A MESSAGE TO ANOTHER STATION
************************************************/

int sendmess(int station, void *msg)
{
   int rval;

   struct {
      NATIVE packetlen;
      BYTE function;
      BYTE numstations;
      BYTE station;
      BYTE msglen;
      BYTE message[MAXMSG];
   } send;

   struct {
      NATIVE packetlen;
      BYTE numstations;
      BYTE recvt;
   } recv;

   send.packetlen = sizeof(send) - sizeof(send.packetlen);
   recv.packetlen = sizeof(recv) - sizeof(recv.packetlen);
   send.function = SENDMSG;
   send.numstations = 1;
   send.station = (BYTE) station;
   send.msglen = (BYTE) strlen(msg);
   strcpy(send.message, msg);
   rval = netcall(COMMFUNC,  &send,  &recv);
   if (rval == LACKOFSPACE) perror("NETWORK LACK OF PIPE SPACE");
   return((int) recv.recvt);
}

/*****************************************
  CLOSEPIPE - CLOSE PIPE TO A STATION
*****************************************/

int closepipe(int station)
{
   struct {
      NATIVE packetlen;
      BYTE function;
      BYTE numstations;
      BYTE station;
   } send;

   struct {
      NATIVE packetlen;
      BYTE numstations;
      BYTE pipestat;
   } recv;

   send.packetlen = sizeof(send) - sizeof(send.packetlen);
   recv.packetlen = sizeof(recv) - sizeof(recv.packetlen);
   send.function = CLOSEPIPE;
   send.numstations = 1;
   send.station = (BYTE) station;
   netcall(COMMFUNC,  &send,  &recv);
   return((int) recv.pipestat);
}

/*****************************************
           ENVIRONMENTAL CALLS
*****************************************/


/*****************************************
     GET THIS STATIONS CONNECTION
*****************************************/

int myconnection(void)
{
   _AH = 0xDC;
   geninterrupt(0x21);
   return((int) _AL);
}

/*****************************************
        SEND - DO A NETWORK SEND
*****************************************/

int netsend(int station, char *msg)
{
   int rval;

   struct {
      NATIVE packetlen;
      BYTE function;
      BYTE numstations;
      BYTE station;
      BYTE messagelen;
      BYTE message[MAXSEND+1];
   } send;

   struct {
      NATIVE packetlen;
      BYTE numstations;
      BYTE recvt;
   } recv;

   send.packetlen = sizeof(send) - sizeof(send.packetlen);
   recv.packetlen = sizeof(recv) - sizeof(recv.packetlen);
   send.numstations = 1;
   send.station = (char) station;
   send.function = BUZZ;
   send.messagelen = (char) strlen(msg);
   strcpy(send.message, msg);

   netcall(SENDFUNC,  &send,  &recv);

   rval = recv.recvt;

   return(rval);
}

/*****************************************
 CASTOFF - DISABLE BROADCAST RECEIVE
*****************************************/

int castoff(void)
{
   int rval;

   struct {
      NATIVE packetlen;
      BYTE function;
   } send;

   struct {
      NATIVE packetlen;
   } recv;

   send.packetlen = sizeof(send) - sizeof(send.packetlen);
   recv.packetlen = sizeof(recv) - sizeof(recv.packetlen);
   send.function = CASTOFF;
   rval = netcall(SENDFUNC,  &send,  &recv);
   return(rval);
}

/*****************************************
   CASTON - ENABLE BROADCAST RECEIVE
*****************************************/

int caston(void)
{
   int rval;

   struct {
      NATIVE packetlen;
      BYTE function;
   } send;

   struct {
      NATIVE packetlen;
   } recv;

   send.packetlen = sizeof(send) - sizeof(send.packetlen);
   recv.packetlen = sizeof(recv) - sizeof(recv.packetlen);
   send.function = CASTON;
   rval = netcall(SENDFUNC,  &send,  &recv);
   return(rval);
}

/*****************************************
  GETBROADCAST - IF BROADCASTS DISABLED
  YOU CAN GET YOUR BROADCAST MESSAGE
  WITH THIS.
*****************************************/

char *getbroadcast(void)
{
   struct {
      NATIVE packetlen;
      BYTE function;
   } send;

   struct {
      NATIVE packetlen;
      BYTE messagelen;
      BYTE message[MAXBROAD+1];
   } recv;

   send.packetlen = sizeof(send) - sizeof(send.packetlen);
   recv.packetlen = sizeof(recv) - sizeof(recv.packetlen);
   send.function = 0x01;
   netcall(SENDFUNC,  &send,  &recv);
   if (recv.messagelen == 0) return(0);
   recv.message[recv.messagelen] = 0;
   return(recv.message);
}

/*****************************************
        BROADCASTMODE
        _AH is mode
        0 accept all
        1 accept console only
        2 accept NO messages
        3 storecvt don't interrupt
        4 return currecvmode
        5 disable timer checks
        6 enable timer checks
*****************************************/

int broadcastmode(int mode)
{
   int rval;
   _AH = 0xDE;
   _DL = (char) mode;
   geninterrupt(0x21);
   rval = (int) _AL;
   return(rval);
}

/*****************************************
      GETPREF - GET PREFERRED SERVER
*****************************************/

int getpref(void)
{
   int rval;
   _AH = PREF;
   _AL = (char) 1;
   geninterrupt(DOSSERVICE);
   rval = (int) _AL;
   return(rval);
}

/*****************************************
     SETPREF - SET PREFSERVER
*****************************************/

int setpref(int servernumber)
{
   int rval;
   _AH = PREF;
   _AL = (char) 0;
   _DL = (char) servernumber;
   geninterrupt(DOSSERVICE);
   rval = (int) _AL;
   return(rval);
}

/*****************************************
      GETEFFECTIVE - GET EFFECTIVE
*****************************************/

int geteffective(void)
{
   int rval;
   _AH = PREF;
   _AL = (char) 2;
   geninterrupt(DOSSERVICE);
   rval = (int) _AL;
   return(rval);
}

/*****************************************
              SHELL VERSION
*****************************************/

int shellversion(void)
{
   int rval;
   _AH = 0xEA;
   _AL = 0;
   geninterrupt(0x21);
   rval = _AH;
   return(rval);
}

/************************************************
                  SERVER DATA
************************************************/

#define GETSERVERINFO 0x11

struct {
   NATIVE packetlen;
   BYTE function;
} send11;

struct {
   NATIVE packetlen;
   BYTE servername[48];
   BYTE version;
   BYTE subversion;
   WORD connectionssupported;
   WORD connectionsinuse;
   WORD maxconnectedvolumes;
   BYTE undefinded[72];
} recv11;

void serverinfo(void)
{
   send11.packetlen = sizeof(send11) - sizeof(send11.packetlen);
   recv11.packetlen = sizeof(recv11) - sizeof(recv11.packetlen);
   send11.function = GETSERVERINFO;
   netcall(LOGFUNC,  &send11,  &recv11);
}

/*****************************************
                VERSION
*****************************************/

int netwareversion(void)
{
   serverinfo();
   return((int) recv11.version);
}

/*****************************************
                SUBVERSION
*****************************************/

int subversion(void)
{
   serverinfo();
   return((int) recv11.subversion);
}

/*****************************************
                SERVERNAME
*****************************************/

char *servername(void)
{
   serverinfo();
   return(recv11.servername);
}

/*****************************************
                CONNECTIONS
*****************************************/

int connections(void)
{
   serverinfo();
   return(swapint(recv11.connectionsinuse));
}

/*****************************************
                OBJECTNAME
*****************************************/

struct {
   NATIVE packetlen;
   LONG objectid;
   WORD objecttype;
   BYTE name[48];
} recv36;

int getobjectdata(LONG id)
{

   int rval;

   struct {
      NATIVE packetlen;
      BYTE function;
      LONG objectid;
   } send;

   send.function = GETIDNAME;
   send.objectid = swaplong(id);
   send.packetlen = sizeof(send) - sizeof(send.packetlen);
   recv36.packetlen = sizeof(recv36) - sizeof(recv36.packetlen);
   rval = netcall(BINDERY,  &send,  &recv36);
   return(rval);
}

char *getobjectname(LONG id)
{
   int rval;
   rval = getobjectdata(id);
   if (rval) recv36.name[0] = 0;
   return(recv36.name);
}

WORD getobjecttype(LONG id)
{

   int rval;
   rval = getobjectdata(id);
   if (rval) recv36.objecttype = 0;
   return(swapint(recv36.objecttype));

}

/*****************************************
                BASEDATA
*****************************************/

struct {
   NATIVE packetlen;
   BYTE function;
   BYTE base;
} send15;

struct {
   NATIVE packetlen;
   WORD sectorsperblock;
   WORD totalblocks;
   WORD availableblocks;
   WORD totaldirs;
   WORD availabledirs;
   BYTE volname[16];
   WORD removeable;
} recv15;

/*****************************************
           CURRENTBASE
*****************************************/

int currentbase(void)
{
   int rval, dval;
   dval = getdisk();
   _AH = 0xe9;
   _AL = 0;
   _DX = dval;
   geninterrupt(0x21);
   rval = _AX;
   return(rval);
   /* remember AL = base number AH = base flags */
}

/****************************************
       FILE ATTRIBUTE FUNCTIONS
****************************************/

struct NETDIRINFO {
   NATIVE packetlen;
   BYTE subdirname[16];
   BYTE creationdate[2];
   BYTE creationtime[2];
   LONG owner;
   BYTE rights;
   BYTE pad;
   WORD subdirnumber;
};

struct NETDIRINFO netdirdata;

#define GETDIRINFO 0x02

struct NETDIRINFO *getdirinfo(char *pathspec)
{
   int rval;
   static int dirslot;

   struct {
      NATIVE packetlen;
      BYTE function;
      BYTE base;
      WORD searchstart;
      BYTE pathlen;
      BYTE pathspec[MAXPATHLENGTH];
   } send;

   send.searchstart = swapint(1);

   if (strchr(pathspec, '*')) send.searchstart = swapint(dirslot);
   if (strchr(pathspec, '?')) send.searchstart = swapint(dirslot);

   send.function = GETDIRINFO;
   send.base = (BYTE) (currentbase() & 127);
   send.pathlen = strlen(pathspec);
   strcpy(send.pathspec, pathspec);
   send.packetlen = 7 + send.pathlen;
   netdirdata.packetlen = 28;
   rval = netcall(DIRFUNC, &send, &netdirdata);
   dirslot = swapint(netdirdata.subdirnumber) + 1;
   if (!strchr(pathspec, '*')) dirslot = 1;
   if (!strchr(pathspec, '?')) dirslot = 1;
   if (!rval) return(&netdirdata);
   memset(&netdirdata, 0, sizeof(netdirdata));
   return((struct NETDIRINFO *) 0);
}

char *getdirname(char *path)
{
   getdirinfo(path);
   return(netdirdata.subdirname);
}

struct NETWORKFILEDATA {
   NATIVE packetlen;
   WORD slotnum;
   BYTE name[14];
   BYTE attribs;
   BYTE exectype;
   LONG size;
   WORD create;
   WORD access;
   WORD updatedate;
   WORD updatetime;
   LONG owner;
   BYTE undefined[120];
};

struct NETWORKFILEDATA netfiledata;

static slotnum = 0xffff;

struct NETWORKFILEDATA *getfileinfo(char *path)
{

   int temp;

   struct {
      NATIVE packetlen;
      BYTE function;
      WORD lastslot;
      BYTE base;
      BYTE attrib;
      BYTE pathlen;
      BYTE path[MAXPATHLENGTH];
   } send;

   send.lastslot = swapint(0xffff);

   if (strchr(path, '*')); send.lastslot = slotnum;
   if (strchr(path, '?')); send.lastslot = slotnum;

   send.function = GETFILEINFO;
   send.base = (BYTE) (currentbase() & 127);
   send.attrib = 0; /* FA_RDONLY | FA_HIDDEN | FA_SYSTEM; */
   strncpy(send.path, path, MAXPATHLENGTH);
   send.pathlen = (char) strlen(send.path);
   send.packetlen = send.pathlen + 6;
   netfiledata.packetlen = 143;
   temp = netcall(LOGFUNC,  &send,  &netfiledata);
   slotnum = netfiledata.slotnum;
   if (!temp) return(&netfiledata);
   memset(&netfiledata, 0, sizeof(netfiledata));
   return((struct NETWORKFILEDATA *) 0);
}

/*****************************************
                FILENAME
*****************************************/

char *filename(char *path)
{
   getfileinfo(path);
   return(netfiledata.name);
}

/*****************************************
                FILEOWNER
*****************************************/

long fileowner(char *path)
{
   getfileinfo(path);
   return(netfiledata.owner);
}

/*****************************************
             FILESIZE
*****************************************/

long filesize(char *path)
{
   getfileinfo(path);
   return(swaplong(netfiledata.size));
}

WORD fileupdatedate(char *path)
{
   getfileinfo(path);
   return(swapint(netfiledata.updatedate));
}

WORD fileupdatetime(char *path)
{
   getfileinfo(path);
   return(swapint(netfiledata.updatetime));
}

WORD filelastaccess(char *path)
{
   getfileinfo(path);
   return(swapint(netfiledata.access));
}

WORD filecreation(char *path)
{
   getfileinfo(path);
   return(swapint(netfiledata.create));
}

BYTE fileattribs(char *path)
{
   getfileinfo(path);
   return(netfiledata.attribs);
}

struct SETFILEDATA {
   NATIVE packetlen;
   BYTE function;
   BYTE attribs;
   BYTE exectype;
   BYTE filler[4];
   WORD create;
   WORD access;
   WORD updatedate;
   WORD updatetime;
   LONG owner;
   BYTE undefined[60];
   BYTE base;
   BYTE search;
   BYTE pathlen;
   BYTE path[MAXPATHLENGTH];
};

struct SETFILEDATA setfile;

struct SETFILEDATA *getcurrentdata(char *path)
{
   struct NETWORKFILEDATA *n;
   n = getfileinfo(path);
   if (!n) return(0);
   setfile.function = 0x10;
   setfile.attribs = n->attribs;
   setfile.exectype = n->exectype;
   setfile.create = n->create;
   setfile.access = n->access;
   setfile.updatedate = n->updatedate;
   setfile.updatetime = n->updatetime;
   setfile.owner = n->owner;
   setfile.base = currentbase();
   setfile.search = 0;
   setfile.pathlen = strlen(n->name);
   strcpy(setfile.path, n->name);
   setfile.packetlen = sizeof(setfile) - 2 - MAXPATHLENGTH + setfile.pathlen;
   return((struct SETFILEDATA *) &setfile);
}

LONG setfileowner(char *path, LONG id)
{
   int rval;

   struct {
      NATIVE packetlen;
   } recv;

   if (!getcurrentdata(path)) return(0);
   setfile.owner = id;
   recv.packetlen = 0;
   rval = netcall(LOGFUNC, &setfile, &recv);
   if (!rval) return(setfile.owner);
   return(0);
}

WORD setfileattribs(char *path, WORD attribs)
{
   int rval;

   struct {
      NATIVE packetlen;
   } recv;

   if (!getcurrentdata(path)) return(-1);
   setfile.attribs = attribs;
   recv.packetlen = 0;
   rval = netcall(LOGFUNC, &setfile, &recv);
   return(rval);
}

WORD setfileupdate(char *path, WORD date)
{
   int rval;

   struct {
      NATIVE packetlen;
   } recv;

   if (!getcurrentdata(path)) return(-1);
   setfile.updatedate = date;
   recv.packetlen = 0;
   rval = netcall(LOGFUNC, &setfile, &recv);
   return(rval);
}

WORD setfilecreate(char *path, WORD date)
{
   int rval;

   struct {
      NATIVE packetlen;
   } recv;

   if (!getcurrentdata(path)) return(-1);
   setfile.create = date;
   recv.packetlen = 0;
   rval = netcall(LOGFUNC, &setfile, &recv);
   return(rval);
}

WORD setfiletime(char *path, WORD time)
{
   int rval;

   struct {
      NATIVE packetlen;
   } recv;

   if (!getcurrentdata(path)) return(-1);
   setfile.updatetime = time;
   recv.packetlen = 0;
   rval = netcall(LOGFUNC, &setfile, &recv);
   return(rval);
}

WORD setfileaccessed(char *path, WORD date)
{
   int rval;

   struct {
      NATIVE packetlen;
   } recv;

   if (!getcurrentdata(path)) return(-1);
   setfile.access = date;
   recv.packetlen = 0;
   rval = netcall(LOGFUNC, &setfile, &recv);
   return(rval);
}


/*****************************************
             GET BASE PATH
*****************************************/

struct {
   NATIVE packetlen;
   BYTE stringlen;
   BYTE path[MAXPATHLENGTH];
} recv01;

char *getbasepath(BYTE base)
{
   int rval;

   struct {
      NATIVE packetlen;
      BYTE function;
      BYTE base;
   } send;

   send.packetlen = sizeof(send) - sizeof(send.packetlen);
   send.function = GETMAP;
   send.base = base;
   rval = netcall(DIRFUNC,  &send,  &recv01);
   if (rval) return(0);
   return(recv01.path);
}

/*****************************************
        ALLOCATE PERMANENT BASE
*****************************************/

int allocatebase(char *dirspec, char drive)
{

   int rval;

   struct {
      NATIVE packetlen;
      BYTE function;
      BYTE sourcebase;
      BYTE drivename;
      BYTE speclength;
      BYTE pathspec[40];
   } send;

   struct {
      NATIVE packetlen;
      BYTE newbase;
      BYTE accessmask;
   } recv;

   send.sourcebase = 0;
   send.drivename = drive;
   send.speclength = strlen(dirspec);
   strcpy(send.pathspec, dirspec);
   send.function = ALLOCATEBASE;
   send.packetlen = sizeof(send) - sizeof(send.pathspec) + strlen(dirspec) - sizeof(send.packetlen);
   recv.packetlen = sizeof(recv) - sizeof(recv.packetlen);
   rval = netcall(DIRFUNC,  &recv,  &recv);
   if (rval) return(0);
   return(recv.newbase);
}

/*****************************************
                GETVOLNUM
*****************************************/

int getvolnum(char *volname)
{

   struct {
      NATIVE packetlen;
      BYTE function;
      BYTE namelength;
      BYTE volname[50];
   } send;

   struct {
      NATIVE packetlen;
      BYTE volnumber;
   } recv;

   int namelen;
   namelen = strlen(volname);
   send.namelength = namelen;
   strcpy(send.volname, volname);
   send.packetlen = namelen + 2;
   recv.packetlen = sizeof(recv) - sizeof(recv.packetlen);
   send.function = 5;
   netcall(DIRFUNC,  &send,  &recv);
   return(recv.volnumber);
}

int netid[14];

int *psn(void)
{
   _AH = 0xEE;
   geninterrupt(0x21);
   netid[0] = _CX;
   netid[1] = _BX;
   netid[2] = _AX;
   netid[3] = 0;
   return(netid);
}

/*****************************************
                GETSTATIONS
*****************************************/

char *getstations(char *objname)
{

   struct {
      NATIVE packetlen;
      BYTE function;
      BYTE namelength;
      BYTE name[60];
   } send;

   struct {
      NATIVE packetlen;
      BYTE listlen;
      BYTE list[80];
   } recv;

   send.packetlen = sizeof(send) - sizeof(send.packetlen);
   send.function = 0x02;
   send.namelength = (char) strlen(objname);
   strcpy(send.name, objname);
   recv.packetlen = sizeof(recv) - sizeof(recv.packetlen);
   netcall(LOGFUNC,  &send,  &recv);
   recv.list[recv.listlen] = 0;
   return(recv.list);
}

/*****************************************
                TRUSTEE PATHS
*****************************************/

WORD lastsequence;

char *trusteepath(char *objname, BYTE vol, WORD objecttype, WORD sequence)
{

   int rval;

   struct {
      NATIVE packetlen;
      BYTE function;
      BYTE volumenumber;
      WORD lastsequence;
      LONG objectid;
   } send;

   struct {
      NATIVE packetlen;
      WORD lastsequence;
      LONG objectid;
      BYTE accessmask;
      BYTE pathlen;
      BYTE path[256];
   } recv;

   send.objectid = getobjectid(objname, objecttype);
   send.packetlen = sizeof(send) - sizeof(send.packetlen);
   send.function = 0x47;
   send.volumenumber = vol;
   if (sequence == 0) send.lastsequence = 0;
   else send.lastsequence = lastsequence;
   recv.packetlen = sizeof(recv) - sizeof(recv.packetlen);
   rval = netcall(BINDERY,  &send,  &recv);
   if (rval) return(0);
   lastsequence = recv.lastsequence;
   recv.path[recv.pathlen] = 0;
   recv.path[recv.pathlen + 1] = recv.accessmask;
   return(recv.path);
}

/*****************************************
                NUMTOVOL
*****************************************/

char *numtovol(int volnum)
{

   struct {
      NATIVE packetlen;
      BYTE function;
      BYTE volnum;
   } send;

  struct {
      NATIVE packetlen;
      BYTE namelen;
      BYTE volname[48];
   } recv;

   send.volnum = volnum;
   send.function = 0x06;
   send.packetlen = sizeof(send) - sizeof(send.packetlen);
   recv.packetlen = sizeof(recv) - sizeof(recv.packetlen);
   netcall(DIRFUNC,  &send,  &recv);
   if (!recv.namelen) return(0);
   return(recv.volname);
}

/*****************************************
           INTERNET ADDRESS
*****************************************/

char *internet(int connection)
{

   struct {
      NATIVE packetlen;
      BYTE function;
      BYTE connection;
   } send;

   struct {
      NATIVE packetlen;
      BYTE internet[4];
      BYTE host[6];
      BYTE socket[2];
   } recv;

   send.packetlen = sizeof(send) - sizeof(send.packetlen);
   recv.packetlen = sizeof(recv) - sizeof(recv.packetlen);
   send.connection = connection;
   send.function = 0x13;
   netcall(LOGFUNC,  &send,  &recv);
   return(recv.internet);
}

/************************************************
                CREATEOBJECT
************************************************/

int createobject(char *name, WORD type, BYTE flags, BYTE security)
{

   int rval;

   struct {
      NATIVE packetlength;
      BYTE function;
      BYTE flags;
      BYTE security;
      WORD type;
      BYTE namelength;
      BYTE name[MAXOBJECTNAMELENGTH];
   } send;

   struct {
      NATIVE packetlength;
   } recv;

   send.flags = flags;
   send.type = swapint(type);
   send.function = CREATEOBJECT;
   send.security = security;
   strncpy(send.name, name, MAXOBJECTNAMELENGTH);
   send.namelength = (BYTE) strlen(send.name);
   send.packetlength = send.namelength + 6;
   recv.packetlength = 0;
   rval = netcall(BINDERY,  &send,  &recv);
   return(rval);
}

/************************************************
                ADDPROPERTY
************************************************/

int addproperty(char *object, WORD objecttype, char *property, char propertytype, char security)
{

   int rval;
   BYTE *offset;

   struct {
      NATIVE packetlength;
      BYTE function;
      WORD objecttype;
      BYTE objectnamelength;
      BYTE remainder[MAXOBJECTNAMELENGTH + MAXPROPERTYNAMELENGTH + 3];
   } send;

   struct {
      NATIVE packetlength;
   } recv;

   send.objecttype = swapint(objecttype);
   send.function = (BYTE) ADDPROPERTY;
   send.objectnamelength = (BYTE) strlen(object);
   strcpy(send.remainder, object);
   offset = strchr(send.remainder, 0);
   strcpy(offset, property);
   *offset++ = (char) propertytype;
   *offset++ = (char) security;
   *offset++ = (char) strlen(property);
   strcpy(offset, property);
   send.packetlength = send.objectnamelength + strlen(property) + 6 + 3;

   recv.packetlength = 0;
   rval = netcall(BINDERY,  &send,  &recv);
   return(rval);
}

/************************************************
             ADDSETPROPERTYVALUE
************************************************/

int addsetvalue(char *object, WORD objecttype, char *property, char *member, WORD membertype)
{

   int rval;
   BYTE *offset;
   WORD *wordptr;

   struct {
      NATIVE packetlength;
      BYTE function;
      WORD objecttype;
      BYTE objectnamelength;
      BYTE remainder[MAXOBJECTNAMELENGTH + MAXPROPERTYNAMELENGTH + 3 + MAXMEMBERNAMELENGTH];
   } send;

   struct {
      NATIVE packetlength;
   } recv;

   send.objecttype = swapint(objecttype);
   send.function = (BYTE) ADDVALUE;
   send.objectnamelength = (BYTE) strlen(object);
   strcpy(send.remainder, object);

   offset = strchr(send.remainder, 0);
   *offset++ = (char) strlen(property);
   strcpy(offset, property);
   offset = strchr(offset, 0);

   wordptr = (WORD *) offset;
   *wordptr = swapint(membertype);
   offset += 2;
   *offset++ = (char) strlen(member);
   strcpy(offset, member);

   send.packetlength = strlen(member) + strlen(property) + strlen(member) + 10;

   recv.packetlength = 0;
   rval = netcall(BINDERY,  &send,  &recv);
   return(rval);
}

/************************************************
             ADDITEMPROPERTYVALUE
************************************************/

int additemvalue(char *object, WORD objecttype, char *property, char *data)
{

   int rval;
   BYTE *offset;

   struct {
      NATIVE packetlength;
      BYTE function;
      WORD objecttype;
      BYTE objectnamelength;
      BYTE remainder[MAXOBJECTNAMELENGTH + MAXPROPERTYNAMELENGTH + 3 + MAXMEMBERNAMELENGTH];
   } send;

   struct {
      NATIVE packetlength;
   } recv;

   send.objecttype = swapint(objecttype);
   send.function = (BYTE) ADDITEMVALUE;
   send.objectnamelength = (BYTE) strlen(object);
   strcpy(send.remainder, object);

   offset = strchr(send.remainder, 0);

   *offset++ = 1;
   *offset++ = 0;
   *offset++ = (char) strlen(property);
   strcpy(offset, property);
   offset = strchr(offset, 0);
   strcpy(offset, data);

   send.packetlength = strlen(object) + strlen(property) + strlen(data) + 9;

   recv.packetlength = 0;
   rval = netcall(BINDERY,  &send,  &recv);
   return(rval);
}

/*****************************************
                ADD TRUSTEE
*****************************************/

int addtrustee(char *directory, LONG userid, BYTE access)
{

   int rval;

   struct {
      NATIVE packetlength;
      BYTE function;
      BYTE sourcebase;
      LONG trustee;
      BYTE accessmask;
      BYTE speclength;
      BYTE pathspec[64];
   } send;

   struct {
      NATIVE packetlength;
   } recv;

   send.function = ADDTRUSTEE;
   send.trustee = swaplong(userid);
   send.sourcebase = 0;
   send.accessmask = access;
   send.speclength = strlen(directory);
   strcpy(send.pathspec, directory);
   send.packetlength = (char)(send.speclength + 8);

   recv.packetlength = 0;
   rval = netcall(DIRFUNC,  &send,  &recv);
   return(rval);
}

/**********************************
 END OF NOVELL NETWORK FUNCTIONS
**********************************/


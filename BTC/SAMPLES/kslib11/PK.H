/* Packet driver interface classes */
#define	CL_NONE				0
#define	CL_ETHERNET			1
#define	CL_PRONET_10		2
#define	CL_IEEE8025			3
#define	CL_OMNINET			4
#define	CL_APPLETALK		5
#define	CL_SERIAL_LINE		6
#define	CL_STARLAN			7
#define	CL_ARCNET			8
#define	CL_AX25				9
#define	CL_KISS				10
#define CL_IEEE8023			11
#define CL_FDDI 			12
#define CL_INTERNET_X25		13
#define CL_LANSTAR			14
#define CL_SLFP 			15
#define	CL_NETROM			16
#define NCLASS				17

/* Packet driver interface types (not a complete list) */
#define	ANYTYPE				0xffff

/* Packet driver function call numbers. From Appendix B. */
#define	DRIVER_INFO			1
#define	ACCESS_TYPE			2
#define	RELEASE_TYPE		3
#define	SEND_PKT			4
#define	TERMINATE			5
#define	GET_ADDRESS			6
#define	RESET_INTERFACE		7
#define GET_PARAMETERS		10
#define AS_SEND_PKT			11
#define	SET_RCV_MODE		20
#define	GET_RCV_MODE		21
#define	SET_MULTICAST_LIST	22
#define	GET_MULTICAST_LIST	23
#define	GET_STATISTICS		24
#define SET_ADDRESS			25

/* Packet driver error return codes. From Appendix C. */

#define	MAXERR			16

#define	NO_ERROR		0	/* What do you think?							*/
#define	BAD_HANDLE		1	/* invalid handle number						*/
#define	NO_CLASS		2	/* no interfaces of specified class found		*/
#define	NO_TYPE			3	/* no interfaces of specified type found		*/
#define	NO_NUMBER		4	/* no interfaces of specified number found		*/
#define	BAD_TYPE		5	/* bad packet type specified					*/
#define	NO_MULTICAST	6	/* this interface does not support multicast	*/
#define	CANT_TERMINATE	7	/* this packet driver cannot terminate			*/
#define	BAD_MODE		8	/* an invalid receiver mode was specified		*/
#define	NO_SPACE		9	/* operation failed because of insufficient space			*/
#define	TYPE_INUSE		10	/* the type had previously been accessed, and not released	*/
#define	BAD_COMMAND		11	/* the command was out of range, or not	implemented			*/
#define	CANT_SEND		12	/* the packet couldn't be sent (usually	hardware error)		*/
#define CANT_SET		13	/* hardware address couldn't be changed (> 1 handle open)	*/
#define BAD_ADDRESS		14	/* hardware address has bad length or format	*/
#define CANT_RESET		15	/* couldn't reset interface (> 1 handle open)	*/
#define	NO_DRIVER		16	/* There is no driver found at any interrupt	*/

/*	Promiscuity mode values				*/

#define	PR_NONE			1	/* Receiver off									*/
#define	PR_D			2	/* Receive direct packets only					*/
#define	PR_DB			3	/* Receive direct and broadcast packets			*/
#define	PR_DBL			4	/* Receive direct, broadcast and lim. multicast	*/
#define	PR_DBM			5	/* Receive direct, broadcast and all multicast	*/
#define	PR_ALL			6	/* Receive all packets							*/

/*
*	Weighted CPU flags values
*/

#define	CARRY_FLAG		0x01

/*
*	Functionality return values
*/

#define	FN_BASIC		0x01
#define	FN_EXTENDED		0x02
#define	FN_HIGHPERF		0x04

/*
*	Structures
*/

struct PKinfo {
	uint	ver;
	uint	class;
	uint	type;
	uint	number;
	char	far *name;
	uint	functions;
};

typedef struct PKinfo PKinfo;

struct PKparam {
	uint8	major_rev;
	uint8	minor_rev;
	uint8	length;
	uint8	addr_len;
	uint	mtu;
	uint	multicast_aval;
	uint	rcv_bufs;
	uint	xmt_bufs;
	uint	int_num;
};

typedef struct PKparam PKparam;

struct PKstat {
	ulong	packets_in;
	ulong	packets_out;
	ulong	bytes_in;
	ulong	bytes_out;
	ulong	errors_in;
	ulong	errors_out;
	ulong	packets_lost;
};

typedef struct PKstat PKstat;

/*
*	Subsidiary functions
*/

int		PKinit(void);
int		PKcheck_int(int);
void	PKerror(int);
void	PKseterr(void (*)());

/*
*	PC/TCP Packet Driver Specification version 1.09 functions
*/

PKinfo	far *PKdriver_info(int);
int		PKaccess_type(int,int,int,char far *,uint,void interrupt (*)());
int		PKrelease_type(int);
int		PKsend_pkt(char far *,uint);
int		PKterminate(int);
int		PKget_address(int,char far *,int);
int		PKreset_interface(int);
PKparam	far *PKget_parameters(void);
int		PKas_send_packet(char far *,uint,void interrupt (*)(void));
int		PKset_rcv_mode(int,int);
int		PKget_rcv_mode(int);
int		PKset_multicast_list(char far *,int);
int		PKget_multicast_list(char far *);
PKstat	far *PKget_statistics(int);
int		PKset_address(char far *,int);

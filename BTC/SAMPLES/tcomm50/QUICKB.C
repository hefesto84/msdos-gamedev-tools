/*
**  QUICKB.C - Quick B Protocol Support routines
**
**	converted to C by Paul M. Resch
**  adapted to LiteComm(tm) ToolBox by Information Technology, Ltd.
*/

/*
** This module implements the B-Protocol Functions.
**
** bp_DLE should be invoked whenever a <DLE> is received.
** bp_ENQ should be called whenever an <ENQ> is received.
** bp_ESC_I should be called when the sequence <ESC><I> is received.
**
** This source was originally derived from QUICKB.INC, written by
** Russ Ranshaw, CompuServe Incorporated.
**
*/

#include "litecomm.h"
#include "litexm.h"
#include <vcstdio.h>

#include <dos.h>

#ifdef M_I86
#include <stdlib.h>
#include <fcntl.h>
#include <io.h>
#include <sys\types.h>
#include <sys\stat.h>
#endif

#ifdef __TURBOC__
#include <conio.h>
#include <fcntl.h>
#include <io.h>
#include <stat.h>
#endif

extern unsigned int port;				/* defined in TTL main */

#define	TRUE	1
#define	FALSE	0

#undef DLE
#undef NAK

#define	DLE		16
#define	ETX		03
#define	NAK		21
#define	ENQ		05
#define	CR		0x0D
#define	LF		0x0A
#define	MAX_BUF_SIZE	1032			/* Largest data block we can handle */
#define	MAX_SA		2					/* Maximum number of waiting packets */

#define	DEF_BUF_SIZE	511				/* Default data block */
#define	DEF_WS		1					/* I can send 2 packets ahead */
#define	DEF_WR		1					/* I can receive single send-ahead */
#define	DEF_BS		8					/* I can handle 1024 bytes */
#define	DEF_CM		1					/* I can handle CRC */
#define	DEF_DQ		1					/* I can handle non-quoted NUL */

#define	MAX_ERRORS	10

#define incr_seq(v)	(v == 9 ? 0 : v+1)	/* macro to incr seq number */
#define incr_SA(v)  (v == MAX_SA ? 0 : v + 1 )
#define	send_enq() (lc_put(port, ENQ))


/*
** Receive States
*/

#define	R_GET_DLE		0
#define	R_GET_B			1
#define	R_GET_SEQ		2
#define	R_GET_DATA		3
#define	R_GET_CHECKSUM	4
#define	R_SEND_ACK		5
#define	R_TIMED_OUT		6
#define	R_SUCCESS		7

/*
** Send States
*/

#define	S_GET_DLE	1
#define	S_GET_NUM	2
#define	S_HAVE_ACK	3
#define	S_GET_PACKET	4
#define	S_TIMED_OUT	5
#define	S_SEND_NAK	6
#define	S_SEND_ENQ	7
#define	S_SEND_DATA	8

typedef	struct	PACKETB
{
	int		seq;						/* Packet's sequence number */
	int		num;						/* Number of bytes in packet */
	unsigned char buf[MAX_BUF_SIZE]; 	/* Actual packet data */
} PACKET;

static	PACKET	SA_Buf[MAX_SA+1];  		/* Send-ahead buffers */

/*
** Table of control characters that need to be masked
*/

static	char mask_table[] =
{
	0, 0, 0, 1, 0, 1, 0, 0,   			/* NUL SOH SOB ETX EOT ENQ SYN BEL */
	0, 0, 0, 0, 0, 0, 0, 0,				/* BS  HT  LF  VT  FF  CR  SO  SI  */
	1, 1, 0, 1, 0, 1, 0, 0,				/* DLE DC1 DC2 DC3 DC4 NAK ^V  ^W  */
	0, 0, 0, 0, 0, 0, 0, 0				/* CAN ^Y  ^Z  ESC ?   ?   ?   ?   */
};

static	char	hex_digit[] = "0123456789ABCDEF";

static	int	seq_num;					/* Current Sequence Number */
static	int	lchecksm;					/* May hold CRC */
static	int	r_size;						/* size of receiver buffer */
static	unsigned int s_counter,
					 r_counter;
static	int	timed_out;					/* we timed out before receiving */
static	int	cchar;						/* current character */
static	int	masked;						/* TRUE if ctrl character 'masked' */
static	int	packet_received;			/* True if a packet was received */
static	unsigned char r_buffer[MAX_BUF_SIZE];

/*
** Other End's Parameters
*/

static	char	His_WS;					/* Sender's Window Send */
static	char	His_WR;					/* Sender's Window Receive */
static	char	His_BS;					/* Sender's Block Size */
static	char	His_CM;					/* Sender's Check Method */

/*
** Negotiated Parameters
*/

static	char	Our_WS;					/* Negotiated Window Send */
static	char	Our_WR;					/* Negotiated Window Receive */
static	char	Our_BS;					/* Negotiated Block Size */
static	char	Our_CM;					/* Negotiated Check Method */

static	int	Quick_B;					/* True if Quick B in effect */
static	int	Use_CRC;					/* True if CRC in effect */
static	int	buffer_size;				/* Our_BS * 4 */
static	int	SA_Max;						/* 1 if SA not enabled, else MAX_SA */
static	int	SA_Enabled;					/* True if Send-Ahead is permitted  */
static	int	ack_SA;						/* Which SA_Buf is waiting for ACK */
static	int	fill_SA;					/* Which SA_Buf is ready, new data */
static	int	SA_Waiting;					/* Num of SA_Buf's waiting for ACK */
static  int blkct;                      /* block counter for display */
extern  char strbuf[];                  /* defined in qbttl */

static	void do_transport_parameters(void);
static int	send_packet();
static	int	SA_Flush();

/*
** crc
**
** Calculates XMODEM-style CRC (uses the CCITT V.41 polynomial but
** completely backwards from the normal bit ordering).
*/


static	unsigned	crc_table[] =
{
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
	0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
	0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
	0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
	0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
	0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
	0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
	0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
	0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
	0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
	0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
	0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
	0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
	0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
	0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
	0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
	0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
	0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
	0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
	0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
	0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
	0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
	0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
	0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
	0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
	0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
	0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
	0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
	0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
	0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
	0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
	0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};

static	unsigned int	crc_16;

/*
** Upd_CRC updates crc_16 and returns the updated value.
*/

static	unsigned int	upd_CRC (value)
unsigned int	value;
{
	crc_16 = crc_table [((crc_16 >> 8) ^ (value)) & 0xff] ^	(crc_16 << 8);
	return( crc_16 );
}

/*
** Update the checksum/CRC
*/

static	void	do_checksum(c)
int	c;
{
	if (Quick_B && Use_CRC)
		lchecksm = upd_CRC (c);
	else
	{
		lchecksm = lchecksm << 1;

		if (lchecksm > 255)
			lchecksm = (lchecksm & 0xFF) + 1;

		lchecksm = lchecksm + c;

		if (lchecksm > 255)
			lchecksm = (lchecksm & 0xFF) + 1;
	}
}

static	void	send_failure( code )
int	code;
{
	register PACKET	*p;

	ack_SA = 0;
	fill_SA = 0;
	SA_Waiting = 0;

	p = &SA_Buf [0];
	p->buf[0] = 'F';
	p->buf[1] = code;

	if ( send_packet (1))
		SA_Flush();   /* Gotta wait for the host to ACK it */
}

/*
** bp_ENQ is called when the terminal emulator receives the character <ENQ>
** from the host.  Its purpose is to initialize for B Protocol and tell the
** host that we support Quick B.
*/

void	bp_ENQ()
{
	seq_num = 0;
	buffer_size = 511;               /* Set up defaults */
	Quick_B     = FALSE;             /* Not Quick B Protocol */
	Use_CRC     = FALSE;             /* Not CRC_16      */
	SA_Enabled  = FALSE;             /* No Send-Ahead by us */
	SA_Max      = 1;                 /* = single packet sent */

	lc_put (port, DLE);
	lc_put (port, '+');

	lc_put (port, DLE);
	lc_put (port, '0');
}

/*
** bp_ESC_I is called when <ESC><I> is received by the terminal emulator.
** Note that Quick B allows +XX to be added to the end of the response, where
** XX is the two hex digits of the standard B Protocol checksum of the
** preceeding characters in the response.  The Qbttl program also supports
** standard VIDTEX cursor control.
*/
static	char	esc_I_response[] = "#VCO,CC,PB,DT,+";

void	bp_ESC_I()
{
	int	save_Use_CRC;
	register int	i;

	save_Use_CRC = Use_CRC;
	Use_CRC = FALSE;
	lchecksm = 0;

	i = 0;
	while( esc_I_response[i] )
	{
		lc_put(port, esc_I_response [i]);
		do_checksum (esc_I_response [i]);
		i++;
	}

/*
** Append two hex digits of checksum to response
*/

	lc_put(port,  hex_digit[ (lchecksm >> 4) & 0x0F ]);
	lc_put(port,  hex_digit[ lchecksm & 0x0F ] );
	lc_put(port, CR);

	Use_CRC = save_Use_CRC;
}


static	void	send_masked_byte(c)
int	c;
{
	c = c & 0xFF;

	if (c < 32)
	{
		if (mask_table [c] != 0)
		{
			lc_put(port, DLE);
			lc_put(port, c + '@');
		}
		else
			lc_put(port, c);
	}
	else
		lc_put(port, c);

	s_counter = (s_counter + 1) % 512;
}

static	void	send_ack()
{
	lc_put(port, DLE);
	lc_put(port, seq_num + '0');
}


static	int	read_byte()
{
	timed_out = FALSE;

	cchar = wait(port, 10);

	if (cchar < 0 )
		return( FALSE );

	r_counter = (r_counter + 1) % 512;
	return( TRUE );
}


static	int	read_masked_byte()
{
	masked = FALSE;

	if (read_byte() == FALSE)
		return( FALSE );

	if (cchar == DLE)
	{
		if (read_byte() == FALSE)
			return( FALSE );
		cchar &= 0x1F;
		masked = TRUE;
	}

	return( TRUE );
}

static	int	read_packet (lead_in_seen, from_send_packet)
int	lead_in_seen, from_send_packet;
/*
** Lead_in_Seen is TRUE if the <DLE><B> has been seen already.
** from_send_packet is TRUE if called from Send_Packet
**	(causes exit on first error detected)
**
** Returns True if packet is available from host.
*/
{
	int	State, next_seq, block_num, errors, new_cks;
	int	i;

	packet_received = FALSE;
	for(i=0; i<buffer_size; i++ )
		r_buffer[i] = 0;
	next_seq = (seq_num +  1) % 10;
	errors = 0;

	if (lead_in_seen)		/* Start off on the correct foot */
		State = R_GET_SEQ;
	else
		State = R_GET_DLE;

	while (TRUE)
	{
		switch  (State)
		{
			case R_GET_DLE :
				if (_abort_flag)
				{
					send_failure ('A');
					return( FALSE );
				}

				if (!read_byte())
	            	State = R_TIMED_OUT;
				else
					if ((cchar & 0x7F) == DLE)
	        	    	State = R_GET_B;
					else
						if ((cchar & 0x7F) == ENQ)
        					State = R_SEND_ACK;
				break;

			case R_GET_B :
				if (!read_byte())
					State = R_TIMED_OUT;
				else
					if ((cchar & 0x7F) == 'B')
						State = R_GET_SEQ;
					else
						if (cchar == ENQ)
							State = R_SEND_ACK;
						else
							State = R_GET_DLE;
				break;

			case R_GET_SEQ :
				if (!read_byte())
					State = R_TIMED_OUT;
				else
					if (cchar == ENQ)
						State = R_SEND_ACK;
					else
					{
						if (Quick_B && Use_CRC)
							lchecksm = crc_16 = -1;
						else
							lchecksm = 0;

						block_num = cchar - '0';

						do_checksum(cchar);

						i = 0;
						State = R_GET_DATA;
					}
				break;

			case R_GET_DATA :
				r_counter = 0;
				if (!read_masked_byte())
					State = R_TIMED_OUT;
				else
					if ((cchar == ETX) && !masked)
					{
						do_checksum(ETX);
						State = R_GET_CHECKSUM;
					}
					else
					{
						r_buffer[i] = cchar;
						i = i + 1;
						do_checksum(cchar);
					}
				break;

			case R_GET_CHECKSUM :
				if (!read_masked_byte())
					State = R_TIMED_OUT;
				else
				{
					if (Quick_B && Use_CRC)
					{
						lchecksm = upd_CRC (cchar);

						if (!read_masked_byte())
							new_cks = lchecksm ^ 0xFF;
						else
						{
							lchecksm = upd_CRC (cchar);
							new_cks = 0;
						}
					}
					else
						new_cks = cchar;

					if (new_cks != lchecksm)
						State = R_TIMED_OUT;
					else
						if (r_buffer[0] == 'F') /* Watch for Failure Packet */
							State = R_SUCCESS;  /* which is always accepted */
						else
							if (block_num == seq_num) /* Watch for dup block */
								State = R_SEND_ACK;
							else
								if (block_num != next_seq)
									State = R_TIMED_OUT; /* Bad sequence number */
								else
									State = R_SUCCESS;
				}
				break;

			case R_TIMED_OUT :
				errors = errors + 1;

				if ((errors > MAX_ERRORS) || (from_send_packet))
					return( FALSE );

				lc_put(port, NAK);

				if (from_send_packet)
					return( FALSE );

				State = R_GET_DLE;
				break;

			case R_SEND_ACK :
				send_ack();
				State = R_GET_DLE;        /* wait for the next block */
				break;

			case R_SUCCESS :
				seq_num = block_num;
				r_size = i;
				packet_received = TRUE;
				return( TRUE );
		}
	}
}

static	void	send_data (Buffer_Number)
int	Buffer_Number;
{
	int	i;
	register PACKET	*p;

	s_counter = 0;
	p = &SA_Buf [Buffer_Number];
	if (Quick_B && Use_CRC)
		lchecksm = crc_16 = -1;
	else
		lchecksm = 0;

	lc_put(port, DLE);
	lc_put(port, 'B');

	lc_put(port, p->seq + '0');
    do_checksum(p->seq + '0');

	for (i = 0; i<=p->num; i++ )
	{
		send_masked_byte(p->buf[i]);
		do_checksum(p->buf[i]);
	}

	lc_put(port, ETX);
	do_checksum (ETX);

	if (Quick_B && Use_CRC)
		send_masked_byte (lchecksm >> 8);

	send_masked_byte(lchecksm);
}

/*
** ReSync is called to restablish syncronism with the remote.  This is
** accomplished by sending <ENQ><ENQ> and waiting for the sequence
** <DLE><d><DLE><d> to be received, ignoring everything else.
**
** Return is -1 on time out, else the digit <d>.
*/
#define	GET_FIRST_DLE		1
#define	GET_FIRST_DIGIT		2
#define	GET_SECOND_DLE		3
#define	GET_SECOND_DIGIT	4

static	int	ReSync()
{
	int	State, Digit_1;

	lc_put(port, ENQ);     /* Send <ENQ><ENQ> */
	lc_put(port, ENQ);
	State = GET_FIRST_DLE;

	while(1)
	{
		switch (State)
		{
			case GET_FIRST_DLE :
				if( !read_byte() )
					return( -1 );
				if( cchar == DLE )
					State = GET_FIRST_DIGIT;
				break;
			case GET_FIRST_DIGIT :
				if( !read_byte() )
					return( -1 );
				if( (cchar >= '0') && (cchar <= '9') )
				{
					Digit_1 = cchar;
					State = GET_SECOND_DLE;
				}
				break;
			case GET_SECOND_DLE :
				if( !read_byte() )
					return( -1 );
				if( cchar == DLE )
					State = GET_SECOND_DIGIT;
				break;
			case GET_SECOND_DIGIT :
				if( !read_byte() )
					return( -1 );
				if( (cchar >= '0') && (cchar <= '9') )
				{
					if( Digit_1 == cchar )
						return( cchar );
					else
					{
						Digit_1 = cchar;
						State = GET_SECOND_DLE;
					}
				}
				else
					State = GET_SECOND_DLE;
				break;
		}
	}
}

/*
** get_ACK is called to wait until the SA_Buf indicated by ack_SA
** has been ACKed by the host.
*/
static	int	get_ACK()
{
	int	State, errors, block_num, i;
	int	Sent_ENQ;
	int	SA_Index;

	packet_received = FALSE;
	errors = 0;
	Sent_ENQ = FALSE;
	State = S_GET_DLE;

	while( TRUE )
	{
		switch (State) {
		case S_GET_DLE :
			if (_abort_flag)
			{
				send_failure ('A');
				return( FALSE );
			}

			if (!read_byte())
				State = S_TIMED_OUT;
			else if (cchar == DLE)
				State = S_GET_NUM;
			else if (cchar == NAK)
			{
				if (++errors > MAX_ERRORS)
					return( FALSE );
				State = S_SEND_ENQ;
			}
			else if (cchar == ETX)
				State = S_SEND_NAK;
			break;

		case S_GET_NUM :
			if (!read_byte())
				State = S_TIMED_OUT;
			else if ((cchar >= '0') && (cchar <= '9'))
				State = S_HAVE_ACK;	/* Received ACK */
			else if (cchar == 'B')
				State = S_GET_PACKET; /* Try to get packet */
			else if (cchar == NAK)
			{
				if (++errors > MAX_ERRORS)
					return( FALSE );
				State = S_SEND_ENQ;
			}
			else
				State = S_TIMED_OUT;
			break;

		case S_GET_PACKET :
			if (read_packet (TRUE, TRUE))
			{
				if (r_buffer [0] == 'F')
				{
					send_ack();
					return( FALSE );
				}
				else
					return( TRUE );
			}

			State = S_TIMED_OUT; /* On a bad receive, try again */
			break;
		case S_HAVE_ACK:
			block_num = cchar - '0';
			if (SA_Buf [ack_SA].seq == block_num)
			{	/* This is the one we're waiting for */
				ack_SA = incr_SA(ack_SA);
				SA_Waiting--;
				return( TRUE );
			}
			else
				if (SA_Buf [incr_SA (ack_SA)].seq == block_num)
				{	/* Must have missed an ACK */
					ack_SA = incr_SA (ack_SA);
					ack_SA = incr_SA (ack_SA);
					SA_Waiting -= 2;
					return( TRUE );
				}
				else 
					if (SA_Buf [ack_SA].seq == incr_seq (block_num))
					{
						if( Sent_ENQ )
							State = S_SEND_DATA;
						else
							State = S_GET_DLE;
					}
					else
						State = S_TIMED_OUT;
			Sent_ENQ = FALSE;
			break;
		case S_TIMED_OUT :
			if (++errors > MAX_ERRORS)
				return( FALSE );

			State = S_SEND_ENQ;
			break;

		case S_SEND_NAK :
			if (++errors > MAX_ERRORS)
				return( FALSE );

			lc_put(port, NAK);

			State = S_GET_DLE;
			break;

		case S_SEND_ENQ :
			if (++errors > MAX_ERRORS)
				return( FALSE );
			cchar = ReSync();
			if( cchar == -1 )
				State = S_SEND_ENQ;
			else
				State = S_HAVE_ACK;
			Sent_ENQ = TRUE;
			break;

		case S_SEND_DATA :
			SA_Index = ack_SA;

			for (i = 1; i<=SA_Waiting; i++ )
			{
				send_data (SA_Index);
				SA_Index = incr_SA (SA_Index);
			}

			State = S_GET_DLE;
			Sent_ENQ = FALSE;
			break;
		}
	}
} /* get_ACK */

static int	send_packet (size)
int	size;
{
	if (SA_Waiting == SA_Max)
		if (!get_ACK())
			return( FALSE );

	seq_num = incr_seq (seq_num);
	SA_Buf [fill_SA].seq = seq_num;
	SA_Buf [fill_SA].num = size;
	send_data (fill_SA);
	fill_SA = incr_SA (fill_SA);
	SA_Waiting = SA_Waiting + 1;
	return( TRUE );
}
/*
** SA_Flush is called after sending the last packet to get host's
** ACKs on outstanding packets.
*/
static	int	SA_Flush()
{
	while( SA_Waiting != 0 )
		if (!get_ACK())
			return( FALSE );
	return( TRUE );
}

/* Send_File is called to send a file to the host */
static	int	send_file(name)
char	name[];
{
	int	fd;
	int	n;
	register PACKET	*p;

	fd = open(name, (O_BINARY | O_RDONLY));

	if (fd < 0)
  	{
		send_failure('E');
		urgentmsg ("ERROR","** Cannot find that file **");
		return( FALSE );
	}

	do
	{
		p = &SA_Buf [fill_SA];
		p->buf[0] = 'N';
		n = read(fd, &p->buf[1], buffer_size);

		if (n > 0)
		{
			if (send_packet (n) == FALSE)
				return( FALSE );
			sprintf(strbuf, "Sent Block: %d", blkct++);
			atsay(4,1,strbuf);
		}
	} while( n == buffer_size );

	close (fd);

	if (n < 0)
	{
		send_failure ('E');
		urgentmsg ("ERROR", "** Read failure...aborting **");
		return(FALSE);
	}

/* Inform host that the file was sent */
	p = &SA_Buf [fill_SA];
	p->buf[0] = 'T';
	p->buf[1] = 'C';

	if (send_packet(2) == FALSE)
		return( FALSE );
	else
	{
		say( "Waiting for host..." );
		if (!SA_Flush())
			return( FALSE );
		return( TRUE );
	}
}

/*
** do_transport_parameters is called when a Packet type of + is received.
** It sends a packet of our local Quick B parameters and sets the Our_xx
** parameters to the minimum of the sender's and our own parameters.
*/
static	void do_transport_parameters()
{
	register PACKET	*p;

	His_WS = r_buffer [1];     /* Pick out Sender's parameters */
	His_WR = r_buffer [2];
	His_BS = r_buffer [3];
	His_CM = r_buffer [4];

	p = &SA_Buf [fill_SA];
	p->buf [0] = '+';  /* Prepare to return our own parameters */
	p->buf [1] = DEF_WS;
	p->buf [2] = DEF_WR;
	p->buf [3] = DEF_BS;
	p->buf [4] = DEF_CM;
	p->buf [5] = DEF_DQ;

	if (!send_packet (5))
		return;

	if (SA_Flush())                 /* Wait for host's ACK on our packet */
	{
/* Take minimal subset of Transport Params. */
/* If he can send ahead, we can receive it. */
		Our_WR = (His_WS < DEF_WR) ? His_WS : DEF_WR;

/* If he can receive send ahead, we can send it. */
		Our_WS = (His_WR < DEF_WS) ? His_WR : DEF_WS;

		Our_BS = His_BS < DEF_BS ? His_BS : DEF_BS;

		Our_CM = His_CM < DEF_CM ? His_CM : DEF_CM;

		if (Our_BS == 0)
			Our_BS = 4;    /* Default */

		buffer_size = Our_BS * 128;

		Quick_B = TRUE;

		if (Our_CM == 1)
			Use_CRC = TRUE;

		if (Our_WS != 0)
		{
			SA_Enabled = TRUE;
			SA_Max     = MAX_SA;
		}
	}
}

/*
  do_application_parameters is called when a ? packet is received.
  This version ignores the host's packet and returns a ? packet
  saying that normal B Protocol File Transfer is supported.
  (Well, actually it says that no extended application packets are
   supported.  The T packet is assumed to be standard.) */

static	void	do_application_parameters()
{
	register PACKET	*p;

	p = &SA_Buf [fill_SA];
	p->buf[0] = '?';     /* Build the ? packet */
	p->buf[1] = 1;             /* The T packet flag  */

	if (send_packet (1))            /* Send the packet    */
		SA_Flush();
}

/* Receive_File is called to receive a file from the host */
static	int	receive_file (name)
char	name[];
{
	int	fd;
	unsigned  bytes;

	_fmode = O_BINARY;
	fd = creat(name,(S_IREAD | S_IWRITE) );

	if (fd < 0)
	{
		urgentmsg ("ERROR", "** Cannot open file...aborting **");
		send_failure('E');
		return( FALSE );
	}

	send_ack();

/* Process each incoming packet until 'TC' packet received or failure */

	while( TRUE )
	{
		if (read_packet (FALSE, FALSE))
		{
			switch (r_buffer[0]) {
			case 'N' :
				bytes = r_size - 1;

				if (write(fd, &r_buffer[1], bytes) != bytes )
				{
					urgentmsg ("ERROR", "** Write failure...aborting **");
					close (fd);
					send_failure ('E');
					return( FALSE );
				}
				send_ack();
				sprintf(strbuf, "Received Block: %d", blkct++);
				atsay(4,1,strbuf);
				break;

			case 'T' :
				if (r_buffer[1] == 'C')
				{
					close(fd);

					send_ack();
					return( TRUE );
				}
				else
				{
					urgentmsg ("ERROR", "** Invalid termination packet...aborting **");
					close (fd);
					send_failure ('N');
					return( FALSE );
				}

			case 'F' :
				send_ack();
				urgentmsg ("ERROR","** Failure packet received...aborting **");
				close (fd);
				return( FALSE );
			}
		}
	 	else
		{
			urgentmsg ("ERROR", "** Failed to receive packet...aborting **");
			close (fd);
			return( FALSE );
		}
	}
}

/*
** bp_DLE is called from the main program when the character <DLE> is
** received from the host.
**
** This routine calls read_packet and dispatches to the appropriate
** handler for the incoming packet.
*/
void	bp_DLE()
{
	int	i;
	char	filename[255];
	char	str[2];
/*
** Begin by getting the next character.  If it is <B> then enter the
** B_Protocol state.  Otherwise simply return.
*/

	if (wait (port, 10) != 'B')
		return;

	strcpy( str, " " );

	ack_SA  = 0;    /* Initialize Send-ahead variables */
	fill_SA = 0;
	SA_Waiting      = 0;
	blkct = 0;
	erase();                    /* clear the window */

/*  <DLE><B> received; begin B Protocol */

	r_counter   = 0;
	s_counter   = 0;

	if (Quick_B)
	{
		say ("*** Quick B is in effect ***\r");

		if (Use_CRC)
			say ("*** Using CRC ***\r");

		if (Our_WS != 0) /* Allow send-ahead if other end agrees */
			say ("*** Send-Ahead enabled ***\r");
	}

	if (read_packet (TRUE, FALSE))
	{
/* Dispatch on the type of packet just received */

	        switch (r_buffer[0]) {
		case 'T':     /* File Transfer Application */
			switch (r_buffer[1]) {
			case 'D' :	/* downloading */
				break;
			case 'U' :	/* uploading */
				break;
			default :
				send_failure('N');
				return;
			}

			switch (r_buffer[2]) {
			case 'A':	/* ascii file */
				break;
			case 'B':	/* binary file */
				break;
			default :
				send_failure('N');        /* not implemented */
				return;
			}

			i = 2;
			strcpy( filename, "" );

			while( (r_buffer[i] != 0) && (i < r_size) )
			{
				i = i + 1;
				str[0] = r_buffer[i];
				strcat( filename, str );
			}

			if (r_buffer[1] == 'U')
			{
				if( send_file(filename) )
					urgentmsg("SUCCESS", "Transfer completed!" );
			}
			else
			{
				if( receive_file(filename) )
					urgentmsg("SUCCESS", "Transfer completed!" );
			}
			break;

		case '+':          /* Received Transport Parameters Packet */
			do_transport_parameters();
			break;

		case '?':          /* Received Application Parameters Packet */
			do_application_parameters();
			break;

		default:	/* Unknown packet; tell host we don't know */
			send_failure ('N');
            		break;

		}  /* of case */
	}     /* of if read_packet the */
}

/*****************************************************************************
 *				   ibmcom.h				     *
 *****************************************************************************
 * DESCRIPTION:	ANSI C function prototypes and other definitions for the     *
 *		routines in ibmcom.c					     *
 *									     *
 * REVISIONS:	18 OCT 89 - RAC - Original code.			     *
 *****************************************************************************/

int		com_carrier(void);
void		com_deinstall(void);
void		com_flush_rx(void);
void		com_flush_tx(void);
int		com_install(int portnum);
void interrupt	com_interrupt_driver();
void		com_lower_dtr(void);
void		com_raise_dtr(void);
char		com_rx(void);
int		com_rx_empty(void);
void		com_set_parity(enum par_code parity, int stop_bits);
void		com_set_speed(unsigned speed);
void		com_tx(char c);
int		com_tx_empty(void);
int		com_tx_ready(void);
void		com_tx_string(char *s);

enum par_code	{ COM_NONE, COM_EVEN, COM_ODD, COM_ZERO, COM_ONE };

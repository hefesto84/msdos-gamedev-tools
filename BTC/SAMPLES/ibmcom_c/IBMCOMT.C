/*****************************************************************************
 *                                 ibmcomt.c                                 *
 *****************************************************************************
 * DESCRIPTION: This is a simple test program for IBMCOM.  It acts like a    *
 *              dump terminal at a fixed speed with no commands except Alt-X *
 *              to exit the program.  Obviously it doesn't test all of       *
 *              all of IBMCOM, but it tests the most important parts --      *
 *              receiving and sending characters.                            *
 *                                                                           *
 * REVISIONS:   18 OCT 89 - RAC - Translated from the Pascal.                *
 *****************************************************************************/

#include        <stdio.h>
#include        "ibmcom.h"

#define PORT    2
#define SPEED   2400

get_key() {
    int         c;
    if (c = getch()) return c;
    else return getch() + 256;
    }

main() {

    int herb;

    if (herb = com_install(PORT)) {
        printf("com_install() error: %d\n", herb);
        return;
        }
    com_raise_dtr();
    com_set_speed(SPEED);
    com_set_parity(COM_NONE, 1);
    clrscr();
    printf("IBMCOM Test\n");
    while (1) {
        if (kbhit()) {
            herb = get_key();
            if (herb == 301 /* Alt-X */) {
                com_deinstall();
                exit();
                }
            com_tx(herb);
            }
        if (herb = com_rx())
            putchar(herb);
        }
    }

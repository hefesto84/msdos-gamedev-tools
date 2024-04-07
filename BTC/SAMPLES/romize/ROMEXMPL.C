/* ROMEXMPL.C   - Example ROMable Turbo C program */

#include <dos.h>


#define  portin         0x300
#define  portout        0x301

/* Here are examples of how to re-create some of
    the library string functions */

void strmv(char *dest, char *source) { /* like copy, but does not move
                                         terminating null */
  while (*source) *dest++ = *source++;
}

int strlen(char *source) {
  int i;
  i = 0;
  while (*source++) i++;
  return (i);
}

void strcpy(char *dest, char *source) {
  while (*source) *dest++ = *source++;
  *dest = *source;  /* move terminator */
}

void send_mess(char *p) {

  while (*p) outportb(portout,*p++);
}

void do_stuff() {

  static char *menu[] = { "Choice A", "Choice B", "Choice C"};
  int i;

  i = inportb(portin);
  send_mess(menu[i]);
}

void main(void) {
  do {
    do_stuff();
  } while (1);

  /* don't return from main, there ain't no place to go */

}

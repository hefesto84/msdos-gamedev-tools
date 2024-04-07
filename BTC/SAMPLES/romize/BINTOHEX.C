/* Binary image to Intel HEX format */

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <dos.h>
#include <alloc.h>

#define true 1
#define false 0
#define romsize 32768
#define lineincr  32     /* # bytes/line */

  unsigned char buffer[romsize];
  FILE  *output;

  int binhandle;

int convert(void)
{
  unsigned i;
  int k;
  unsigned char *j;
  unsigned cksum;
  unsigned bytesread;

  bytesread = _read(binhandle,buffer,romsize);
  for (i = 0; i < bytesread; i+=lineincr) {
    j = &buffer[i];
    fprintf(output,":%02X%04X00",lineincr,i);
    cksum = lineincr + (i >>8) + (i & 0xff);
    for (k = 0; k < lineincr; k++,j++) {
      cksum += *j;
      fprintf(output,"%02X",*j);
    }
    fprintf(output,"%02X\n",-(cksum & 0xff));
  }
  fprintf(output,":0000000000\n");
  return(0);
}


int main(int argc, char *argv[])
{
  printf("Binary image to Intel HEX format\n");

  if (argc < 3) {
    printf("Usage:  bintohex input.bin output.hex \n");
    return (1);
  }

  binhandle = _open(argv[1],O_RDONLY|O_BINARY);
  if (binhandle < 0) {
    printf("Trouble opening input file\n");
    return(1);
  }

  output = fopen(argv[2],"wt");
  if (!output) {
    printf("Trouble opening output\n");
    return(1);
  }
  convert();


  close(binhandle);
  fclose(output);
  return(0);
}
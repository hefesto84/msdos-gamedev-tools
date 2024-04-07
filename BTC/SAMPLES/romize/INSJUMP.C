/* INSJUMP.C Program to put the restart jump
  and a checksum  in the ROM   */

#include <stdio.h>
#include <mem.h>
#include <fcntl.h>
#include <string.h>

  typedef unsigned char byte;

  byte  Jump[] = "\xEA\0\0\0\xF8";    /* JMP FAR F800:0  for 32K */

  byte  IOBuffer[4096];

  int       BinFileIn;
  int       BinFileOut;

  unsigned  Actual;
  unsigned  i;
  unsigned  long total_bytes;
  unsigned  long rom_size;
  int       bytes_read;
  unsigned  cksum;

void main(int argc, char *argv[])
{

  if (argc < 2) {
    printf("Usage: INSJUMP filename [64K]\n");
    exit(1);
  }
  if (!stricmp(argv[2],"64K")) {
    rom_size = 65536;
    printf("Processing a 64K ROM\n");
    Jump[4] = 0xF0;    /* change jump offset for 64K */
  }
  else {
    rom_size = 32768;
    printf("Processing a 32K ROM\n");
  }

  if((BinFileIn = open(argv[1],O_BINARY | O_RDONLY)) < 0) {
    printf("Couldn't open %s \n",argv[1]);
    exit(1);
  }

  if((BinFileOut = _creat("TEMP.BIN",0)) < 0 ) {
    printf("Couldn't open temp file for rewriting\n",argv[1]);
    exit(1);
  }

  total_bytes = 0;
  cksum = 0;

  do {
    if (!eof(BinFileIn)) {
      bytes_read = read(BinFileIn,IOBuffer,sizeof(IOBuffer));
      /* printf("%d  bytes read\n",bytes_read); */
    }
    else bytes_read = 0;

    if (bytes_read != sizeof(IOBuffer))  /* fill unused with FF's  */
      memset(&IOBuffer[bytes_read],0xFF,sizeof(IOBuffer)-bytes_read);

    total_bytes += sizeof(IOBuffer);
    if (total_bytes == rom_size) {  /* last block -- put jump and cksum */
      memcpy(&IOBuffer[sizeof(IOBuffer)-16],&Jump[0],5);  /* put in the jump */
      for (i = 0; i < sizeof(IOBuffer)-2; i += 2)  /* cksum all but last word */
        cksum += cksum + (unsigned) IOBuffer[i];
      cksum -= 0x10000;
      IOBuffer[sizeof(IOBuffer)-2] = cksum & 0xFF;
      IOBuffer[sizeof(IOBuffer)-1] = cksum >> 8;
    }
    else {
      for (i = 0; i < sizeof(IOBuffer); i += 2)
        cksum += cksum + (unsigned) IOBuffer[i];
    }
    write(BinFileOut,IOBuffer,sizeof(IOBuffer));
  } while (total_bytes != rom_size);

  close(BinFileIn);
  close(BinFileOut);

  unlink(argv[1]);
  rename("TEMP.BIN",argv[1]);


}


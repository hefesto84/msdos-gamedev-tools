romexmpl.bin: c0romx.obj romexmpl.obj
  \turboc\tlink /c/m c0romx romexmpl,romexmpl, romexmpl
  exe2bin romexmpl
  del romexmpl.exe
  insjump romexmpl.bin

romexmpl.obj: romexmpl.c
  tcc -mc -S -O -Z -a romexmpl
  romize romexmpl.asm romexmpr.asm
  masm romexmpr/mx/T/n,romexmpl;

c0romx.obj: c0romx.asm
  masm c0romx/mx/t/n,;



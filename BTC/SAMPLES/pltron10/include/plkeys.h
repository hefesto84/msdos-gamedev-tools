
/*
  plkeys.h. Include file for the low level keyboard handler.

  Copyright 1997 Liouros Thanasis.

  This file is part of PLATFORMTRON game library. See the file license.txt
  for more details. If that file is missing then please contact me.

*/

#ifndef plkeys_h
#define plkeys_h

char pl_installkeys();
char pl_keysdone();
char pl_testkey(unsigned char makecode);

#define          mcESC               01
#define          mc1                 02
#define          mc2                 03
#define          mc3                 04
#define          mc4                 05
#define          mc5                 06
#define          mc6                 07
#define          mc7                 08
#define          mc8                 09
#define          mc9                 10
#define          mc0                 11
#define          mcMINUS             12
#define          mcPLUS              13
#define          mcBACKSPACE         14
#define          mcTAB               15
#define          mcQ                 16
#define          mcW                 17
#define          mcE                 18
#define          mcR                 19
#define          mcT                 20
#define          mcY                 21
#define          mcU                 22
#define          mcI                 23
#define          mcO                 24
#define          mcP                 25
#define          mcLBRACKET          26
#define          mcRBRACKET          27
#define          mcENTER             28
#define          mcCTRL              29
#define          mcA                 30
#define          mcS                 31
#define          mcD                 32
#define          mcF                 33
#define          mcG                 34
#define          mcH                 35
#define          mcJ                 36
#define          mcK                 37
#define          mcL                 38
#define          mcSEMICOLON         39
#define          mcQUOTES            40
#define          mcTILDE             41
#define          mcLSHIFT            42
#define          mcBACKSLASH         43
#define          mcZ                 44
#define          mcX                 45
#define          mcC                 46
#define          mcV                 47
#define          mcB                 48
#define          mcN                 49
#define          mcM                 50
#define          mcCOMMA             51
#define          mcDOT               52
#define          mcQMARK             53
#define          mcRSHIFT            54
   // PRTsc
#define          mcALT               56
#define          mcSPACE             57
#define          mcCAPSLOCK          58
#define          mcF1                59
#define          mcF2                60
#define          mcF3                61
#define          mcF4                62
#define          mcF5                63
#define          mcF6                64
#define          mcF7                65
#define          mcF8                66
#define          mcF9                67
#define          mcF10               68
#define          mcNUMLOCK           69
#define          mcSCROLLLOCK        70
#define          mcNUM7              71
#define          mcNUM8              72
#define          mcNUM9              73
#define          mcNUMMINUS          74
#define          mcNUM4              75
#define          mcNUM5              76
#define          mcNUM6              77
#define          mcNUMPLUS           78
#define          mcNUM1              79
#define          mcNUM2              80
#define          mcNUM3              81
#define          mcNUM0              82
#define          mcNUMDOT            83
// ALT-PRTsc
#define          mcF11               87
#define          mcF12               88

#endif

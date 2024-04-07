
/*     D‚finitions diverses pour les Entr‚es-Sorties */


/* Constant defines */


#define EFOPENR		-200
#define EFOPENW		-201


/* Useful macro */

#define SYSMSG(x, fp ) werrmsg(__FILE__, __DATE__, __TIME__, (x), (fp))


/*	Now the provided routines     */
/*	C-C++ compatibility hack      */

#ifdef __cplusplus
extern "C" {
#endif


/*	void werrmsg(char *orgid, char *date, char *time,
		int code,
		FILE *fp);

	This  function   ouputs  an   error  or  warning  message
	associated to a predefined code:

	Input parameters:
	----------------
	orgid : Message indicating  the posistion  of the  call  to
		werrmsg.(usually __FILE__ or __LINE__ or a combination of
		the two).
	date  : Date of  the  last  release  of  the  file  (usually
		__DATE__).
	time  : time of  the  last  release  of  the  file  (usually
		__TIME__).
	code  : Error code returned by the last Anum called function.
	fp    : Stream, the output should be performed to which.

	Note:
	-----
	If code  is zero,  no message  is output. You can
		then insert a call to werrmsg after any function call
		to anum returning an error code at debugging stage.
	The SYSMSG macro defined  above helps programmers to automate
		calls  to standard ANUM error and warning messages.
*/
  void werrmsg(char *orgid, char *date, char *time,
		int code, FILE *fp);


/*	void ena_wrn(void)

	This function enables the generation of warning messages.
*/
   void ena_wrn(void);

/*	void disa_wrn(void)

	This  function   disables  the   generation  of   warning
	messages.
*/
  void disa_wrn(void);



/*	void ena_err(void)

	This function enables the generation of error messages.
*/
  void ena_err(void);




/*	void disa_err(void)

	This function disables the generation of error messages.
*/
  void disa_err(void);



/*	void ena_m_beep(void)

	This function  enables the  generation of an audible tone
	while printing error or warning messages.
*/
  void ena_m_beep(void);



/*	void disa_m_beep(void)

	This function  disables the generation of an audible tone
	while printing error or warning messages.
*/
  void disa_m_beep(void);


/*
	void beep (void)

	This function outputs an audible tone to the speaker provided
	it is allowed by ena_m_beep
*/
  void beep(void);


/*	void errmsg(char *orgid, char *date, char *time,
		int code,
		char *msg
		FILE *fp)

	This function outputs user defined error messages.

	Input parameters:
	----------------
	orgid	: origin  identifier  (usually  the  caller  __FILE__
		  pseudo variable)
	date 	: date (usually the caller __DATE__ pseudo variable)
	time	: time (usually the caller __TIME__ pseudo variable)
	code	: error code
	msg	: explanatory message to be printed
	fp	: file stream descriptor on which output must be
		  performed

	Note:
	----
	If the code is zero (ENOERROR) no message. You can then insert a
		call to werrmsg after any function which returns
		an error code.
	The SYSMSG macro - defined in this file - helps programmers to
		automate standard error and warning messages generation.
*/
  void errmsg(char *, char *, char *,
	int ,
	char *,
	FILE *);


/*  void wrnmsg(char *orgid, char *date, char *time,
		int code,
		char *msg,
		FILE *fp)

	This function outputs user defined warning messages.

	Input parameters:
	----------------
	orgid : origin identifier (usually the caller __FILE__
		pseudo variable)
	date  : date (usually the caller __DATE__ pseudo variable)
	time  : time (usually the caller __TIME__ pseudo variable)
	code  : error code
	msg   : explanatory message to be printed
	fp    : file stream descriptor on which output must be
		performed
*/
 void wrnmsg(char *, char *, char *,
    int,
    char *,
    FILE *);


#ifdef __cplusplus
}
#endif
char *strstr (keystr, targetstr)
char *keystr, *targetstr;

/*
  Features of Operation:

  -  If keystr or targetstr is NULL, a NULL pointer (failure) is returned
     (i.e., NULL pointer input is handled gracefully).

  -  If keystr or targetstr contains only a NULL character, a NULL pointer
     (failure) is returned (i.e., NULL's are not considered characters for
     the purpose of this function -- they are considered string terminators).

  -  The search terminates successfully when '\0' of keystr is encountered
     before (or at the same time) as '\0' of targetstr.  The search terminates
     unsuccessfully when '\0' of targetstr is reached before '\0' of keystr.
*/

{
  register unsigned short i;

  if (keystr == (char *) 0 || targetstr == (char *) 0)
    return ((char *) 0);

  if (*keystr == '\0' || *targetstr == '\0')
    return ((char *) 0);

  do {
    for (i = 0; *(keystr+i) && (*(keystr+i) == *(targetstr+i)); i++);
    if ( !(*(keystr+i)) )
      return (targetstr);
  } while (*(targetstr++));

  return ((char *) 0);

}

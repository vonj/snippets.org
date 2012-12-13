/*
 /-------------------------------------\
|     TIME_MAC_CONV                     |------------------------------------|
|\-------------------------------------/
|
| This routine converts the macro  __TIME__ to standard format
|
| Example:   "11:50:21" -> "115021"
|
|----------------------------------------------------------------------------|
| CALL:
|    strcpy(string, time_mac_conv(string) );
|
| HEADER:
|    string.h    : strcpy
|    stdio.h     : sscanf, sprintf
|
| GLOBALE VARIABLES:
|    %
|
| ARGUMENTS:
|    pszTime      : String with time in  __TIME__ format (HH:MM:SS)
|
| PROTOTYPE:
|    char far *time_mac_conv(char *pszTime);
|
| RETURN VALUE:
|    char szStr   : Time in format HHMMSS
|
| MODULE:
|    time__.c
|----------------------------------------------------------------------------|
|
|
|----------------------------------------------------------------------------|
|1992-09-16/Erik Bachmann
\---------------------------------------------------------------------------|*/

char _CfnTYPE *time_mac_conv(char *pszTime)
{
      char  szStr[12];                                /* Convertion string */
      char  hh[3],                                    /* Hour     */
            mm[3],                                    /* Minutes  */
            ss[3];                                    /* Seconds  */

      /*----------------------------------------------*/

      strcpy(szStr, pszTime);                         /* Copy string */

      sscanf(szStr, "%2s %*c %2s %*c %2s", hh, mm, ss);

      /* Split the string into basics */

      sprintf(szStr, "%s%s%s", hh,mm,ss);

      /* Assemble new string */

      return((char *) szStr);                         /* Return new string */
}

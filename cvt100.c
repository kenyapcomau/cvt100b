/* CVT100 Main module */
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#ifndef	__BCC__
#include <conio.h>
#endif

#include "cvt100.h"

/****************************************************************************/
/* Function prototypes                                                      */

int  main( void );
void CVT100Init( void );
void terminal( void );
void badexit( char * );

/****************************************************************************/
/* Global Data                                                              */

char modeline[] =
#ifdef	NODOS
#ifdef	USECMOS
"F5-Communications  F6-Video  F7-Keyboard  F8-VT emulation  F9-Setup";
#else
"F5-Communications  F6-Video  F7-Keyboard  F8-VT emulation";
#endif
char idstring[] = ID;
#else
"F5-Communications  F6-Video  F7-Keyboard  F8-VT emulation  F9-File     F10-Exit";
#endif

/****************************************************************************/
/* Local Static Data                                                        */

#ifndef	NODOS
static char id[] = "$Id: cvt100.c,v 1.2 1998/07/07 11:25:46 ken Exp $";
#endif

/****************************************************************************/
/****************************************************************************/

/* M A I N -- Main function for emulator */

main()
{

   CVT100Init();                  /* Initialization */
   ttopen();                      /* Open selected com port */
   terminal();                    /* Emulate the terminal for awhile */
   ttclose();                     /* Close the communications port */
   CloseLogFile();                /* Close log file if open */
   gotoxy(1,25);                  /* When leaving set cursor to screen bottom*/
   return(0);                     /* Thats all */
}


/* C V T 1 0 0 I N I T -- Initialize the VT100 emulator */

void CVT100Init(void) {

   FileInit();                    /* Initialize File System *first* */
   TTinit();                      /* Initialize communications system */
   VidInit();                     /* Initialize video system */
   KeyInit();                     /* Initialize Keyboard system */
   VTInit();                      /* Initialize VT Emulation */
}

/* T E R M I N A L -- Perform terminal emulation */

void terminal(void)
{
   int thru = 0;
   int key;
   int c;

   while (!thru) {                /* Continue until exited */
      if (ConChk()) {                /* If a keystroke is present */
         if (DoKey() == -1)          /* Then interpret it */
            thru = 1;                /* Watch for done flag here */
      }
      if (ttchk())                /* If incoming communications chars */
         ConOut( ttinc() );       /* Output the character to emulation system*/
   }
}



/* B A D E X I T -- Exit the program displaying a fatal error message */

#ifdef	NODOS
#ifdef	__BCC__
void badexit( char * message ) { exit(1); }
#else
void badexit( char * message ) { geninterrupt(0x19); }
#endif
#else
void badexit( char * message ) {
   char line[132];
   register char * s = line;

   ttclose();
   gotoxy(1,25);                  /* start message on last line of display*/

   while ( (*s = *message++) != '\0')
      s++;
   *s++ = '\a';                   /* Concantonate a '$' to the message */
   *s = '$';

   _DX = (unsigned) line;
   _AH = 9;
   geninterrupt(0x21);            /* Display message using DOS function 9 */
   exit(1);                       /* Exit with errorlevel 1 */
}
#endif

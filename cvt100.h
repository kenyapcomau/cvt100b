/* CVT100.H common definitions file */
/* from CVT100.C */

/* $Id: cvt100.h,v 1.1 1998/06/24 13:07:06 ken Exp $ */

void badexit( char * );

/* from COMIO.C */
extern void TTinit(void);                /* Initialize the communications system */
extern int ttopen(void);                 /* Open a port for communications */
extern int ttclose( void );              /* Close the communications port */
extern int ttchk( void );                /* Return count of received characters */
extern void ttoc( unsigned char );       /* Output a character to the com port */
extern int ttinc( void );                /* Input a character from circular buffer */
/* from FILEIO.C */
extern void FileInit( void );            /* Initialize the file system */
extern void SaveSetup( void );           /* Save setup information to file */
extern int  GetTTSetup( void );          /* Set Communications setup values */
extern int  GetVidSetup( void );         /* Set Video setup values */
extern int GetKeySetup(void) ;
extern int GetVTSetup(void) ;

extern void OpenLogFile( void );         /* Open the logfile */
extern void WriteLog( char );            /* Write a character to the log file */
extern void FlushLogBuff( void );        /* Flush the log file buffer to disk */
extern void CloseLogFile( void );        /* Close the log file */
/* from VIDIO.C */
extern void VidInit( void );             /* Initialize the video system */
extern void SetVattr( unsigned char );   /* Set the video attribute */
extern void AddVattr( unsigned char );   /* Add attribute to current video attribute*/
extern void SubVattr( unsigned char );   /* Sub attribute from current vid attribute*/
extern void BrkAtt( unsigned char );     /* Break attribute into extra and base */
extern unsigned char AddAtt( void );     /* Add extra and base attributes to get */
extern void SetCursorVisibility( int );  /* Set the cursor visibility mode */
extern void SetBackGround( int );        /* Set background video attribute */
extern void SetColor( void );            /* Set the screen colors */
extern int  vtprintf( int, int, int,     /* Printf for Emulator to row and column */
         char *, ... );                  /* in regular or reverse video */
extern void RestoreScreen( void );       /* Restore contents of video memory */
extern void SetScreenWidth( int );       /* Set the logical width of the screen */
extern void SaveScreen( void );          /* Save contents of video memory */
extern void ClearScreen( void );         /* Clear the terminal screen */

/* from KEYIO.C */
extern void         KeyInit( void );     /* Initialize the keyboard system */
extern int          ConChk( void );      /* Check the keyboard for keystrokes */
extern unsigned int GetKey( void );      /* Retrieve a scan code from keyboard */
extern int          DoKey( void );       /* Transmit a key sequence */
/* from VTTIO.C */
extern void VTInit( void );
extern void ConOut( unsigned char );

#ifndef	BLINK
#define	BLINK		128
#endif

#ifdef	__BCC__
#define	MK_FP(s,o)	o
#define	bioskey(x)	bios_getc()
#define	delay(secs)
#define	sound(freq)
#define	nosound()
#endif

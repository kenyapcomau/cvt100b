#include <stdio.h>
#include <dos.h>
#include <string.h>

#include "cvt100.h"

#define F1    0x3B00              /* Scancode for function key F1  */
#define F2    0x3C00              /* Scancode for function key F2  */
#define F3    0x3D00              /* Scancode for function key F3  */
#define F4    0x3E00              /* Scancode for function key F4  */
#define F5    0x3F00              /* Scancode for function key F5  */
#define F6    0x4000              /* Scancode for function key F6  */
#define F7    0x4100              /* Scancode for function key F7  */
#define F8    0x4200              /* Scancode for function key F8  */
#define F9    0x4300              /* Scancode for function key F9  */
#define F10   0x4400              /* Scancode for function key F10 */
#define ESC   0x011b              /* Scancode for ESCape key */

/*****************************************************************************/
/* function prototypes                                                       */

void TTSetup( void );             /* Communications setup function */
void VidSetup( void );            /* Video setup function */
void KeySetup( void );            /* Keyboard setup function */
void VTSetup( void );             /* VT emulation setup */
void FileSetup( void );           /* File system setup */

static void ToggleBaud( void );        /* Toggle the baud setting */
static void TogglePort( void );        /* Toggle the port setting */
static void ToggleParity( void );      /* Toggle the parity setting */
static void ToggleDataBits( void );    /* Toggle the databits setting */
static void ToggleStopBits( void );    /* Toggle the stopbits setting */
static void DisplayBaud( void );       /* Display the baud setting */
static void DisplayPort( void );       /* Display the port setting */
static void DisplayParity( void );     /* Display the parity setting */
static void DisplayDataBits( void );   /* Display the databits setting */
static void DisplayStopBits( void );   /* Display the stopbits setting */
static void ToggleRetrace( void );     /* Toggle the video retrace mode */
static void DisplayRetrace( void );    /* Display the video retrace mode */
static void ToggleBackSpace( void );   /* Toggle the backspace interpretation */
static void DisplayBackSpace( void );  /* Display backspace interpretation */
static void ToggleKeyClick( void );    /* Toggle the key click mode */
static void DisplayKeyClick( void );   /* Display the Keyclick mode */
static void ToggleKeyPadMode( void );  /* Toggle the key pad mode */
static void DisplayKeyPadMode( void ); /* Display the Key Pad mode */
static void ToggleForeColor( void );   /* Toggle foreground colors */
static void DisplayForeColor( void );  /* Display the foreground colors */
static void ToggleBackColor( void );   /* Toggle background colors */
static void DisplayBackColor( void );  /* Display the background colors */
static void ToggleOriginMode( void );  /* Toggle the origin Mode */
static void ToggleInsertMode( void );  /* Toggle the Insert/replace Mode */
static void ToggleAutoWrapMode( void );/* Toggle auto wrap mode */
static void ToggleNewLine( void );     /* Toggle new line mode */
static void ToggleCursorVisible( void );/* Toggle cursor visibility */
static void ToggleReverseBackGround( void );/* Toggle reversed background mode */
static void ToggleScreenWid( void );   /* Toggle the screen width setting */
static void DisplayOriginMode( void ); /* Display the origin Mode */
static void DisplayInsertMode( void ); /* Display the Insert/replace Mode */
static void DisplayAutoWrapMode( void );/* Display auto wrap mode */
static void DisplayNewLine( void );    /* Display new line mode */
static void DisplayCursorVisible( void );/* Display cursor visibility */
static void DisplayReverseBackGround( void );/* Display reversed background mode */
static void DisplayScreenWid( void );  /* Display the screen width setting */
static void DisplaySaveSetup( void );  /* Display Setup saved status */
static void ToggleLogMode( void );     /* Toggle Log File mode */
static void DisplayLogMode( void );    /* Display status of Log File */
static void StartSetup(char *);        /* Begin a setup screen */
static void EndSetup( void );          /* End a setup screen */

unsigned int GetKey( void );      /* Function to retrieve a keystroke */

/*****************************************************************************/
/* Global Data                                                               */


/*****************************************************************************/
/* External Data                                                             */

extern unsigned int port;         /* COM port */
extern unsigned int speed;        /* BAUD rate */
extern char parity[5];            /* Parity setting */
extern unsigned int databits;     /* Number of Data bits */
extern unsigned int stopbits;     /* Number of Stop bits */

extern unsigned char retracemode; /* Video snow retrace wait mode flag */
extern unsigned char forecolor;   /* Default foreground color */
extern unsigned char backcolor;   /* Default background color */

extern unsigned char backspace;   /* Backspace interpretation flag */
extern unsigned char keyclick;    /* Keyclick on/off flag */
extern unsigned char applkeypad;  /* Application key pad mode flag */

extern unsigned originmode;       /* Origin mode, relative or absolute */
extern unsigned insertmode;       /* Insert mode, off or on */
extern unsigned autowrap;         /* Automatic wrap mode, off or on */
extern unsigned newline;          /* Newline mode, off or on,  GLOBAL data!*/
extern unsigned cursorvisible;    /* Cursor visibility, on or hidden */
extern unsigned reversebackground;/* Reverse background attribute, on or off*/
extern unsigned screenwid;        /* Screen column width */
extern unsigned char log;         /* Logging characters */

extern char setupfile[];          /* Filename of setup file */
extern char logfile[];            /* Filename of log file */
extern unsigned char vidmode;     /* Video mode */

/*****************************************************************************/
/* Local Static Data                                                               */

static char *vcolors[] = {        /* Array of color names */
    "BLACK",        "BLUE",           "GREEN",
    "CYAN",         "RED",            "MAGENTA",
    "BROWN",        "LIGHTGRAY",      "DARKGRAY",
    "LIGHTBLUE",    "LIGHTGREEN",     "LIGHTCYAN",
    "LIGHTRED",     "LIGHTMAGENTA",   "YELLOW",
    "WHITE",
    };

static unsigned holdbackground;   /* Temporary holder for background setting */
static unsigned char saved;       /* Flag indication setup already saved */

#ifndef	NODOS
static char id[] = "$Id: vtsetup.c,v 1.2 1998/07/07 11:26:33 ken Exp $";
#endif

/*****************************************************************************/
/*****************************************************************************/

/*  T T S E T U P -- Enter Communications Setup Mode */

void TTSetup(void) {
   int thru = 0;
   int changes = 0;


   StartSetup("Communications");  /* Begin this Setup Screen */

   DisplayPort();                 /* Display initial current port setting */
   DisplayBaud();                 /* Display initial current baud rate setting */
   DisplayParity();               /* Display initial current parity setting */
   DisplayDataBits();             /* Display initial current data bits setting */
   DisplayStopBits();             /* Display initial current stop bits setting */

   while( !thru ) {

      switch (GetKey()) {
          case F1:                /* Toggle the COM port setting */
              TogglePort();
              DisplayPort();
              changes = 1;
              break;
          case F2:                /* Toggle the baud rate setting */
              ToggleBaud();
              DisplayBaud();
              changes = 1;
              break;
          case F3:                /* Toggle the parity setting */
              ToggleParity();
              DisplayParity();
              changes = 1;
              break;
          case F4:                /* Toggle the data bits setting */
              ToggleDataBits();
              DisplayDataBits();
              changes = 1;
              break;
          case F5:                /* Toggle the stop bits setting */
              ToggleStopBits();
              DisplayStopBits();
              changes = 1;
              break;
          case F10:
          case ESC:
              thru = 1;
              break;
          default:
              ;
      }
   }
   RestoreScreen();               /* Restore the initial emulation screen */
   if (changes) {                 /* If changes have been made */
      ttclose();                    /* Close the port */
      ttopen();                     /* Reopen the port to update COM values */
   }
}




/*  T O G G L E P O R T -- toggle the COM port setting */

static void TogglePort(void) {
   static int vports[] = {        /* Array of Valid port settings */
       1, 2,
       0 };                       /* Terminated by a 0 */
   static int * pptr = vports;    /* Pointer to port value setting in array */

   ++pptr;                      /* look at next port setting in array */
   if (*pptr == 0)              /* if setting is 0 then time for wrap */
      pptr = vports;
   port = *pptr;                /* set the port to this setting */
}



/*  T O G G L E B A U D -- Toggle the baud rate setting */

static void ToggleBaud(void) {
   static int vbauds[] = {        /* Array of valid baud rate settings */
       50, 75, 110, 150, 300,
       600, 1200, 1800, 2000,
       2400, 4800, 9600, 19200,
       38400,                     /* Terminated by a 0 */
       0 };
   static int *bptr = &vbauds[9]; /* Pointer to baud rate setting in array */

   ++bptr;                        /* look at next baud rate setting in array */
   if (*bptr == 0)                /* if setting is 0 then time for wrap */
      bptr = vbauds;
   speed = *bptr;                 /* Set the speed to this setting */
}



/*  T O G G L E P A R I T Y -- Toggle the parity setting */

static void ToggleParity(void) {
   static char *vparitys[] = {    /* Array of valid parity settings */
       "NONE", "EVEN", "ODD",
       NULL };                    /* Terminated by a NULL */
   static char **pptr = vparitys; /* Pointer to parity setting in array */

   ++pptr;                        /* look at next parity setting in array */
   if (*pptr == NULL)             /* if setting == NULL then time for wrap */
      pptr = vparitys;
   strcpy(parity,*pptr);          /* set the parity to this setting */
}



/*  T O G G L E D A T A B I T S -- Toggle databits setting */

static void ToggleDataBits(void) {
   static int vdatabits[] = {     /* Array of valid databit settings */
       5,6,7,8,
       0 };                       /* Terminated by a 0 */
   static int *dptr=vdatabits;    /* Pointer to databit setting in array */

   ++dptr;                        /* look at next databit setting in array */
   if (*dptr == 0)                /* if setting is 0 then time for wrap */
      dptr = vdatabits;
   databits = *dptr;              /* set the databits to this setting */
}



/*  T O G G L E S T O P B I T S -- Toggle StopBits setting */

static void ToggleStopBits(void) {
   static int vstopbits[] = {     /* Array of valid stopbit settings */
       1,2,
       0 };                       /* Terminated by a 0 */
   static int *sptr=&vstopbits[0];/* Pointer to stopbit setting in array */

   ++sptr;                        /* look at next stopbits setting in array */
   if (*sptr == 0)                /* if setting is 0 then time for wrap */
       sptr = vstopbits;
   stopbits = *sptr;              /* set the stopbits to this setting */
}

/*  D I S P L A Y P O R T -- Display COM port setting */

static void DisplayPort(void) {
   vtprintf(5,2,0," F1)  COM port  = %5d",port);
}


/*  D I S P L A Y B A U D -- Display baud rate setting */

static void DisplayBaud(void) {
   vtprintf(7,2,0," F2)  Baud rate = %5u",speed);
}


/*  D I S P L A Y P A R I T Y -- Display parity setting */

static void DisplayParity(void) {
   vtprintf(9,2,0," F3)  Parity =     %4s",parity);
}


/*  D I S P L A Y D A T A B I T S -- Display databits setting */

static void DisplayDataBits(void) {
   vtprintf(11,2,0," F4)  Data bits = %4d",databits);
}


/*  D I S P L A Y S T O P B I T S -- Display stopbits setting */

static void DisplayStopBits(void) {
   vtprintf(13,2,0," F5)  Stop bits = %4d",stopbits);
}

/*****************************************************************************/


/*  V I D S E T U P -- Enter Video Setup Mode */

void VidSetup(void) {
   int thru = 0;
   int colorchange = 0;

   StartSetup("Video");           /* Begin this Setup Screen */


   DisplayRetrace();              /* Display initial retrace mode setting */
   DisplayForeColor();            /* Display initial foreground color */
   DisplayBackColor();            /* Display initial background color */

   while( !thru ) {

      switch (GetKey()) {
          case F1:                /* Toggle the Retrace mode setting */
              ToggleRetrace();
              DisplayRetrace();
              break;
          case F2:                /* Toggle the Foreground color */
              ToggleForeColor();
              DisplayForeColor();
              colorchange = 1;
              break;
          case F3:                /* Toggle the Background color */
              ToggleBackColor();
              DisplayBackColor();
              colorchange = 1;
              break;
          case F10:
          case ESC:
              thru = 1;
              break;
          default:
              ;
      }
   }

   RestoreScreen();               /* Restore the initial emulation screen */
   if (colorchange)
      SetColor();
}


/*  T O G G L E R E T R A C E -- Toggle Video Retrace mode setting */

static void ToggleRetrace(void) {

   if (vidmode != 7)              /* Don't allow toggling on a monochrome */
      retracemode ^= 1;
   else {
      retracemode = 0;
      sound(324);
      delay(400);
      nosound();
   }
}

/*  T O G G L E F O R E C O L O R -- Toggle the foreground color */

static void ToggleForeColor(void) {

   if (++forecolor >= 16)         /* Index to next color Setting */
      forecolor = 0;              /*  watch for wrap when past WHITE */
}

/*  T O G G L E B A C K C O L O R -- Toggle the background color */

static void ToggleBackColor(void) {

   if (++backcolor >= 8)          /* Index to next color Setting */
      backcolor = 0;              /*  watch for wrap when past LIGHTGREY */
}


/*  D I S P L A Y R E T R A C E -- Display video retrace mode setting */

static void DisplayRetrace(void) {
   vtprintf(5,2,0," F1)  Video Snow Retrace Wait = %s",
      (retracemode == 0 ? "OFF" : "ON "));
}


/*  D I S P L A Y F O R E C O L O R -- Display foreground color */

static void DisplayForeColor(void) {
   vtprintf(7,2,0," F2)  Foreground Color = %12s",vcolors[forecolor]);
}


/*  D I S P L A Y B A C K C O L O R -- Display background color */

static void DisplayBackColor(void) {
   vtprintf(9,2,0," F3)  Background Color = %12s",vcolors[backcolor]);
}




/*****************************************************************************/


/*  K E Y S E T U P -- Enter Keyboard Setup Mode */

void KeySetup(void) {
   int thru = 0;

   StartSetup("KeyBoard");        /* Begin this Setup Screen */

   DisplayBackSpace();            /* Display initial retrace mode setting */
   DisplayKeyClick();             /* Display initial Key Click mode setting */
   DisplayKeyPadMode();           /* Display initial Key Pad mode setting */

   while( !thru ) {

      switch (GetKey()) {
          case F1:                /* Toggle the Backspace key interpretation*/
              ToggleBackSpace();
              DisplayBackSpace();
              break;
          case F2:                /* Toggle the KeyClick Setting */
              ToggleKeyClick();
              DisplayKeyClick();
              break;
          case F3:                /* Toggle the KeyPadMode Setting */
              ToggleKeyPadMode();
              DisplayKeyPadMode();
              break;
          case F10:
          case ESC:
              thru = 1;
              break;
          default:
              ;
      }
   }
   RestoreScreen();               /* Restore the initial emulation screen */

}


/*  T O G G L E B A C K S P A C E -- Toggle Backspace interpretation */

static void ToggleBackSpace(void) {

   backspace ^= 1;
}

/*  T O G G L E K E Y C L I C K -- Toggle Keyclick mode */

static void ToggleKeyClick(void) {

   keyclick ^= 1;
}

/*  T O G G L E K E Y P A D M O D E -- Toggle KeyPad mode */

static void ToggleKeyPadMode(void) {

   applkeypad ^= 1;
}


/*  D I S P L A Y B A C K S P A C E -- Display backspace interpretation */

static void DisplayBackSpace(void) {
   vtprintf(5,2,0," F1)  BackSpace Interpretation = %s",
      (backspace == 0 ? "DELETE   " : "BACKSPACE"));
}


/*  D I S P L A Y K E Y C L I C K -- Display keyclick mode */

static void DisplayKeyClick(void) {
   vtprintf(7,2,0," F2)  KeyClick is %s",
      (keyclick == 0 ? "OFF" : "ON "));
}

/*  D I S P L A Y K E Y P A D M O D E -- Display keypad mode */

static void DisplayKeyPadMode(void) {
   vtprintf(9,2,0," F3)  Key Pad is set to %s",
      (applkeypad == 0 ? "NUMERIC    " : "APPLICATION"));
}


/*****************************************************************************/


/*  V T S E T U P -- Enter Emulation Setup Mode */

void VTSetup(void) {
   int thru = 0;
   int cursorchange = 0;
   int backgroundchange = 0;
   int screenwidchange = 0;

   holdbackground = reversebackground; /* Get value of background setting */

   StartSetup("Emulation");       /* Begin this Setup Screen */

   DisplayOriginMode();           /* Display initial origin mode setting */
   DisplayInsertMode();           /* Display initial insert mode setting */
   DisplayAutoWrapMode();         /* Display initial autowrap mode setting */
   DisplayNewLine();              /* Display initial newline mode setting */
   DisplayCursorVisible();        /* Display initial cursorvisible setting */
   DisplayReverseBackGround();    /* Display initial setting for backgroun */
   DisplayScreenWid();            /* Display initial logical screen width */

   while( !thru ) {

      switch (GetKey()) {
          case F1:                /* Toggle origin mode setting */
              ToggleOriginMode();
              DisplayOriginMode();
              break;
          case F2:                /* Toggle insert mode setting */
              ToggleInsertMode();
              DisplayInsertMode();
              break;
          case F3:                /* Toggle autowrap mode setting */
              ToggleAutoWrapMode();
              DisplayAutoWrapMode();
              break;
          case F4:                /* Toggle newline mode setting */
              ToggleNewLine();
              DisplayNewLine();
              break;
          case F5:                /* Toggle cursor visibility */
              ToggleCursorVisible();
              DisplayCursorVisible();
              cursorchange = 1;
              break;
          case F6:                /* Toggle background appearance */
              ToggleReverseBackGround();
              DisplayReverseBackGround();
              backgroundchange = 1;
              break;
          case F7:                /* Toggle screen width */
              ToggleScreenWid();
              DisplayScreenWid();
              screenwidchange = 1;
              break;

          case F10:
          case ESC:
              thru = 1;
              break;
          default:
              ;
      }
   }

   RestoreScreen();               /* Restore the initial emulation screen */
   if (cursorchange)              /* Change the cursor type if needed */
      SetCursorVisibility(cursorvisible);
   if (backgroundchange)          /* Change the background if needed */
      SetBackGround(holdbackground);
   if (screenwidchange)           /* Change the screenwidth if needed */
      SetScreenWidth(screenwid);
}

/*  T O G G L E O R I G I N M O D E -- Toggle the origin mode */

static void ToggleOriginMode(void) {

   originmode ^= 1;
}

/*  T O G G L E I N S E R T M O D E -- Toggle insert/replace mode */

static void ToggleInsertMode(void) {

   insertmode ^= 1;
}


/*  T O G G L E A U T O W R A P -- Toggle the autowrap mode */

static void ToggleAutoWrapMode(void) {

   autowrap ^= 1;
}


/*  T O G G L E N E W L I N E -- Toggle the newline mode */

static void ToggleNewLine(void) {

   newline ^= 1;
}


/*  T O G G L E C U R S O R V I S I B L E -- Toggle the cursor visibility */

static void ToggleCursorVisible(void) {

   cursorvisible ^= 1;
}


/*  T O G G L E R E V E R S E B A C K G R O U N D -- Toggle background */

static void ToggleReverseBackGround(void) {

   holdbackground ^= 1;
}

/*  T O G G L E S C R E E N W I D -- Toggle the screen width */

static void ToggleScreenWid(void) {

   if (screenwid == 80)
       screenwid = 132;
   else
       screenwid = 80;
}


/*  D I S P L A Y O R I G I N M O D E -- Display the origin mode */

static void DisplayOriginMode(void) {
   vtprintf(7,2,0," F1)  Origin Mode is %s",
      (originmode == 0 ? "ABSOLUTE" : "RELATIVE"));

}

/*  D I S P L A Y I N S E R T M O D E -- Display insert/replace mode */

static void DisplayInsertMode(void) {
   vtprintf(9,2,0," F2)  Insert/Replace mode is set to %s",
      (insertmode == 0 ? "REPLACE" : "INSERT  "));

}


/*  D I S P L A Y A U T O W R A P -- Display the autowrap mode */

static void DisplayAutoWrapMode(void) {
   vtprintf(11,2,0," F3)  Auto Wrap mode is %s",
      (autowrap == 0 ? "OFF" : "ON "));

}


/*  D I S P L A Y N E W L I N E -- Display the newline mode */

static void DisplayNewLine(void) {
   vtprintf(13,2,0," F4)  New Line mode is %s",
      (newline == 0 ? "OFF" : "ON "));

}


/*  D I S P L A Y C U R S O R V I S I B L E -- Display the cursor visibility */

static void DisplayCursorVisible(void) {
   vtprintf(15,2,0," F5)  Cursor is %s",
      (cursorvisible == 0 ? "HIDDEN " : "VISIBLE"));

}


/*  D I S P L A Y R E V E R S E B A C K G R O U N D -- Display background */

static void DisplayReverseBackGround(void) {
   vtprintf(17,2,0," F6)  Background is %s",
      (holdbackground == 0 ? "NORMAL  " : "REVERSED"));

}

/*  D I S P L A Y S C R E E N W I D -- Display the screen width */

static void DisplayScreenWid(void) {
   vtprintf(19,2,0," F7)  Logical Screen width is %3d columns",screenwid);
}

/*****************************************************************************/


/*  F I L E S E T U P -- Enter File Setup Mode */

void FileSetup(void) {
#if	!defined(NODOS) || defined(USECMOS)
   int thru = 0;

   StartSetup("File");           /* Begin this Setup Screen */
   saved = 0;                    /* Start with information not saved */

   DisplaySaveSetup();           /* Display initially unsaved setup */
#ifndef	USECMOS
   DisplayLogMode();             /* Display status of Log File */
#endif

   while( !thru ) {

      switch (GetKey()) {
          case F1:                /* Save the setup information to disk */
              SaveSetup();
              saved = 1;
              DisplaySaveSetup();
              break;
#ifndef	USECMOS
          case F2:                /* Set/reset logging to disk */
              ToggleLogMode();
              DisplayLogMode();
              if (log)
                 OpenLogFile();
              else
                 CloseLogFile();
              break;
#endif
          case F10:
          case ESC:
              thru = 1;
              break;
          default:
              ;
      }
   }

   RestoreScreen();               /* Restore the initial emulation screen */
#endif
}


#if	!defined(NODOS) || defined(USECMOS)
/*  T O G G L E L O G M O D E -- Toggle the log file status */

static void ToggleLogMode(void) {
   log ^= 1;
}


/*  D I S P L A Y S A V E S E T U P -- Display video retrace mode setting */

static void DisplaySaveSetup(void) {
   vtprintf(5,2,0," F1)  Save Setup to '%s' %s",setupfile,
      (saved == 0 ? " " : " *DONE*"));
}

#ifndef	USECMOS
/*  D I S P L A Y L O G M O D E -- Display log file status */

static void DisplayLogMode(void) {

   if (log)
       vtprintf(7,2,0," F2)  Logging incoming characters to '%s'",logfile);
   else
       vtprintf(7,2,0," F2)  Logging of incoming characters is OFF            ");
}
#endif
#endif


/*****************************************************************************/


/* S T A R T S E T U P -- Begin a setup mode screen */

static void StartSetup(char * title) {

   SaveScreen();
   ClearScreen();
   vtprintf(0,0,1,"%80s"," ");
   vtprintf(0,0,1,"%s %s",title,
       "Setup Mode,   Use the function keys to toggle settings");
   vtprintf(24,2,0,"%s %s %s","ESC) or F10)   Exit",title,"Setup");
}


/* E N D S E T U P -- End the setup mode screen */

static void EndSetup(void) {

   RestoreScreen();               /* Restore the pre-setup screen */
}






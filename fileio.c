#include <stdio.h>
#include <stdlib.h>
#ifndef	__BCC__
#include <io.h>
#include <mem.h>
#endif
#include <fcntl.h>

#include "cvt100.h"

#define LOGBUFFSIZE 1024

#ifndef	NODOS
static char id[] = "$Id: fileio.c,v 1.2 1998/07/07 11:24:03 ken Exp $";
#endif

/**************************************************************************/
/* Function prototypes                                                    */

void FileInit( void );            /* Initialize the file system */
void SaveSetup( void );           /* Save setup information to file */
int  GetTTSetup( void );          /* Set Communications setup values */
int  GetVidSetup( void );         /* Set Video setup values */

void OpenLogFile( void );         /* Open the logfile */
void WriteLog( char );            /* Write a character to the log file */
void FlushLogBuff( void );        /* Flush the log file buffer to disk */
void CloseLogFile( void );        /* Close the log file */


/**************************************************************************/
/* Global Variables                                                       */

#ifdef	NODOS
char setupfile[] = "CMOS";  /* Filename of setup file */
#else
char setupfile[] = "CVT100.SET";  /* Filename of setup file */
#endif
#ifndef	USECMOS
char logfile[] = "CVT100.LOG";    /* Filename of log file */
#endif


/**************************************************************************/
/* External Variables                                                     */

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
extern unsigned char log;         /* Flag to indicate status of Log */

#ifdef	NODOS

#ifndef	USECMOS

void FileInit(void) { }
void SaveSetup(void) { }
int GetTTSetup(void) { return 0; }
int GetVidSetup(void) { return 0; }
int GetVTSetup(void) { return 0; }
int GetKeySetup(void) { return 0; }

#else		/* Using CMOS */

static int		havecmos = 0, cvtvalid = 0;
/* contents of CMOS */
static union
{
	unsigned char 	bytes[0x40];
	struct
	{
		unsigned char	pad[0x38];	/* leave 0x0 to 0x38 alone */
		unsigned int	speed;
		unsigned char	parbitstop, port;
		unsigned char	retracemode, forecolor, backcolor;
		unsigned char	csum;
	} settings;
} cmos;

void FileInit(void)
{
	int	i, sum;

	for (i = 0x10; i < 0x40; ++i)
	{
		outportb(0x70, i);
		(void)inportb(0x64);	/* read of kbd status to delay */
		cmos.bytes[i] = inportb(0x71);
	}
	for (sum = 0, i = 0x10; i <= 0x2d; ++i)
		sum += cmos.bytes[i];
	/* verify checksum */
	if (sum >> 8 == cmos.bytes[0x2e] && (sum & 0xff) == cmos.bytes[0x2f])
		havecmos = 1;
	for (sum = 0, i = 0x38; i < 0x40; ++i)
	{
		sum += cmos.bytes[i];
	}
	cvtvalid = (sum == 0);
}

void SaveSetup(void)
{
	int	i;

	if (!havecmos)
		return;
	cmos.settings.port = port;
	cmos.settings.speed = speed;
	if (strcmp(parity, "ODD") == 0)
		i = 0x8;
	else if (strcmp(parity, "EVEN") == 0)
		i = 0x18;
	else
		i = 0;
	cmos.settings.parbitstop =
		(databits - 5) |
		(stopbits == 2 ? 0x4 : 0) |
		i;
	cmos.settings.retracemode = retracemode;
	cmos.settings.forecolor = forecolor;
	cmos.settings.backcolor = backcolor;
	cmos.settings.csum = 0;
	for (i = 0x38; i < 0x3f; ++i)
		cmos.settings.csum += cmos.bytes[i];
	cmos.settings.csum = -cmos.settings.csum;
	for (i = 0x38; i < 0x40; ++i)
	{
		outportb(0x70, i);
		(void)inportb(0x64);	/* read of kbd status to delay */
		outportb(0x71, cmos.bytes[i]);
	}
}

int GetTTSetup(void)
{
	char		*p;

	if (havecmos == 0 || cvtvalid == 0)
		return(0);		/* Then return 0 for no action taken */
	port = cmos.settings.port;	/* Set communications parameters */
	speed = cmos.settings.speed;	/* to saved values */
	databits = 5 + (cmos.settings.parbitstop & 0x3);
	stopbits = (cmos.settings.parbitstop & 0x4) ? 2 : 1;
	switch (cmos.settings.parbitstop & 0x18)
	{
	case 0x0: case 0x10:
		p = "NONE";
		break;
	case 0x8:
		p = "ODD";
		break;
	case 0x18:
		p = "EVEN";
		break;
	}
	strcpy(parity, p);
	return(1);			/* Return 1 for action taken */
}

int GetVidSetup(void)
{
	if (havecmos == 0 || cvtvalid == 0)
		return(0);		/* Then return 0 for no action taken */
	retracemode = cmos.settings.retracemode;
	forecolor = cmos.settings.forecolor;
	backcolor = cmos.settings.backcolor;
	return(1);			/* Return 1 for action taken */
}

int GetVTSetup(void) { return 0; }
int GetKeySetup(void) { return 0; }

#endif		/* USECMOS */

void OpenLogFile(void) { }
void WriteLog(char c) { }
void FlushLogBuff(void) { }
void CloseLogFile(void) { }

#else		/* Have DOS */

/**************************************************************************/
/* Local Static Data                                                      */

static char initfilefound = 0;    /* Flag to indication Setup File found */
static int loghandle;             /* Handle of Log file */
static unsigned char logfileopen = 0; /* Flag to indicate log file is open */

static char logbuff[LOGBUFFSIZE]; /* Log file buffer */
static int logcount;              /* Count of characters written to logfile*/

static struct com_struct {        /* Structure to hold com parameters */
    int port;
    unsigned speed;
    char parity[5];
    int databits;
    int stopbits;
} com;

static struct vid_struct {        /* Structure to hold video parameters */
    unsigned char retracemode;
    unsigned char forecolor;
    unsigned char backcolor;
} vid;

static struct key_struct {        /* Structure to hold keyboard parameters */
    unsigned char keyclick;
    unsigned char backspace;
    unsigned char applkeypad;
} key;

static struct emul_struct {       /* Structure to hold emulation parameters */
    unsigned int originmode;
    unsigned int insertmode;
    unsigned int autowrap;
    unsigned int newline;
    unsigned int cursorvisible;
    unsigned int reversebackground;
    unsigned int screenwid;
    unsigned char log;
} emul;


static struct setup_save_struct { /* Setup File block definition */
    char crecord[128];               /* space for communications record */
    char vrecord[128];               /* space for video record */
    char krecord[128];               /* space for keyboard record */
    char erecord[128];               /* space for emulation record */
    } setup;


/**************************************************************************/
/**************************************************************************/


/* F I L E I N I T -- initialize the file system */

void FileInit(void) {
    int handle;
    int bytes;
                                  /* Try and open setup file for input */
    if ( (handle = _open(setupfile,O_RDONLY)) == -1)
        return;                      /* If failed then just return */

                                  /* Try and read a setup block from file */
    bytes = _read(handle,&setup,sizeof(setup));
    if (bytes < 0)                   /* If failed then this is a fatal error */
       badexit("IO error reading Setup file");
    else if (bytes != sizeof(setup))
       badexit("Number of bytes read error on Setup file");
    if (_close(handle) != 0)      /* Close the setup file */
       badexit("File Close error on Setup file");

    initfilefound = 1;            /* Set setup information read flag */

                                  /* Move saved information to parm struct's*/
    memcpy(&com,setup.crecord,sizeof(com));
    memcpy(&vid,setup.vrecord,sizeof(vid));
    memcpy(&key,setup.krecord,sizeof(key));
    memcpy(&emul,setup.erecord,sizeof(emul));

}


/* S A V E S E T U P -- save the setup information */

void SaveSetup(void) {
    int handle;
    int bytes;


    com.port = port;              /* Save communications parameters */
    com.speed = speed;
    memcpy(com.parity,parity,sizeof(com.parity));
    com.databits = databits;
    com.stopbits = stopbits;

    vid.retracemode = retracemode;/* Save video parameters */
    vid.forecolor = forecolor;
    vid.backcolor = backcolor;

    key.backspace = backspace;    /* Save KeyBoard parameters */
    key.keyclick = keyclick;
    key.applkeypad = applkeypad;

    emul.originmode = originmode; /* Save VT emulation values */
    emul.insertmode = insertmode;
    emul.autowrap = autowrap;
    emul.newline = newline;
    emul.cursorvisible = cursorvisible;
    emul.reversebackground = reversebackground;
    emul.screenwid = screenwid;
    emul.log = log;

                                  /* Initialize setup block to zeros */
    memset(&setup,'\0',sizeof(setup));
                                  /* Move parm information to setup block */
    memcpy(setup.crecord,&com,sizeof(com));
    memcpy(setup.vrecord,&vid,sizeof(vid));
    memcpy(setup.krecord,&key,sizeof(key));
    memcpy(setup.erecord,&emul,sizeof(emul));

                                  /* Try and create a setup file for writing */
    if ( (handle = _creat(setupfile,0)) == -1)
        badexit("IO error Opening setup file for output");


                                  /* Try and write the setup block to disk*/
    bytes = _write(handle,&setup,sizeof(setup));
    if (bytes != sizeof(setup))
       badexit("Number of bytes write error on Setup file");
    if (_close(handle) != 0)      /* Try and close the output file */
       badexit("File Close error on Setup file");

    initfilefound = 1;            /* Set flag indicating setup is good */

}




/* G E T T T S E T U P -- Get the communications setup saved in a file */

int GetTTSetup( void ) {

    if (initfilefound == 0)       /* If no setup file was found */
        return(0);                   /* Then return 0 for no action taken */

    port = com.port;              /* Set communications parameters */
    speed = com.speed;            /* to saved values */
    memcpy(parity,com.parity,sizeof(com.parity));
    databits = com.databits;
    stopbits = com.stopbits;
    return(1);                    /* Return 1 for action taken */
}

/* G E T V I D S E T U P -- Get the video setup saved in file */

int GetVidSetup( void ) {

    if (initfilefound == 0)       /* If no setup file was found */
        return(0);                   /* Then return 0 for no action taken */

    retracemode = vid.retracemode;/* Set video parameters */
    forecolor = vid.forecolor;    /* to saved values */
    backcolor = vid.backcolor;
    return(1);                    /* Return 1 for action taken */
}

/* G E T K E Y S E T U P -- Get the keyboard setup saved in file */

int GetKeySetup( void ) {

    if (initfilefound == 0)       /* If no setup file was found */
        return(0);                   /* Then return 0 for no action taken */

    backspace = key.backspace;    /* Set KeyBoard parameters */
    keyclick = key.keyclick;      /* to saved values */
    applkeypad = key.applkeypad;
    return(1);                    /* Return 1 for action taken */
}


/* G E T V T S E T U P -- Get the emulation setup saved in file */

int GetVTSetup( void ) {

    if (initfilefound == 0)       /* If no setup file was found */
        return(0);                   /* Then return 0 for no action taken */

    originmode = emul.originmode; /* Set VT emulation values */
    insertmode = emul.insertmode;    /* to saved values */
    autowrap = emul.autowrap;
    newline = emul.newline;
    cursorvisible = emul.cursorvisible;
    reversebackground = emul.reversebackground;
    screenwid = emul.screenwid;
    log = emul.log;

    if (log)                      /* Check if logging specified in setup file*/
        OpenLogFile();

    return(1);                    /* Return 1 for action taken */
}


/* O P E N L O G F I L E -- Open the logfile */

void OpenLogFile(void) {

    if (logfileopen)              /* Ignore if already open */
        return;
                                  /* Try and open log file for output */
    if ( (loghandle = open(logfile,O_RDWR | O_APPEND)) == -1)
        if ( (loghandle = _creat(logfile,0)) == -1)
            badexit("IO error opening log file for output");

    logfileopen = 1;              /* Save Flag indicating log file is open */
    logcount = 0;
}


/* W R I T E L O G -- Put a char in the logfile buffer, flush if necessary */

void WriteLog( char c) {

    logbuff[logcount++] = c;      /* Store this character in the buffer */

    if (logcount == LOGBUFFSIZE)  /* Check if time to flush buffer */
        FlushLogBuff();
}

/* F L U S H L O G B U F -- Flush the log buffer to disk */

void FlushLogBuff(void) {

    if (logcount > 0)             /* Try and write the chars if any exist */
        if ( write(loghandle,logbuff,logcount) != logcount)
            badexit("IO error writing to log file");

    logcount = 0;                 /* Reset the count of logged characters */
}

/* C L O S E L O G F I L E -- Close the logfile */

void CloseLogFile(void) {

    if (!logfileopen)             /* Ignore if not open */
        return;

    FlushLogBuff();               /* Flush any remaining characters */
                                  /* Try and close the log file */
    if ( close(loghandle) == -1)
       badexit("IO error closing log file");

    logfileopen = 0;              /* Save Flag indicating log file is closed */
}

#endif		/* NODOS */

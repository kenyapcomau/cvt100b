CC=		tcc
DATE=		2000-01-17
CFLAGS=		-mt -DNODOS -DID="$(DATE)"

cvt100.fd:	comboot.bin cvt100.com
		command /c copy/b comboot.bin+cvt100.com cvt100.fd
		command /c rawrite -f cvt100.fd -d a

cvt100.com:	s0.obj comio.obj cvt100.obj fileio.obj keyio.obj vidio.obj vtsetup.obj vttio.obj alink.exe
		-alinkit

s0.obj:		s0.asm
		tasm s0 /l /mx

comio.obj:	comio.c
		$(CC) $(CFLAGS) -c $*.c

cvt100.obj:	cvt100.c
		$(CC) $(CFLAGS) -c $*.c

fileio.obj:	fileio.c
		$(CC) $(CFLAGS) -c $*.c

keyio.obj:	keyio.c
		$(CC) $(CFLAGS) -c $*.c

vidio.obj:	vidio.c
		$(CC) $(CFLAGS) -c $*.c

vtsetup.obj:	vtsetup.c
		$(CC) $(CFLAGS) -c $*.c

vttio.obj:	vttio.c
		$(CC) $(CFLAGS) -c $*.c

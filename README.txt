This is software for a terminal emulator for x86 computers that can be run from
a floppy disk or put on a BIOS extension.

The file tcj.txt contains technical background for the project.

I have no idea what is needed to make this work again. I have no use case for
it so I'm releasing it as-is.

Original README:

cvt100a is a program that turns your PC (even an XT or an AT) with a
serial port into VT100 terminal. It can be put on a floppy disk without
DOS and it will boot directly to the emulator. It was derived from cvt100
from the Simtel archive. I fixed up some syntax errors and ifdefed out
the file handling.

It assumes that com1 is at 0x378 and uses IRQ4 and that com2 is at 0x278
and uses IRQ3. If your hardware doesn't match, it won't work.

The default port settings are 9600,n,8,1 and a rebuild of the program is
required to make different defaults. (You can always change the settings
while the program is running but they don't persist.)  If someone can
suggest a way to save settings on an AT (no non-volatile memory on
diskless XTs), let me know. There is experimental code to save the comm
and video settings to CMOS but unfortunately on my AT motherboard it
causes a checksum error and the BIOS resets the affected bytes to some
sane value so the saved settings can't be recovered.

To create the floppy disk, create cvt100.fd by concatenating comboot.bin
and cvt100.com. Then use rawrite (from rawrite.zip) to write to a floppy
disk, starting at block 0.

You can also run it as a ordinary program under DOS and this might be
a good idea to check out any hardware problems first.  If you want
to be able to return to the DOS prompt, remove the define of NODOS
in the Makefile and recompile.  To recompile from sources, you need
to have TurboC 2.0. It will compile with BC3.1, but I haven't tested
the linking with alink. If you wish to try, look at res3 and Makefil3,
which are replacements for resp and Makefile respectively. Thanks to
Dave Baldwin for the mods to make it ANSI-C.  Copy cs.lib from the
TurboC lib directory into the current directory. Or edit alinkit.bat
(but watch out for command line length limit).

Alink.exe comes from alink.zip, a free linker written by Joseph Allen.
See the doc file for more info. I didn't have much success using tlink
so that's why I used alink.

Two ROMable versions are included in the distribution, called cvt100.rom
and cvt100.lzrom. The latter is compressed. However as both are between 8
and 16kB, you don't save any ROM space, so either one will do.  They are
extension BIOSes, not a main BIOS, so you can't replace the main BIOS
of a motherboard with it. How you install an extension BIOS depends on
your resoucefulness. Traditionally extension BIOSes are found on XT
hard disk controllers, VGA adapters, SCSI adapters, network adapters
(boot ROMs) and so forth. Some motherboards, especially the old ATs,
have spare sockets for extension BIOSes. Otherwise you could look for a
defunct board of the above varieties to install the PROM on.  You need
to be able to install a 16kB PROM (27128).

If you are in Sydney, Australia and need some EPROMs made, feel free to
email me to ask.

The files needed to rebuild the ROM are not in this package but are in
zloader.zip, found in the same place you got cvt100a.zip.  You will
need the bcc (Bruce Evans C compiler) and bin86 tools from the ELKS
(Embedded Linux Kernel Subset) development toolkit. A Linux platform is
also a very good idea but other Unixes should work ok.

Distribution conditions: This software is totally free under the GNU
Public License (conditions of which can be found at the Free Software
Foundation archive) and you can use it in any way.  So there are no fees
or anything to use it. But if you like it, please spread it. Give copies
to friends, send it to a bulletin board, put a link to it on your Web
site, etc. And send me a nice note, it will make my day.  I hate to see
useful hardware go to waste, especially when they are totally adequate
for the job.  If you make any derived versions, please be courteous
enough to retain my credits and that of Jerry Joplin, who wrote the
original version.  If you make any fixes or improvements, please send
them to me for incorporation in any future versions. In particular I
don't know how good a VT100 emulation it is. If you can make it a better
ANSI emulation, please do.

	Ken Yap
	February 2000
	ken_yap@xoommail.com

Changes: In the B version I added a character map to do the best it can
with ISO8859-1 characters.

#include	<dos.h>
#include	"int.h"

union REGS	regs;
struct SREGS	sregs;
int		_BP;

#ifndef	NODOS
static char id[] = "$Id: int.c,v 1.1 1998/06/24 13:07:06 ken Exp $";
#endif

void interrupt10(unsigned ax, unsigned bx, unsigned cx, unsigned dx)
{
#asm
	push	bp
	mov	bp,sp
	mov	ax,4[bp]
	mov	bx,6[bp]
	mov	cx,8[bp]
	mov	dx,10[bp]
	int	#0x10
	pop	bp
#endasm
}

void geninterrupt(int i)
{
	/* we know geninterrupt is only called for int 0x10 */
#asm
	mov	ax,[_regs+0]
	mov	bx,[_regs+2]
	mov	cx,[_regs+4]
	mov	dx,[_regs+6]
	mov	__BP,bp
	int	#0x10
	mov	bp,__BP
	mov	[_regs+0],ax
	mov	[_regs+2],bx
	mov	[_regs+4],cx
	mov	[_regs+6],dx
#endasm
}

void gotoxy(int x, int y)
{
	interrupt10(0x0200, 0x0000, 0x0000, (y - 1) << 16 | (x - 1));
}

void enable(void)
{
#asm
	sti
#endasm
}

void disable(void)
{
#asm
	cli
#endasm
}

int inportb(int port)
{
#asm
	pop	bx
	pop	dx
	dec	sp
	dec	sp
	inb
	sub	ah,ah
	jmp	bx
#endasm
}

void outportb(int port, char value)
{
#asm
	pop	bx
	pop	dx
	pop	ax
	sub	sp,*4
	outb
	jmp	bx
#endasm
}

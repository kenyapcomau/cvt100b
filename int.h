/* $Id: int.h,v 1.1 1998/06/24 13:07:06 ken Exp $ */

extern union REGS	regs;
extern struct SREGS	sregs;
extern int	_BP;

#define	_AX	regs.x.ax
#define	_BX	regs.x.bx
#define	_CX	regs.x.cx
#define	_DX	regs.x.dx
#define	_DI	regs.x.di
#define	_AH	regs.h.ah
#define	_AL	regs.h.al
#define	_BH	regs.h.bh
#define	_CL	regs.h.cl

#define	_ES	sregs.es

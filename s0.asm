; $Id: s0.asm,v 1.2 1998/07/07 11:24:03 ken Exp $

DGROUP	GROUP	_TEXT, _DATA, _EMUSEG, _CRTSEG, _CVTSEG, _SCNSEG, _BSS, _BSSEND
_TEXT	SEGMENT BYTE PUBLIC 'CODE'
_TEXT	ENDS
_DATA	SEGMENT PARA PUBLIC 'DATA'
_DATA	ENDS
_EMUSEG	SEGMENT WORD COMMON 'DATA'
_EMUSEG	ENDS
_CRTSEG	SEGMENT WORD COMMON 'DATA'
_CRTSEG	ENDS
_CVTSEG	SEGMENT WORD PUBLIC 'DATA'
_CVTSEG	ENDS
_SCNSEG	SEGMENT WORD PUBLIC 'DATA'
_SCNSEG	ENDS
_BSS	SEGMENT WORD PUBLIC 'BSS'
_BSS	ENDS
_BSSEND	SEGMENT BYTE PUBLIC 'STACK'
_BSSEND	ENDS

_TEXT	SEGMENT
	assume	cs:_TEXT, ds:DGROUP
	extrn	_main : near
	public	_exit,_abort
	org	100h
	proc	start near
	mov	ax,cs
	mov	ds,ax
	mov	es,ax
	mov	DGROUP@,ax
	cli
	mov	ss,ax
	mov	sp,0FFFEH
	sti
;	Clear BSS
	cld
	xor	ax,ax
	mov	di,offset DGROUP:bdata@
	mov	cx,offset DGROUP:edata@
	sub	cx,di
	rep	stosb
	push	envp
	push	argv
	push	argc
	call	_main
_exit:
_abort:
	mov	ax,4c00h	;we reach here only when DOS is used
	int	21h
	endp
_TEXT	ENDS

_DATA	SEGMENT
	public	DGROUP@
dgroup@	dw	?
	public	_errno
_errno	dw	0
	public	___brklvl
___brklvl	dw	DGROUP:edata@
	public	__RealCvtVector
__RealCvtVector	dw	?
argc	dw	0
argv	dw	offset string
envp	dw	offset string
string	db	0
_DATA	ENDS

_BSS	SEGMENT
bdata@	label	byte
_BSS	ENDS

_BSSEND	SEGMENT
edata@	label	byte
_BSSEND	ENDS

	end

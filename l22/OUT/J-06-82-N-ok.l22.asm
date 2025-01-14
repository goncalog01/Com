segment	.text
align	4
global	_main:function
_main:
	push	ebp
	mov	ebp, esp
	sub	esp, 0
	push	dword $argc
	pop	eax
	push	dword [eax]
	pop	eax
	call	eax
	push	eax
	call	printi
	add	esp, 4
	call	println
	push	dword 0
	pop	eax
	leave
	ret
	leave
	ret
extern	argc
extern	printi
extern	println

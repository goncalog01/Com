segment	.text
align	4
global	_main:function
_main:
	push	ebp
	mov	ebp, esp
	sub	esp, 0
	push	dword 100
	push	dword 100
	pop	eax
	sub	dword [esp], eax
	call	printi
	add	esp, 4
	call	println
	push	dword 0
	pop	eax
	leave
	ret
	leave
	ret
extern	printi
extern	println

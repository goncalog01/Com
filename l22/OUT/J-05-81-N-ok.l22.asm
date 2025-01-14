segment	.data
align	4
pi:
segment	.text._L0
align	4
_L0:
	push	ebp
	mov	ebp, esp
	sub	esp, 0
	push	dword 314
	pop	eax
	leave
	ret
	leave
	ret
segment	.data
	dd	_L0
segment	.text
align	4
global	_main:function
_main:
	push	ebp
	mov	ebp, esp
	sub	esp, 0
	push	dword $pi
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
extern	printi
extern	println

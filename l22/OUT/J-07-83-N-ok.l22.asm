segment	.data
align	4
f:
segment	.text._L0
align	4
_L0:
	push	ebp
	mov	ebp, esp
	sub	esp, 4
	push	dword 1
	lea	eax, [ebp+-4]
	push	eax
	pop	ecx
	pop	eax
	mov	[ecx], eax
	push	dword 2
	lea	eax, [ebp+-4]
	push	eax
	pop	eax
	push	dword [eax]
	pop	eax
	imul	dword eax, [esp]
	mov	[esp], eax
	lea	eax, [ebp+-4]
	push	eax
	pop	eax
	push	dword [eax]
	pop	eax
	add	dword [esp], eax
	call	printi
	add	esp, 4
	call	println
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
	push	dword $f
	pop	eax
	push	dword [eax]
	pop	eax
	call	eax
	push	eax
	add	esp, 0
	push	dword 0
	pop	eax
	leave
	ret
	leave
	ret
extern	printi
extern	println

segment	.data
align	4
r:
	dq	61.0000
segment	.data
align	4
global	main:object
main:
segment	.text._L0
align	4
_L0:
	push	ebp
	mov	ebp, esp
	sub	esp, 0
	lea	eax, [ebp+8]
	push	eax
	pop	eax
	push	dword [eax]
	push	dword 0
	push	dword 8
	pop	eax
	imul	dword eax, [esp]
	mov	[esp], eax
	pop	eax
	add	dword [esp], eax
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
	fld	qword [esp]
	add	esp, byte 8
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
	push	dword $r
	push	dword $main
	pop	eax
	push	dword [eax]
	pop	eax
	call	eax
	add	esp, 4
	push	eax
	call	printd
	add	esp, 8
	call	println
	push	dword 0
	pop	eax
	leave
	ret
	leave
	ret
extern	printd
extern	println

segment	.data
align	4
f:
segment	.text._L0
align	4
_L0:
	push	ebp
	mov	ebp, esp
	sub	esp, 0
	lea	eax, [ebp+12]
	push	eax
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
	lea	eax, [ebp+8]
	push	eax
	pop	eax
	push	dword [eax]
	fild	dword [esp]
	sub	esp, byte 4
	fstp	qword [esp]
	fld	qword [esp]
	add	esp, byte 8
	fld	qword [esp]
	faddp	st1
	fstp	qword [esp]
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
	push	dword 10
	fild	dword [esp]
	sub	esp, byte 4
	fstp	qword [esp]
	push	dword 3
	push	dword $f
	pop	eax
	push	dword [eax]
	pop	eax
	call	eax
	add	esp, 12
	push	eax
	call	printd
	add	esp, 8
	call	println
	push	dword 3
	fild	dword [esp]
	sub	esp, byte 4
	fstp	qword [esp]
	push	dword 2
	push	dword $f
	pop	eax
	push	dword [eax]
	pop	eax
	call	eax
	add	esp, 12
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

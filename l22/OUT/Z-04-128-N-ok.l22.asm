segment	.text
align	4
global	_main:function
_main:
	push	ebp
	mov	ebp, esp
	sub	esp, 0
	push	dword 1
	call	printi
	add	esp, 4
	push	dword 0
	call	_L1
segment	.text._L1
align	4
_L1:
	push	ebp
	mov	ebp, esp
	sub	esp, 0
	lea	eax, [ebp+4]
	push	eax
	pop	eax
	push	dword [eax]
	pop	eax
	call	eax
	push	dword 1
	pop	eax
	add	dword [esp], eax
	pop	eax
	leave
	ret
	leave
	ret
segment	.text
	push	dword $_L1
	add	esp, 4
	push	eax
	push	dword 1
	pop	eax
	add	dword [esp], eax
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

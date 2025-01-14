segment	.data
align	4
f:
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
	push	dword 1
	pop	eax
	add	dword [esp], eax
	pop	eax
	leave
	ret
	leave
	ret
segment	.data
	dd	_L0
segment	.data
align	4
g:
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
	lea	eax, [ebp+12]
	push	eax
	pop	eax
	push	dword [eax]
	pop	eax
	call	eax
	add	esp, 4
	push	eax
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
	push	dword $f
	pop	eax
	push	dword [eax]
	push	dword 3
	push	dword $g
	pop	eax
	push	dword [eax]
	pop	eax
	call	eax
	add	esp, 8
	push	eax
	call	printi
	add	esp, 4
	call	println
segment	.text._L32765
align	4
_L32765:
	push	ebp
	mov	ebp, esp
	sub	esp, 0
	lea	eax, [ebp+4]
	push	eax
	pop	eax
	push	dword [eax]
	push	dword 2
	pop	eax
	imul	dword eax, [esp]
	mov	[esp], eax
	pop	eax
	leave
	ret
	leave
	ret
segment	.text
	push	dword $_L32765
	push	dword 3
	push	dword $g
	pop	eax
	push	dword [eax]
	pop	eax
	call	eax
	add	esp, 8
	push	eax
	call	printi
	add	esp, 4
	call	println
segment	.text._L32765
align	4
_L32765:
	push	ebp
	mov	ebp, esp
	sub	esp, 0
	lea	eax, [ebp+4]
	push	eax
	pop	eax
	push	dword [eax]
	push	dword 2
	pop	eax
	imul	dword eax, [esp]
	mov	[esp], eax
	pop	eax
	leave
	ret
	leave
	ret
segment	.text
	push	dword $_L32765
	push	dword 8
	call	_L1
segment	.text._L1
align	4
_L1:
	push	ebp
	mov	ebp, esp
	sub	esp, 0
	push	dword 7
	lea	eax, [ebp+4]
	push	eax
	pop	eax
	push	dword [eax]
	pop	eax
	call	eax
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
	push	dword $g
	pop	eax
	push	dword [eax]
	pop	eax
	call	eax
	add	esp, 8
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

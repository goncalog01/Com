segment	.bss
align	4
global	g:object
g:
	resb	4
segment	.data
align	4
f:
segment	.text._L0
align	4
_L0:
	push	ebp
	mov	ebp, esp
	sub	esp, 4
	push	dword 9
	lea	eax, [ebp+-8]
	push	eax
	pop	ecx
	pop	eax
	mov	[ecx], eax
	lea	eax, [ebp+8]
	push	eax
	pop	eax
	push	dword [eax]
	push	dword 0
	pop	eax
	xor	ecx, ecx
	cmp	[esp], eax
	sete	cl
	mov	[esp], ecx
	pop	eax
	cmp	eax, byte 0
	je	near _L1
	push	dword 0
	push	dword [esp]
	lea	eax, [ebp+-8]
	push	eax
	pop	ecx
	pop	eax
	mov	[ecx], eax
	add	esp, 4
	jmp	dword _L2
_L1:
	lea	eax, [ebp+8]
	push	eax
	pop	eax
	push	dword [eax]
	lea	eax, [ebp+8]
	push	eax
	pop	eax
	push	dword [eax]
	push	dword 1
	pop	eax
	sub	dword [esp], eax
	push	dword $g
	pop	eax
	push	dword [eax]
	pop	eax
	call	eax
	add	esp, 4
	push	eax
	pop	eax
	add	dword [esp], eax
	push	dword [esp]
	lea	eax, [ebp+-8]
	push	eax
	pop	ecx
	pop	eax
	mov	[ecx], eax
	add	esp, 4
_L2:
	lea	eax, [ebp+-8]
	push	eax
	pop	eax
	push	dword [eax]
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
	push	dword 3
	push	dword $f
	pop	eax
	push	dword [eax]
	pop	eax
	call	eax
	add	esp, 4
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

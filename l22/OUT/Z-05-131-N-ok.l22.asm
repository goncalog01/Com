segment	.text
align	4
global	_main:function
_main:
	push	ebp
	mov	ebp, esp
	sub	esp, 0
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
	push	dword 1
	pop	eax
	add	dword [esp], eax
	pop	eax
	leave
	ret
	leave
	ret
segment	.text
	push	dword $_L32765
	push	dword [esp]
	lea	eax, [ebp+-8]
	push	eax
	pop	ecx
	pop	eax
	mov	[ecx], eax
	push	dword [esp]
	lea	eax, [ebp+-12]
	push	eax
	pop	ecx
	pop	eax
	mov	[ecx], eax
	add	esp, 4
	push	dword 1
	lea	eax, [ebp+-12]
	push	eax
	pop	eax
	push	dword [eax]
	pop	eax
	call	eax
	add	esp, 4
	push	eax
	call	printi
	add	esp, 4
	call	println
	push	dword 2
	lea	eax, [ebp+-8]
	push	eax
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

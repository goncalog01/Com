segment	.bss
align	4
f:
	resb	4
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
	sub	esp, 4
	push	dword 3
	lea	eax, [ebp+-4]
	push	eax
	pop	ecx
	pop	eax
	mov	[ecx], eax
	lea	eax, [ebp+-4]
	push	eax
	pop	eax
	push	dword [eax]
	call	printi
	add	esp, 4
	call	println
	leave
	ret
segment	.text
	push	dword $_L32765
	push	dword [esp]
	push	dword $f
	pop	ecx
	pop	eax
	mov	[ecx], eax
	add	esp, 4
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

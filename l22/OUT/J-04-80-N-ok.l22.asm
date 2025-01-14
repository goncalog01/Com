segment	.bss
align	4
global	f:object
f:
	resb	4
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

segment	.text
align	4
global	_main:function
_main:
	push	ebp
	mov	ebp, esp
	sub	esp, 0
	push	dword 4
	call	printi
	add	esp, 4
	call	println
	leave
	ret
extern	printi
extern	println

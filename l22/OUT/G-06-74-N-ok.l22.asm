segment	.data
align	4
i:
	dq	1.10000
segment	.text
align	4
global	_main:function
_main:
	push	ebp
	mov	ebp, esp
	sub	esp, 0
	push	dword 8
	call	printi
	add	esp, 4
	call	println
	leave
	ret
extern	printi
extern	println

%include 'auxillary.asm'
BITS 32
GLOBAL _start
SECTION	.bss
b5b567       resd		1
c3bd         resd		1
d3           resd		1
bo1 resd 255
bo0 resd 255
SECTION	.text
_start:
mov	eax,0
mov	ebx,0
	add eax,1
	mov [b5b567],eax
mov	eax,0
mov	ebx,0
	add eax,0
	mov [d3],eax
	Label0: 	mov eax,[b5b567]
	mov ebx,7
	cmp eax,ebx
	setle dl
je Label1
	call _stdin1
	mov [c3bd],eax
	mov	eax,0
mov	ebx,0
	add eax,[d3]
	add eax,[c3bd]
	mov [d3],eax
mov	eax,0
mov	ebx,0
	add eax,[b5b567]
	add eax,1
	mov [b5b567],eax
	JMP Label0
	Label1: 	mov eax,[d3]
	call _stdout1
		mov eax,1
	 mov ebx,0
	 int 80h

%include 'auxillary.asm'
BITS 32
GLOBAL _start
SECTION	.bss
d5cb34567    resd		1
b3c2.maths   resd		1
b3c2.physics   resd		1
b3c2.chemistry   resd		1
b5           resd		1
d4.maths   resd		1
d4.physics   resd		1
d4.chemistry   resd		1
bufferop1 resd 255
bufferop0 resd 255
SECTION	.text
_start:
mov	eax,0
mov	ebx,0
	add eax,1
	mov [b5],eax
	call _stdin1
	mov [d5cb34567],eax
	mov	eax,0
mov	ebx,0
	add eax,0
	mov [d4.maths],eax
mov	eax,0
mov	ebx,0
	add eax,0
	mov [d4.physics],eax
mov	eax,0
mov	ebx,0
	add eax,0
	mov [d4.chemistry],eax
	Label0: 	mov eax,[b5]
	mov ebx,[d5cb34567]
	cmp eax,ebx
	setle dl
je Label1
	call _stdin1
	mov [b3c2.maths],eax
		call _stdin1
	mov [b3c2.physics],eax
		call _stdin1
	mov [b3c2.chemistry],eax
	mov	eax,0
mov	ebx,0
push eax
 mov eax,[b3c2.maths]
mov [bufferop1+0],eax
pop eax
push eax
 mov eax,[b3c2.physics]
mov [bufferop1+4],eax
pop eax
push eax
 mov eax,[b3c2.chemistry]
mov [bufferop1+8],eax
pop eax
push eax
push ebx
mov eax,[d4.maths]
 mov ebx,[bufferop1+0]
add ebx,eax
mov [bufferop1+0],ebx
pop ebx
pop eax
push eax
push ebx
mov eax,[d4.physics]
 mov ebx,[bufferop1+4]
add ebx,eax
mov [bufferop1+4],ebx
pop ebx
pop eax
push eax
push ebx
mov eax,[d4.chemistry]
 mov ebx,[bufferop1+8]
add ebx,eax
mov [bufferop1+8],ebx
pop ebx
pop eax
push eax
 mov eax,[bufferop1+0]
mov [d4.maths],eax
pop eax
push eax
 mov eax,[bufferop1+4]
mov [d4.physics],eax
pop eax
push eax
 mov eax,[bufferop1+8]
mov [d4.chemistry],eax
pop eax
mov	eax,0
mov	ebx,0
	add eax,[b5]
	add eax,1
	mov [b5],eax
	JMP Label0
	Label1: 	mov eax,[d4.maths]
	call _stdout1
	mov eax,[d4.physics]
	call _stdout1
		mov eax,[d4.chemistry]
	call _stdout1
		mov eax,1
	 mov ebx,0
	 int 80h

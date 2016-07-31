_stdin1:
section .bss
num	resd	1
arr	resb	255
neg1	resb	1	
section .text
		pushad		
		pushfd		
		mov	esi, arr
		mov	edi, 0	
.loop1:	
		mov	eax, 03	
		mov	ebx, 0	
		mov	ecx, esi
		mov	edx, 1	
		int	0x80	
		cmp	byte[esi], 0
		je	 .parse
		cmp	byte[esi], 10
		je	 .parse
		inc	esi
		inc	edi	
		jmp	.loop1
.parse:
		mov	esi, arr	
		mov	ecx, edi	
		mov	dword[num], 0
		mov	byte[neg1], 0
.negativ:	
 		cmp	byte[esi], '-'
		jne	.loop
		inc	esi
		inc	byte[neg1]
.loop:	  mov	ebx, 0
		mov	bl, byte[esi]	
 		cmp	bl, 10	
  		je	 .done	
		cmp	bl, '0'
		jb	 .done
		cmp	bl, '9'
		ja	 .done	
		mov	edx, 10
		mov	eax, dword[num]
		mul	edx	
 		sub	bl, '0'
		add	eax, ebx
		mov	dword[num], eax
		inc	esi
		loop    .loop
.done:
  		cmp	byte[neg1], 0
		je	 .return
		neg	eax
		mov	dword [num], eax
.return:
		popfd
		popad
		mov	eax, [num]
		ret
_Print:
section		.bss
count		resd		1
deci		resb		10
section .text
		pushad
		pushfd
		mov		 dword[count],0
		mov		 edi,deci
		add		 edi,9
		xor		 edx,edx	
label1:
		mov		 ebx,10	
		div		 ebx	
		add		 edx,'0'
		mov		 byte[edi],dl
		dec		 edi
		inc		 dword[count] 
		xor		 edx,edx
		cmp		 eax,0
		jne		 label1
		inc		 edi
		mov		 ecx, edi	 
		mov		 edx, [count]
		mov		 eax, 4
		mov		 ebx, 1
		int		 0x80
		popfd
		popad
		ret
_stdout1:
section .data
		minus db '-'
		lf		 db		10
section .text
		pushad
		pushfd
		cmp eax, 0000h
		jge X1
		neg eax
		push eax
		mov eax,4
		mov ebx, 1
		mov ecx, minus
		mov edx, 1
		int 80h
		pop eax
		X1:
		call _Print
		popfd
		popad
		push		ecx
		push		edx
		mov		ecx, lf
		mov		edx, 1
		push		eax
		push		ebx
		mov		eax,4
		mov		ebx,1
		int		0x80
		pop		ebx
		pop		eax
		pop		edx
		pop		ecx
		ret

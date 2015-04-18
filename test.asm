.686p
.model flat
includelib MSVCRT.lib
.data
extrn _printf: proc
fmtd db '%d', 10, 0
_a dword 0
.code
main proc
	push offset _a
	push dword ptr 3
	push dword ptr 1
	pop ebx
	pop eax
	xor ecx, ecx
	cmp eax, ebx
	setg cl
	push ecx
	pop ebx
	pop eax
	mov [eax], ebx
	push eax
	pop eax
	push _a
	push offset fmtd
	call _printf
	add esp, 8
ret
main endp
end

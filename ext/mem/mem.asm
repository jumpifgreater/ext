.code

Mem_ReadVirtualMemory PROC
	mov r10, rcx
	mov eax, 63
	syscall
	ret
Mem_ReadVirtualMemory ENDP

Mem_WriteVirtualMemory PROC
	mov r10, rcx
	mov eax, 58
	syscall
	ret
Mem_WriteVirtualMemory ENDP

END
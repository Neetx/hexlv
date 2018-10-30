extern kernel_main
global start

section .text
bits 32
start:
	; Link p4 first entry to p3 first entry
	mov eax, p3_table
	or eax, 0b11					; Set fist 2 bits(present and writable bit) at 1
	mov dword [p4_table + 0], eax

	; Link p3 to p2
	mov eax, p2_table
	or eax, 0b11
	mov dword [p3_table + 0], eax

	; Link each page table two entry to a page
	mov ecx, 0	; counter
	.map_p2_table:
		mov eax, 0x200000	; 2Mib size of each page
		mul ecx
		or eax, 0b10000011	; Extra bit called huge page bit, without it we have 4KiB pages
		mov [p2_table + ecx * 8], eax

		inc ecx
		cmp ecx, 512
		jne .map_p2_table

	; Move page table addr to cr3
	mov eax, p4_table
	mov cr3, eax 	; control register 3

	; Physical Address Extension ---> PAE
	mov eax, cr4
	or eax, 1 << 5
	mov cr4, eax

	; long mode bit
	mov ecx, 0xC0000080
	rdmsr				; read model specific register
	or eax, 1 << 8
	wrmsr				; write model specific register

	; enable pagining
	mov eax, cr0
	or eax, 1 << 31
	or eax, 1 << 16
	mov cr0, eax   ; cr0 was the target

	; now we are in compatibility mode

	lgdt [gdt64.pointer]	; load global descriptor table

	;gdt loaded
	;update segment registers, they are 16 bit so we use ax

	mov ax, gdt64.data
	mov ss, ax		;stack segment
	mov ds, ax		;data segment
	mov es, ax		;extra segment

	;jump to long mode
	jmp gdt64.code:kernel_main  ;long jump to update code selector register with our entry in gdt
	;code selector cannot be updated manually

	hlt

section .bss
align 4096

p4_table:
	resb 4096
p3_table:
	resb 4096
p2_table:
	resb 4096

section .rodata
gdt64:
	dq 0

;gdt64.code as sublabel referenced by an offset
.code: equ $ - gdt64
	dq (1<<44) | (1<<47) | (1<<41) | (1<<43) | (1<<53)  ; pipe used because here can t be asm code, it means or
	; 44 descriptor type, 47 present: valid entry, 41 readwrite: readable, 43 executable, 53 64bit

.data: equ $ - gdt64
	dq (1<<44) | (1<<47) | (1<<41)  ;41 tells data segments are writable

; 2 bytes for length an 8 byte for address, to pass to lgdt
.pointer:
	dw .pointer - gdt64 - 1
	dq gdt64
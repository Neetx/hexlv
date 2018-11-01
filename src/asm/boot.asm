extern kernel_main
global start

section .text
bits 32
start:
	
	mov esp, stack_top

	call check_multiboot
	call check_cpuid
	call check_long
	call setup_paging
	call enable_paging

	; now we are in compatibility mode 44 6f 6e 65

	lgdt [gdt64.pointer]	; load global descriptor table

	;gdt loaded

	;update segment registers, they are 16 bit so we use ax
	mov ax, gdt64.data
	mov ss, ax		; stack segment
	mov ds, ax		; data segment
	mov es, ax		; extra segment
	mov fs, ax		; TIB pointer
	mov gs, ax		; TLS pointer

	;jump to long mode	
	jmp gdt64.code:start64
	;long jump to update code selector register with our entry in gdt
	;code selector cannot be updated manually

	hlt

check_multiboot:
	cmp eax, 0x36d76289 ; multiboot bootloader put this magic number to eax, we can check
	jne .no_multiboot
	ret
.no_multiboot:
	mov al, "0"
	jmp error

; we work with flags indirectly
check_cpuid:
	pushfd			; copy flags to eax thanks to the stack
	pop eax 		
	mov ecx, eax 	; copy to ecx
	xor eax, 1<<21	; flig the ID bit, if we can -> cpuid is available to get cpu info
	push eax 		; copy eax to flag by stack
	popfd			
	pushfd			; flags to eax
	pop eax
	push ecx 		; restore old flags from ecx
	popfd
	cmp eax, ecx 	; if they are equal -> CPUID not supported
	je .no_cpuid
	ret
.no_cpuid:
	mov al, "1"
	jmp error

check_long:
	; test extended processor infp
	mov eax, 0x80000000
	cpuid 					; get highest supported argument
	cmp eax, 0x80000001		; if highest is less cpu understand us
	jb .no_long				; old cpu
	
	; use extended info to test long mode
	mov eax, 0x80000001
	cpuid 					; get output in ecx and edx
	test edx, 1<<29			; Long Mode bit is set in D-register
	jz .no_long				; if not -> no long mode
	ret
.no_long:
	mov al, "2"
	jmp error

setup_paging:
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

	ret

enable_paging:
	; Move page table addr to cr3
	mov eax, p4_table
	mov cr3, eax 	; control register 3

	; Physical Address Extension ---> enable PAE flag
	mov eax, cr4
	or eax, 1 << 5
	mov cr4, eax

	; long mode bit in EFER MSR
	mov ecx, 0xC0000080
	rdmsr				; read model specific register
	or eax, 1 << 8
	wrmsr				; write model specific register

	; enable pagining
	mov eax, cr0
	or eax, 1 << 31
	or eax, 1 << 16
	mov cr0, eax   ; cr0 was the target

	ret

error:
	mov dword[0xb8000], 0x4f524f45
	mov dword[0xb8004], 0x4f3a4f52
	mov dword[0xb8008], 0x4f204f20
	mov byte[0xb800a], al
	hlt
	
section .bss
align 4096

p4_table:
	resb 4096
p3_table:
	resb 4096
p2_table:
	resb 4096
stack_bottom:
	resb 64
stack_top:

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

;long mode
section .text
bits 64

global start64
start64:
	call kernel_main
	cli
	hlt
section .note.GNU-stack noalloc noexec nowrite progbits
section .text
global factorial_asm

; Input: EDI (int num)
; Output: RAX (long result)
factorial_asm:
    mov eax, 1            ; Initialize result = 1
    test edi, edi
    jle .done             ; Return 1 if num <= 0
    
    mov ecx, edi          ; Counter = num
    
.loop:
    imul rax, rcx         ; result *= counter
    dec ecx               ; counter--
    jnz .loop             ; Continue while counter > 0
    
.done:
    ret

section .text
    global isMagic

isMagic:
    push rbp
    mov rbp, rsp
    sub rsp, 16            ; Reserve space if needed

    mov rax, rdi           ; rax = num

.top_loop:
    cmp rax, 9
    jle .check             ; If num <= 9, stop

    xor rsi, rsi           ; sum = 0

.sum_digits:
    cmp rax, 0
    je .update_sum
    xor rdx, rdx
    mov rbx, 10
    div rbx               ; rdx = digit, rax = num / 10
    add rsi, rdx          ; sum += digit
    jmp .sum_digits

.update_sum:
    mov rax, rsi          ; num = sum
    jmp .top_loop

.check:
    cmp rax, 1
    je .magic
    mov rax, 0
    jmp .done

.magic:
    mov rax, 1

.done:
    add rsp, 16
    pop rbp
    ret

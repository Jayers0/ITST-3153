#include <iostream>

using namespace std;

int main()
{ 
    __asm__ volatile(
section .data
    grid_size equ 15
    block_percentage equ 10

section .bss
    grid resb grid_size * grid_size * 16  ; Each node is 16 bytes

section .text
    global _start

_start:
    ; Seed the random number generator
    mov eax, 0
    mov ecx, 1
    xor edx, edx
    int 0x80

    ; Create grid
    call create_grid

    ; Display grid
    call display_grid

    ; Find path
    call find_path

    ; Cleanup and exit
    call free_grid
    mov eax, 1  ; exit syscall
    xor ebx, ebx
    int 0x80

create_grid:
    ; Allocate memory for the grid
    mov eax, grid_size * grid_size * 16
    mov ebx, 0x01  ; Allocate memory syscall
    mov ecx, 0x03  ; PROT_READ | PROT_WRITE
    mov edx, 0x22  ; MAP_PRIVATE | MAP_ANONYMOUS
    mov esi, -1
    mov edi, 0
    int 0x80
    mov dword [grid], eax  ; Store grid pointer in grid variable

    ; Generate grid nodes
    xor esi, esi
    mov ecx, grid_size
create_grid_loop:
    push ecx
    xor edi, edi
    mov ecx, grid_size
create_grid_inner_loop:
    mov dword [eax + esi * 16], edi  ; Node x
    mov dword [eax + esi * 16 + 4], esi  ; Node y
    cmp dword [esi * 16 + esi * 4], 0
    jnl create_grid_set_blocked
    mov dword [eax + esi * 16 + 8], 1  ; Blocked flag
    jmp create_grid_continue
create_grid_set_blocked:
    xor dword [eax + esi * 16 + 8], 0  ; Blocked flag (clear)
create_grid_continue:
    inc edi
    add esi, 4
    loop create_grid_inner_loop
    pop ecx
    dec ecx
    jnz create_grid_loop
    ret

display_grid:
    ; Display grid
    xor esi, esi
    mov ecx, grid_size
display_grid_loop:
    push ecx
    xor edi, edi
    mov ecx, grid_size
display_grid_inner_loop:
    cmp dword [eax + esi * 16 + 8], 0  ; Blocked flag
    jnz display_grid_blocked
    mov ebx, 0x2E  ; ASCII code for '.'
    jmp display_grid_print_char
display_grid_blocked:
    mov ebx, 0x23  ; ASCII code for '#'
display_grid_print_char:
    mov eax, 0x04  ; write syscall
    mov ecx, 0x01  ; stdout
    xor edx, edx
    mov dl, 0x01  ; character count
    int 0x80

    inc edi
    add esi, 4
    loop display_grid_inner_loop
    pop ecx
    dec ecx
    jnz display_grid_loop
    ret

find_path:
    ; Find path
    ; TODO: Implement the A* algorithm in assembly
    ;       using the provided grid

    ret

free_grid:
    ; Free memory allocated for the grid
    mov eax, [grid]
    mov ebx, 0x02  ; munmap syscall
    xor ecx, ecx
    mov edx, grid_size * grid_size * 16
    int 0x80
    ret
)
    return 0;
}
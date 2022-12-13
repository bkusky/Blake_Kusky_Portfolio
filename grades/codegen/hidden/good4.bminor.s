.file "/escnfs/home/nziems2/compiler-fa22-solution/hidden-tests/codegen/good4.bminor"
.data
.text
.globl collatz
collatz:
	pushq %rbp
	movq %rsp, %rbp
	pushq %rdi
	pushq %rsi
	subq $0, %rsp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	movq -8(%rbp), %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
call print_integer
	popq %r11
	popq %r10
	movq $10, %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
call print_character
	popq %r11
	popq %r10
	movq -8(%rbp), %rbx
	movq $1, %r10
	cmp %r10, %rbx
	je .L3
	movq $0, %rbx
	jmp .L4
.L3:
	movq $1, %rbx
.L4:
	cmp $0, %rbx
	je .L1
	movq -16(%rbp), %rbx
	movq %rbx, %rax
	jmp .collatz_epilogue
	jmp .L2
.L1:
.L2:
	movq -8(%rbp), %rbx
	movq $2, %r10
	movq %rbx, %rax
	movq $0, %rbx
	movq $0, %rcx
	movq $0, %rdx
	idivq %r10
	movq %rdx, %r10
	movq $0, %rbx
	cmp %rbx, %r10
	je .L7
	movq $0, %rbx
	jmp .L8
.L7:
	movq $1, %rbx
.L8:
	cmp $0, %rbx
	je .L5
	movq -8(%rbp), %rbx
	movq $2, %r10
	movq %rbx, %rax
	movq $0, %rbx
	movq $0, %rcx
	movq $0, %rdx
	idivq %r10
	movq %rax, %r10
	movq -8(%rbp), %rbx
	movq $1, %r11
	addq %rbx, %r11
	movq %r10, %rdi
	movq %r11, %rsi
	pushq %r10
	pushq %r11
	call collatz
	popq %r11
	popq %r10
	movq %rax, %rbx
	movq %rbx, %rax
	jmp .collatz_epilogue
	jmp .L6
.L5:
	movq $3, %rbx
	movq -8(%rbp), %r10
	movq %rbx, %rax
	imulq %r10
	movq %rax, %r10
	movq $1, %rbx
	addq %r10, %rbx
	movq -8(%rbp), %r10
	movq $1, %r11
	addq %r10, %r11
	movq %rbx, %rdi
	movq %r11, %rsi
	pushq %r10
	pushq %r11
	call collatz
	popq %r11
	popq %r10
	movq %rax, %rbx
	movq %rbx, %rax
	jmp .collatz_epilogue
.L6:
.collatz_epilogue:
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	movq %rbp, %rsp
	popq %rbp
	ret
.globl main
main:
	pushq %rbp
	movq %rsp, %rbp
	subq $0, %rsp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	movq $27, %rbx
	movq $0, %r10
	movq %rbx, %rdi
	movq %r10, %rsi
	pushq %r10
	pushq %r11
	call collatz
	popq %r11
	popq %r10
	movq %rax, %rbx
	movq %rbx, %rax
	jmp .main_epilogue
.main_epilogue:
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	movq %rbp, %rsp
	popq %rbp
	ret

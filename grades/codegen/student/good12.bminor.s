.file "./codegen_tests/good12.bminor"
.data
.text
.globl main
main:
	pushq %rbp
	movq %rsp, %rbp
	subq $8, %rsp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	movq $0, -8(%rbp)
	movq $5, %rbx
	movq $2, %r10
	movq $1, %r12
	movq $0, %r11
.L1:
	cmp %r11, %r10
	je .L2
	addq $1, %r11
	movq %rbx, %rax
	imulq %r12
	movq %rax, %r12
	jmp .L1
.L2:
	movq %r12, %r10
	movq %r10, -8(%rbp)
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
	movq $0, %rbx
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

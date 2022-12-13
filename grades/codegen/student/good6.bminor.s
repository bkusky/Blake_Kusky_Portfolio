.file "./codegen_tests/good6.bminor"
.data
.text
.globl main
main:
	pushq %rbp
	movq %rsp, %rbp
	subq $32, %rsp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	movq $5, %rbx
	movq %rbx, -8(%rbp)
	movq -8(%rbp), %rbx
	movq $5, %r10
	addq %rbx, %r10
	movq $10, %rbx
	movq %r10, %rax
	imulq %rbx
	movq %rax, %rbx
	movq $50, %r10
	subq %r10, %rbx
	movq %rbx, -16(%rbp)
	movq -16(%rbp), %rbx
	movq $19, %r10
	addq %rbx, %r10
	movq %r10, -24(%rbp)
	movq -24(%rbp), %rbx
	movq $1000, %r10
	movq %rbx, %rax
	imulq %r10
	movq %rax, %r10
	movq -16(%rbp), %rbx
	movq $8, %r11
	movq %rbx, %rax
	imulq %r11
	movq %rax, %r11
	addq %r10, %r11
	movq -8(%rbp), %rbx
	movq $4, %r10
	movq %rbx, %rax
	imulq %r10
	movq %rax, %r10
	addq %r11, %r10
	movq %r10, -32(%rbp)
	movq -32(%rbp), %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
call print_integer
	popq %r11
	popq %r10
.data
.L1:
	.string "\n"
.text
	leaq .L1, %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
call print_string
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

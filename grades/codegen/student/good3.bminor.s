.file "./codegen_tests/good3.bminor"
.data
.text
.globl main
main:
	pushq %rbp
	movq %rsp, %rbp
	subq $24, %rsp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	movq $1, %rbx
	movq %rbx, -8(%rbp)
	movq $10, %rbx
	movq %rbx, -16(%rbp)
	movq -16(%rbp), %rbx
	movq -8(%rbp), %r10
	subq %r10, %rbx
	movq %rbx, -24(%rbp)
	movq -24(%rbp), %rbx
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

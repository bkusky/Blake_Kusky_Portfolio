.file "./codegen_tests/good5.bminor"
.data
.text
.globl main
main:
	pushq %rbp
	movq %rsp, %rbp
	subq $16, %rsp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	movq $2, %rbx
	movq %rbx, -8(%rbp)
	movq $16, %rbx
	movq -8(%rbp), %r10
	movq %rbx, %rax
	movq $0, %rbx
	movq $0, %rcx
	movq $0, %rdx
	idivq %r10
	movq %rax, %r10
	movq %r10, -16(%rbp)
	movq -16(%rbp), %rbx
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

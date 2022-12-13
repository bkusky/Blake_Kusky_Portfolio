.file "/escnfs/home/nziems2/compiler-fa22-solution/hidden-tests/codegen/good15.bminor"
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
.data
.L1:
	.string "sssss"
.text
	leaq .L1, %rbx
	movq %rbx, -8(%rbp)
	movq -8(%rbp), %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
	call strdup
	popq %r11
	popq %r10
	movq %rax, %rbx
	movq %rbx, -16(%rbp)
	movq -8(%rbp), %rbx
	movq -16(%rbp), %r10
	cmp %r10, %rbx
	je .L4
	movq $0, %rbx
	jmp .L5
.L4:
	movq $1, %rbx
.L5:
	cmp $0, %rbx
	je .L2
	movq $22, %rbx
	movq %rbx, %rax
	jmp .main_epilogue
	jmp .L3
.L2:
	movq $11, %rbx
	movq %rbx, %rax
	jmp .main_epilogue
.L3:
.main_epilogue:
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	movq %rbp, %rsp
	popq %rbp
	ret

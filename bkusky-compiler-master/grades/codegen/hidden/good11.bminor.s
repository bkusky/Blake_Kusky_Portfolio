.file "/escnfs/home/nziems2/compiler-fa22-solution/hidden-tests/codegen/good11.bminor"
.data
.text
.globl main
main:
	pushq %rbp
	movq %rsp, %rbp
	subq $40, %rsp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	movq $20, %rbx
	movq %rbx, -8(%rbp)
	movq -8(%rbp), %rbx
	movq %rbx, -16(%rbp)
	movq $10, %rbx
	movq %rbx, -8(%rbp)
	movq -16(%rbp), %rbx
	movq -8(%rbp), %r10
	addq %rbx, %r10
	movq %r10, -16(%rbp)
	movq -16(%rbp), %rbx
	movq -8(%rbp), %r11
	addq %rbx, %r11
	movq %r11, -16(%rbp)
	movq $2, %rbx
	movq %rbx, -8(%rbp)
	movq -16(%rbp), %rbx
	movq -8(%rbp), %r12
	addq %rbx, %r12
	movq %r12, -16(%rbp)
	movq -16(%rbp), %rbx
	movq -8(%rbp), %r13
	addq %rbx, %r13
	movq %r13, -16(%rbp)
	movq $3, %rbx
	movq %rbx, -8(%rbp)
	movq -16(%rbp), %rbx
	movq -8(%rbp), %r14
	addq %rbx, %r14
	movq %r14, -16(%rbp)
	movq -16(%rbp), %rbx
	movq -8(%rbp), %r15
	addq %rbx, %r15
	movq %r15, %rax
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

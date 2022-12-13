.file "/escnfs/home/nziems2/compiler-fa22-solution/hidden-tests/codegen/good12.bminor"
.data
.text
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
	movq $4, %rbx
	movq $9, %r10
	movq $2, %r11
	neg %r11
	movq $5, %r12
	neg %r12
	subq %r12, %r11
	movq %r10, %rax
	movq $0, %rbx
	movq $0, %rcx
	movq $0, %rdx
	idivq %r11
	movq %rax, %r11
	addq %rbx, %r11
	movq $2, %rbx
	movq $1, %r10
	addq %rbx, %r10
	movq $4, %rbx
	movq %r10, %rax
	imulq %rbx
	movq %rax, %rbx
	movq $1, %r10
	subq %r10, %rbx
	movq $3, %r10
	movq %rbx, %rax
	movq $0, %rbx
	movq $0, %rcx
	movq $0, %rdx
	idivq %r10
	movq %rdx, %r10
	movq $7, %rbx
	movq %r10, %rax
	imulq %rbx
	movq %rax, %rbx
	addq %r11, %rbx
	movq $2, %r10
	movq $3, %r11
	movq $1, %r13
	movq $0, %r12
.L1:
	cmp %r12, %r11
	je .L2
	addq $1, %r12
	movq %r10, %rax
	imulq %r13
	movq %rax, %r13
	jmp .L1
.L2:
	movq %r13, %r11
	subq %r11, %rbx
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

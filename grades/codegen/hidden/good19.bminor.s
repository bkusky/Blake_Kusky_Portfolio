.file "/escnfs/home/nziems2/compiler-fa22-solution/hidden-tests/codegen/good19.bminor"
.data
	c1: .quad 1
	c3: .quad 100
.text
.globl C
C:
	pushq %rbp
	movq %rsp, %rbp
	pushq %rdi
	pushq %rsi
	subq $24, %rsp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	movq $11, %rbx
	movq %rbx, -24(%rbp)
	movq $12, %rbx
	movq %rbx, -32(%rbp)
	movq $13, %rbx
	movq %rbx, -40(%rbp)
	movq -8(%rbp), %rbx
	movq -16(%rbp), %r10
	addq %rbx, %r10
	movq -24(%rbp), %rbx
	addq %r10, %rbx
	movq -32(%rbp), %r10
	addq %rbx, %r10
	movq %r10, %rax
	jmp .C_epilogue
.C_epilogue:
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
	movq $1, %rbx
	movq $2, %r10
	movq %rbx, %rdi
	movq %r10, %rsi
	pushq %r10
	pushq %r11
	call C
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

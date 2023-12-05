.file "/escnfs/home/nziems2/compiler-fa22-solution/hidden-tests/codegen/good9.bminor"
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
	movq $60, %rbx
	movq %rbx, (null)
	movq $20, %r10
	movq %rbx, %rax
	movq $0, %rbx
	movq $0, %rcx
	movq $0, %rdx
	idivq %r10
	movq %rax, %r10
	movq $3, %rbx
	movq %r10, %rax
	imulq %rbx
	movq %rax, %rbx
	addq %rbx, %rbx
	subq %rbx, %rbx
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

.file "/escnfs/home/nziems2/compiler-fa22-solution/hidden-tests/codegen/good14.bminor"
.data
	s: .string "no\n"
.text
.globl f
f:
	pushq %rbp
	movq %rsp, %rbp
	subq $8, %rsp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	movq s, %rbx
	movq %rbx, -8(%rbp)
.data
.L1:
	.string "ok\n"
.text
	leaq .L1, %rbx
	movq %rbx, s
	movq -8(%rbp), %r10
	movq %r10, %rax
	jmp .f_epilogue
.f_epilogue:
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
	pushq %r10
	pushq %r11
	call f
	popq %r11
	popq %r10
	movq %rax, %r10
	movq %r10, %rdi
	pushq %r10
	pushq %r11
	popq %r11
	popq %r10
	movq s, %r10
	leaq s, %rdi
	pushq %r10
	pushq %r11
call print_string
	popq %r11
	popq %r10
	movq $0, %r10
	movq %r10, %rax
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
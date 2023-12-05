.file "/escnfs/home/nziems2/compiler-fa22-solution/hidden-tests/codegen/good13.bminor"
.data
.text
.globl foo_epilogue
foo_epilogue:
	pushq %rbp
	movq %rsp, %rbp
	subq $0, %rsp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	movq $2, %rbx
	movq %rbx, %rax
	jmp .foo_epilogue_epilogue
.foo_epilogue_epilogue:
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
	call foo
	popq %r11
	popq %r10
	movq %rax, %rbx
	movq $11, %r10
	movq %rbx, %rax
	imulq %r10
	movq %rax, %r10
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
.globl main_epilogue
main_epilogue:
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
	call foo_epilogue
	popq %r11
	popq %r10
	movq %rax, %rbx
	movq $3, %r10
	addq %rbx, %r10
	movq %r10, %rax
	jmp .main_epilogue_epilogue
.main_epilogue_epilogue:
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	movq %rbp, %rsp
	popq %rbp
	ret
.globl foo
foo:
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
	call main_epilogue
	popq %r11
	popq %r10
	movq %rax, %rbx
	movq $1, %r10
	subq %r10, %rbx
	movq %rbx, %rax
	jmp .foo_epilogue
.foo_epilogue:
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	movq %rbp, %rsp
	popq %rbp
	ret

.file "/escnfs/home/nziems2/compiler-fa22-solution/hidden-tests/codegen/good6.bminor"
.data
	x: .quad 4
.text
.globl f
f:
	pushq %rbp
	movq %rsp, %rbp
	pushq %rdi
	subq $0, %rsp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	movq -8(%rbp), %rbx
	not %rbx
	movq %rbx, %rax
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
	subq $8, %rsp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	movq $10, %rbx
	movq %rbx, -8(%rbp)
	movq -8(%rbp), %rbx
	movq -8(%rbp), %r10
	cmp %r10, %rbx
	jle .L3
	movq $0, %rbx
	jmp .L4
.L3:
	movq $1, %rbx
.L4:
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
	call f
	popq %r11
	popq %r10
	movq %rax, %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
	call f
	popq %r11
	popq %r10
	movq %rax, %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
	call f
	popq %r11
	popq %r10
	movq %rax, %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
	call f
	popq %r11
	popq %r10
	movq %rax, %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
	call f
	popq %r11
	popq %r10
	movq %rax, %rbx
	cmp $0, %rbx
	je .L1
	movq $1, %rbx
	movq %rbx, %rax
	jmp .main_epilogue
	jmp .L2
.L1:
	movq x, %rbx
	movq -8(%rbp), %r10
	movq x, %r11
	movq -8(%rbp), %r12
	movq %r11, %rax
	imulq %r12
	movq %rax, %r12
	subq %r12, %r10
	cmp %r10, %rbx
	jl .L7
	movq $0, %rbx
	jmp .L8
.L7:
	movq $1, %rbx
.L8:
	cmp $0, %rbx
	je .L5
	movq $4, %rbx
	movq %rbx, %rax
	jmp .main_epilogue
	jmp .L6
.L5:
	movq $2, %rbx
	movq %rbx, %rax
	jmp .main_epilogue
.L6:
.L2:
.main_epilogue:
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	movq %rbp, %rsp
	popq %rbp
	ret

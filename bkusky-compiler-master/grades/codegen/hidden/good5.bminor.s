.file "/escnfs/home/nziems2/compiler-fa22-solution/hidden-tests/codegen/good5.bminor"
.data
.text
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
	movq $100, %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
	call B
	popq %r11
	popq %r10
	movq %rax, %rbx
	movq $1, %r10
	subq %r10, %rbx
	movq $10, %r10
	movq %rbx, %rdi
	movq %r10, %rsi
	pushq %r10
	pushq %r11
	call A
	popq %r11
	popq %r10
	movq %rax, %rbx
	cmp $0, %rbx
	je .L1
	movq $3, %rbx
	movq %rbx, -8(%rbp)
	movq $2, %rbx
	movq $33, %r10
	movq %r10, %rdi
	pushq %r10
	pushq %r11
	call B
	popq %r11
	popq %r10
	movq %rax, %r10
	movq %rbx, %rax
	imulq %r10
	movq %rax, %r10
	movq -8(%rbp), %rbx
	addq %r10, %rbx
	movq %rbx, %rax
	jmp .main_epilogue
	jmp .L2
.L1:
.L2:
.data
.L3:
	.string "hello"
.text
	leaq .L3, %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
	call C
	popq %r11
	popq %r10
	movq %rax, %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
	call B
	popq %r11
	popq %r10
	movq %rax, %rbx
	movq $3, %r10
	movq %rbx, %rax
	movq $0, %rbx
	movq $0, %rcx
	movq $0, %rdx
	idivq %r10
	movq %rdx, %r10
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
.globl A
A:
	pushq %rbp
	movq %rsp, %rbp
	pushq %rdi
	pushq %rsi
	subq $0, %rsp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	movq -8(%rbp), %rbx
	movq -16(%rbp), %r10
	cmp %r10, %rbx
	je .L4
	movq $0, %rbx
	jmp .L5
.L4:
	movq $1, %rbx
.L5:
	movq %rbx, %rax
	jmp .A_epilogue
.A_epilogue:
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	movq %rbp, %rsp
	popq %rbp
	ret
.globl B
B:
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
	movq $33, %r10
	cmp %r10, %rbx
	je .L8
	movq $0, %rbx
	jmp .L9
.L8:
	movq $1, %rbx
.L9:
	cmp $0, %rbx
	je .L6
	movq $10, %rbx
	movq %rbx, %rax
	jmp .B_epilogue
	jmp .L7
.L6:
	movq $11, %rbx
	movq %rbx, %rax
	jmp .B_epilogue
.L7:
.B_epilogue:
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	movq %rbp, %rsp
	popq %rbp
	ret
.globl C
C:
	pushq %rbp
	movq %rsp, %rbp
	pushq %rdi
	subq $0, %rsp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	movq $33, %rbx
	movq %rbx, %rax
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

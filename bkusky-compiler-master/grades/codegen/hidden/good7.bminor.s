.file "/escnfs/home/nziems2/compiler-fa22-solution/hidden-tests/codegen/good7.bminor"
.data
.text
.globl f
f:
	pushq %rbp
	movq %rsp, %rbp
	pushq %rdi
	subq $8, %rsp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	movq $0, -16(%rbp)
	movq $0, %rbx
	movq %rbx, -16(%rbp)
.L1:
	movq -8(%rbp), %rbx
	movq $0, %r10
	cmp %r10, %rbx
	jg .L3
	movq $0, %rbx
	jmp .L4
.L3:
	movq $1, %rbx
.L4:
	cmp $0, %rbx
	je .L2
	movq -16(%rbp), %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
call print_integer
	popq %r11
	popq %r10
	movq -16(%rbp), %rbx
	movq $1, %r10
	addq %rbx, %r10
	movq %r10, -16(%rbp)
	movq -8(%rbp), %rbx
	movq %rbx, %r11
	subq $1, %rbx
	movq %rbx, -8(%rbp)
	jmp .L1
.L2:
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
	movq $6, %rbx
	movq %rbx, -8(%rbp)
.L5:
	movq -8(%rbp), %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
	call f
	popq %r11
	popq %r10
	movq %rax, %rbx
	movq $10, %r11
	movq %r11, %rdi
	pushq %r10
	pushq %r11
call print_character
	popq %r11
	popq %r10
	movq -8(%rbp), %r11
	movq $0, %r12
	cmp %r12, %r11
	je .L9
	movq $0, %r11
	jmp .L10
.L9:
	movq $1, %r11
.L10:
	cmp $0, %r11
	je .L7
	movq $0, %r11
	movq %r11, %rax
	jmp .main_epilogue
	jmp .L8
.L7:
.L8:
	movq -8(%rbp), %r11
	movq %r11, %r12
	subq $1, %r11
	movq %r11, -8(%rbp)
	jmp .L5
.L6:
	movq $1, %r11
	movq %r11, %rax
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

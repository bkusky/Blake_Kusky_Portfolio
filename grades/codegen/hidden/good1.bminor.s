.file "/escnfs/home/nziems2/compiler-fa22-solution/hidden-tests/codegen/good1.bminor"
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
.data
.L3:
	.string "hi"
.text
	leaq .L3, %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
	call strlen
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
.data
.L4:
	.string "there"
.text
	leaq .L4, %r10
	movq %r10, %rdi
	pushq %r10
	pushq %r11
	call strlen
	popq %r11
	popq %r10
	movq %rax, %r10
	cmp %r10, %rbx
	jg .L5
	movq $0, %rbx
	jmp .L6
.L5:
	movq $1, %rbx
.L6:
	cmp $0, %rbx
	je .L1
.data
.L7:
	.string "no\n"
.text
	leaq .L7, %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
call print_string
	popq %r11
	popq %r10
	jmp .L2
.L1:
.L2:
.data
.L10:
	.string "hi"
.text
	leaq .L10, %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
	call strlen
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
.data
.L11:
	.string "there"
.text
	leaq .L11, %r10
	movq %r10, %rdi
	pushq %r10
	pushq %r11
	call strlen
	popq %r11
	popq %r10
	movq %rax, %r10
	cmp %r10, %rbx
	jg .L12
	movq $0, %rbx
	jmp .L13
.L12:
	movq $1, %rbx
.L13:
	cmp $0, %rbx
	je .L8
.data
.L14:
	.string "ok\n"
.text
	leaq .L14, %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
call print_string
	popq %r11
	popq %r10
	jmp .L9
.L8:
.L9:
	movq $0, %rbx
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
	movq $2, %r10
	movq %rbx, %rax
	imulq %r10
	movq %rax, %r10
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

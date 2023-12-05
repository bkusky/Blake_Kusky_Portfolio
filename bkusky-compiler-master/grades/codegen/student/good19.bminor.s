.file "./codegen_tests/good19.bminor"
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
	movq $0, %rbx
	movq %rbx, -8(%rbp)
.L1:
	movq -8(%rbp), %rbx
	movq $10, %r10
	cmp %r10, %rbx
	jl .L3
	movq $0, %rbx
	jmp .L4
.L3:
	movq $1, %rbx
.L4:
	cmp $0, %rbx
	je .L2
	movq -8(%rbp), %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
call print_integer
	popq %r11
	popq %r10
	movq $10, %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
call print_character
	popq %r11
	popq %r10
	movq -8(%rbp), %rbx
	movq %rbx, %r10
	addq $1, %rbx
	movq %rbx, -8(%rbp)
	jmp .L1
.L2:
	movq $0, %rbx
	movq %rbx, -8(%rbp)
.L5:
	movq -8(%rbp), %rbx
	movq $10, %r10
	cmp %r10, %rbx
	je .L9
	movq $0, %rbx
	jmp .L10
.L9:
	movq $1, %rbx
.L10:
	cmp $0, %rbx
	je .L7
	movq $0, %rbx
	movq %rbx, %rax
	jmp .main_epilogue
	jmp .L8
.L7:
.L8:
	movq -8(%rbp), %rbx
	movq %rbx, %r10
	addq $1, %rbx
	movq %rbx, -8(%rbp)
	jmp .L5
.L6:
.main_epilogue:
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	movq %rbp, %rsp
	popq %rbp
	ret

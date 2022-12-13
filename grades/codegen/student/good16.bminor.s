.file "./codegen_tests/good16.bminor"
.data
.text
.globl main
main:
	pushq %rbp
	movq %rsp, %rbp
	subq $16, %rsp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	movq $20, %rbx
	movq %rbx, -8(%rbp)
	movq $10, %rbx
	movq %rbx, -16(%rbp)
	movq -8(%rbp), %rbx
	movq -16(%rbp), %r10
	cmp %r10, %rbx
	jl .L3
	movq $0, %rbx
	jmp .L4
.L3:
	movq $1, %rbx
.L4:
	cmp $0, %rbx
	je .L1
.data
.L5:
	.string "NOOO!\n"
.text
	leaq .L5, %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
call print_string
	popq %r11
	popq %r10
	jmp .L2
.L1:
	movq -8(%rbp), %rbx
	movq -16(%rbp), %r10
	cmp %r10, %rbx
	je .L8
	movq $0, %rbx
	jmp .L9
.L8:
	movq $1, %rbx
.L9:
	cmp $0, %rbx
	je .L6
.data
.L10:
	.string "NOOO!\n"
.text
	leaq .L10, %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
call print_string
	popq %r11
	popq %r10
	jmp .L7
.L6:
	movq -8(%rbp), %rbx
	movq -16(%rbp), %r10
	cmp %r10, %rbx
	jle .L13
	movq $0, %rbx
	jmp .L14
.L13:
	movq $1, %rbx
.L14:
	cmp $0, %rbx
	je .L11
.data
.L15:
	.string "NOOO!\n"
.text
	leaq .L15, %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
call print_string
	popq %r11
	popq %r10
	jmp .L12
.L11:
.data
.L16:
	.string "yes\n"
.text
	leaq .L16, %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
call print_string
	popq %r11
	popq %r10
.L12:
.L7:
.L2:
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

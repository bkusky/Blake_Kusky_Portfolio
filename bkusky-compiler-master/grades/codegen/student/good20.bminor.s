.file "./codegen_tests/good20.bminor"
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
	movq $0, %rbx
	movq %rbx, -8(%rbp)
.L1:
	movq -8(%rbp), %rbx
	movq $100, %r10
	cmp %r10, %rbx
	jle .L3
	movq $0, %rbx
	jmp .L4
.L3:
	movq $1, %rbx
.L4:
	cmp $0, %rbx
	je .L2
	movq -8(%rbp), %rbx
	movq $5, %r10
	movq %rbx, %rax
	movq $0, %rbx
	movq $0, %rcx
	movq $0, %rdx
	idivq %r10
	movq %rdx, %r10
	movq $0, %rbx
	cmp %rbx, %r10
	je .L7
	movq $0, %rbx
	jmp .L8
.L7:
	movq $1, %rbx
.L8:
	cmp $0, %rbx
	je .L5
	movq -8(%rbp), %rbx
	movq $3, %r10
	movq %rbx, %rax
	movq $0, %rbx
	movq $0, %rcx
	movq $0, %rdx
	idivq %r10
	movq %rdx, %r10
	movq $0, %rbx
	cmp %rbx, %r10
	je .L11
	movq $0, %rbx
	jmp .L12
.L11:
	movq $1, %rbx
.L12:
	cmp $0, %rbx
	je .L9
.data
.L13:
	.string "FizzBuzz\n"
.text
	leaq .L13, %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
call print_string
	popq %r11
	popq %r10
	jmp .L10
.L9:
.data
.L14:
	.string "Buzz\n"
.text
	leaq .L14, %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
call print_string
	popq %r11
	popq %r10
.L10:
	jmp .L6
.L5:
	movq -8(%rbp), %rbx
	movq $3, %r10
	movq %rbx, %rax
	movq $0, %rbx
	movq $0, %rcx
	movq $0, %rdx
	idivq %r10
	movq %rdx, %r10
	movq $0, %rbx
	cmp %rbx, %r10
	je .L17
	movq $0, %rbx
	jmp .L18
.L17:
	movq $1, %rbx
.L18:
	cmp $0, %rbx
	je .L15
.data
.L19:
	.string "Fizz\n"
.text
	leaq .L19, %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
call print_string
	popq %r11
	popq %r10
	jmp .L16
.L15:
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
.L16:
.L6:
	movq -8(%rbp), %rbx
	movq %rbx, %r10
	addq $1, %rbx
	movq %rbx, -8(%rbp)
	jmp .L1
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

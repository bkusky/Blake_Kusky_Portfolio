.file "./codegen_tests/good8.bminor"
.data
	c1: .quad 72
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
	movq $105, %rbx
	movq %rbx, -8(%rbp)
	movq c1, %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
call print_character
	popq %r11
	popq %r10
	movq -8(%rbp), %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
call print_character
	popq %r11
	popq %r10
	movq $32, %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
call print_character
	popq %r11
	popq %r10
	movq $10, %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
call print_character
	popq %r11
	popq %r10
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

.file "./codegen_tests/good14.bminor"
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
	movq $1, %rbx
	movq %rbx, -8(%rbp)
	movq -8(%rbp), %rbx
	neg %rbx
	movq %rbx, -8(%rbp)
	movq -8(%rbp), %r10
	movq %r10, %rdi
	pushq %r10
	pushq %r11
call print_integer
	popq %r11
	popq %r10
	movq $10, %r10
	movq %r10, %rdi
	pushq %r10
	pushq %r11
call print_character
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

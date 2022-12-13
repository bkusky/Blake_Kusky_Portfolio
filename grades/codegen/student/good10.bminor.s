.file "./codegen_tests/good10.bminor"
.data
	b: .quad 1
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
	movq b, %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
call print_boolean
	popq %r11
	popq %r10
.data
.L1:
	.string " is true and "
.text
	leaq .L1, %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
call print_string
	popq %r11
	popq %r10
	movq -8(%rbp), %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
call print_boolean
	popq %r11
	popq %r10
.data
.L2:
	.string " is false\n"
.text
	leaq .L2, %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
call print_string
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

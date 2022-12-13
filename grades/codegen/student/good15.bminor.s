.file "./codegen_tests/good15.bminor"
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
	movq $10, %rbx
	movq %rbx, -8(%rbp)
	movq $9, %rbx
	movq %rbx, -16(%rbp)
	movq -8(%rbp), %rbx
	movq -16(%rbp), %r10
	cmp %r10, %rbx
	jg .L3
	movq $0, %rbx
	jmp .L4
.L3:
	movq $1, %rbx
.L4:
	cmp $0, %rbx
	je .L1
.data
.L5:
	.string "a < b\n"
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
.data
.L6:
	.string "nooo!\n"
.text
	leaq .L6, %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
call print_string
	popq %r11
	popq %r10
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

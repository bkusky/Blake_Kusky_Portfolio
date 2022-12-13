.file "./codegen_tests/good11.bminor"
.data
.text
.globl func
func:
	pushq %rbp
	movq %rsp, %rbp
	subq $0, %rsp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	movq $1, %rbx
	movq $1, %r10
	addq %rbx, %r10
	movq %r10, %rax
	jmp .func_epilogue
.func_epilogue:
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
	pushq %r10
	pushq %r11
	call func
	popq %r11
	popq %r10
	movq %rax, %rbx
	movq $1, %r10
	addq %rbx, %r10
	movq %r10, -8(%rbp)
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
	pushq %r10
	pushq %r11
	call func
	popq %r11
	popq %r10
	movq %rax, %rbx
	movq $2, %r10
	subq %r10, %rbx
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

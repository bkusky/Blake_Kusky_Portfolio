.file "/escnfs/home/nziems2/compiler-fa22-solution/hidden-tests/codegen/good10.bminor"
.data
	w: .string "no"
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
.data
.L1:
	.string "ok"
.text
	leaq .L1, %rbx
	movq %rbx, -8(%rbp)
	movq -8(%rbp), %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
call print_string
	popq %r11
	popq %r10
.data
.L2:
	.string "\n"
.text
	leaq .L2, %rbx
	movq %rbx, %rdi
	pushq %r10
	pushq %r11
call print_string
	popq %r11
	popq %r10
.data
.L3:
	.string "bad!\n"
.text
	leaq .L3, %rbx
	movq %rbx, -8(%rbp)
	movq -8(%rbp), %r10
	movq %r10, %rdi
	pushq %r10
	pushq %r11
call print_string
	popq %r11
	popq %r10
	movq $32, %r10
	movq %r10, %rdi
	pushq %r10
	pushq %r11
call print_character
	popq %r11
	popq %r10
	movq w, %r10
	leaq w, %rdi
	pushq %r10
	pushq %r11
call print_string
	popq %r11
	popq %r10
.data
.L4:
	.string " "
.text
	leaq .L4, %r10
	movq %r10, %rdi
	pushq %r10
	pushq %r11
call print_string
	popq %r11
	popq %r10
.data
.L5:
	.string "changed\n"
.text
	leaq .L5, %r10
	movq %r10, -8(%rbp)
	movq -8(%rbp), %r11
	movq %r11, %rdi
	pushq %r10
	pushq %r11
call print_string
	popq %r11
	popq %r10
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
.data
.L6:
	.string "good"
.text
	leaq .L6, %r11
	movq %r11, -8(%rbp)
	movq -8(%rbp), %r11
	movq %r11, %rdi
	pushq %r10
	pushq %r11
	call f
	popq %r11
	popq %r10
	movq %rax, %r11
	movq -8(%rbp), %r12
	movq %r12, %rdi
	pushq %r10
	pushq %r11
call print_string
	popq %r11
	popq %r10
	movq $0, %r12
	movq %r12, %rax
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

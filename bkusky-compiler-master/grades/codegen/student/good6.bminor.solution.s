.text

.global main
main:
	pushq %rbp
	movq  %rsp, %rbp
	subq $32,%rsp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	movq $5,%rbx
	movq %rbx, -8(%rbp)
	movq -8(%rbp),%rbx
	movq $5,%r10
	add %rbx, %r10
	movq $10,%rbx
	mov %r10, %rax
	imul %rbx
	mov %rax, %rbx
	movq $50,%r10
	sub %r10, %rbx
	movq %rbx, -16(%rbp)
	movq -16(%rbp),%rbx
	movq $19,%r10
	add %rbx, %r10
	movq %r10, -24(%rbp)
	movq -24(%rbp),%rbx
	movq $1000,%r10
	mov %rbx, %rax
	imul %r10
	mov %rax, %r10
	movq -16(%rbp),%rbx
	movq $8,%r11
	mov %rbx, %rax
	imul %r11
	mov %rax, %r11
	add %r10, %r11
	movq -8(%rbp),%rbx
	movq $4,%r10
	mov %rbx, %rax
	imul %r10
	mov %rax, %r10
	add %r11, %r10
	movq %r10, -32(%rbp)
	movq -32(%rbp),%rbx
	pushq %r10
	pushq %r11
	mov %rbx, %rdi
	call print_integer
	popq %r11
	popq %r10
.data
.S0:	.string "\n"
.text
	leaq .S0,%rbx
	pushq %r10
	pushq %r11
	mov %rbx, %rdi
	call print_string
	popq %r11
	popq %r10
	movq $0,%rbx
	movq %rbx,%rax
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	movq %rbp, %rsp
	popq %rbp
	ret
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	movq  %rbp, %rsp
	popq %rbp
	ret

	.file	"cacheStuff.c"
	.text
.globl get_page
	.type	get_page, @function
get_page:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$40, %esp
	call	getpagesize
	movl	$0, 20(%esp)
	movl	$-1, 16(%esp)
	movl	$34, 12(%esp)
	movl	$3, 8(%esp)
	movl	%eax, 4(%esp)
	movl	$0, (%esp)
	call	mmap
	movl	%eax, -4(%ebp)
	cmpl	$-1, -4(%ebp)
	jne	.L2
	call	__errno_location
	movl	(%eax), %eax
	movl	%eax, -8(%ebp)
	movl	$1, (%esp)
	call	exit
.L2:
	movl	-4(%ebp), %edx
	addl	$4, %edx
	movl	-4(%ebp), %eax
	movl	%edx, 4(%eax)
	movl	-4(%ebp), %edx
	addl	$4, %edx
	movl	-4(%ebp), %eax
	movl	%edx, 8(%eax)
	movl	-4(%ebp), %edx
	addl	$12, %edx
	movl	-4(%ebp), %eax
	movl	%edx, 12(%eax)
	movl	-4(%ebp), %edx
	addl	$12, %edx
	movl	-4(%ebp), %eax
	movl	%edx, 16(%eax)
	movl	-4(%ebp), %edx
	addl	$24, %edx
	movl	-4(%ebp), %eax
	movl	%edx, 24(%eax)
	movl	-4(%ebp), %edx
	addl	$24, %edx
	movl	-4(%ebp), %eax
	movl	%edx, 28(%eax)
	movl	-4(%ebp), %eax
	movl	$0, 40(%eax)
	movl	-4(%ebp), %eax
	leave
	ret
	.size	get_page, .-get_page
.globl add_page
	.type	add_page, @function
add_page:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	call	getpagesize
	addl	12(%ebp), %eax
	movl	%eax, -8(%ebp)
	movl	12(%ebp), %edx
	movl	8(%ebp), %eax
	movl	%eax, (%edx)
	movl	12(%ebp), %eax
	movl	$0, 32(%eax)
	movl	12(%ebp), %eax
	movl	$0, 36(%eax)
	movl	12(%ebp), %edx
	movl	16(%ebp), %eax
	movl	%eax, 20(%edx)
	movl	8(%ebp), %eax
	movl	36(%eax), %eax
	leal	1(%eax), %edx
	movl	8(%ebp), %eax
	movl	%edx, 36(%eax)
	movl	8(%ebp), %eax
	movl	36(%eax), %edx
	movl	8(%ebp), %eax
	movl	40(%eax), %eax
	cmpl	%eax, %edx
	jbe	.L6
	movl	8(%ebp), %eax
	movl	40(%eax), %eax
	leal	1(%eax), %edx
	movl	8(%ebp), %eax
	movl	%edx, 40(%eax)
.L6:
	movl	8(%ebp), %eax
	movl	16(%eax), %eax
	testl	%eax, %eax
	jne	.L8
	movl	8(%ebp), %edx
	movl	12(%ebp), %eax
	movl	%eax, 16(%edx)
	jmp	.L10
.L8:
	movl	8(%ebp), %eax
	movl	16(%eax), %eax
	addl	$4, %eax
	movl	12(%ebp), %edx
	addl	$4, %edx
	movl	%eax, 4(%esp)
	movl	%edx, (%esp)
	call	list_add_tail
.L10:
	movl	16(%ebp), %eax
	movl	%eax, -4(%ebp)
	jmp	.L11
.L12:
	movl	12(%ebp), %eax
	addl	$24, %eax
	movl	-4(%ebp), %edx
	movl	%eax, 4(%esp)
	movl	%edx, (%esp)
	call	list_add_tail
	movl	8(%ebp), %eax
	movl	44(%eax), %eax
	leal	1(%eax), %edx
	movl	8(%ebp), %eax
	movl	%edx, 44(%eax)
	movl	12(%ebp), %eax
	movl	32(%eax), %eax
	leal	1(%eax), %edx
	movl	12(%ebp), %eax
	movl	%edx, 32(%eax)
	movl	8(%ebp), %eax
	movl	32(%eax), %eax
	movl	%eax, %edx
	leal	-4(%ebp), %eax
	addl	%edx, (%eax)
.L11:
	movl	8(%ebp), %eax
	movl	32(%eax), %eax
	addl	-4(%ebp), %eax
	cmpl	-8(%ebp), %eax
	jb	.L12
	leave
	ret
	.size	add_page, .-add_page
	.type	list_add_tail, @function
list_add_tail:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	movl	12(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	list_add
	leave
	ret
	.size	list_add_tail, .-list_add_tail
	.type	list_add, @function
list_add:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %edx
	movl	12(%ebp), %eax
	movl	%eax, (%edx)
	movl	12(%ebp), %eax
	movl	4(%eax), %edx
	movl	8(%ebp), %eax
	movl	%edx, 4(%eax)
	movl	12(%ebp), %edx
	movl	8(%ebp), %eax
	movl	%eax, 4(%edx)
	movl	8(%ebp), %eax
	movl	4(%eax), %edx
	movl	8(%ebp), %eax
	movl	%eax, (%edx)
	popl	%ebp
	ret
	.size	list_add, .-list_add
.globl put_page
	.type	put_page, @function
put_page:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$20, %esp
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	16(%eax), %eax
	cmpl	8(%ebp), %eax
	je	.L22
	movl	8(%ebp), %eax
	movl	(%eax), %ecx
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	44(%eax), %edx
	movl	8(%ebp), %eax
	movl	32(%eax), %eax
	movl	%edx, %ebx
	subl	%eax, %ebx
	movl	%ebx, %eax
	movl	%eax, 44(%ecx)
	movl	8(%ebp), %eax
	movl	(%eax), %edx
	movl	36(%edx), %eax
	decl	%eax
	movl	%eax, 36(%edx)
	call	getpagesize
	movl	%eax, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	munmap
.L22:
	addl	$20, %esp
	popl	%ebx
	popl	%ebp
	ret
	.size	put_page, .-put_page
.globl cache_create
	.type	cache_create, @function
cache_create:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$40, %esp
	call	get_page
	movl	%eax, -4(%ebp)
	movl	-4(%ebp), %eax
	addl	$44, %eax
	movl	%eax, -8(%ebp)
	movl	-8(%ebp), %eax
	movl	$0, 16(%eax)
	movl	-8(%ebp), %edx
	movl	8(%ebp), %eax
	movl	%eax, 32(%edx)
	movl	-8(%ebp), %eax
	movl	$0, 48(%eax)
	movl	-8(%ebp), %eax
	movl	$0, 52(%eax)
	movl	-8(%ebp), %eax
	movl	$0, 40(%eax)
	movl	-8(%ebp), %edx
	movl	-4(%ebp), %eax
	movl	%eax, 20(%edx)
	movl	-8(%ebp), %edx
	addl	$24, %edx
	movl	-8(%ebp), %eax
	movl	%edx, 24(%eax)
	movl	-8(%ebp), %edx
	addl	$24, %edx
	movl	-8(%ebp), %eax
	movl	%edx, 28(%eax)
	movl	-8(%ebp), %eax
	addl	$56, %eax
	movl	%eax, 8(%esp)
	movl	-4(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	-8(%ebp), %eax
	movl	%eax, (%esp)
	call	add_page
	movl	-8(%ebp), %eax
	leave
	ret
	.size	cache_create, .-cache_create
.globl cache_destroy
	.type	cache_destroy, @function
cache_destroy:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	movl	8(%ebp), %eax
	movl	16(%eax), %eax
	movl	8(%eax), %eax
	movl	%eax, -12(%ebp)
	movl	-12(%ebp), %eax
	movl	4(%eax), %eax
	movl	%eax, -8(%ebp)
	jmp	.L26
.L27:
	movl	-12(%ebp), %eax
	subl	$4, %eax
	movl	%eax, -4(%ebp)
	movl	-4(%ebp), %eax
	movl	%eax, (%esp)
	call	put_page
	movl	-8(%ebp), %eax
	movl	%eax, -12(%ebp)
	movl	-8(%ebp), %eax
	movl	4(%eax), %eax
	movl	%eax, -8(%ebp)
.L26:
	movl	8(%ebp), %eax
	movl	16(%eax), %eax
	addl	$4, %eax
	cmpl	-12(%ebp), %eax
	jne	.L27
	movl	8(%ebp), %eax
	movl	16(%eax), %eax
	movl	%eax, -4(%ebp)
	movl	8(%ebp), %eax
	movl	$0, 16(%eax)
	movl	-4(%ebp), %eax
	movl	%eax, (%esp)
	call	put_page
	leave
	ret
	.size	cache_destroy, .-cache_destroy
.globl cache_get_object
	.type	cache_get_object, @function
cache_get_object:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$40, %esp
	movl	8(%ebp), %eax
	addl	$24, %eax
	movl	8(%ebp), %edx
	movl	28(%edx), %edx
	cmpl	%edx, %eax
	jne	.L31
	movl	8(%ebp), %eax
	movl	20(%eax), %eax
	testl	%eax, %eax
	jne	.L33
	call	get_page
	movl	%eax, -8(%ebp)
	movl	-8(%ebp), %eax
	addl	$1936, %eax
	movl	%eax, 8(%esp)
	movl	-8(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	add_page
	jmp	.L35
.L33:
	movl	8(%ebp), %eax
	movl	20(%eax), %eax
	movl	%eax, -8(%ebp)
	movl	8(%ebp), %eax
	movl	$0, 20(%eax)
.L35:
	movl	8(%ebp), %eax
	addl	$24, %eax
	movl	-8(%ebp), %edx
	addl	$12, %edx
	movl	%eax, 4(%esp)
	movl	%edx, (%esp)
	call	list_add_tail
	jmp	.L36
.L31:
	movl	8(%ebp), %eax
	movl	28(%eax), %eax
	subl	$12, %eax
	movl	%eax, -8(%ebp)
.L36:
	movl	-8(%ebp), %eax
	movl	28(%eax), %eax
	movl	%eax, -4(%ebp)
	movl	-4(%ebp), %eax
	movl	%eax, (%esp)
	call	list_remove
	movl	-8(%ebp), %eax
	movl	36(%eax), %eax
	leal	1(%eax), %edx
	movl	-8(%ebp), %eax
	movl	%edx, 36(%eax)
	movl	8(%ebp), %eax
	movl	48(%eax), %eax
	leal	1(%eax), %edx
	movl	8(%ebp), %eax
	movl	%edx, 48(%eax)
	movl	8(%ebp), %eax
	movl	48(%eax), %edx
	movl	8(%ebp), %eax
	movl	52(%eax), %eax
	cmpl	%eax, %edx
	jbe	.L37
	movl	8(%ebp), %eax
	movl	52(%eax), %eax
	leal	1(%eax), %edx
	movl	8(%ebp), %eax
	movl	%edx, 52(%eax)
.L37:
	movl	-8(%ebp), %eax
	movl	36(%eax), %edx
	movl	-8(%ebp), %eax
	movl	32(%eax), %eax
	cmpl	%eax, %edx
	jne	.L39
	movl	-8(%ebp), %eax
	addl	$12, %eax
	movl	%eax, (%esp)
	call	list_remove
.L39:
	movl	-4(%ebp), %eax
	leave
	ret
	.size	cache_get_object, .-cache_get_object
	.type	list_remove, @function
list_remove:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %eax
	movl	(%eax), %edx
	movl	8(%ebp), %eax
	movl	4(%eax), %eax
	movl	%eax, 4(%edx)
	movl	8(%ebp), %eax
	movl	4(%eax), %edx
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, (%edx)
	popl	%ebp
	ret
	.size	list_remove, .-list_remove
.globl cache_put_object
	.type	cache_put_object, @function
cache_put_object:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	find_page
	movl	%eax, -4(%ebp)
	movl	-4(%ebp), %eax
	addl	$24, %eax
	movl	8(%ebp), %edx
	movl	%eax, 4(%esp)
	movl	%edx, (%esp)
	call	list_add_tail
	movl	-4(%ebp), %eax
	movl	36(%eax), %edx
	movl	-4(%ebp), %eax
	movl	32(%eax), %eax
	cmpl	%eax, %edx
	jne	.L45
	movl	-4(%ebp), %eax
	movl	(%eax), %eax
	addl	$24, %eax
	movl	-4(%ebp), %edx
	addl	$12, %edx
	movl	%eax, 4(%esp)
	movl	%edx, (%esp)
	call	list_add_tail
.L45:
	movl	-4(%ebp), %eax
	movl	(%eax), %edx
	movl	48(%edx), %eax
	decl	%eax
	movl	%eax, 48(%edx)
	movl	-4(%ebp), %eax
	movl	36(%eax), %eax
	leal	-1(%eax), %edx
	movl	-4(%ebp), %eax
	movl	%edx, 36(%eax)
	movl	-4(%ebp), %eax
	movl	36(%eax), %eax
	testl	%eax, %eax
	jne	.L53
	movl	-4(%ebp), %eax
	addl	$12, %eax
	movl	%eax, (%esp)
	call	list_remove
	movl	-4(%ebp), %eax
	movl	(%eax), %eax
	movl	20(%eax), %eax
	testl	%eax, %eax
	je	.L49
	movl	-4(%ebp), %eax
	movl	(%eax), %eax
	movl	16(%eax), %eax
	cmpl	-4(%ebp), %eax
	jne	.L51
	movl	-4(%ebp), %eax
	movl	(%eax), %eax
	movl	20(%eax), %eax
	movl	%eax, -4(%ebp)
	movl	-4(%ebp), %eax
	movl	(%eax), %edx
	movl	-4(%ebp), %eax
	movl	(%eax), %eax
	movl	16(%eax), %eax
	movl	%eax, 20(%edx)
.L51:
	movl	-4(%ebp), %eax
	addl	$4, %eax
	movl	%eax, (%esp)
	call	list_remove
	movl	-4(%ebp), %eax
	movl	%eax, (%esp)
	call	put_page
	jmp	.L53
.L49:
	movl	-4(%ebp), %eax
	movl	(%eax), %edx
	movl	-4(%ebp), %eax
	movl	%eax, 20(%edx)
.L53:
	leave
	ret
	.size	cache_put_object, .-cache_put_object
	.type	find_page, @function
find_page:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	call	getpagesize
	negl	%eax
	movl	%eax, %edx
	movl	8(%ebp), %eax
	andl	%edx, %eax
	leave
	ret
	.size	find_page, .-find_page
	.ident	"GCC: (GNU) 4.0.3"
	.section	.note.GNU-stack,"",@progbits

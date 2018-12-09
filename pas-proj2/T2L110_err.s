 #    2
 #    3
 #    4
 #    5
 #    6
 #    7
 #    8
				# b_global_decl (K, alignment = 4, size = 4)
.globl K
	.data
	.align	4
	.type	K, @object
	.size	K, 4
K:
	.zero	4
				# b_global_decl (J, alignment = 4, size = 4)
.globl J
	.align	4
	.type	J, @object
	.size	J, 4
J:
	.zero	4
				# b_global_decl (I, alignment = 4, size = 4)
.globl I
	.align	4
	.type	I, @object
	.size	I, 4
I:
	.zero	4
 #    9
				# b_global_decl (Y, alignment = 4, size = 4)
.globl Y
	.align	4
	.type	Y, @object
	.size	Y, 4
Y:
	.zero	4
				# b_global_decl (X, alignment = 4, size = 4)
.globl X
	.align	4
	.type	X, @object
	.size	X, 4
X:
	.zero	4
 #   10
				# b_global_decl (D, alignment = 8, size = 8)
.globl D
	.align	8
	.type	D, @object
	.size	D, 8
D:
	.zero	8
 #   11
				# b_global_decl (C, alignment = 1, size = 1)
.globl C
	.align	1
	.type	C, @object
	.size	C, 1
C:
	.zero	1
 #   12
				# b_global_decl (B, alignment = 1, size = 1)
.globl B
	.align	1
	.type	B, @object
	.size	B, 1
B:
	.zero	1
 #   13
 #   14
 #   15
 #   16
 #   17
 #   18
 #   19
				# b_init_formal_param_offset ()
 #   20
				# b_global_decl (Tmp, alignment = 8, size = 8)
.globl Tmp
	.align	8
	.type	Tmp, @object
	.size	Tmp, 8
Tmp:
	.zero	8
 #   21
				# b_func_prologue (Swap)
	.text
.global Swap
	.type	Swap, @function
Swap:
	pushl	%ebp
	movl	%esp, %ebp
				# b_alloc_local_vars ( size = 0 )
 #   22
 #   23
 #   24
 #   25
				# b_push_ext_addr (B)
	subl	$8, %esp
	movl	$B, (%esp)
				# b_push_ext_addr (Tmp)
	subl	$8, %esp
	movl	$Tmp, (%esp)
				# b_deref (double)
	movl	(%esp), %eax
	fldl	(%eax)
	fstpl	(%esp)
				# b_func_epilogue (Swap)
	leave
	ret
	.size	Swap, .-Swap
 #   26
 #   27
 #   28
 #   29
				# b_init_formal_param_offset ()
 #   30
				# b_global_decl (I, alignment = 4, size = 4)
.globl I
	.data
	.align	4
	.type	I, @object
	.size	I, 4
I:
	.zero	4
 #   31
				# b_func_prologue (Inner1)
	.text
.global Inner1
	.type	Inner1, @function
Inner1:
	pushl	%ebp
	movl	%esp, %ebp
				# b_alloc_local_vars ( size = 0 )
 #   32
				# b_push_ext_addr (I)
	subl	$8, %esp
	movl	$I, (%esp)
				# b_push_ext_addr (K)
	subl	$8, %esp
	movl	$K, (%esp)
				# b_deref (signed long int)
	movl	(%esp), %eax
	movl	(%eax), %edx
	movl	%edx, (%esp)
				# b_assign (signed long int)
	movl	(%esp), %edx
	addl	$8, %esp
	movl	(%esp), %eax
	movl	%edx, (%eax)
	movl	%edx, (%esp)
				# b_pop ()
	addl	$8, %esp
 #   33
 #   34
				# b_alloc_arglist (24 bytes)
	movl	%esp, %eax
	subl	$4, %esp
	andl	$-16, %esp
	movl	%eax, (%esp)
	subl	$32, %esp
				# b_push_const_string ("f.inner1: c = %ld; i = %ld\n")
	.section	.rodata
.LC0:
.string	"f.inner1: c = %ld; i = %ld\n"
	.text
				# b_push_ext_addr (.LC0)
	subl	$8, %esp
	movl	$.LC0, (%esp)
				# b_load_arg (pointer)
	movl	(%esp), %eax
	addl	$8, %esp
	movl	%eax, 0(%esp)
				# b_push_ext_addr (C)
	subl	$8, %esp
	movl	$C, (%esp)
				# b_deref (unsigned char)
	movl	(%esp), %eax
	movzbl	(%eax), %edx
	movb	%dl, (%esp)
				# b_convert (unsigned char -> signed long int)
	movzbl	(%esp), %eax
	movzbl	%al, %eax
	movl	%eax, (%esp)
				# b_load_arg (signed long int)
	movl	(%esp), %eax
	addl	$8, %esp
	movl	%eax, 4(%esp)
				# b_push_ext_addr (I)
	subl	$8, %esp
	movl	$I, (%esp)
				# b_deref (signed long int)
	movl	(%esp), %eax
	movl	(%eax), %edx
	movl	%edx, (%esp)
				# b_load_arg (signed long int)
	movl	(%esp), %eax
	addl	$8, %esp
	movl	%eax, 8(%esp)
				# b_funcall_by_name (Printf, void)
	call	Printf
	addl	$32, %esp
	movl	(%esp), %ecx
	movl	%ecx, %esp
				# b_func_epilogue (Inner1)
	leave
	ret
	.size	Inner1, .-Inner1
 #   35
 #   36
 #   37
				# b_init_formal_param_offset ()
 #   38
				# b_global_decl (I, alignment = 4, size = 4)
.globl I
	.data
	.align	4
	.type	I, @object
	.size	I, 4
I:
	.zero	4
 #   39
				# b_func_prologue (Inner2)
	.text
.global Inner2
	.type	Inner2, @function
Inner2:
	pushl	%ebp
	movl	%esp, %ebp
				# b_alloc_local_vars ( size = 0 )
 #   40
 #   41
				# b_func_epilogue (Inner2)
	leave
	ret
	.size	Inner2, .-Inner2
 #   42
 #   43
 #   44
				# b_init_formal_param_offset ()
 #   45
				# b_global_decl (I, alignment = 4, size = 4)
.globl I
	.data
	.align	4
	.type	I, @object
	.size	I, 4
I:
	.zero	4
 #   46
				# b_func_prologue (F)
	.text
.global F
	.type	F, @function
F:
	pushl	%ebp
	movl	%esp, %ebp
				# b_alloc_local_vars ( size = 0 )
 #   47
				# b_push_ext_addr (I)
	subl	$8, %esp
	movl	$I, (%esp)
				# b_push_const_int (19)
	movl	$19, %eax
	subl	$8, %esp
	movl	%eax, (%esp)
				# b_assign (signed long int)
	movl	(%esp), %edx
	addl	$8, %esp
	movl	(%esp), %eax
	movl	%edx, (%eax)
	movl	%edx, (%esp)
				# b_pop ()
	addl	$8, %esp
 #   48
				# b_alloc_arglist (24 bytes)
	movl	%esp, %eax
	subl	$4, %esp
	andl	$-16, %esp
	movl	%eax, (%esp)
	subl	$32, %esp
				# b_push_const_string ("f: c = %ld; i = %ld\n")
	.section	.rodata
.LC1:
.string	"f: c = %ld; i = %ld\n"
	.text
				# b_push_ext_addr (.LC1)
	subl	$8, %esp
	movl	$.LC1, (%esp)
				# b_load_arg (pointer)
	movl	(%esp), %eax
	addl	$8, %esp
	movl	%eax, 0(%esp)
				# b_push_ext_addr (C)
	subl	$8, %esp
	movl	$C, (%esp)
				# b_deref (unsigned char)
	movl	(%esp), %eax
	movzbl	(%eax), %edx
	movb	%dl, (%esp)
				# b_convert (unsigned char -> signed long int)
	movzbl	(%esp), %eax
	movzbl	%al, %eax
	movl	%eax, (%esp)
				# b_load_arg (signed long int)
	movl	(%esp), %eax
	addl	$8, %esp
	movl	%eax, 4(%esp)
				# b_push_ext_addr (I)
	subl	$8, %esp
	movl	$I, (%esp)
				# b_deref (signed long int)
	movl	(%esp), %eax
	movl	(%eax), %edx
	movl	%edx, (%esp)
				# b_load_arg (signed long int)
	movl	(%esp), %eax
	addl	$8, %esp
	movl	%eax, 8(%esp)
				# b_funcall_by_name (Printf, void)
	call	Printf
	addl	$32, %esp
	movl	(%esp), %ecx
	movl	%ecx, %esp
 #   49
				# b_alloc_arglist (0 bytes)
	movl	%esp, %eax
	subl	$4, %esp
	andl	$-16, %esp
	movl	%eax, (%esp)
	subl	$0, %esp
				# b_funcall_by_name (Inner1, void)
	call	Inner1
	addl	$0, %esp
	movl	(%esp), %ecx
	movl	%ecx, %esp
 #   50
				# b_alloc_arglist (24 bytes)
	movl	%esp, %eax
	subl	$4, %esp
	andl	$-16, %esp
	movl	%eax, (%esp)
	subl	$32, %esp
				# b_push_const_string ("f: c = %ld; i = %ld\n")
	.section	.rodata
.LC2:
.string	"f: c = %ld; i = %ld\n"
	.text
				# b_push_ext_addr (.LC2)
	subl	$8, %esp
	movl	$.LC2, (%esp)
				# b_load_arg (pointer)
	movl	(%esp), %eax
	addl	$8, %esp
	movl	%eax, 0(%esp)
				# b_push_ext_addr (C)
	subl	$8, %esp
	movl	$C, (%esp)
				# b_deref (unsigned char)
	movl	(%esp), %eax
	movzbl	(%eax), %edx
	movb	%dl, (%esp)
				# b_convert (unsigned char -> signed long int)
	movzbl	(%esp), %eax
	movzbl	%al, %eax
	movl	%eax, (%esp)
				# b_load_arg (signed long int)
	movl	(%esp), %eax
	addl	$8, %esp
	movl	%eax, 4(%esp)
				# b_push_ext_addr (I)
	subl	$8, %esp
	movl	$I, (%esp)
				# b_deref (signed long int)
	movl	(%esp), %eax
	movl	(%eax), %edx
	movl	%edx, (%esp)
				# b_load_arg (signed long int)
	movl	(%esp), %eax
	addl	$8, %esp
	movl	%eax, 8(%esp)
				# b_funcall_by_name (Printf, void)
	call	Printf
	addl	$32, %esp
	movl	(%esp), %ecx
	movl	%ecx, %esp
 #   51
				# b_alloc_arglist (8 bytes)
	movl	%esp, %eax
	subl	$4, %esp
	andl	$-16, %esp
	movl	%eax, (%esp)
	subl	$16, %esp
				# b_funcall_by_name (Inner2, void)
	call	Inner2
	addl	$16, %esp
	movl	(%esp), %ecx
	movl	%ecx, %esp
 #   52
				# b_push_ext_addr (I)
	subl	$8, %esp
	movl	$I, (%esp)
				# b_push_ext_addr (I)
	subl	$8, %esp
	movl	$I, (%esp)
				# b_deref (signed long int)
	movl	(%esp), %eax
	movl	(%eax), %edx
	movl	%edx, (%esp)
				# b_push_const_int (1)
	movl	$1, %eax
	subl	$8, %esp
	movl	%eax, (%esp)
				# b_arith_rel_op ( + , signed long int)
	movl	(%esp), %ecx
	addl	$8, %esp
	movl	(%esp), %eax
	addl	%ecx, %eax
	movl	%eax, (%esp)
				# b_assign (signed long int)
	movl	(%esp), %edx
	addl	$8, %esp
	movl	(%esp), %eax
	movl	%edx, (%eax)
	movl	%edx, (%esp)
				# b_pop ()
	addl	$8, %esp
 #   53
 #   54
				# b_alloc_arglist (24 bytes)
	movl	%esp, %eax
	subl	$4, %esp
	andl	$-16, %esp
	movl	%eax, (%esp)
	subl	$32, %esp
				# b_push_const_string ("f: c = %ld; i = %ld\n")
	.section	.rodata
.LC3:
.string	"f: c = %ld; i = %ld\n"
	.text
				# b_push_ext_addr (.LC3)
	subl	$8, %esp
	movl	$.LC3, (%esp)
				# b_load_arg (pointer)
	movl	(%esp), %eax
	addl	$8, %esp
	movl	%eax, 0(%esp)
				# b_push_ext_addr (C)
	subl	$8, %esp
	movl	$C, (%esp)
				# b_der
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
				# b_global_decl (Z, alignment = 4, size = 4)
.globl Z
	.align	4
	.type	Z, @object
	.size	Z, 4
Z:
	.zero	4
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
				# b_global_decl (Ip, alignment = 4, size = 4)
.globl Ip
	.align	4
	.type	Ip, @object
	.size	Ip, 4
Ip:
	.zero	4
 #   13
				# b_global_decl (Rp, alignment = 4, size = 4)
.globl Rp
	.align	4
	.type	Rp, @object
	.size	Rp, 4
Rp:
	.zero	4
 #   14
 #   15
 #   16
 #   17
 #   18
 #   19
				# b_init_formal_param_offset ()
				# b_func_prologue (Xyz)
	.text
.global Xyz
	.type	Xyz, @function
Xyz:
	pushl	%ebp
	movl	%esp, %ebp
				# b_alloc_local_vars ( size = 0 )
 #   20
 #   21
				# b_push_ext_addr (D)
	subl	$8, %esp
	movl	$D, (%esp)
				# b_push_const_double (1.9989999999999998e+01)
	.section	.rodata
	.align	8
.LC0:
	.long	-1546188227
	.long	1077149040
	.text
	fldl	.LC0
	subl	$8, %esp
	fstpl	(%esp)
				# b_assign (double)
	fldl	(%esp)
	addl	$8, %esp
	movl	(%esp), %eax
	fstpl	(%eax)
	fstpl	(%esp)
				# b_pop ()
	addl	$8, %esp
				# b_func_epilogue (Xyz)
	leave
	ret
	.size	Xyz, .-Xyz
 #   22
 #   23
 #   24
				# b_init_formal_param_offset ()
				# b_func_prologue (Rst)
.global Rst
	.type	Rst, @function
Rst:
	pushl	%ebp
	movl	%esp, %ebp
				# b_alloc_local_vars ( size = 0 )
 #   25
				# b_push_const_int (10)
	movl	$10, %eax
	subl	$8, %esp
	movl	%eax, (%esp)
 #   26
 #   27
				# b_func_epilogue (Rst)
	leave
	ret
	.size	Rst, .-Rst
 #   28
 #   29
 #   30
				# b_init_formal_param_offset ()
				# b_func_prologue (Abc)
.global Abc
	.type	Abc, @function
Abc:
	pushl	%ebp
	movl	%esp, %ebp
				# b_alloc_return_value ( )
				# b_alloc_local_vars ( size = 8 )
	subl	$8, %esp
				# b_alloc_local_vars ( size = 0 )
 #   31
 #   32
 #   33
 #   34
				# b_push_const_int (19)
	movl	$19, %eax
	subl	$8, %esp
	movl	%eax, (%esp)
				# b_set_return (signed long int)
	movl	(%esp), %eax
	movl	%eax, -8(%ebp)
	addl	$8, %esp
				# b_prepare_return (signed long int)
	movl	-8(%ebp), %eax
				# b_func_epilogue (Abc)
	leave
	ret
	.size	Abc, .-Abc
 #   35
 #   36
 #   37
				# b_func_prologue (main)
.global main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	andl	$-16, %esp
 #   38
 #   39
				# b_alloc_arglist (8 bytes)
	movl	%esp, %eax
	subl	$4, %esp
	andl	$-16, %esp
	movl	%eax, (%esp)
	subl	$16, %esp
				# b_push_const_string ("this is a debugging message\n")
	.section	.rodata
.LC1:
.string	"this is a debugging message\n"
	.text
				# b_push_ext_addr (.LC1)
	subl	$8, %esp
	movl	$.LC1, (%esp)
				# b_load_arg (pointer)
	movl	(%esp), %eax
	addl	$8, %esp
	movl	%eax, 0(%esp)
				# b_funcall_by_name (Printf, void)
	call	Printf
	addl	$16, %esp
	movl	(%esp), %ecx
	movl	%ecx, %esp
 #   40
 #   41
				# b_alloc_arglist (0 bytes)
	movl	%esp, %eax
	subl	$4, %esp
	andl	$-16, %esp
	movl	%eax, (%esp)
	subl	$0, %esp
				# b_funcall_by_name (Xyz, void)
	call	Xyz
	addl	$0, %esp
	movl	(%esp), %ecx
	movl	%ecx, %esp
 #   42
				# b_alloc_arglist (0 bytes)
	movl	%esp, %eax
	subl	$4, %esp
	andl	$-16, %esp
	movl	%eax, (%esp)
	subl	$0, %esp
				# b_funcall_by_name (Rst, void)
	call	Rst
	addl	$0, %esp
	movl	(%esp), %ecx
	movl	%ecx, %esp
 #   43
 #   44
 #   45
				# b_push_ext_addr (K)
	subl	$8, %esp
	movl	$K, (%esp)
				# b_alloc_arglist (0 bytes)
	movl	%esp, %eax
	subl	$4, %esp
	andl	$-16, %esp
	movl	%eax, (%esp)
	subl	$0, %esp
				# b_funcall_by_name (Abc, signed long int)
	call	Abc
	addl	$0, %esp
	movl	(%esp), %ecx
	movl	%ecx, %esp
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
				# b_func_epilogue (main)
	leave
	ret
	.size	main, .-main
 #   46

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
 #   32
				# b_init_formal_param_offset ()
 #   33
				# b_global_decl (I, alignment = 4, size = 4)
.globl I
	.align	4
	.type	I, @object
	.size	I, 4
I:
	.zero	4
 #   34
				# b_func_prologue (Inner1)
	.text
.global Inner1
	.type	Inner1, @function
Inner1:
	pushl	%ebp
	movl	%esp, %ebp
				# b_alloc_local_vars ( size = 0 )
 #   35
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
 #   36
 #   37
				# b_func_epilogue (Inner1)
	leave
	ret
	.size	Inner1, .-Inner1
 #   38
 #   39
				# b_init_formal_param_offset ()
 #   40
				# b_global_decl (I, alignment = 4, size = 4)
.globl I
	.data
	.align	4
	.type	I, @object
	.size	I, 4
I:
	.zero	4
 #   41
				# b_func_prologue (Inner2)
	.text
.global Inner2
	.type	Inner2, @function
Inner2:
	pushl	%ebp
	movl	%esp, %ebp
				# b_alloc_local_vars ( size = 0 )
 #   42
 #   43
				# b_func_epilogue (Inner2)
	leave
	ret
	.size	Inner2, .-Inner2
 #   44
 #   45
				# b_func_prologue (F)
.global F
	.type	F, @function
F:
	pushl	%ebp
	movl	%esp, %ebp
				# b_alloc_local_vars ( size = 0 )
 #   46
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
 #   47
 #   48
				# b_alloc_arglist (0 bytes)
	movl	%esp, %eax
	subl	$4, %esp
	andl	$-16, %esp
	movl	%eax, (%esp)
	subl	$0, %esp
				# b_funcall_by_name (Inner2, void)
	call	Inner2
	addl	$0, %esp
	movl	(%esp), %ecx
	movl	%ecx, %esp
				# b_func_epilogue (F)
	leave
	ret
	.size	F, .-F
 #   49
 #   50
 #   51
 #   52
				# b_init_formal_param_offset ()
 #   53
				# b_global_decl (I1, alignment = 4, size = 4)
.globl I1
	.data
	.align	4
	.type	I1, @object
	.size	I1, 4
I1:
	.zero	4
 #   54
				# b_global_decl (C1, alignment = 1, size = 1)
.globl C1
	.align	1
	.type	C1, @object
	.size	C1, 1
C1:
	.zero	1
 #   55
				# b_global_decl (D1, alignment = 8, size = 8)
.globl D1
	.align	8
	.type	D1, @object
	.size	D1, 8
D1:
	.zero	8
 #   56
 #   57
 #   58
				# b_global_decl (I2, alignment = 4, size = 4)
.globl I2
	.align	4
	.type	I2, @object
	.size	I2, 4
I2:
	.zero	4
 #   59
				# b_global_decl (D2, alignment = 8, size = 8)
.globl D2
	.align	8
	.type	D2, @object
	.size	D2, 8
D2:
	.zero	8
 #   60
 #   61
				# b_init_formal_param_offset ()
				# b_func_prologue (Innerinner)
	.text
.global Innerinner
	.type	Innerinner, @function
Innerinner:
	pushl	%ebp
	movl	%esp, %ebp
				# b_alloc_return_value ( )
				# b_alloc_local_vars ( size = 8 )
	subl	$8, %esp
				# b_alloc_local_vars ( size = 0 )
 #   62
				# b_alloc_arglist (8 bytes)
	movl	%esp, %eax
	subl	$4, %esp
	andl	$-16, %esp
	movl	%eax, (%esp)
	subl	$16, %esp
				# b_funcall_by_name (F, void)
	call	F
	addl	$16, %esp
	movl	(%esp), %ecx
	movl	%ecx, %esp
 #   63
 #   64
				# b_push_ext_addr (I)
	subl	$8, %esp
	movl	$I, (%esp)
				# b_deref (float)
	movl	(%esp), %eax
	movl	(%eax), %edx
	movl	%edx, (%esp)
				# b_convert (float -> double)
	flds	(%esp)
	fstpl	(%esp)
				# b_push_ext_addr (I1)
	subl	$8, %esp
	movl	$I1, (%esp)
				# b_deref (signed long int)
	movl	(%esp), %eax
	movl	(%eax), %edx
	movl	%edx, (%esp)
				# b_convert (signed long int -> double)
	fildl	(%esp)
	fstpl	(%esp)
				# b_arith_rel_op ( + , double)
	fldl	8(%esp)
	fldl	(%esp)
	addl	$8, %esp
	faddp	%st, %st(1)
	fstpl	(%esp)
				# b_push_ext_addr (I2)
	subl	$8, %esp
	movl	$I2, (%esp)
				# b_deref (signed long int)
	movl	(%esp), %eax
	movl	(%eax), %edx
	movl	%edx, (%esp)
				# b_convert (signed long int -> double)
	fildl	(%esp)
	fstpl	(%esp)
				# b_arith_rel_op ( + , double)
	fldl	8(%esp)
	fldl	(%esp)
	addl	$8, %esp
	faddp	%st, %st(1)
	fstpl	(%esp)
				# b_push_ext_addr (D)
	subl	$8, %esp
	movl	$D, (%esp)
				# b_deref (double)
	movl	(%esp), %eax
	fldl	(%eax)
	fstpl	(%esp)
				# b_arith_rel_op ( + , double)
	fldl	8(%esp)
	fldl	(%esp)
	addl	$8, %esp
	faddp	%st, %st(1)
	fstpl	(%esp)
				# b_push_ext_addr (D1)
	subl	$8, %esp
	movl	$D1, (%esp)
				# b_deref (double)
	movl	(%esp), %eax
	fldl	(%eax)
	fstpl	(%esp)
				# b_arith_rel_op ( + , double)
	fldl	8(%esp)
	fldl	(%esp)
	addl	$8, %esp
	faddp	%st, %st(1)
	fstpl	(%esp)
				# b_push_ext_addr (D2)
	subl	$8, %esp
	movl	$D2, (%esp)
				# b_deref (double)
	movl	(%esp), %eax
	fldl	(%eax)
	fstpl	(%esp)
				# b_arith_rel_op ( + , double)
	fldl	8(%esp)
	fldl	(%esp)
	addl	$8, %esp
	faddp	%st, %st(1)
	fstpl	(%esp)
				# b_set_return (double)
	movl	(%esp), %eax
	movl	%eax, -8(%ebp)
	movl	4(%esp), %edx
	movl	%edx, -4(%ebp)
	addl	$8, %esp
				# b_prepare_return (double)
	fldl	-8(%ebp)
				# b_func_epilogue (Innerinner)
	leave
	ret
	.size	Innerinner, .-Innerinner
 #   65
				# b_func_prologue (Inner1)
.global Inner1
	.type	Inner1, @function
Inner1:
	pushl	%ebp
	movl	%esp, %ebp
				# b_alloc_local_vars ( size = 0 )
 #   66
 #   67
				# b_push_ext_addr (D1)
	subl	$8, %esp
	movl	$D1, (%esp)
				# b_alloc_arglist (0 bytes)
	movl	%esp, %eax
	subl	$4, %esp
	andl	$-16, %esp
	movl	%eax, (%esp)
	subl	$0, %esp
				# b_funcall_by_name (Innerinner, double)
	call	Innerinner
	addl	$0, %esp
	movl	(%esp), %ecx
	movl	%ecx, %esp
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
				# b_func_epilogue (Inner1)
	leave
	ret
	.size	Inner1, .-Inner1
 #   68
 #   69
				# b_func_prologue (G)
.global G
	.type	G, @function
G:
	pushl	%ebp
	movl	%esp, %ebp
				# b_alloc_local_vars ( size = 0 )
 #   70
				# b_alloc_arglist (0 bytes)
	movl	%esp, %eax
	subl	$4, %esp
	andl	$-16, %esp
	movl	%eax, (%esp)
	subl	$0, %esp
				# b_funcall_by_name (Inner2, void)
	call	Inner2
	addl	$0, %esp
	movl	(%esp), %ecx
	movl	%ecx, %esp
 #   71
				# b_push_ext_addr (D1)
	subl	$8, %esp
	movl	$D1, (%esp)
				# b_alloc_arglist (0 bytes)
	movl	%esp, %eax
	subl	$4, %esp
	andl	$-16, %esp
	movl	%eax, (%esp)
	subl	$0, %esp
				# b_funcall_by_name (Innerinner, double)
	call	Innerinner
	addl	$0, %esp
	movl	(%esp), %ecx
	movl	%ecx, %esp
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
 #   72
 #   73
				# b_alloc_arglist (8 bytes)
	movl	%esp, %eax
	subl	$4, %esp
	andl	$-16, %esp
	movl	%eax, (%esp)
	subl	$16, %esp
				# b_funcall_by_name (F, void)
	call	F
	addl	$16, %esp
	movl	(%esp), %ecx
	movl	%ecx, %esp
				# b_func_epilogue (G)
	leave
	ret
	.size	G, .-G
 #   74
 #   75
 #   76
 #   77
 #   78
 #   79
 #   80
				# b_init_formal_param_offset ()
 #   81
				# b_global_decl (I, alignment = 4, size = 4)
.globl I
	.data
	.align	4
	.type	I, @object
	.size	I, 4
I:
	.zero	4
 #   82
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
 #   83
 #   84
				# b_func_prologue (Main)
	.text
.global Main
	.type	Main, @function
Main:
	pushl	%ebp
	movl	%esp, %ebp
				# b_alloc_local_vars ( size = 0 )
 #   85
 #   86
 #   87
				# b_alloc_arglist (8 bytes)
	movl	%esp, %eax
	subl	$4, %esp
	andl	$-16, %esp
	movl	%eax, (%esp)
	subl	$16, %esp
				# b_funcall_by_name (F, void)
	call	F
	addl	$16, %esp
	movl	(%esp), %ecx
	movl	%ecx, %esp
 #   88
				# b_alloc_arglist (16 bytes)
	movl	%esp, %eax
	subl	$4, %esp
	andl	$-16, %esp
	movl	%eax, (%esp)
	subl	$16, %esp
				# b_funcall_by_name (F, void)
	call	F
	addl	$16, %esp
	movl	(%esp), %ecx
	movl	%ecx, %esp
 #   89
				# b_alloc_arglist (8 bytes)
	movl	%esp, %eax
	subl	$4, %esp
	andl	$-16, %esp
	movl	%eax, (%esp)
	subl	$16, %esp
				# b_funcall_by_name (G, void)
	call	G
	addl	$16, %esp
	movl	(%esp), %ecx
	movl	%ecx, %esp
 #   90
				# b_alloc_arglist (16 bytes)
	movl	%esp, %eax
	subl	$4, %esp
	andl	$-16, %esp
	movl	%eax, (%esp)
	subl	$16, %esp
				# b_funcall_by_name (Swap, void)
	call	Swap
	addl	$16, %esp
	movl	(%esp), %ecx
	movl	%ecx, %esp
 #   91
 #   92
				# b_func_epilogue (Main)
	leave
	ret
	.size	Main, .-Main
 #   93
 #   94
 #   95
				# b_func_prologue (main)
.global main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	andl	$-16, %esp
 #   96
 #   97
 #   98
 #   99
				# b_alloc_arglist (0 bytes)
	movl	%esp, %eax
	subl	$4, %esp
	andl	$-16, %esp
	movl	%eax, (%esp)
	subl	$0, %esp
				# b_funcall_by_name (Main, void)
	call	Main
	addl	$0, %esp
	movl	(%esp), %ecx
	movl	%ecx, %esp
				# b_func_epilogue (main)
	leave
	ret
	.size	main, .-main
 #  100

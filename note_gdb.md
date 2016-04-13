# gdb 反汇编二进制可执行程序
```base
$gdb a.out
...
(gdb) disassemble /m main
Dump of assembler code for function main:
	0x0000000000400916 <+0>:     push   %rbp
	0x0000000000400917 <+1>:     mov    %rsp,%rbp
	0x000000000040091a <+4>:     sub    $0x140,%rsp
	0x0000000000400921 <+11>:    mov    %edi,-0x134(%rbp)
	0x0000000000400927 <+17>:    mov    %rsi,-0x140(%rbp)
	0x000000000040092e <+24>:    mov    %fs:0x28,%rax
	0x0000000000400937 <+33>:    mov    %rax,-0x8(%rbp)
	0x000000000040093b <+37>:    xor    %eax,%eax
	0x000000000040093d <+39>:    movl   $0x10,-0x12c(%rbp)
	0x0000000000400947 <+49>:    movl   $0x1,-0x130(%rbp)
	0x0000000000400951 <+59>:    cmpl   $0x2,-0x134(%rbp)
	0x0000000000400958 <+66>:    je     0x40099e <main+136>
	0x000000000040095a <+68>:    callq  0x400730 <__errno_location@plt>
	0x000000000040095f <+73>:    mov    (%rax),%eax
	0x0000000000400961 <+75>:    mov    %eax,%edi
	0x0000000000400963 <+77>:    callq  0x400800 <strerror@plt>
	0x0000000000400968 <+82>:    mov    %rax,%rdx
	0x000000000040096b <+85>:    mov -0x140(%rbp),%rax
	0x0000000000400972 <+92>:    mov    (%rax),%rax
	0x0000000000400975 <+95>:    mov    %rdx,%r8
	0x0000000000400978 <+98>:    mov $0x22,%ecx
	0x000000000040097d <+103>:   mov $0x400dc3,%edx
	0x0000000000400982 <+108>:   mov %rax,%rsi
	0x0000000000400985 <+111>:   mov $0x400cc8,%edi
	0x000000000040098a <+116>: mov    $0x0,%eax
	0x000000000040098f <+121>: callq  0x400770 <printf@plt>
	0x0000000000400994 <+126>:   mov $0x1,%edi
	0x0000000000400999 <+131>:   callq 0x4007e0 <exit@plt>
	0x000000000040099e <+136>:   mov $0x0,%edx
	0x00000000004009a3 <+141>:   mov $0x1,%esi
	---Type <return> to continue, or q <return> to quit---q
	Quit (gdb) q
```

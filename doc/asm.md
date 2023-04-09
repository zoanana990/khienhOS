# Inline Assembly

Note: Assembly template is basically GAS-compatible format, 

The basic assembly syntax is shown as following:
```c
asm ("assembly code");
```

The syntax for inline assembly
```c
asm ( "assembly code"
      : output operands                  /* optional */
      : input operands                   /* optional */
      : list of clobbered registers      /* optional */
);
```

Reference:
- [Extended Asm - Assembler Instructions with C Expression Operands](https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html)

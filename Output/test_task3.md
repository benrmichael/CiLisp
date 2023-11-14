# Task 3 Tests:

```bash
ben@comp232:~/LAB11/TASK3$ run
+ yacc -d cilisp.y
+ lex cilisp.l
+ cat cilisp_funcs.c cilisp.c lex.yy.c y.tab.c
+ gcc -g t.c -o cilisp -lm
```

```bash
ben@comp232:~/LAB11/TASK3$ cilisp < INPUTS/task_3.cilisp
```

- WARNING: Precision loss on int cast from 1.25 to 1  
Integer: 2

- Double: 30.000000

- Double: 9.000000

## Task 2 Tests:

```bash
ben@comp232:~/LAB11/TASK2$ run
+ yacc -d cilisp.y
+ lex cilisp.l
+ cat cilisp.c cilisp_funcs.c lex.yy.c y.tab.c
+ gcc -g t.c -o cilisp -lm
```

```bash
  ben@comp232:~/LAB11/TASK2$ cilisp < INPUTS/task_2.cilisp
```

- WARNING: Undefined Symbol "x" evaluated! NAN returned!
Double: nan

- Integer: 1

- WARNING: Duplicate assignment to symbol "x" detected in the same scope! Only the first assignment is kept!
Integer: 1

- Integer: 2

- Integer: 3

- Integer: 5

- WARNING: Undefined Symbol "b" evaluated! NAN returned!
Double: nan

- Integer: 1

- WARNING: Undefined Symbol "a" evaluated! NAN returned!
Double: nan

- Integer: 1
- Integer: 1
- Integer: 6
- Double: 4.242641
- Double: 5.398346
- Double: 17.414214
- Integer: 4

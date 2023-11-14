# Task 4 Tests:

```bash
ben@comp232:~/LAB11/TASK4$ run
+ yacc -d cilisp.y
+ lex cilisp.l
+ cat cilisp_funcs.c cilisp.c lex.yy.c y.tab.c
+ gcc -g t.c -o cilisp -lm
```

```bash
ben@comp232:~/LAB11/TASK4$ cilisp
```

- (print)  
WARNING: print called with no operands! nan returned!  
Double: nan

- (print 1)    
Integer: 1    
Integer: 1

- (print 1 2)  
WARNING: print called with extra (ignored) operands!  
Integer: 1  
Integer: 1

- (add 1 (print 2))  
Integer: 2  
Integer: 3  

- (rand)  
Double: 0.840188

- (rand)  
Double: 0.394383

- (rand)  
Double: 0.783099

- (rand)  
Double: 0.798440

- (rand)  
Double: 0.911647

- (rand)  
Double: 0.197551

- (rand)  
Double: 0.335223

- (read)  
read :: 1  
Integer: 1

- (read)  
read :: asdf  
WARNING: Invalid read entry! NAN returned!  
Double: nan

- (read)  
read :: .5  
WARNING: Invalid read entry! NAN returned!  
Double: nan

- (add (read) (read))  
read :: 10  
read :: -10  
Integer: 0

- (equal 0 0)  
Integer: 1

- (equal 0 0.0)  
Integer: 1

- (equal 0 0.0001)  
Integer: 0

- (less 0 0)  
Integer: 0

- (less -1 0)  
Integer: 1

- (less 0 -0.00001)  
Integer: 0

- (greater 0 1)  
Integer: 0

- (greater 1 0)  
Integer: 1

- (greater 0 0.0)  
Integer: 0

- ((let(x 0)(y 1))(less x y))  
Integer: 1

- (cond 0 5 6)  
Integer: 6

- (cond 1 5 6)  
Integer: 5

- (cond (less 10 1) 5 6)  
Integer: 5

- (cond (less 1 0) 5 6)  
Integer: 6

- ((let(x(read))(y(read)))(add(print x)(print x)(print y)(print y)))  
read :: -17.2  
Double: -17.200000  
Double: -17.200000  
read :: +127  
Integer: 127  
Integer: 127  
Double: 219.600000

- ((let(x(rand)))(add (print x) (print x)))  
Double: 0.768230  
Double: 0.768230  
Double: 1.536459

- quit
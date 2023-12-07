#ifndef __clt_h_
#define __clt_h_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <libgen.h>
#include <stdarg.h>

#define BUFFER_LEN 1024
#define RED 		"\033[31m"
#define RESET_COLOR "\033[0m"
#define GREEN		"\x1B[32m"

typedef enum test_type {
	ADD,
	ABS,
	CBRT,
	COMP,
	DIV,
	EXP,
	EXP2,
    HYPOT,
    LOG,
    MAX,
    MIN,
    MULT,
    NEG,
    NUMBER,
    POW,
    REM,
    SQRT,
    SUB,
    TASK2,
    TASK3,
    TASK4,
    TASK5
} file_t;

typedef enum num_type {
    INT,
    DOUBLE
} num_t;

file_t determineTestFile(char*);
int checkForMatch(regex_t*, char*, char*, int*, int, file_t);
int testFile(file_t);
int test(file_t, int, ...);
char *getExpectedValue(file_t, int);
char *genReg(num_t type, double value);
void setupFileVar(char*, char*, int);
void printTableHeader();
void printCase(file_t ft, char *given, int n, int passed);
void runTest(char*);
void clear(void);
void runMultipleTest(char*);
void error(int, char*, ...);

const char *test_f[] = {
	"add.cilisp",
	"abs.cilisp",
	"cbrt.cilisp",
	"composition.cilisp",
	"div.cilisp",
	"exp.cilisp",
	"exp2.cilisp",
	"hypot.cilisp",
    "log.cilisp",
    "max.cilisp",
    "min.cilisp",
    "mult.cilisp",
    "neg.cilisp",
    "number.cilisp",
    "pow.cilisp",
    "remainder.cilisp",
    "sqrt.cilisp",
    "sub.cilisp",
    "task_2.cilisp",
    "task_3.cilisp",
    "task_4.cilisp",
    "task_5.cilisp",
    "",
};

// Used to tell them what their output should have looked like :)
char *add_case_expected[] = {
	"(add)^WARNING: add called with no operands!", "(add)^Integer : 0", "(add 1)^Integer : 1", "(add 1.0)^Double : 1.000000",
	"(add 1 2 3 4 5)^Integer : 15", "(add 1 -2 3 -4 5 -6)^Integer : -3", "(add 0.0 1 -2 3 -4 5 -6)^Double : -3.000000", 
	"(add 1 -1.0)^Double : 0.000000", ""
};
char *abs_case_expected[] = {
	"(abs 1)^Integer : 1", "(abs 1,2)^Double : 1.200000", "(abs -3)^Integer : 3", "(abs 0)^Integer : 0", 
	"(abs 0.0)^Double : 0.000000", "(abs -1.4)^Double : 1.400000", "(abs)^WARNING: abs called with no operands!",
	"Double : nan", "(abs -1 2)^WARNING: abs called with extra (ignored) operands!", "Integer : 1", "" 
};
char *cbrt_case_expected[] = {
	"(cbrt)^WARNING: cbrt called with no operands! nan returned!",
	"(cbrt)^Double : nan",
	"(cbrt 0)^Double : 0.000000",
	"(cbrt 0.0)^Double : 0.000000",
	"(cbrt -1)^Double : -1.000000",
	"(cbrt -1.0)^Double : -1.000000",
	"(cbrt 1)^Double : 1.000000",
	"(cbrt 1.0)^Double : 1.000000",
	"(cbrt 27)^Double : 3.000000",
	"(cbrt 27.0)^Double : 3.000000",
	"(cbrt 4)^Double : 1.587401",
	"(cbrt 1 2)^WARNING: cbrt called with extra (ignored) operands!, Double : 1.000000",
	"(cbrt 1 2)^Double : 1.000000",
    ""
};
char *comp_case_expected[] = {
    "(exp (log 1))^Double : 1.000000",
    "(exp2 (div 1 2.0))^Double : 1.414214",
    "(cbrt (pow 3 3))^Double : 3.000000",
    "(cbrt (pow 3 6))^Double : 9.000000",
    "(log (exp (log (exp 1))))^Double : 1.000000",
    "(sub (mult 1 2 3 4) (add 1 2 3 4))^Integer : 14",
    "(sub (mult 1 2 3 -4.0) (add -1 -2 -3 -4))^Double : -14.000000",
    "(hypot (sqrt (div 100 7.0)) (mult 6 (sqrt (div 100.0 7))))^Double : 22.990681",
    "(hypot (sqrt (div 100 7.0)) (sqrt (mult 6 (div 100.0 7))))^Double : 10.000000",
    "(add 1 (add 2 (add 3 (add 4 (add 5 (add 6 (add 7)))))))^Integer : 28",
    "(add 1 (add 2 (add 3 (add 4 (add 5 (add 6 (sub 0 -7.0)))))))^Double : 28.000000",
    ""
};
char *div_case_expected[] = {
    "(div)^WARNING: div called with no operands! nan returned!",
    "(div)^Double : nan",
    "(div 1)^WARNING: div called with only one arg! nan returned!",
    "(div 1)^Double : nan",
    "(div 1.0)^WARNING: div called with only one arg! nan returned!",
    "(div 1.0)^Double : nan",
    "(div 1 2)^Integer : 0",
    "(div 1.0 2)^Double : 0.500000",
    "(div 2 1)^Integer : 2",
    "(div 2.0 1)^Double : 2.000000",
    "(div 5 2.0)^Double : 2.500000",
    "(div -20.0 4)^Double : -5.000000",
    "(div 1 2 3 4)^WARNING: div called with extra (ignored) operands!",
    "(div 1 2 3 4)^Integer : 0",
    "(div 1 2 3)^WARNING: div called with extra (ignored) operands!",
    "(div 1 2 3)^Integer : 0",
    "(div 5.0 2 3)^WARNING: div called with extra (ignored) operands!",
    "(div 5.0 2 3)^Double : 2.500000",
    ""
};

char *exp_case_expected[] = {
    "(exp)^WARNING: exp called with no operands! nan returned!",
    "(exp)^Double : nan",
    "(exp 1)^Double : 2.718282",
    "(exp -1)^Double : 0.367879",
    "(exp 5.0)^Double : 148.413159",
    "(exp -2.0)^Double : 0.135335",
    "(exp 1 2)^WARNING: exp called with extra (ignored) operands!",
    "(exp 1 2)^Double : 2.718282",
    ""
};
char *exp2_case_expected[] = {
    "(exp2)^WARNING: exp2 called with no operands! nan returned!",
    "(exp2)^Double : nan",
    "(exp2 1)^Integer : 2",
    "(exp2 1.0)^Double : 2.000000",
    "(exp2 0)^Integer : 1",
    "(exp2 0.0)^Double : 1.000000",
    "(exp2 0.5)^Double : 1.414214",
    "(exp2 -2)^Double : 0.250000",
    "(exp2 20.0)^Double : 1048576.000000",
    "(exp2 1 2)^WARNING: exp2 called with extra (ignored) operands!",
    "(exp2 1 2)^Integer : 2",
    ""
};
char *hypot_case_expected[] = {
    "(hypot)^WARNING: hypot called with no operands! 0 returned!",
    "(hypot)^Double : 0.000000",
    "(hypot 1)^Double : 1.000000",
    "(hypot 1.0)^Double : 1.000000",
    "(hypot 3 4)^Double : 5.000000",
    "(hypot -3 4)^Double : 5.000000",
    "(hypot -30 -40.0)^Double : 50.000000",
    "(hypot 4 4 7)^Double : 9.000000",
    "(hypot 7.0 4 4.0)^Double : 9.000000",
    "(hypot 12 13 14)^Double : 22.561028",
    "(hypot 5 5 5)^Double : 8.660254",
    "(hypot -5 -5.0 (sqrt 25))^Double : 8.660254",
    "(hypot 0 0 0.0 -3 0 0 0 0 4 0.0 -0.0 12)^Double : 13.000000",
    ""
};
char *log_case_expected[] = {
    "(log)^WARNING: log called with no operands! nan returned!",
    "(log)^Double : nan",
    "(log 1)^Double : 0.000000",
    "(log 0)^Double : -inf",
    "(log -1)^Double : nan",
    "(log 0.0)^Double : -inf",
    "(log -1.0)^Double : nan",
    "(log (exp 1))^Double : 1.000000",
    "(div (log 27) (log 3))^Double : 3.000000",
    "(div (log 27.0) (log 3))^Double : 3.000000",
    "(log 1 2)^WARNING: log called with extra (ignored) operands!",
    "(log 1 2)^Double : 0.000000",
    ""
};
char *max_case_expected[] = {
    "(max)^WARNING: max called with no operands! nan returned!",
    "(max)^Double : nan",
    "(max 1)^Integer : 1",
    "(max -1)^Integer : -1",
    "(max 1.0)^Double : 1.000000",
    "(max 232311.121)^Double : 232311.121000",
    "(max 1 2 3 4 5 6 7 8.0 9)^Integer : 9",
    "(max 1 2 25.0 -26.0 12)^Double : 25.000000",
    ""
};
char *min_case_expected[] = {
    "(min)^WARNING: min called with no operands! nan returned!",
    "(min)^Double : nan",
    "(min 1)^Integer : 1",
    "(min 0.0)^Double : 0.000000",
    "(min 0)^Integer : 0",
    "(min -1 2 -3 4 -5 6)^Integer : -5",
    "(min -1.0 -12.0 12)^Double : -12.000000",
    ""
};
char *mult_case_expected[] = {
    "(mult)^WARNING: mult called with no operands! 1 returned!",
    "(mult)^Integer : 1",
    "(mult 1)^Integer : 1",
    "(mult 1.0)^Double : 1.000000",
    "(mult -1)^Integer : -1",
    "(mult -1 -1.0)^Double : 1.000000",
    "(mult 1 -2 3 -4 5)^Integer : 120",
    "(mult -1.0 2 -3.0 4 -5)^Double : -120.000000",
    ""
};
char *neg_case_expected[] = {
    "(neg 5)^Integer : -5",
    "(neg 5.5)^Double : -5.500000",
    "(neg -5.0)^Double : 5.000000",
    "(neg -5)^Integer : 5",
    "(neg)^WARNING: neg called with no operands! nan returned!",
    "(neg)^Double : nan",
    "(neg 1 2)^WARNING: neg called with extra (ignored) operands!",
    "(neg 1 2)^Integer : -1",
    ""
};
char *literal_case_expected[] = {
    "0^Integer : 0",
    "0.^Double : 0.000000",
    "1^Integer : 1",
    "1.^Double : 1.000000",
    "0.0^Double : 0.000000",
    "0.5^Double : 0.500000",
    "+0^Integer : 0",
    "+10.55^Double : 10.550000",
    "-12.87^Double : -12.870000",
    "-12.^Double : -12.000000",
    "-12^Integer : -12",
    ".34^WARNING: Invalid character >>.<<",
    ".34^Integer : 34",
    ""
};
char *pow_case_expected[] = {
    "(pow)^WARNING: pow called with no operands! nan returned!",
    "(pow)^Double : nan",
    "(pow 1)^WARNING: pow called with only one operand! nan returned!",
    "(pow 1)^Double : nan",
    "(pow 1.0)^WARNING: pow called with only one operand! nan returned!",
    "(pow 1.0)^Double : nan",
    "(pow 1 1)^Integer : 1",
    "(pow 1 1.0)^Double : 1.000000",
    "(pow 2 1)^Integer : 2",
    "(pow 2.1 1)^Double : 2.100000",
    "(pow -2 0.5)^Double : nan",
    "(pow -2 0)^Integer : 1",
    "(pow -2.0 0.0)^Double : 1.000000",
    "(pow -2.0 0)^Double : 1.000000",
    "(pow 3 3)^Integer : 27",
    "(pow 3.0 3)^Double : 27.000000",
    "(pow 27 (div 1 3.0))^Double : 3.000000",
    "(pow 1 2 3)^WARNING: pow called with extra (ignored) operands!",
    "(pow 1 2 3)^Integer : 1",
    ""
};
char *remainder_case_expected[] = {
    "(remainder)^WARNING: remainder called with no operands! nan returned!",
    "(remainder)^Double : nan",
    "(remainder 1)^WARNING: remainder called with one arg! nan returned!, Double : nan",
    "(remainder 1)^Double : nan",
    "(remainder 0)^WARNING: remainder called with one arg! nan returned!",
    "(remainder 0)^Double : nan",
    "(remainder -1.0)^WARNING: remainder called with one arg! nan returned!",
    "(remainder -1.0)^Double : nan",
    "(remainder 1 2)^Integer : 1",
    "(remainder 2 1)^Integer : 0",
    "(remainder 2.5 1)^Double : 0.500000",
    "(remainder 2 3)^Integer : 2",
    "(remainder -6 10)^Integer : 4",
    "(remainder -6.0 10.0)^Double : 4.000000",
    "(remainder -6.0 -10.0)^Double : 4.000000",
    "(remainder 1 2 3)^WARNING: remainder called with extra (ignored) operands!",
    "(remainder 1 2 3)^Integer : 1",
    "(remainder 23 7 10)^WARNING: remainder called with extra (ignored) operands!",
    "(remainder 23 7 10)^Integer : 2",
    ""
};
char *sqrt_case_expected[] = {
    "(sqrt)^WARNING: sqrt called with no operands! nan returned!",
    "(sqrt)^Double : nan",
    "(sqrt 1)^Double : 1.000000",
    "(sqrt 1.0)^Double : 1.000000",
    "(sqrt 0)^Double : 0.000000",
    "(sqrt 0.0)^Double : 0.000000",
    "(sqrt -1)^Double : nan",
    "(sqrt -1.0)^Double : nan",
    "(sqrt 4)^Double : 2.000000",
    "(sqrt 170.0)^Double : 13.038405",
    "(sqrt 2)^Double : 1.414214",
    "(sqrt 1 2)^WARNING: sqrt called with extra (ignored) operands!",
    "(sqrt 1 2)^Double : 1.000000",
    ""
};
char *sub_case_expected[] = {
    "(sub)^WARNING: sub called with no operands! Double : nan",
    "(sub)^Double : nan",
    "(sub 1)^WARNING: sub called with only one arg!",
    "(sub 1)^Double : nan",
    "(sub 1.0)^WARNING: sub called with only one arg!",
    "(sub 1.0)^Double : nan",
    "(sub 1 2)^Integer : -1",
    "(sub 2 1)^Integer : 1",
    "(sub 2 -1)^Integer : 3",
    "(sub 2.0 1)^Double : 1.000000",
    "(sub 2.0 -1)^Double : 3.000000",
    "(sub 1 1.0)^Double : 0.000000",
    "(sub 2.0 1.0)^Double : 1.000000",
    "(sub 1 2 3)^WARNING: sub called with extra (ignored) operands!",
    "(sub 1 2 3)^Integer : -1",
    ""
};
char *task_2_expected[] = {
	"x^WARNING: Undefined Symbol \"x\" evaluated! NAN returned!",
	"x^Double : nan",
	"((let (x 1)) x)^Integer : 1",
	"((let (x 1) (x 2)) x)^WARNING: Duplicate assignment to symbol \"x\" detected in the same scope! Only the first assignment is kept!",
    "((let (x 1) (x 2)) x)^Integer : 1",
	"((let (x 1)) ((let (x 2)) x))^Integer : 2",
	"((let (x 1)) ((let (y 2)) (add x y)))^Integer : 3",
	"((let (a ((let (b 2)) (add b 3)))) a)^Integer : 5",
	"((let (a ((let (b 2)) (add b 3)))) (add a b))^WARNING: Undefined Symbol \"b\" evaluated! NAN returned!",
    "((let (a ((let (b 2)) (add b 3)))) (add a b))^Double : nan",
	"((let (a 1) (b a)) ((let (a 2)) b))^Integer : 1",
	"((let (b a)) ((let (a 2)) b))^WARNING: Undefined Symbol \"a\" evaluated! NAN returned!",
    "((let (b a)) ((let (a 2)) b))^Double : nan",
	"((let (a 1) (b a)) ((let (c ((let (a 2)) a))) b))^Integer : 1",
	"((let (y 1) (x y)) ((let (y 2) (c x)) x))^Integer : 1",
	"(add ((let (abc 1)) (sub 3 abc)) 4)^Integer : 6",
	"(mult ((let (a 1) (b 2)) (add a b)) (sqrt 2))^Double : 4.242641",
	"(add ((let (a ((let (b 2)) (mult b (sqrt 10))))) (div a 2)) ((let (c 5)) (sqrt c)))^Double : 5.398346",
	"((let (first (sub 5 1)) (second 2)) (add (pow 2 first) (sqrt second)))^Double : 17.414214",
	"((let (abc 1)) (sub ((let (abc 2) (de 3)) (add abc de)) abc))^Integer : 4",
    ""
};
char *task_3_expected[] = {
    "((let (int a 1.25))(add a 1))^WARNING: Precision loss",
    "((let (int a 1.25))(add a 1))^Integer : 2",
    "((let (double a 5))(add a 25))^Double : 30.000000",
    "((let (double $_1 (add 1 2))) (mult $_1 3))^Double : 9.000000",
    ""
};
char *task_4_expected[] = {
    "(print)^WARNING: print called with no operands!",
    "(print)^Double : nan",
    "(print 1)^Integer : 1",
    "(print 1)^Integer : 1",
    "(print 1 2)^WARNING: print called with extra (ignored) operands!",
    "(print 1 2)^Integer : 1",
    "(print 1 2)^Integer : 1",
    "(add 1 (print 2))^Integer : 2",
    "(add 1 (print 2))^Integer : 3",
    "(rand)^Double : [0 <= n <= 1]",
    "(rand)^Double : [0 <= n <= 1]",
    "(rand)^Double : [0 <= n <= 1]",
    "(rand)^Double : [0 <= n <= 1]",
    "(rand)^Double : [0 <= n <= 1]",
    "(rand)^Double : [0 <= n <= 1]",
    "(rand)^Double : [0 <= n <= 1]",
    "(rand)^Double : [0 <= n <= 1]",
    "(rand)^Double : [0 <= n <= 1]",
    "(rand)^Double : [0 <= n <= 1]",
    "(read)^read :: 1",
    "(read)^Integer : 1",
    "(read)^read :: asdf",
    "(read)^WARNING: Invalid read entry!",
    "(read)^Double : nan",
    "(read)^read :: .5",
    "(read)^WARNING: Invalid read entry!",
    "(read)^Double : nan",
    "(read)^read :: -5.5",
    "(read)^Double : -5.500000",
    "(add (read) (read))^read :: 10",
    "(add (read) (read))^read :: -10",
    "(add (read) (read))^Integer : 0",
    "(equal 0 0)^Integer : 1",
    "(equal 0 0.0)^Integer : 1",
    "(equal 0 0.0001)^Integer : 0",
    "(less 0 0)^Integer : 0",
    "(less -1 0)^Integer : 1",
    "(less 0 -0.00001)^Integer : 0",
    "(greater 0 1)^Integer : 0",
    "(greater 1 0)^Integer : 1",
    "(greater 0 0.0)^Integer : 0",
    "((let (x 0) (y 1)) (less x y))^Integer : 1",
    "(cond 0 5 6)^Integer : 6",
    "(cond 1 5 6)^Integer : 5",
    "(cond (less 0 1) 5 6)^Integer : 5",
    "(cond (less 1 0) 5 6)^Integer : 6",
    "((let (x (read)) (y (read))) (add (print x) (print x) (print y) (print y)))^read :: -17.2",
    "((let (x (read)) (y (read))) (add (print x) (print x) (print y) (print y)))^Double : -17.200000",
    "((let (x (read)) (y (read))) (add (print x) (print x) (print y) (print y)))^Double : -17.200000",
    "((let (x (read)) (y (read))) (add (print x) (print x) (print y) (print y)))^read :: +127",
    "((let (x (read)) (y (read))) (add (print x) (print x) (print y) (print y)))^Integer : 127",
    "((let (x (read)) (y (read))) (add (print x) (print x) (print y) (print y)))^Integer : 127",
    "((let (x (read)) (y (read))) (add (print x) (print x) (print y) (print y)))^Double : 219.600000",
    "((let (x (rand))) (add (print x) (print x)))^Double : [0 <= n <= 1]",
    "((let (x (rand))) (add (print x) (print x)))^Double : [0 <= n <= 1]",
    "((let (x (rand))) (add (print x) (print x)))^Double : [0 <= n <= 1]",
    ""
};
char *task_5_expected[] = {
    "( (let (int integerAdd lambda (x y) (add x y))) (integerAdd 1.495 1.495) )^WARNING: Precision loss",
    "( (let (int integerAdd lambda (x y) (add x y))) (integerAdd 1.495 1.495) )^Integer : 2",
    "( (let (x 1.495) (int x lambda (a b) (add a b))) (x x x) )^WARNING: Precision loss",
    "( (let (x 1.495) (int x lambda (a b) (add a b))) (x x x) )^Integer : 2",
    "( (let (double myFunc lambda (x y) (mult (add x 5) (sub y 2)))) (sub (myFunc 3 5)2) )^Double : 22.000000",
    "( (let (f lambda (x y) (add x y))) (f (sub 5 2) (mult 2 3)) )^Integer : 9",
    "( (let (int a 1) (f lambda (x y) (add x y))) (f 2 (f a 3)) )^Integer : 6",
    "( (let (x lambda () (rand))) (add (print (x)) (print (x))) )^Double : [0 <= n <= 1]",
    "( (let (x lambda () (rand))) (add (print (x)) (print (x))) )^Double : [0 <= n <= 1]",
    "( (let (x lambda () (rand))) (add (print (x)) (print (x))) )^Double : [0 <= n <= 1]",
    "((let (double gcd lambda (x y) (cond (greater y x) (gcd y x) (cond (equal y 0) x (gcd y (remainder x y)))))) (gcd 95.5 55))^Double : 0.500000",
    ""
};
#endif
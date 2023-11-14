# Shell Script

```bash
$ run
+ yacc -d cilisp.y
+ lex cilisp.l
+ cat cilisp.c lex.yy.c y.tab.c
+ gcc -g t.c -o cilisp -lm
```

# Task 1 Tests:

## (abs)

```bash
$ cilisp < INPUTS/task_1/abs.cilisp 
```

- Integer : 1
- Double : 1.200000
- Integer : 3
- Integer : 0
- Double : 0.000000
- Double : 1.400000
- WARNING: abs called with no operands! nan returned!
- Double : nan
- WARNING: abs called with extra (ignored) operands!
- Integer : 1

## (exp)

```bash
$ cilisp < INPUTS/task_1/exp.cilisp 
```

- WARNING: exp called with no operands! nan returned!
- Double : nan
- Double : 2.718282
- Double : 0.367879
- Double : 148.413159
- Double : 0.135335
- WARNING: exp called with extra (ignored) operands!
- Double : 2.718282

## (exp2)

```bash
$ cilisp < INPUTS/task_1/exp2.cilisp
``` 

- Double : 7.389056 
- WARNING: exp called with extra (ignored) operands!
- Double : 7.389056
- WARNING: exp called with extra (ignored) operands!
- Double : 7.389056
- WARNING: exp called with extra (ignored) operands!
- Double : 7.389056
- WARNING: exp called with extra (ignored) operands!
- Double : 7.389056
- WARNING: exp called with extra (ignored) operands!
- Double : 7.389056
- WARNING: exp called with extra (ignored) operands!
- Double : 7.389056
- WARNING: exp called with extra (ignored) operands!
- Double : 7.389056
- WARNING: exp called with extra (ignored) operands!
- Double : 7.389056

## (log)

```bash
$ cilisp < INPUTS/task_1/log.cilisp
``` 

- WARNING: log called with no operands! nan returned!
- Double : nan
- Double : 0.000000
- Double : -inf
- Double : -nan
- Double : -inf
- Double : -nan
- Double : 1.000000
- Double : 3.000000
- Double : 3.000000
- WARNING: log called with extra (ignored) operands!
- Double : 0.000000
## (log)

```bash
$ cilisp < INPUTS/task_1/log.cilisp
``` 

- WARNING: log called with no operands! nan returned!
- Double : nan
- Double : 0.000000
- Double : -inf
- Double : -nan
- Double : -inf
- Double : -nan
- Double : 1.000000
- Double : 3.000000
- Double : 3.000000
- WARNING: log called with extra (ignored) operands!
- Double : 0.000000

## (sqrt)

```bash
$ cilisp < INPUTS/task_1/sqrt.cilisp
```

- WARNING: sqrt called with no operands! nan returned!
- Double : nan
- Double : 1.000000
- Double : 1.000000
- Double : 0.000000
- Double : 0.000000
- Double : -nan
- Double : -nan
- Double : 2.000000
- Double : 13.038405
- Double : 1.414214
- WARNING: sqrt called with extra (ignored) operands!
- Double : 1.000000

## (cbrt)

```bash
$ cilisp < INPUTS/task_1/cbrt.cilisp
```

- WARNING: cbrt called with no operands! nan returned!
- Double : nan
- Double : 0.000000
- Double : 0.000000
- Double : -1.000000
- Double : -1.000000
- Double : 1.000000
- Double : 1.000000
- Double : 3.000000
- Double : 3.000000
- Double : 1.587401
- WARNING: cbrt called with extra (ignored) operands!
- Double : 1.000000

## (neg)

```bash
$ cilisp < INPUTS/task_1/neg.cilisp
```

- Integer : -5
- Double : -5.500000
- Double : 5.000000
- Integer : 5
- WARNING: neg called with no operands! nan returned!
- Double : nan
- WARNING: neg called with extra (ignored) operands!
- Integer : -1

## (add)

```bash
$ cilisp < INPUTS/task_1/add.cilisp
```

- WARNING: add called with no operands! 0 returned!
- Integer : 0
- Integer : 1
- Double : 1.000000
- Integer : 15
- Integer : -3
- Double : -3.000000
- Double : 0.000000

## (div)

```bash
$ cilisp < INPUTS/task_1/div.cilisp
```

- WARNING: div called with no operands! nan returned!
- Double : nan
- WARNING: div called with only one arg! nan returned!
- Double : nan
- WARNING: div called with only one arg! nan returned!
- Double : nan
- Integer : 0
- Double : 0.500000
- Integer : 2
- Double : 2.000000
- Double : 2.500000
- Double : -5.000000
- WARNING: div called with extra (ignored) operands!
- Integer : 0
- WARNING: div called with extra (ignored) operands!
- Integer : 0
- WARNING: div called with extra (ignored) operands!
- Double : 2.500000

## (remainder)

```bash
$ cilisp < INPUTS/task_1/remainder.cilisp
```

- WARNING: remainder called with no operands! nan returned!
- Double : nan
- WARNING: remainder called with only one arg! nan returned!
- Double : nan
- WARNING: remainder called with only one arg! nan returned!
- Double : nan
- WARNING: remainder called with only one arg! nan returned!
- Double : nan
- Integer : 1
- Integer : 0
- Double : 0.500000
- Integer : 2
- Integer : -6
- Double : -6.000000
- Double : -6.000000
- WARNING: remainder called with extra (ignored) operands!
- Integer : 1
- WARNING: remainder called with extra (ignored) operands!
- Integer : 2

## (hypot)

```bash
$ cilisp < INPUTS/task_1/hypot.cilisp
```

- WARNING: hypot called with no operands! 0 returned!
- Double : 0.000000
- Double : 1.000000
- Double : 1.000000
- Double : 5.000000
- Double : 5.000000
- Double : 50.000000
- Double : 9.000000
- Double : 9.000000
- Double : 22.561028
- Double : 8.660254
- Double : 8.660254
- Double : 13.000000

## (pow)

```bash
$ cilisp < INPUTS/task_1/pow.cilisp
```

- Integer : 0
- Double : 0.000000
- Integer : 1
- Double : 1.000000
- Double : 0.000000
- Double : 0.500000
- Integer : 0
- Double : 10.550000
- Double : -12.870000
- Double : -12.000000
- Integer : -12
- LEX: INVALID "."
- WARNING: Invalid character >>.<<
- Integer : 34

## (number)

```bash
$ cilisp < INPUTS/task_1/number.cilisp
```

- WARNING: log called with no operands! nan returned!
- Double : nan
- Double : 0.000000
- Double : -inf
- Double : -nan
- Double : -inf
- Double : -nan
- Double : 1.000000
- Double : 3.000000
- Double : 3.000000
- WARNING: log called with extra (ignored) operands!
- Double : 0.000000

## (log)

```bash
$ cilisp < INPUTS/task_1/log.cilisp
```

- WARNING: sub called with no operands! nan returned!
- Double : nan
- WARNING: sub called with only one arg! nan returned!
- Double : nan
- WARNING: sub called with only one arg! nan returned!
- Double : nan
- Integer : -1
- Integer : 1
- Integer : 3
- Double : 1.000000
- Double : 3.000000
- Double : 0.000000
- Double : 1.000000
- WARNING: sub called with extra (ignored) operands!
- Integer : -1

## (sub)

```bash
$ cilisp < INPUTS/task_1/sub.cilisp
```

- WARNING: min called with no operands! nan returned!
- Double : nan
- Integer : 1
- Double : 0.000000
- Integer : 0
- Integer : -5
- Double : -12.000000

## (min)

```bash
$ cilisp < INPUTS/task_1/min.cilisp
```

- WARNING: max called with no operands! nan returned!
- Double : nan
- Integer : 1
- Integer : -1
- Double : 1.000000
- Double : 232311.121000
- Integer : 9
- Double : 25.000000

## (max)

```bash
$ cilisp < INPUTS/task_1/max.cilisp
```

- Double : 1.000000
- WARNING: exp called with extra (ignored) operands!
- Double : 7.389056
- Double : 3.000000
- Double : 9.000000
- Double : 1.000000
- Integer : 14
- Double : -14.000000
- Double : 22.990681
- Double : 10.000000
- Integer : 28
- Double : 28.000000

## (composition)

```bash
$ cilisp < INPUTS/task_1/composition.cilisp
```

- Double : 1.000000
- WARNING: exp called with extra (ignored) operands!
- Double : 7.389056
- Double : 3.000000
- Double : 9.000000
- Double : 1.000000
- Integer : 14
- Double : -14.000000
- Double : 22.990681
- Double : 10.000000
- Integer : 28
- Double : 28.000000

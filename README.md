# CiLisp
COMP-232 final project. Building an interpreter for a Lisp-like language. Below are the instructions:

## LAB 11 - ciLisp Final Project
John Dempsey
COMP-232 Programming Languages
California State University, Channel Islands
November 8, 2023
Final Due Date: December 7, 2023 @ 11:59 PM

In this project, you will implement an interpreter for CI LISP, a Channel Islands Lisp-like language. Each task below will expand the grammar. Your job is to expand the lexer, parser, data structures, parsing methods, and evaluation methods to meet the new requirements. The files provided will build and run (by running the “run” script), but isn't capable of actually doing anything other than crashing or quitting.

## Tasks
- **TASK 1**: Tokenization, Parsing and Evaluation of Cambridge Polish Notation arithmetic
- **TASK 2**: Symbol definition and evaluation
- **TASK 3**: Typecasting
- **TASK 4**: Conditionals, comparisons, read, random and print
- **TASK 5**: Composable, recursion-capable user-defined functions

## Point Breakdown
- **Task 1**: 50 points
- **Task 2**: 15 points
- **Task 3**: 15 points
- **Task 4**: 10 points
- **Task 5**: 10 points

## Notes
1. After completing a task, you need to upload your program to LAB11/TASK# on comp232.com so instructor can review your work.
2. After completing one task, you need to copy your code from the previous task and start with this code for the next task. Your code written for each task needs to be found in your LAB11/TASK# directories.
3. You must be able to compile and run your code on comp232.com. If instructor can’t compile and/or run your code on comp232.com, I can’t grade it.
4. You need to provide either a “run” script or a Makefile so that the instructor can run your code easily on comp232.com.
5. You need to run the test cases found in the INPUT directory for each task. After you have run the test cases on your system, you need to upload your test output files, which must end in .txt, into your LAB11/TASK# directories so instructor can review your work for each task.
6. On at least a weekly basis, you should upload all your current work to comp232.com in the event that the server is down on or before May 18th.
7. On or before December 7th, all work must be uploaded to comp232.com. You will be given four (4) weeks to complete this assignment, but please manage your time wisely.

Student needs to create a LAB11 directory and individual task directories on comp232.com and upload all source code and sample test run outputs regardless if the student has completed the task or not. Instructor needs to see what tasks have been completed on-time or at least what progress has been made. Points can be deducted if student does not meet all task requirements by the above due dates. If student falls behind, instructor can request a one-on-one session to review and help with student’s progress.

Test input files can be found in the /home/LAB11/INPUTS directory. Sample test outputs from running your ciLisp program need to be found in your home directory under ~/LAB11/TASK# directory with file names ending in .txt on comp232.com.

### Tasks must be completed in order. There is no extra credit for starting a new task if the previous task isn’t completed first.

During finals week on December 7th, you need to have submitted all work before 11:59 PM. Late work will not be accepted as it’s finals week.

## Downloading /home/LAB11 Files
Files for this project are found on comp232.com in the /home/LAB11 directory. To download these files from comp232.com onto your system, run the following commands:

```bash
john@oho:~$ pwd  Start in your home directory on your laptop.
/home/john
john@oho:~$ mkdir LAB11  Create LAB11 directory.
john@oho:~$ cd LAB11  cd to LAB11.
john@oho:~/LAB11$ ls  Directory is empty.
john@oho:~/LAB11$ sftp comp232.com  Run sftp comp232.com.
Connected to comp232.com.
sftp> cd /home/LAB11  cd to /home/LAB11
sftp> mget -r *  Run mget -r * DON’T FORGET THE -r
... (output omitted for brevity)
sftp> quit
john@oho:~/LAB11$ ls  INPUTS is a directory containing test files.
INPUTS bison_flex.log cilisp.c cilisp.h cilisp.l cilisp.y yyreadprint.c
john@oho:~/LAB11$ ls -lR  Run a recursive list.
... (output omitted for brevity)
```

Double : 0.131546
> ((let (double gcd lambda (x y) (cond (greater y x) (gcd y x) (cond (equal y 0) x
(gcd y (remainder x y)))))) (gcd 95.5 55))
Double : 0.500000
> quit

Network programming Assignment 1
Author : Divesh Uttamchandani
ID: 2016A7PS0045P

A program to generate a tree of processes

Instructions:
1. Running `make` and it will create an executable tree
   Run `./tree` to run the program

2. Run `make debug` the debug file will be compiled which has a bit verbose
   output format
   Run `./tree_debug`
   
3. Run `make clean` to clean the executable files.


[NOTE1]: I have assumed the level of the initial parent to be 0. as the question
states that the parent creates n processes at level **1**. [End_of_Note1]

[NOTE2]: For the positon and level of processes, I have **exactly** followed the
diagram given in the question. If we look at the tree diagram on a single level,
there may be more than one processes with same postion number.
e.g in the diagram at level 2 we have numbering from 1 to n-1 for
__parent 1__ at level 1 and then 1 to n-1 for parent 2 at __level 1__.
This may create some confusion in the output for verbose output run 
`make debug` Which seems a bit better.
[End_of_Note2]

[NOTE3]: I am not printing "All children exited" for the leaf processes as 
question states that Parent exits printing "...". And leafs are not parents.
[End_of_Note3]

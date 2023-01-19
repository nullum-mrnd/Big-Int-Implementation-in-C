## Big Int Implementation in C

It's a project done in C, with the objective of implementing Big Integers (numbers bigger than the integer limit in the language) and five basic arithmetic operations (Addition, Subtraction, Multiplication, Division and Remainder) by using Linked Lists.

The way I did this was by using Doubly Linked Lists with a descriptor node which contained some important informations about the number, such as its sign (positive or negative), and pointers to its first and last digits.

The program receives either the inputs of the numbers and the arithmetic operation symbol, or a file containing one or more of the same inputs. It's possible to keep inserting direct inputs and seeing the results in the console, and also keep inserting files, but in this last case all the results will be available in a file after closing the program.
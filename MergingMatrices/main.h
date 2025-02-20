/*
Merging Matrices and Binary Search in a Dynamic Vector

Objective:
Develop an ANSI C application that reads two square matrices from text files, merges them into a new matrix using custom rules, and writes the resulting matrix to a file. The program should use pointer arithmetic and optimize memory usage.

Detailed Requirements:

Reading Matrices:

Input Files:
You will have two files: matrix1.txt and matrix2.txt.

File Format:

The first line contains a single integer N, which is the dimension of the square matrix.
The next N lines each contain N real numbers (floating-point values) separated by the vertical bar character (|), with no extra spaces.
Example of a 3×3 matrix file (matrix1.txt):

3
1.0|2.0|3.0
4.0|5.0|6.0
7.0|8.0|9.0
Merging the Matrices:

Create a new matrix merged[N][N] by combining matrix1 and matrix2 with the following rules:

Elements Above the Main Diagonal:
Use the corresponding element from matrix1.

Elements Below the Main Diagonal:
Use the corresponding element from matrix2.

Elements on the Main Diagonal:
Compute the element using a custom rule. For example, you can:

Option A: Set the element to the sum of all elements in the corresponding row from both matrices.
Option B: Set the element to 0.
Example using Option A:
For row 0, if matrix1 has elements 1.0, 2.0, 3.0 and matrix2 has 10.0, 11.0, 12.0, then the diagonal element becomes:
(1.0 + 2.0 + 3.0) + (10.0 + 11.0 + 12.0) = 6.0 + 33.0 = 39.0.

Writing the Merged Matrix:

Output the resulting matrix to a new file named merged_matrix.txt.
The output should maintain the same format as the input: one line per row, with elements separated by |.
Additional Features:

Menu-Driven Interface:
Provide a simple menu that lets the user:

Load the matrices from files.
Merge the matrices.
Display the original matrices and/or the merged matrix.
Exit the program.
Error Handling:

Handle errors such as file not found, incorrect file format, or mismatched dimensions gracefully.
Ensure that memory allocation errors are detected and reported.
ANSI C Compliance:

Use only ANSI C standard libraries.
Use pointer arithmetic to manipulate arrays.
Do not use Variable Length Arrays (VLAs).
*/

#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libmat/libmat.h"

#define FILE_PATH_MAT1 "Archs/matrix1.txt"
#define FILE_PATH_MAT2 "Archs/matrix2.txt"
#define FILE_PATH_MERGED "Archs/merged_matrix.txt"


#endif // MAIN_H_INCLUDED

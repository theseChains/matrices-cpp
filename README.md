# Matrix Class Implementation

## Introduction
This project provides an implementation of a Matrix class in C++. The Matrix class supports various matrix operations such as addition, subtraction, multiplication, and transpose. The class is designed to handle matrices of any size and provides a simple interface for interacting with matrix data.

## Features
- **Matrix Creation:** Initialize matrices with specified dimensions or copy from other matrices.
- **Matrix Comparison:** Check if two matrices are equal.
- **Matrix Addition and Subtraction:** Add or subtract two matrices.
- **Matrix Multiplication:** Multiply two matrices or a matrix by a scalar.
- **Matrix Transpose:** Transpose the given matrix.
- **Matrix Determinant:** Calculate the determinant of a matrix.
- **Matrix Inverse:** Compute the inverse of a matrix.
- **Matrix Complements:** Calculate the algebraic complements of a matrix.
- **Dynamic Resizing:** Change the dimensions of a matrix.
- **Element Access:** Access and modify matrix elements using the function call operator.

## Example Code
Here's an example of how to use the S21Matrix class:
```cpp
#include <iostream>
#include "s21_matrix_oop.h"

int main() {
    s21::S21Matrix mat1(2, 2); // Create a 2x2 matrix
    s21::S21Matrix mat2(2, 2); // Create another 2x2 matrix

    // Fill the matrices with some values
    mat1(0, 0) = 1;
    mat1(0, 1) = 2;
    mat1(1, 0) = 3;
    mat1(1, 1) = 4;

    mat2(0, 0) = 5;
    mat2(0, 1) = 6;
    mat2(1, 0) = 7;
    mat2(1, 1) = 8;

    // Add the matrices
    s21::S21Matrix result = mat1 + mat2;

    // Print the result
    std::cout << "Matrix 1:\n" << mat1;
    std::cout << "Matrix 2:\n" << mat2;
    std::cout << "Result of addition:\n" << result;

    return 0;
}
```

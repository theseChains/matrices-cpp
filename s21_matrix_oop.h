#ifndef CPP1_S21_MATRIXPLUS_S21_MATRIX_OOP_H_
#define CPP1_S21_MATRIXPLUS_S21_MATRIX_OOP_H_

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <utility>

namespace s21 {
class S21Matrix {
 public:
  S21Matrix();
  S21Matrix(int rows, int cols);
  S21Matrix(const S21Matrix& other);
  S21Matrix(S21Matrix&& other) noexcept;
  ~S21Matrix();

  [[nodiscard]] bool EqMatrix(const S21Matrix& other) const;
  void SumMatrix(const S21Matrix& other);
  void SubMatrix(const S21Matrix& other);
  void MulNumber(const double number);
  void MulMatrix(const S21Matrix& other);
  [[nodiscard]] S21Matrix Transpose() const;
  [[nodiscard]] S21Matrix CalcComplements() const;
  [[nodiscard]] double Determinant() const;
  [[nodiscard]] S21Matrix InverseMatrix() const;

  [[nodiscard]] S21Matrix operator+(const S21Matrix& other) const;
  [[nodiscard]] S21Matrix operator-(const S21Matrix& other) const;
  [[nodiscard]] S21Matrix operator*(const S21Matrix& other) const;
  [[nodiscard]] S21Matrix operator*(const double number) const;
  [[nodiscard]] bool operator==(const S21Matrix& other) const;
  S21Matrix& operator=(const S21Matrix& other);
  S21Matrix& operator=(S21Matrix&& other) noexcept;
  S21Matrix& operator+=(const S21Matrix& other);
  S21Matrix& operator-=(const S21Matrix& other);
  S21Matrix& operator*=(const S21Matrix& other);
  S21Matrix& operator*=(const double number);
  [[nodiscard]] double& operator()(int row, int column);
  [[nodiscard]] const double& operator()(int row, int column) const;

  [[nodiscard]] int GetCols() const;
  [[nodiscard]] int GetRows() const;

  void SetRows(int new_rows);
  void SetCols(int new_cols);

 private:
  void ChangeSize(int rows, int cols);

  S21Matrix GetMinorMatrix(int removed_row, int removed_col) const;

  void AllocateMemory();
  void FreeMemory();
  void CopyElements(const S21Matrix& other);

 private:
  int rows_{};
  int cols_{};
  double** matrix_{};
};

std::ostream& operator<<(std::ostream& out, const S21Matrix& matrix);
S21Matrix operator*(const double number, S21Matrix& matrix);
}  // namespace s21

#endif  // CPP1_S21_MATRIXPLUS_S21_MATRIX_OOP_H_

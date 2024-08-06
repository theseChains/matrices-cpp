#include "s21_matrix_oop.h"

namespace s21::constants {
constexpr double kPrecision{1e-7};
constexpr int kDefaultMatrixSize{3};
}  // namespace s21::constants

namespace s21 {
S21Matrix::S21Matrix()
    : S21Matrix{constants::kDefaultMatrixSize, constants::kDefaultMatrixSize} {}

S21Matrix::S21Matrix(int rows, int cols) : rows_{rows}, cols_{cols} {
  if (rows_ <= 0 || cols_ <= 0) {
    throw std::invalid_argument{
        "S21Matrix::S21Matrix(int, int): Matrix has improper dimensions. "
        "Rows and columns must be greater than zero."};
  }

  AllocateMemory();
}

S21Matrix::S21Matrix(const S21Matrix& other)
    : rows_{other.rows_}, cols_{other.cols_} {
  AllocateMemory();
  CopyElements(other);
}

S21Matrix::S21Matrix(S21Matrix&& other) noexcept
    : rows_{std::exchange(other.rows_, 0)},
      cols_{std::exchange(other.cols_, 0)},
      matrix_{std::exchange(other.matrix_, nullptr)} {}

S21Matrix::~S21Matrix() { FreeMemory(); }

[[nodiscard]] bool S21Matrix::EqMatrix(const S21Matrix& other) const {
  if (rows_ != other.rows_ || cols_ != other.cols_) return false;

  bool are_equal{true};
  for (int i{0}; i < rows_ && are_equal; ++i) {
    for (int j{0}; j < cols_ && are_equal; ++j) {
      if (std::abs(matrix_[i][j] - other(i, j)) > constants::kPrecision)
        are_equal = false;
    }
  }

  return are_equal;
}

void S21Matrix::SumMatrix(const S21Matrix& other) {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    throw std::invalid_argument{
        "S21Matrix::SumMatrix(const S21Matrix&): Matrix dimensions are not "
        "compatible for addition. "
        "Both matrices must have the same number of rows and columns."};
  }

  for (int i{0}; i < rows_; ++i) {
    for (int j{0}; j < cols_; ++j) {
      matrix_[i][j] += other(i, j);
    }
  }
}

void S21Matrix::SubMatrix(const S21Matrix& other) {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    throw std::invalid_argument{
        "S21Matrix::SubMatrix(const S21Matrix&): Matrix dimensions are not "
        "compatible for addition. "
        "Both matrices must have the same number of rows and columns."};
  }

  for (int i{0}; i < rows_; ++i) {
    for (int j{0}; j < cols_; ++j) {
      matrix_[i][j] -= other(i, j);
    }
  }
}

void S21Matrix::MulNumber(const double number) {
  for (int i{0}; i < rows_; ++i) {
    for (int j{0}; j < cols_; ++j) {
      matrix_[i][j] *= number;
    }
  }
}

void S21Matrix::MulMatrix(const S21Matrix& other) {
  if (cols_ != other.rows_) {
    throw std::invalid_argument{
        "S21Matrix::MulMatrix(const S21Matrix&): Matrix dimensions are not "
        "compatible for multiplication. "
        "Number of columns in the first matrix must be equal to the number of "
        "rows in the second matrix."};
  }

  S21Matrix result{other.rows_, cols_};
  for (int i{0}; i < rows_; ++i) {
    for (int j{0}; j < other.cols_; ++j) {
      double element{0.0};
      for (int k{0}; k < other.rows_; ++k) {
        element += matrix_[i][k] * other(k, j);
      }
      result(i, j) = element;
    }
  }

  *this = result;
}

[[nodiscard]] S21Matrix S21Matrix::Transpose() const {
  S21Matrix transposed{cols_, rows_};

  for (int i{0}; i < rows_; ++i) {
    for (int j{0}; j < cols_; ++j) {
      transposed(j, i) = matrix_[i][j];
    }
  }

  return transposed;
}

[[nodiscard]] S21Matrix S21Matrix::CalcComplements() const {
  if (cols_ != rows_) {
    throw std::invalid_argument{
        "S21Matrix::CalcComplements(): Matrix dimensions are not compatible "
        "for complements calculation. "
        "The matrix must be square."};
  }

  S21Matrix complements{rows_, cols_};
  if (rows_ == 1) {
    complements(0, 0) = 1;
  } else {
    for (int row{0}; row < rows_; ++row) {
      for (int column{0}; column < cols_; ++column) {
        S21Matrix minor_matrix{GetMinorMatrix(row, column)};
        complements(row, column) =
            pow(-1, row + column) * minor_matrix.Determinant();
      }
    }
  }

  return complements;
}

[[nodiscard]] double S21Matrix::Determinant() const {
  if (cols_ != rows_) {
    throw std::invalid_argument{
        "S21Matrix::Determinant(): Matrix dimensions are not compatible for "
        "determinant calculation. "
        "The matrix must be square."};
  }

  double determinant{0.0};

  int matrix_size{rows_};
  if (matrix_size == 1) {
    determinant = matrix_[0][0];
  } else if (matrix_size == 2) {
    determinant += matrix_[0][0] * matrix_[1][1];
    determinant -= matrix_[0][1] * matrix_[1][0];
  } else {
    for (int row{0}, column{0}; row < matrix_size; ++row) {
      S21Matrix cofactor_matrix{GetMinorMatrix(row, column)};
      double cofactor{cofactor_matrix.Determinant()};
      cofactor *= std::pow(-1, row + column) * matrix_[row][column];
      determinant += cofactor;
    }
  }

  return determinant;
}

[[nodiscard]] S21Matrix S21Matrix::InverseMatrix() const {
  if (cols_ != rows_) {
    throw std::invalid_argument{
        "S21Matrix::InverseMatrix(): Matrix dimensions are not compatible for "
        "inverse matrix calculation. "
        "The matrix must be square."};
  }
  if (Determinant() == 0.0) {
    throw std::runtime_error{
        "S21Matrix::InverseMatrix(): Matrix is singular, and its inverse does "
        "not exist. "
        "The determinant of the matrix is zero."};
  }

  S21Matrix inverse{rows_, cols_};
  if (rows_ == 1) {
    inverse(0, 0) = 1.0 / matrix_[0][0];
  } else {
    inverse = CalcComplements().Transpose();
    inverse.MulNumber(1.0 / Determinant());
  }

  return inverse;
}

[[nodiscard]] S21Matrix S21Matrix::operator+(const S21Matrix& other) const {
  S21Matrix result{*this};
  result.SumMatrix(other);
  return result;
}

[[nodiscard]] S21Matrix S21Matrix::operator-(const S21Matrix& other) const {
  S21Matrix result{*this};
  result.SubMatrix(other);
  return result;
}

[[nodiscard]] S21Matrix S21Matrix::operator*(const S21Matrix& other) const {
  S21Matrix result{*this};
  result.MulMatrix(other);
  return result;
}

[[nodiscard]] S21Matrix S21Matrix::operator*(const double number) const {
  S21Matrix result{*this};
  result.MulNumber(number);
  return result;
}

S21Matrix operator*(const double number, S21Matrix& matrix) {
  matrix.MulNumber(number);
  return matrix;
}

[[nodiscard]] bool S21Matrix::operator==(const S21Matrix& other) const {
  return EqMatrix(other);
}

S21Matrix& S21Matrix::operator=(const S21Matrix& other) {
  if (this == &other) return *this;

  FreeMemory();
  rows_ = other.rows_;
  cols_ = other.cols_;
  AllocateMemory();
  CopyElements(other);

  return *this;
}

S21Matrix& S21Matrix::operator=(S21Matrix&& other) noexcept {
  if (this == &other) return *this;

  FreeMemory();
  rows_ = std::exchange(other.rows_, 0);
  cols_ = std::exchange(other.cols_, 0);
  matrix_ = std::exchange(other.matrix_, nullptr);

  return *this;
}

S21Matrix& S21Matrix::operator+=(const S21Matrix& other) {
  SumMatrix(other);
  return *this;
}

S21Matrix& S21Matrix::operator-=(const S21Matrix& other) {
  SubMatrix(other);
  return *this;
}

S21Matrix& S21Matrix::operator*=(const S21Matrix& other) {
  MulMatrix(other);
  return *this;
}

S21Matrix& S21Matrix::operator*=(const double number) {
  MulNumber(number);
  return *this;
}

[[nodiscard]] double& S21Matrix::operator()(int row, int column) {
  if (row < 0 || column < 0 || row >= rows_ || column >= cols_) {
    throw std::out_of_range{
        "S21Matrix::operator()(int, int): Index out of range. "
        "Row and column indices must be non-negative and within the matrix "
        "dimensions."};
  }

  return matrix_[row][column];
}

[[nodiscard]] const double& S21Matrix::operator()(int row, int column) const {
  if (row < 0 || column < 0 || row >= rows_ || column >= cols_) {
    throw std::out_of_range{
        "S21Matrix::operator()(int, int) const: Index out of range. "
        "Row and column indices must be non-negative and within the matrix "
        "dimensions."};
  }

  return matrix_[row][column];
}

[[nodiscard]] int S21Matrix::GetCols() const { return cols_; }

[[nodiscard]] int S21Matrix::GetRows() const { return rows_; }

void S21Matrix::SetRows(int new_rows) {
  if (new_rows <= 0) {
    throw std::out_of_range{
        "S21Matrix::SetRows(int): New number of rows is out of range. "
        "It must be a positive integer."};
  }
  if (new_rows == rows_) return;

  ChangeSize(new_rows, cols_);
}

void S21Matrix::SetCols(int new_cols) {
  if (new_cols <= 0) {
    throw std::out_of_range{
        "S21Matrix::SetCols(int): New number of columns is out of range. "
        "It must be a positive integer."};
  }
  if (new_cols == cols_) return;

  ChangeSize(rows_, new_cols);
}

void S21Matrix::ChangeSize(int new_rows, int new_cols) {
  S21Matrix result{new_rows, new_cols};
  int number_of_rows_to_copy{std::min(new_rows, rows_)};
  int number_of_cols_to_copy{std::min(new_cols, cols_)};
  for (int i{0}; i < number_of_rows_to_copy; ++i) {
    for (int j{0}; j < number_of_cols_to_copy; ++j) {
      result(i, j) = matrix_[i][j];
    }
  }

  *this = result;
}

S21Matrix S21Matrix::GetMinorMatrix(int removed_row, int removed_col) const {
  S21Matrix minor_matrix{rows_ - 1, cols_ - 1};
  int minor_matrix_row{0};
  for (int row{0}; row < rows_; ++row) {
    int minor_matrix_column{0};
    for (int column{0}; column < cols_; ++column) {
      if (row != removed_row && column != removed_col) {
        minor_matrix(minor_matrix_row, minor_matrix_column++) =
            matrix_[row][column];
      }
    }
    if (row != removed_row) ++minor_matrix_row;
  }

  return minor_matrix;
}

void S21Matrix::AllocateMemory() {
  matrix_ = new double* [static_cast<std::size_t>(rows_)] {};
  for (int i{0}; i < rows_; ++i) {
    matrix_[i] = new double[static_cast<std::size_t>(cols_)]{};
  }
}

void S21Matrix::FreeMemory() {
  if (matrix_) {
    for (int i{0}; i < rows_; ++i) {
      if (matrix_[i]) {
        delete[] matrix_[i];
      }
    }
    delete[] matrix_;
    matrix_ = nullptr;
  }
}

void S21Matrix::CopyElements(const S21Matrix& other) {
  for (int i{0}; i < rows_; ++i) {
    for (int j{0}; j < cols_; ++j) {
      matrix_[i][j] = other(i, j);
    }
  }
}

std::ostream& operator<<(std::ostream& out, const S21Matrix& matrix) {
  for (int i{0}; i < matrix.GetRows(); ++i) {
    for (int j{0}; j < matrix.GetCols(); ++j) {
      out << matrix(i, j) << ' ';
    }
    out << '\n';
  }

  return out;
}
}  // namespace s21

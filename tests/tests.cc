#include "tests.h"

namespace s21 {
void S21MatrixTest::SetUp() {
  matrix1x1(0, 0) = 5;

  SetUp2x2Matrix();
  SetUp2x3Matrix();
  SetUp3x3Matrix();
  SetUp7x7Matrix();
}

void S21MatrixTest::SetUp2x2Matrix() {
  matrix2x2(0, 0) = 1;
  matrix2x2(0, 1) = 2;
  matrix2x2(1, 0) = 3;
  matrix2x2(1, 1) = 4;
}

void S21MatrixTest::SetUp2x3Matrix() {
  matrix2x3(0, 0) = 1;
  matrix2x3(0, 1) = 2;
  matrix2x3(0, 2) = 3;

  matrix2x3(1, 0) = 4;
  matrix2x3(1, 1) = 3;
  matrix2x3(1, 2) = 4;
}

void S21MatrixTest::SetUp3x3Matrix() {
  matrix3x3(0, 0) = 1;
  matrix3x3(0, 1) = 2;
  matrix3x3(0, 2) = 3;

  matrix3x3(1, 0) = 4;
  matrix3x3(1, 1) = 5;
  matrix3x3(1, 2) = 6;

  matrix3x3(2, 0) = 7;
  matrix3x3(2, 1) = 8;
  matrix3x3(2, 2) = 9;
}

void S21MatrixTest::SetUp7x7Matrix() {
  for (int i{0}; i < 7; ++i) {
    for (int j{0}; j < 7; ++j) {
      matrix7x7(i, j) = i * j;
    }
  }
}

TEST_F(S21MatrixTest, ConstructorTest) {
  S21Matrix matrix{10, 15};
  ASSERT_EQ(matrix.GetRows(), 10);
  ASSERT_EQ(matrix.GetCols(), 15);
  EXPECT_THROW((S21Matrix{5, 0}), std::invalid_argument);

  S21Matrix other{matrix};
  ASSERT_EQ(other.GetRows(), 10);
  ASSERT_EQ(other.GetCols(), 15);
  ASSERT_TRUE(matrix.EqMatrix(other));

  S21Matrix otherMoved{std::move(other)};
  ASSERT_EQ(otherMoved.GetRows(), 10);
  ASSERT_EQ(otherMoved.GetCols(), 15);
  ASSERT_TRUE(matrix.EqMatrix(otherMoved));
}

TEST_F(S21MatrixTest, EqMatrixTest) {
  ASSERT_TRUE(matrix2x2.EqMatrix(matrix2x2));
  ASSERT_FALSE(matrix3x3.EqMatrix(matrix2x2));
  S21Matrix matrix{3, 3};
  ASSERT_FALSE(matrix3x3.EqMatrix(matrix));
}

TEST_F(S21MatrixTest, SumMatrixTest) {
  matrix3x3.SumMatrix(matrix3x3);
  s21::S21Matrix result{3, 3};
  result(0, 0) = 2;
  result(0, 1) = 4;
  result(0, 2) = 6;

  result(1, 0) = 8;
  result(1, 1) = 10;
  result(1, 2) = 12;

  result(2, 0) = 14;
  result(2, 1) = 16;
  result(2, 2) = 18;

  ASSERT_EQ(result, matrix3x3);

  EXPECT_THROW(matrix3x3.SumMatrix(matrix2x2), std::invalid_argument);
}

TEST_F(S21MatrixTest, SubMatrixTest) {
  matrix3x3.SubMatrix(matrix3x3);
  s21::S21Matrix result{3, 3};
  ASSERT_EQ(result, matrix3x3);

  EXPECT_THROW(matrix3x3.SubMatrix(matrix2x2), std::invalid_argument);
}

TEST_F(S21MatrixTest, MulMatrixTest) {
  matrix3x3.MulMatrix(matrix3x3);
  s21::S21Matrix result{3, 3};
  result(0, 0) = 30;
  result(0, 1) = 36;
  result(0, 2) = 42;

  result(1, 0) = 66;
  result(1, 1) = 81;
  result(1, 2) = 96;

  result(2, 0) = 102;
  result(2, 1) = 126;
  result(2, 2) = 150;

  ASSERT_EQ(result, matrix3x3);

  EXPECT_THROW(matrix3x3.MulMatrix(matrix2x2), std::invalid_argument);
}

TEST_F(S21MatrixTest, MulNumberTest) {
  matrix3x3.MulNumber(-10);
  s21::S21Matrix result{3, 3};
  result(0, 0) = -10;
  result(0, 1) = -20;
  result(0, 2) = -30;

  result(1, 0) = -40;
  result(1, 1) = -50;
  result(1, 2) = -60;

  result(2, 0) = -70;
  result(2, 1) = -80;
  result(2, 2) = -90;

  ASSERT_EQ(result, matrix3x3);
}

TEST_F(S21MatrixTest, TransposeTest) {
  S21Matrix transposed{matrix2x3.Transpose()};
  s21::S21Matrix result{3, 2};
  result(0, 0) = 1;
  result(0, 1) = 4;

  result(1, 0) = 2;
  result(1, 1) = 3;

  result(2, 0) = 3;
  result(2, 1) = 4;

  ASSERT_EQ(transposed, result);
}

TEST_F(S21MatrixTest, CalcComplementsTest) {
  S21Matrix matrix{matrix1x1.CalcComplements()};
  S21Matrix identity{1, 1};
  identity(0, 0) = 1;
  ASSERT_EQ(matrix, identity);

  S21Matrix complements{matrix2x2.CalcComplements()};
  s21::S21Matrix result{2, 2};
  result(0, 0) = 4;
  result(0, 1) = -3;

  result(1, 0) = -2;
  result(1, 1) = 1;

  ASSERT_EQ(complements, result);

  EXPECT_THROW([[maybe_unused]] auto discard{matrix2x3.CalcComplements()},
               std::invalid_argument);
}

TEST_F(S21MatrixTest, DeterminantTest) {
  double determinant2x2{matrix2x2.Determinant()};
  double determinant3x3{matrix3x3.Determinant()};
  double determinant7x7{matrix7x7.Determinant()};

  ASSERT_DOUBLE_EQ(determinant2x2, -2.0);
  ASSERT_DOUBLE_EQ(determinant3x3, 0.0);
  ASSERT_DOUBLE_EQ(determinant7x7, 0.0);

  EXPECT_THROW([[maybe_unused]] auto discard{matrix2x3.Determinant()},
               std::invalid_argument);
}

TEST_F(S21MatrixTest, InverseMatrixTest) {
  S21Matrix inverse1x1{matrix1x1.InverseMatrix()};
  s21::S21Matrix result1x1{1, 1};
  result1x1(0, 0) = 0.2;
  ASSERT_EQ(inverse1x1, result1x1);

  S21Matrix inverse{matrix2x2.InverseMatrix()};
  s21::S21Matrix result{2, 2};
  result(0, 0) = -2;
  result(0, 1) = 1;

  result(1, 0) = 1.5;
  result(1, 1) = -0.5;

  ASSERT_EQ(inverse, result);

  EXPECT_THROW([[maybe_unused]] auto discard{matrix2x3.InverseMatrix()},
               std::invalid_argument);
  EXPECT_THROW([[maybe_unused]] auto discard{matrix3x3.InverseMatrix()},
               std::runtime_error);
}

TEST_F(S21MatrixTest, OperatorPlusTest) {
  matrix3x3 = matrix3x3 + matrix3x3;
  s21::S21Matrix result{3, 3};
  result(0, 0) = 2;
  result(0, 1) = 4;
  result(0, 2) = 6;

  result(1, 0) = 8;
  result(1, 1) = 10;
  result(1, 2) = 12;

  result(2, 0) = 14;
  result(2, 1) = 16;
  result(2, 2) = 18;

  ASSERT_EQ(result, matrix3x3);
}

TEST_F(S21MatrixTest, OperatorMinusTest) {
  matrix3x3 = matrix3x3 - matrix3x3;
  s21::S21Matrix result{3, 3};

  ASSERT_EQ(result, matrix3x3);
}

TEST_F(S21MatrixTest, OperatorPreMulNumberTest) {
  matrix2x2 = 3 * matrix2x2;
  s21::S21Matrix result{2, 2};
  result(0, 0) = 3;
  result(0, 1) = 6;

  result(1, 0) = 9;
  result(1, 1) = 12;

  ASSERT_EQ(result, matrix2x2);
}

TEST_F(S21MatrixTest, OperatorPostMulNumberTest) {
  matrix3x3 = matrix3x3 * 3;
  s21::S21Matrix result{3, 3};
  result(0, 0) = 3;
  result(0, 1) = 6;
  result(0, 2) = 9;

  result(1, 0) = 12;
  result(1, 1) = 15;
  result(1, 2) = 18;

  result(2, 0) = 21;
  result(2, 1) = 24;
  result(2, 2) = 27;

  ASSERT_EQ(result, matrix3x3);
}

TEST_F(S21MatrixTest, OperatorMulMatrixTest) {
  matrix3x3 = matrix3x3 * matrix3x3;
  s21::S21Matrix result{3, 3};
  result(0, 0) = 30;
  result(0, 1) = 36;
  result(0, 2) = 42;

  result(1, 0) = 66;
  result(1, 1) = 81;
  result(1, 2) = 96;

  result(2, 0) = 102;
  result(2, 1) = 126;
  result(2, 2) = 150;

  ASSERT_EQ(result, matrix3x3);
}

TEST_F(S21MatrixTest, OperatorEqualsTest) {
  ASSERT_TRUE(matrix3x3 == matrix3x3);
  ASSERT_FALSE(matrix2x3 == matrix3x3);
}

TEST_F(S21MatrixTest, OperatorAssignmentTest) {
  S21Matrix matrix{};
  matrix = matrix3x3;
  ASSERT_EQ(matrix, matrix3x3);

  s21::S21Matrix other{3, 3};
  other(0, 0) = 30;
  other(0, 1) = 36;
  other(0, 2) = 42;

  other(1, 0) = 66;
  other(1, 1) = 81;
  other(1, 2) = 96;

  other(2, 0) = 102;
  other(2, 1) = 126;
  other(2, 2) = 150;

  matrix3x3 = other;
  ASSERT_EQ(matrix3x3, other);

  matrix3x3 = matrix3x3;
  ASSERT_EQ(matrix3x3, matrix3x3);
}

TEST_F(S21MatrixTest, OperatorPlusEqualsTest) {
  matrix3x3 += matrix3x3;
  s21::S21Matrix result{3, 3};
  result(0, 0) = 2;
  result(0, 1) = 4;
  result(0, 2) = 6;

  result(1, 0) = 8;
  result(1, 1) = 10;
  result(1, 2) = 12;

  result(2, 0) = 14;
  result(2, 1) = 16;
  result(2, 2) = 18;

  ASSERT_EQ(matrix3x3, result);
}

TEST_F(S21MatrixTest, OperatorMinusEqualsTest) {
  matrix3x3 -= matrix3x3;
  s21::S21Matrix result{3, 3};
  ASSERT_EQ(matrix3x3, result);
}

TEST_F(S21MatrixTest, OperatorMulEqualsMatrixTest) {
  matrix3x3 *= matrix3x3;
  s21::S21Matrix result{3, 3};
  result(0, 0) = 30;
  result(0, 1) = 36;
  result(0, 2) = 42;

  result(1, 0) = 66;
  result(1, 1) = 81;
  result(1, 2) = 96;

  result(2, 0) = 102;
  result(2, 1) = 126;
  result(2, 2) = 150;

  ASSERT_EQ(result, matrix3x3);
}

TEST_F(S21MatrixTest, OperatorMulEqualsNumberTest) {
  matrix3x3 *= -10;
  s21::S21Matrix result{3, 3};
  result(0, 0) = -10;
  result(0, 1) = -20;
  result(0, 2) = -30;

  result(1, 0) = -40;
  result(1, 1) = -50;
  result(1, 2) = -60;

  result(2, 0) = -70;
  result(2, 1) = -80;
  result(2, 2) = -90;

  ASSERT_EQ(result, matrix3x3);
}

TEST_F(S21MatrixTest, OperatorSubscriptTest) {
  ASSERT_EQ(matrix3x3(0, 0), 1);
  ASSERT_EQ(matrix3x3(2, 2), 9);

  EXPECT_THROW([[maybe_unused]] auto result{matrix3x3(1, -1)},
               std::out_of_range);
  EXPECT_THROW([[maybe_unused]] auto result{matrix3x3(3, 0)},
               std::out_of_range);

  const S21Matrix matrix{2, 2};
  ASSERT_EQ(matrix(0, 0), 0);
  EXPECT_THROW([[maybe_unused]] auto result{matrix(3, 0)}, std::out_of_range);
}

TEST_F(S21MatrixTest, AccessorsTest) {
  ASSERT_EQ(matrix3x3.GetCols(), 3);
  ASSERT_EQ(matrix1x1.GetRows(), 1);
  ASSERT_EQ(matrix7x7.GetCols(), matrix7x7.GetRows());
  ASSERT_EQ(matrix7x7.GetCols(), 7);
}

TEST_F(S21MatrixTest, MutatorsTest) {
  matrix3x3.SetRows(10);
  ASSERT_EQ(matrix3x3.GetRows(), 10);
  matrix3x3.SetRows(10);
  ASSERT_EQ(matrix3x3.GetRows(), 10);
  ASSERT_EQ(matrix3x3(7, 0), 0);

  matrix3x3.SetCols(1);
  ASSERT_EQ(matrix3x3.GetCols(), 1);
  matrix3x3.SetCols(1);
  ASSERT_EQ(matrix3x3.GetCols(), 1);
  ASSERT_EQ(matrix3x3(0, 0), 1);

  EXPECT_THROW([[maybe_unused]] auto result{matrix3x3(5, 3)},
               std::out_of_range);
  EXPECT_THROW(matrix3x3.SetRows(-5), std::out_of_range);
  EXPECT_THROW(matrix3x3.SetCols(-5), std::out_of_range);
}
}  // namespace s21

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

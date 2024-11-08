#include <gtest/gtest.h>
#include <cmath>
#include "matrix.hpp"


TEST(MatrixTest, MatrixCtor)
{
    matrix::Matrix<int> matrix1;
    ASSERT_EQ(matrix1.GetData(), nullptr);
    
    matrix::Matrix<int> matrix2(2, 3);
    ASSERT_EQ(matrix2.GetRowCount(), 2);
    ASSERT_EQ(matrix2.GetColumnCount(), 3);

    auto ptr = matrix2.GetData();
    for (size_t i = 0; i < 6; i++)
        ASSERT_EQ(ptr[i], 0);
}

TEST(MatrixTest, MatrixCtorOnIter)
{
    matrix::Matrix<int> matrix1(4);
    ASSERT_NE(matrix1.GetData(), nullptr);
    
    ASSERT_EQ(matrix1.GetRowCount(), 4);
    ASSERT_EQ(matrix1.GetColumnCount(), 4);

    std::vector<int> vector{};
    for (size_t i = 0; i < 16; i++)
        vector.push_back(i);

    matrix::Matrix<int> matrix2(4, vector.begin(), vector.end());

    auto ptr = matrix2.GetData();
    for (size_t i = 0; i < 16; i++)
        ASSERT_EQ(ptr[i], i);
}

TEST(MatrixTest, MatrixCopySemantic)
{
    std::vector<int> vector1{};
    for (size_t i = 0; i < 9; i++)
        vector1.push_back(i * 2);
    
    std::vector<int> vector2{};
    for (size_t i = 0; i < 9; i++)
        vector2.push_back(i);

    matrix::Matrix<int> matrix1(3, vector1.begin(), vector1.end());
    matrix::Matrix<int> matrix2(3, vector2.begin(), vector2.end());

    auto ptr1 = matrix1.GetData();
    for (size_t i = 0; i < 9; i++)
        ASSERT_EQ(ptr1[i], 2 * i);

    matrix::Matrix<int> matrix3 = matrix2;
    matrix1 = matrix2;

    auto ptr2 = matrix3.GetData();
    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(ptr1[i], i);
        ASSERT_EQ(ptr2[i], i);
    }
}

TEST(MatrixTest, MatrixMoveSemantic)
{
    std::vector<int> vector1{};
    for (size_t i = 0; i < 9; i++)
        vector1.push_back(i * 2);
    
    std::vector<int> vector2{};
    for (size_t i = 0; i < 9; i++)
        vector2.push_back(i);

    matrix::Matrix<int> matrix1(3, vector1.begin(), vector1.end());
    matrix::Matrix<int> matrix2(3, vector2.begin(), vector2.end());
    
    auto ptr1 = matrix1.GetData();
    for (size_t i = 0; i < 9; i++)
        ASSERT_EQ(ptr1[i], 2 * i);

    matrix::Matrix<int> matrix3(std::move(matrix2));
    ASSERT_EQ(matrix2.GetData(), nullptr);

    auto ptr2 = matrix3.GetData();
    for (size_t i = 0; i < 9; i++)
        ASSERT_EQ(ptr2[i], i);

    matrix3 = std::move(matrix1);
    
    auto ptr3 = matrix3.GetData();
    for (size_t i = 0; i < 9; i++)
        ASSERT_EQ(ptr3[i], i * 2);
}

TEST(MatrixTest, MatrixSimpleArithmeticSemantic)
{
    std::vector<int> vector1{};
    for (size_t i = 0; i < 9; i++)
        vector1.push_back(i * 3);

    std::vector<int> vector2{};
    for (size_t i = 0; i < 9; i++)
        vector2.push_back(i * 2);

    matrix::Matrix<int> matrix1(3, vector1.begin(), vector1.end());
    matrix::Matrix<int> matrix2(3, vector2.begin(), vector2.end());

    matrix::Matrix<int> matrix3 = matrix1 + matrix2;
    matrix::Matrix<int> matrix4 = matrix1 - matrix2;

    auto ptr1 = matrix3.GetData();
    auto ptr2 = matrix4.GetData();
    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(ptr1[i], 5 * i);
        ASSERT_EQ(ptr2[i], i);
    }

    matrix3 += 2;
    matrix::Matrix<int> matrix5 = 2 + matrix4;
    matrix::Matrix<int> matrix6 = matrix4 - 1;

    auto ptr3 = matrix3.GetData();
    auto ptr4 = matrix5.GetData();
    auto ptr5 = matrix6.GetData();

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(ptr3[i], 5 * i + 2);
        ASSERT_EQ(ptr4[i], i + 2);
        ASSERT_EQ(ptr5[i], i - 1);
    }
}

TEST(MatrixTest, MatrixTransposeSemantic)
{
    std::vector<int> vector1{};
    for (size_t i = 0; i < 8; i++)
        vector1.push_back(i);

    std::vector<int> vector2 {0, 4, 1, 5, 2, 6, 3, 7};

    matrix::Matrix<int> matrix1(2, 4, vector1.begin(), vector1.end());
    
    matrix::Matrix<int> matrix2 = matrix1.Transpose();
    ASSERT_EQ(matrix2.GetRowCount(), matrix1.GetColumnCount());
    ASSERT_EQ(matrix1.GetRowCount(), matrix2.GetColumnCount());

    matrix::Matrix<int> matrix3 = matrix2.Transpose();
    ASSERT_EQ(matrix1.GetRowCount(), matrix3.GetRowCount());
    ASSERT_EQ(matrix1.GetColumnCount(), matrix3.GetColumnCount());

    auto ptr1 = matrix2.GetData();
    auto ptr2 = matrix3.GetData();
    for (size_t i = 0; i < 8; i++)
    {
        ASSERT_EQ(ptr1[i], vector2[i]);
        ASSERT_EQ(ptr2[i], i);
    }
}

TEST(MatrixTest, MatrixComplexArithmeticSemantic1)
{
    std::vector<int> vector1{1, 1, 2, 1, 1, 0};
    std::vector<int> vector2{0, 1, 0, 2, 0, 1};

    matrix::Matrix<int> matrix1(3, 2, vector1.begin(), vector1.end());
    matrix::Matrix<int> matrix2(2, 3, vector2.begin(), vector2.end());

    matrix::Matrix<int> matrix3 = matrix1 * 2;
    matrix::Matrix<int> matrix4 = matrix2 * 4;

    auto ptr1 = matrix3.GetData();
    auto ptr2 = matrix4.GetData();

    for (size_t i = 0; i < 6; i++)
        ASSERT_EQ(ptr1[i], vector1[i] * 2);

    for (size_t i = 0; i < 6; i++)
        ASSERT_EQ(ptr2[i], vector2[i] * 4);
}

TEST(MatrixTest, MatrixComplexArithmeticSemantic2)
{
    std::vector<int> vector1{1, 1, 2, 1, 1, 0};
    std::vector<int> vector2{0, 1, 0, 2, 0, 1};

    std::vector<int> vector3{2, 1, 1, 2, 2, 1, 0, 1, 0};
    std::vector<int> vector4{2, 1, 3, 2};

    matrix::Matrix<int> matrix1(3, 2, vector1.begin(), vector1.end());
    matrix::Matrix<int> matrix2(2, 3, vector2.begin(), vector2.end());
    matrix::Matrix<int> matrix3 = matrix1 * matrix2;
    matrix::Matrix<int> matrix4 = matrix2 * matrix1;

    ASSERT_EQ(matrix3.GetRowCount(), 3);
    ASSERT_EQ(matrix3.GetColumnCount(), 3);
    
    ASSERT_EQ(matrix4.GetRowCount(), 2);
    ASSERT_EQ(matrix4.GetColumnCount(), 2);
    
    auto ptr1 = matrix3.GetData();
    auto ptr2 = matrix4.GetData();

    for (size_t i = 0; i < 9; i++)
        ASSERT_EQ(ptr1[i], vector3[i]);

    for (size_t i = 0; i < 4; i++)
        ASSERT_EQ(ptr2[i], vector4[i]);
}

TEST(MatrixTest, MatrixComplexArithmeticSemantic3)
{
    std::vector<int> vector1{1, 1, 1, 3, 0, -1, 1, 0, 1, -2, 0, 1, -3, 1, 2};
    std::vector<int> vector2{0, 1, 3, 2, 0, -2, 0, -3, 0, 0, -3, 1, 1, 0, 0};

    std::vector<int> vector3{2, -11, 4, 0, -4, -4, 4, 6, -1};
    std::vector<int> vector4{-1, 4, -9, 4, 4, 2, 0, 8, 4, -4, 3, -3, 0, -3, 6, 3, -2, -3, -2, 8, 1, 1, 1, 3, 0};

    matrix::Matrix<int> matrix1(3, 5, vector1.begin(), vector1.end());
    matrix::Matrix<int> matrix2(5, 3, vector2.begin(), vector2.end());
    matrix::Matrix<int> matrix3 = matrix1 * matrix2;
    matrix::Matrix<int> matrix4 = matrix2 * matrix1;

    ASSERT_EQ(matrix3.GetRowCount(), 3);
    ASSERT_EQ(matrix3.GetColumnCount(), 3);
    
    ASSERT_EQ(matrix4.GetRowCount(), 5);
    ASSERT_EQ(matrix4.GetColumnCount(), 5);
    
    auto ptr1 = matrix3.GetData();
    auto ptr2 = matrix4.GetData();

    for (size_t i = 0; i < 9; i++)
        ASSERT_EQ(ptr1[i], vector3[i]);

    for (size_t i = 0; i < 25; i++)
        ASSERT_EQ(ptr2[i], vector4[i]);
}

TEST(MatrixTest, MatrixIndexing)
{
    std::vector<int> vector{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    matrix::Matrix<int> matrix(3, 5, vector.begin(), vector.end());

    for (size_t i = 0; i < 3; i++)
        for (size_t j = 0; j < 5; j++)
            ASSERT_EQ(matrix[i][j], vector[i * 5 + j]);
}

TEST(MatrixTest, MatrixDeterminant)
{
    std::vector<int> vector1{0, 1, 1, 0};
    matrix::Matrix<int> matrix1(2, vector1.begin(), vector1.end());
    ASSERT_EQ(matrix1.GetDeterminant(), -1);

    std::vector<int> vector2{0, 1, 2, 0, 3, 4, 0, 5, 6};
    matrix::Matrix<int> matrix2(3, vector2.begin(), vector2.end());
    ASSERT_EQ(matrix2.GetDeterminant(), 0);

    std::vector<int> vector3{0, 1, 2, 7, 3, 4, 1, 5, 6};
    matrix::Matrix<int> matrix3(3, vector3.begin(), vector3.end());
    ASSERT_EQ(std::round(matrix3.GetDeterminant()), 26);

    std::vector<int> vector4{0, 1, 2, 0, 3, 4, 0, 5, 6};
    matrix::Matrix<int> matrix4(3, vector4.begin(), vector4.end());
    ASSERT_EQ(matrix4.GetDeterminant(), 0);
}

TEST(RowTest, RowCtor)
{
    matrix::Row<int> row1(6);
    ASSERT_EQ(row1.GetSize(), 6);
    ASSERT_NE(row1.GetData(), nullptr);
}

TEST(RowTest, RowCtorOnIter)
{
    std::vector<int> vector1{};
    for (size_t i = 0; i < 16; i++)
        vector1.push_back(i);

    matrix::Row<int> row1(16, vector1.begin(), vector1.end());

    auto ptr1 = row1.GetData();
    for (size_t i = 0; i < 16; i++)
        ASSERT_EQ(ptr1[i], i);
}

TEST(RowTest, RowCopySemantic)
{
    std::vector<int> vector1{};
    for (size_t i = 0; i < 9; i++)
        vector1.push_back(i * 2);
    
    std::vector<int> vector2{};
    for (size_t i = 0; i < 9; i++)
        vector2.push_back(i);

    matrix::Row<int> row1(9, vector1.begin(), vector1.end());
    matrix::Row<int> row2(9, vector2.begin(), vector2.end());

    auto ptr1 = row1.GetData();
    for (size_t i = 0; i < 9; i++)
        ASSERT_EQ(ptr1[i], 2 * i);

    matrix::Row<int> row3 = row2;
    row1 = row2;

    auto ptr2 = row3.GetData();
    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(ptr1[i], i);
        ASSERT_EQ(ptr2[i], i);
    }
}

TEST(RowTest, RowMoveSemantic)
{
    std::vector<int> vector1{};
    for (size_t i = 0; i < 9; i++)
        vector1.push_back(i * 2);
    
    std::vector<int> vector2{};
    for (size_t i = 0; i < 9; i++)
        vector2.push_back(i);

    matrix::Row<int> row1(9, vector1.begin(), vector1.end());
    matrix::Row<int> row2(9, vector2.begin(), vector2.end());
    
    auto ptr1 = row1.GetData();
    for (size_t i = 0; i < 9; i++)
        ASSERT_EQ(ptr1[i], 2 * i);

    matrix::Row<int> row3(std::move(row2));
    ASSERT_EQ(row2.GetData(), nullptr);

    auto ptr2 = row3.GetData();
    for (size_t i = 0; i < 9; i++)
        ASSERT_EQ(ptr2[i], i);

    row3 = std::move(row1);
    
    auto ptr3 = row3.GetData();
    for (size_t i = 0; i < 9; i++)
        ASSERT_EQ(ptr3[i], i * 2);
}

TEST(RowTest, RowSimpleArithmeticSemantic)
{
    std::vector<int> vector1{};
    for (size_t i = 0; i < 9; i++)
        vector1.push_back(i * 3);

    std::vector<int> vector2{};
    for (size_t i = 0; i < 9; i++)
        vector2.push_back(i * 2);

    matrix::Row<int> row1(9, vector1.begin(), vector1.end());
    matrix::Row<int> row2(9, vector2.begin(), vector2.end());

    matrix::Row<int> row3 = row1 + row2;
    matrix::Row<int> row4 = row1 - row2;

    auto ptr1 = row3.GetData();
    auto ptr2 = row4.GetData();
    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(ptr1[i], 5 * i);
        ASSERT_EQ(ptr2[i], i);
    }

    row3 += 2;
    matrix::Row<int> row5 = 2 + row4;
    matrix::Row<int> row6 = row4 - 1;

    auto ptr3 = row3.GetData();
    auto ptr4 = row5.GetData();
    auto ptr5 = row6.GetData();

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(ptr3[i], 5 * i + 2);
        ASSERT_EQ(ptr4[i], i + 2);
        ASSERT_EQ(ptr5[i], i - 1);
    }
}

TEST(RowTest, RowComplexArithmeticSemantic)
{
    std::vector<int> vector1{};
    for (size_t i = 0; i < 9; i++)
        vector1.push_back(i * 3);

    std::vector<int> vector2{};
    for (size_t i = 0; i < 9; i++)
        vector2.push_back(i * 2);

    matrix::Row<int> row1(9, vector1.begin(), vector1.end());
    matrix::Row<int> row2(9, vector2.begin(), vector2.end());
    ASSERT_EQ(row1.GetSum(), 108);
    ASSERT_EQ(row2.GetSum(), 72);

    matrix::Row<int> row3 = row1 * row2;

    auto ptr1 = row3.GetData();
    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(ptr1[i], 6 * i * i);
    }

    matrix::Row<int> row4 = 2 + row1;
    matrix::Row<int> row5 = row2 - 1;
    
    ASSERT_EQ(row4.GetSum(), 108 + 9 * 2);
    ASSERT_EQ(row5.GetSum(), 72 - 9);

    auto ptr2 = row4.GetData();
    auto ptr3 = row5.GetData();

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(ptr2[i], 3 * i + 2);
        ASSERT_EQ(ptr3[i], 2 * i - 1);
    }
}

TEST(RowTest, RowIndexing)
{
    std::vector<int> vector{};
    for (size_t i = 0; i < 9; i++)
        vector.push_back(i * 2);

    matrix::Row<int> row(9, vector.begin(), vector.end());

    for (size_t i = 0; i < 9; i++)
        ASSERT_EQ(row[i], 2 * i);
}
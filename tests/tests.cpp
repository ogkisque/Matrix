#include <gtest/gtest.h>
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

TEST(MatrixTest, MatrixArithmeticSemantic)
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
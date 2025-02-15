#include "matrix.hpp"
#include <cmath>
#include <gtest/gtest.h>
#include <string>

TEST(MatrixTest, MatrixCtor) {
    matrix::Matrix<int> matrix1(2, 3);
    ASSERT_EQ(matrix1.GetRowCount(), 2);
    ASSERT_EQ(matrix1.GetColumnCount(), 3);
}

TEST(MatrixTest, MatrixCtorOnIter) {
    matrix::Matrix<int> matrix1(4);

    ASSERT_EQ(matrix1.GetRowCount(), 4);
    ASSERT_EQ(matrix1.GetColumnCount(), 4);

    std::vector<int> vector{};
    for (size_t i = 0; i < 16; i++)
        vector.push_back(i);

    matrix::Matrix<int> matrix2(4, vector.begin(), vector.end());

    for (size_t i = 0; i < 4; i++)
        for (size_t j = 0; j < 4; j++)
            ASSERT_EQ(matrix2[i][j], i * 4 + j);
}

TEST(MatrixTest, MatrixCopySemantic) {
    std::vector<int> vector1{};
    for (size_t i = 0; i < 9; i++)
        vector1.push_back(i * 2);

    std::vector<int> vector2{};
    for (size_t i = 0; i < 9; i++)
        vector2.push_back(i);

    matrix::Matrix<int> matrix1(3, vector1.begin(), vector1.end());
    matrix::Matrix<int> matrix2(3, vector2.begin(), vector2.end());

    for (size_t i = 0; i < 3; i++)
        for (size_t j = 0; j < 3; j++)
            ASSERT_EQ(matrix1[i][j], 2 * (i * 3 + j));

    matrix::Matrix<int> matrix3 = matrix2;
    matrix1 = matrix2;

    for (size_t i = 0; i < 3; i++)
        for (size_t j = 0; j < 3; j++) {
            ASSERT_EQ(matrix2[i][j], i * 3 + j);
            ASSERT_EQ(matrix3[i][j], i * 3 + j);
        }
}

TEST(MatrixTest, MatrixMoveSemantic) {
    std::vector<int> vector1{};
    for (size_t i = 0; i < 9; i++)
        vector1.push_back(i * 2);

    std::vector<int> vector2{};
    for (size_t i = 0; i < 9; i++)
        vector2.push_back(i);

    matrix::Matrix<int> matrix1(3, vector1.begin(), vector1.end());
    matrix::Matrix<int> matrix2(3, vector2.begin(), vector2.end());

    for (size_t i = 0; i < 3; i++)
        for (size_t j = 0; j < 3; j++)
            ASSERT_EQ(matrix1[i][j], 2 * (i * 3 + j));

    matrix::Matrix<int> matrix3(std::move(matrix2));

    for (size_t i = 0; i < 3; i++)
        for (size_t j = 0; j < 3; j++)
            ASSERT_EQ(matrix3[i][j], i * 3 + j);

    matrix3 = std::move(matrix1);

    for (size_t i = 0; i < 3; i++)
        for (size_t j = 0; j < 3; j++)
            ASSERT_EQ(matrix3[i][j], 2 * (i * 3 + j));
}

TEST(MatrixTest, MatrixSimpleArithmeticSemantic) {
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

    for (size_t i = 0; i < 3; i++)
        for (size_t j = 0; j < 3; j++) {
            ASSERT_EQ(matrix3[i][j], 5 * (i * 3 + j));
            ASSERT_EQ(matrix4[i][j], i * 3 + j);
        }
}

TEST(MatrixTest, MatrixTransposeSemantic) {
    std::vector<int> vector1{0, 4, 1, 5, 2, 6, 3, 7};
    std::vector<int> vector2{0, 2, 4, 6, 1, 3, 5, 7};

    matrix::Matrix<int> matrix1(2, 4, vector1.begin(), vector1.end());

    matrix::Matrix<int> matrix2 = matrix1.Transpose();
    ASSERT_EQ(matrix2.GetRowCount(), matrix1.GetColumnCount());
    ASSERT_EQ(matrix1.GetRowCount(), matrix2.GetColumnCount());

    matrix::Matrix<int> matrix3 = matrix2.Transpose();
    ASSERT_EQ(matrix1.GetRowCount(), matrix3.GetRowCount());
    ASSERT_EQ(matrix1.GetColumnCount(), matrix3.GetColumnCount());

    for (size_t i = 0; i < 2; i++)
        for (size_t j = 0; j < 4; j++)
            ASSERT_EQ(matrix3[i][j], vector1[i * 4 + j]);

    for (size_t i = 0; i < 4; i++)
        for (size_t j = 0; j < 2; j++)
            ASSERT_EQ(matrix2[i][j], vector2[i * 2 + j]);
}

TEST(MatrixTest, MatrixComplexArithmeticSemantic1) {
    std::vector<int> vector1{1, 1, 2, 1, 1, 0};
    std::vector<int> vector2{0, 1, 0, 2, 0, 1};

    matrix::Matrix<int> matrix1(3, 2, vector1.begin(), vector1.end());
    matrix::Matrix<int> matrix2(2, 3, vector2.begin(), vector2.end());

    matrix::Matrix<int> matrix3 = matrix1 * 2;
    matrix::Matrix<int> matrix4 = matrix2 * 4;

    for (size_t i = 0; i < 3; i++)
        for (size_t j = 0; j < 2; j++)
            ASSERT_EQ(matrix3[i][j], 2 * vector1[i * 2 + j]);

    for (size_t i = 0; i < 2; i++)
        for (size_t j = 0; j < 3; j++)
            ASSERT_EQ(matrix4[i][j], 4 * vector2[i * 3 + j]);
}

TEST(MatrixTest, MatrixComplexArithmeticSemantic2) {
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

    for (size_t i = 0; i < 3; i++)
        for (size_t j = 0; j < 3; j++)
            ASSERT_EQ(matrix3[i][j], vector3[i * 3 + j]);

    for (size_t i = 0; i < 2; i++)
        for (size_t j = 0; j < 2; j++)
            ASSERT_EQ(matrix4[i][j], vector4[i * 2 + j]);
}

TEST(MatrixTest, MatrixComplexArithmeticSemantic3) {
    std::vector<int> vector1{1, 1, 1, 3, 0, -1, 1, 0, 1, -2, 0, 1, -3, 1, 2};
    std::vector<int> vector2{0, 1, 3, 2, 0, -2, 0, -3, 0, 0, -3, 1, 1, 0, 0};

    std::vector<int> vector3{2, -11, 4, 0, -4, -4, 4, 6, -1};
    std::vector<int> vector4{-1, 4, -9, 4,  4,  2,  0, 8, 4, -4, 3, -3, 0,
                             -3, 6, 3,  -2, -3, -2, 8, 1, 1, 1,  3, 0};

    matrix::Matrix<int> matrix1(3, 5, vector1.begin(), vector1.end());
    matrix::Matrix<int> matrix2(5, 3, vector2.begin(), vector2.end());
    matrix::Matrix<int> matrix3 = matrix1 * matrix2;
    matrix::Matrix<int> matrix4 = matrix2 * matrix1;

    ASSERT_EQ(matrix3.GetRowCount(), 3);
    ASSERT_EQ(matrix3.GetColumnCount(), 3);
    ASSERT_EQ(matrix4.GetRowCount(), 5);
    ASSERT_EQ(matrix4.GetColumnCount(), 5);

    for (size_t i = 0; i < 3; i++)
        for (size_t j = 0; j < 3; j++)
            ASSERT_EQ(matrix3[i][j], vector3[i * 3 + j]);

    for (size_t i = 0; i < 5; i++)
        for (size_t j = 0; j < 5; j++)
            ASSERT_EQ(matrix4[i][j], vector4[i * 5 + j]);
}

TEST(MatrixTest, MatrixIntDeterminant) {
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

    std::vector<int> vector5{3, 2,  3, 4, 0, 4, -3, -10,
                             0, 10, 9, 5, 0, 5, -3, -5};
    matrix::Matrix<int> matrix5(4, vector5.begin(), vector5.end());
    ASSERT_EQ(matrix5.GetDeterminant(), 1215);
}

TEST(MatrixTest, MatrixFloatDeterminant) {
    std::vector<float> vector1{0.02, 0.01, 0, 0, 1, 2, 1,   0,
                               0,    1,    2, 1, 0, 0, 100, 200};
    matrix::Matrix<float> matrix1(4, vector1.begin(), vector1.end());
    ASSERT_TRUE(real_nums::equal(matrix1.GetDeterminant(), 5.0f));

    std::vector<float> vector2{0.02, 0.01, 0, 5, 2, 1, 0,   500,
                               0,    1,    2, 1, 0, 0, 100, 200};
    matrix::Matrix<float> matrix2(4, vector2.begin(), vector2.end());
    ASSERT_TRUE(real_nums::equal(matrix2.GetDeterminant(), 0.0f));
}

TEST(MatrixTest, TemplateTest1) {
    std::vector<int> vector1{0, 2, 2, 2, 1, 1, 0, 2, 1};
    std::vector<int> vector2{0, 1, 2, 0, 3, 2, 0, 2, 0};
    
    matrix::Matrix<int> matrix1(3, vector1.begin(), vector1.end());
    matrix::Matrix<int> matrix2(3, vector2.begin(), vector2.end());

    std::vector<matrix::Matrix<int>> vector3{matrix1, matrix2};
    std::vector<matrix::Matrix<int>> vector4{matrix2, matrix1};

    matrix::Matrix<matrix::Matrix<int>> matrix3(1, 2, vector3.begin(), vector3.end());
    ASSERT_EQ(matrix3[0][0], matrix1);
    ASSERT_EQ(matrix3[0][1], matrix2);
    matrix::Matrix<matrix::Matrix<int>> matrix4(1, 2, vector4.begin(), vector4.end());
    ASSERT_EQ(matrix4[0][0], matrix2);
    ASSERT_EQ(matrix4[0][1], matrix1);
    
    matrix::Matrix<matrix::Matrix<int>> matrix5 = matrix3 + matrix4;
    
    for (size_t j = 0; j < 2; j++)
        ASSERT_EQ(matrix5[0][j], matrix1 + matrix2);
}

TEST(MatrixTest, TemplateTest2) {
    std::vector<std::string> vector1{"a", "b", "c", "d"};
    std::vector<std::string> vector2{"x", "y", "z", "w"};
    
    matrix::Matrix<std::string> matrix1(2, vector1.begin(), vector1.end());
    matrix::Matrix<std::string> matrix2(2, vector2.begin(), vector2.end());
    
    auto matrix3 = matrix1 + matrix2;
    
    ASSERT_EQ(matrix3[0][0], "ax");
    ASSERT_EQ(matrix3[0][1], "by");
    ASSERT_EQ(matrix3[1][0], "cz");
    ASSERT_EQ(matrix3[1][1], "dw");
}

TEST(MatrixTest, TemplateTest3) {
    std::vector<std::string> vector1{"a", "b", "c", "d"};
    std::vector<std::string> vector2{"x", "y", "z", "w"};
    
    matrix::Matrix<std::string> matrix1(2, vector1.begin(), vector1.end());
    matrix::Matrix<std::string> matrix2(2, vector2.begin(), vector2.end());
    
    auto matrix3 = matrix1 + matrix2;
    
    ASSERT_EQ(matrix3[0][0], "ax");
    ASSERT_EQ(matrix3[0][1], "by");
    ASSERT_EQ(matrix3[1][0], "cz");
    ASSERT_EQ(matrix3[1][1], "dw");
}

TEST(MatrixTest, OperatorEqual) {
    std::vector<int> vector1{0, 1, 2, 3, 4, 5};
    
    matrix::Matrix<int> matrix1(2, 3, vector1.begin(), vector1.end());
    matrix::Matrix<int> matrix2(3, 2, vector1.begin(), vector1.end());
    auto matrix3 = matrix1;
    auto matrix4 = matrix2;

    ASSERT_NE(matrix1, matrix2);
    ASSERT_EQ(matrix3, matrix1);
    ASSERT_EQ(matrix4, matrix2);
}
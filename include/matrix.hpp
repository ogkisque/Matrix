#pragma once

#include <iostream>
#include <iterator>
#include <algorithm>

#include "real_nums.hpp"

namespace matrix {

template <typename T>
class ProxyRow
{
public:
    ProxyRow(size_t size, T *data) : size_(size), data_(new (data) T [size]) {}
    
    T &operator[](int num_elem)
    {
        return data_[num_elem];
    }

    const T &operator[](int num_elem) const
    {
        return data_[num_elem];
    }

    ProxyRow &operator=(const ProxyRow<T> &other)
    {
        if (this == &other)
            return *this;
        
        if (size_ != other.size_)
            throw std::logic_error("Rows sizes do not match");

        for (size_t i = 0; i < size_; ++i)
            data_[i] = other.data_[i];

        return *this;
    }

    ProxyRow &operator+=(const ProxyRow<T> &other)
    {
        if (size_ != other.size_)
            throw std::logic_error("Rows sizes do not match");
        
        if (!std::is_fundamental<T>::value)
            throw std::logic_error("Element type is not fundamental");

        for (size_t i = 0; i < size_; ++i)
            data_[i] += other.data_[i];

        return *this;
    }
    
    ProxyRow &operator-=(const ProxyRow<T> &other)
    {
        if (size_ != other.size_)
            throw std::logic_error("Rows sizes do not match");
        
        if (!std::is_fundamental<T>::value)
            throw std::logic_error("Element type is not fundamental");

        for (size_t i = 0; i < size_; ++i)
            data_[i] -= other.data_[i];

        return *this;
    }

    ProxyRow &operator+=(T val)
    {
        if (!std::is_fundamental<T>::value)
            throw std::logic_error("Element type is not fundamental");

        for (size_t i = 0; i < size_; ++i)
            data_[i] += val;

        return *this;
    }

    ProxyRow &operator-=(T val)
    {
        if (!std::is_fundamental<T>::value)
            throw std::logic_error("Element type is not fundamental");

        for (size_t i = 0; i < size_; ++i)
            data_[i] -= val;

        return *this;
    }

    ProxyRow &operator*=(const ProxyRow<T> &other)
    {
        if (size_ != other.size_)
            throw std::logic_error("Rows sizes do not match");
        
        if (!std::is_fundamental<T>::value)
            throw std::logic_error("Element type is not fundamental");

        for (size_t i = 0; i < size_; ++i)
            data_[i] *= other.data_[i];

        return *this;
    }

    ProxyRow &operator*=(T val)
    {
        if (!std::is_fundamental<T>::value)
            throw std::logic_error("Element type is not fundamental");
    
        for (size_t i = 0; i < size_; ++i)
            data_[i] *= val;

        return *this;
    }

    size_t GetSize() const
    {
        return size_;
    }

    T GetSum() const
    {
        if (!std::is_fundamental<T>::value)
            throw std::logic_error("Element type is not fundamental");
    
        T temp = 0;

        for (size_t i = 0; i < size_; ++i)
            temp += data_[i];

        return temp;
    }

    void print() const
    {
        std::cout << "Row:" << std::endl;
        for (size_t i = 0; i < size_; ++i)
            std::cout << data_[i] << " ";

        std::cout << std::endl;
    }

private:
    T *data_ = nullptr;
    size_t size_ = 0;
}; // class ProxyRow

template <typename T> 
ProxyRow<T> operator+(ProxyRow<T> &lhs, ProxyRow<T> &rhs)
{
    ProxyRow<T> temp = lhs;
    temp += rhs;
    return temp;
}

template <typename T>
ProxyRow<T> operator+(ProxyRow<T> &lhs, T rhs)
{
    ProxyRow<T> temp = lhs;
    temp += rhs;
    return temp;
}

template <typename T>
ProxyRow<T> operator+(T lhs, ProxyRow<T> &rhs)
{
    ProxyRow<T> temp = rhs;
    temp += lhs;
    return temp;
}

template <typename T> 
ProxyRow<T> operator-(ProxyRow<T> &lhs, ProxyRow<T> &rhs)
{
    ProxyRow<T> temp = lhs;
    temp -= rhs;
    return temp;
}

template <typename T>
ProxyRow<T> operator-(ProxyRow<T> &lhs, T rhs)
{
    ProxyRow<T> temp = lhs;
    temp -= rhs;
    return temp;
}

template <typename T> 
ProxyRow<T> operator*(ProxyRow<T> &lhs, ProxyRow<T> &rhs)
{
    ProxyRow<T> temp = lhs;
    temp *= rhs;
    return temp;
}

template <typename T>
ProxyRow<T> operator*(ProxyRow<T> &lhs, T rhs)
{
    ProxyRow<T> temp = lhs;
    temp *= rhs;
    return temp;
}

template <typename T>
ProxyRow<T> operator*(T lhs, ProxyRow<T> &rhs)
{
    ProxyRow<T> temp = rhs;
    temp *= lhs;
    return temp;
}

template <typename T>
void Construct(T *ptr, const T &other) 
{
    new (ptr) T{other};
}

template <typename T>
void Construct(T *ptr, T &&other) 
{
    new (ptr) T{std::move(other)};
}

template <typename T>
void Destroy(T *ptr)
{
    ptr->~T();
}

template <typename Iterator> 
void Destroy(Iterator begin, Iterator end) {
    for (Iterator it = begin; it != end; ++it)
        Destroy(it);
}

template <typename T>
class MatrixBuf
{
protected:
    MatrixBuf(size_t size = 0) : size_(size), 
                                 data_((size == 0) ? nullptr : static_cast<T*>(::operator new(size * sizeof(T)))) 
                                 {}
    
    MatrixBuf(const MatrixBuf<T> &other) = delete;
    MatrixBuf &operator=(const MatrixBuf<T> &other) = delete;

    MatrixBuf(MatrixBuf<T> &&other) noexcept
    {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
    }

    MatrixBuf &operator=(MatrixBuf<T> &&other) noexcept
    {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        return *this;
    }

    ~MatrixBuf()
    {
        Destroy(data_, data_ + used_);
        ::operator delete(data_);
    }

    bool operator==(const MatrixBuf<T> &other) const
    {
        return (data_ == other.data_) && (size_ == other.size_);
    }

    bool operator!=(const MatrixBuf<T> &other) const
    {
        return !(*this == other);
    }

    MatrixBuf<T> &operator+=(const MatrixBuf<T> &other)
    {
        if (size_ != other.size_)
            throw std::logic_error("MatrixBufs sizes do not match");
        
        if (!std::is_fundamental<T>::value)
            throw std::logic_error("Element type is not fundamental");

        for (size_t i = 0; i < size_; ++i)
            data_[i] += other.data_[i];

        return *this;
    }
    
    MatrixBuf<T> &operator-=(const MatrixBuf<T> &other)
    {
        if (size_ != other.size_)
            throw std::logic_error("MatrixBufs sizes do not match");
        
        if (!std::is_fundamental<T>::value)
            throw std::logic_error("Element type is not fundamental");

        for (size_t i = 0; i < size_; ++i)
            data_[i] -= other.data_[i];

        return *this;
    }

protected:
    
    T *data_ = nullptr;
    size_t size_ = 0;
    size_t used_ = 0;
}; // class MatrixBuf


template <typename T>
class Matrix;

template <typename T>
static int SwapRows(Matrix<T> &matrix, size_t column_count, size_t from, size_t to)
{
    Matrix<T> tmp_matrix{1, column_count};
    ProxyRow<T> tmp_row = tmp_matrix[0]; 

    int mult = 1;
    for (size_t i = from + 1; i < to && real_nums::is_zero(matrix[from][from]); ++i, mult *= -1)
    {
        tmp_row = matrix[from];
        matrix[from] = matrix[i];
        matrix[i] = tmp_row;
    }

    if (real_nums::is_zero(matrix[from][from]))
        return 0;

    return mult;
}


template <typename T>
class Matrix : private MatrixBuf<T>
{
    using MatrixBuf<T>::used_;
    using MatrixBuf<T>::size_;
    using MatrixBuf<T>::data_;
    
public:
    Matrix(size_t size = 0) : MatrixBuf<T>(size * size), row_count_(size), column_count_(size) {}

    template <typename InputIterator>
    Matrix(size_t size, InputIterator begin, InputIterator end) : MatrixBuf<T>(size * size), row_count_(size), column_count_(size)
    {
        Construct(begin, end);
    }
    
    Matrix(size_t row_count, size_t column_count) : MatrixBuf<T>(row_count * column_count), row_count_(row_count), column_count_(column_count) {}

    template <typename InputIterator>
    Matrix(size_t row_count, 
           size_t column_count,
           InputIterator begin,
           InputIterator end) : MatrixBuf<T>(row_count * column_count), row_count_(row_count), column_count_(column_count)
    {
        Construct(begin, end);
    }

    Matrix(const Matrix &other) : MatrixBuf<T>(other.size_), row_count_(other.row_count_), column_count_(other.column_count_)
    {
        Construct(other.data_, other.data_ + other.size_);
    }

    Matrix &operator=(const Matrix &other)
    {
        Matrix tmp{other};
        std::swap(*this, tmp);
        return *this;
    }

    Matrix(Matrix &&other) = default;
    Matrix &operator=(Matrix &&other) = default;

    ProxyRow<T> operator[](int num_row) const
    {
        if (num_row >= row_count_)
            throw std::range_error("Row index is more count of exist rows");

        return ProxyRow(column_count_, data_ + num_row * column_count_);
    }

    bool operator==(const Matrix<T> &other) const
    {
        return (row_count_ == other.row_count_) &&
               (column_count_ == other.column_count_);
    }

    bool operator!=(const Matrix<T> &other) const
    {
        return !(*this == other);
    }

    size_t GetRowCount()
    {
        return row_count_;
    }

    size_t GetColumnCount()
    {
        return column_count_; 
    }

    Matrix<T> Transpose() const
    {
        Matrix<T> transpose(column_count_, row_count_);

        for (size_t i = 0; i < row_count_; ++i)
            for (size_t j = 0; j < column_count_; ++j)
                transpose[j][i] = (*this)[i][j];

        return transpose;
    }

    Matrix<T> &operator+=(const Matrix<T> &other)
    {
        if (row_count_ != other.row_count_ || column_count_ != other.column_count_)
            throw std::logic_error("Matrixs sizes do not match");

        if (!std::is_fundamental<T>::value)
            throw std::logic_error("Element type is not fundamental");

        for (size_t i = 0; i < size_; ++i)
            data_[i] += other.data_[i];

        return *this;
    }
    
    Matrix<T> &operator-=(const Matrix<T> &other)
    {
        if (row_count_ != other.row_count_ || column_count_ != other.column_count_)
            throw std::logic_error("Matrixs sizes do not match");

        if (!std::is_fundamental<T>::value)
            throw std::logic_error("Element type is not fundamental");

        for (size_t i = 0; i < size_; ++i)
            data_[i] -= other.data_[i];

        return *this;
    }

    Matrix<T> &operator*=(T val)
    {
        if (!std::is_fundamental<T>::value)
            throw std::logic_error("Element type is not fundamental");
        
        for (size_t i = 0; i < size_; ++i)
            data_[i] *= val;

        return *this;
    }

    Matrix<T> &operator*=(const Matrix<T> &other)
    {
        if (column_count_ != other.row_count_)
            throw std::logic_error("Matrixes sizes do not valid for multiply");

        Matrix<T> transpose = other.Transpose();
        Matrix<T> result{row_count_, other.column_count_};
    
        Matrix<T> tmp_matrix{1, column_count_};
        ProxyRow<T> tmp_row = tmp_matrix[0];
        
        for (size_t i = 0; i < result.row_count_; ++i)
        {
            ProxyRow<T> tmp1((*this)[i]);
            
            for (size_t j = 0; j < result.column_count_; ++j)
            {
                ProxyRow<T> tmp2(transpose[j]);
                tmp_row = tmp2;
                tmp_row *= tmp1;
                result[i][j] = tmp_row.GetSum();   
            }
        }

        *this = result;
        return *this;
    }

    long double GetDeterminant() const
    {
        if (row_count_ != column_count_)
            throw std::logic_error("Matrix rows and columns counts do not match");

        long double det = 1;
        Matrix<long double> matrix{row_count_};

        for (size_t i = 0; i < row_count_; ++i)
            for (size_t j = 0; j < row_count_; ++j)
                matrix[i][j] = (*this)[i][j];

        Matrix<long double> tmp_matrix{1, row_count_};
        ProxyRow<long double> tmp_row = tmp_matrix[0];

        for (size_t i = 0; i < row_count_ - 1; ++i)
        {
            for (size_t j = i + 1; j < column_count_; ++j)
            {
                det *= SwapRows(matrix, column_count_, i, row_count_);
                if (det == 0)
                    return 0;

                tmp_row = matrix[i];
                tmp_row *= matrix[j][i] / matrix[i][i];
                
                matrix[j] -= tmp_row;
            }
        }

        for (size_t i = 0; i < row_count_; ++i)
            det *= matrix[i][i];
        
        return det;
    }

    void print() const
    {
        std::cout << "Matrix:" << std::endl;

        for (size_t i = 0; i < row_count_; ++i)
        {
            for (size_t j = 0; j < column_count_; ++j)
                std::cout << (*this)[i][j] << " ";

            std::cout << std::endl;
        }
    }

private:

    template <typename InputIterator>
    void Construct(InputIterator begin, InputIterator end)
    {
        size_t i = 0;
        for (InputIterator it = begin; it != end; ++it, ++i, used_++)
        {
            if (i >= size_)
                throw std::range_error("MatrixBuf size and number of elements between iterators dont match");

                matrix::Construct(data_ + i, *it);
        }

        if (i != size_)
            throw std::logic_error("Number of elements between iterators more than MatrixBuf size");
    }

    size_t row_count_ = 0;
    size_t column_count_ = 0;
}; // class Matrix

template <typename T> 
Matrix<T> operator+(Matrix<T> lhs, Matrix<T> rhs)
{
    Matrix<T> temp = lhs;
    temp += rhs;
    return temp;
}

template <typename T> 
Matrix<T> operator-(Matrix<T> lhs, Matrix<T> rhs)
{
    Matrix<T> temp = lhs;
    temp -= rhs;
    return temp;
}

template <typename T> 
Matrix<T> operator*(Matrix<T> lhs, Matrix<T> rhs)
{
    Matrix<T> temp = lhs;
    temp *= rhs;
    return temp;
}

template <typename T>
Matrix<T> operator*(Matrix<T> lhs, T rhs)
{
    Matrix<T> temp = lhs;
    temp *= rhs;
    return temp;
}

template <typename T>
Matrix<T> operator*(T lhs, Matrix<T> rhs)
{
    Matrix<T> temp = rhs;
    temp *= lhs;
    return temp;
}

} // namespace matrix
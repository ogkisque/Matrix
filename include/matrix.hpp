#pragma once

#include <iostream>
#include <iterator>

namespace matrix {

template <typename T>
class MatrixBuf
{
public:
    MatrixBuf() = default;
    MatrixBuf(std::nullptr_t) {};

    MatrixBuf(size_t size) : size_(size), data_(new T[size] {}) {}

    template <typename InputIterator>
    MatrixBuf(size_t size, InputIterator begin, InputIterator end) : size_(size), data_(new T[size] {})
    {
        size_t i = 0;
        for (auto it = begin; it != end; ++it, ++i)
        {
            assert(i <= size_);
            data_[i] = *it;
        }

        assert(i == size_);
    }


    MatrixBuf(const MatrixBuf<T> &other) : size_(other.size_), data_(new T[other.size_] {})
    {
        for (size_t i = 0; i < size_; ++i)
            data_[i] = other.data_[i];
    }

    MatrixBuf &operator=(const MatrixBuf<T> &other)
    {
        if (*this != other)
        {
            if (size_ != other.size_)
            {
                delete[] data_;
                data_ = new T[other.size_] {};
                size_ = other.size_;
            }

            for (size_t i = 0; i < size_; ++i)
                data_[i] = other.data_[i];
        }

        return *this;
    }

    MatrixBuf(MatrixBuf<T> &&other)
    {
        std::swap(data_, other.data_);
        size_ = other.size_;
    }

    MatrixBuf &operator=(MatrixBuf<T> &&other)
    {
        if (*this != other)
        {
            std::swap(data_, other.data_);
            size_ = other.size_;
        }

        return *this;
    }

    ~MatrixBuf()
    {
        delete[] data_;
    }

    bool operator==(const MatrixBuf<T> &other)
    {
        return (data_ == other.data_) && (size_ == other.size_);
    }

    bool operator!=(const MatrixBuf<T> &other)
    {
        return !(*this == other);
    }

    T *GetData() const
    {
        return data_;
    }

    MatrixBuf<T> &operator+=(const MatrixBuf<T> &other)
    {
        assert(size_ == other.size_);

        for (size_t i = 0; i < size_; ++i)
            data_[i] += other.data_[i];

        return *this;
    }
    
    MatrixBuf<T> &operator-=(const MatrixBuf<T> &other)
    {
        assert(size_ == other.size_);

        for (size_t i = 0; i < size_; ++i)
            data_[i] -= other.data_[i];

        return *this;
    }

    MatrixBuf<T> &operator+=(T rhs)
    {
        for (size_t i = 0; i < size_; ++i)
            data_[i] += rhs;

        return *this;
    }

    MatrixBuf<T> &operator-=(T rhs)
    {
        for (size_t i = 0; i < size_; ++i)
            data_[i] -= rhs;

        return *this;
    }

private:
    T *data_ = nullptr;
    size_t size_ = 0;
}; // class MatrixBuf



template <typename T>
class Matrix
{
public:
    Matrix() = default;
    Matrix(std::nullptr_t) : buf_(nullptr) {}

    Matrix(size_t size) : 
           row_count_(size),
           column_count_(size),
           buf_(size * size) {}

    template <typename InputIterator>
    Matrix(size_t size, 
           InputIterator begin,
           InputIterator end) 
           : row_count_(size),
           column_count_(size),
           buf_(size * size, begin, end) {}
    
    Matrix(size_t row_count, size_t column_count) : 
           row_count_(row_count),
           column_count_(column_count),
           buf_(row_count * column_count) {}

    template <typename InputIterator>
    Matrix(size_t row_count, 
           size_t column_count,
           InputIterator begin,
           InputIterator end)
           : row_count_(row_count),
           column_count_(column_count), 
           buf_(row_count * column_count, begin, end) {}

    Matrix(const Matrix<T> &other) : 
           row_count_(other.row_count_),
           column_count_(other.column_count_),
           buf_(other.buf_) {}

    Matrix &operator=(const Matrix<T> &other)
    {
        if (*this != other)
        {
            row_count_ = other.row_count_; 
            column_count_ = other.column_count_;
            buf_ = other.buf_;
        }

        return *this;
    }

    Matrix(Matrix<T> &&other) : row_count_(other.row_count_), column_count_(other.column_count_)
    {
        std::swap(buf_, other.buf_);
    }

    Matrix &operator=(Matrix<T> &&other)
    {
        if (*this != other)
        {
            row_count_ = other.row_count_; 
            column_count_ = other.column_count_;
            std::swap(buf_, other.buf_);
        }

        return *this;
    }

    ~Matrix() {}

    bool operator==(const Matrix<T> &other)
    {
        return (buf_ == other.buf_) &&
               (row_count_ == other.row_count_) &&
               (column_count_ == other.column_count_);
    }

    bool operator!=(const Matrix<T> &other)
    {
        return !(*this == other);
    }

    T *GetData() const
    {
        return buf_.GetData();
    }

    size_t GetRowCount()
    {
        return row_count_;
    }

    size_t GetColumnCount()
    {
        return column_count_; 
    }

    Matrix<T> &operator+=(const Matrix<T> &other)
    {
        assert(row_count_ == other.row_count_ && column_count_ == other.column_count_);
        buf_ += other.buf_;
        return *this;
    }
    
    Matrix<T> &operator-=(const Matrix<T> &other)
    {
        assert(row_count_ == other.row_count_ && column_count_ == other.column_count_);
        buf_ -= other.buf_;
        return *this;
    }

    Matrix<T> &operator+=(T rhs)
    {
        buf_ += rhs;
        return *this;
    }

    Matrix<T> &operator-=(T rhs)
    {
        buf_ -= rhs;
        return *this;
    }


private:
    MatrixBuf<T> buf_;
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
Matrix<T> operator+(Matrix<T> lhs, T rhs)
{
    Matrix<T> temp = lhs;
    temp += rhs;
    return temp;
}

template <typename T>
Matrix<T> operator+(T lhs, Matrix<T> rhs)
{
    Matrix<T> temp = rhs;
    temp += lhs;
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
Matrix<T> operator-(Matrix<T> lhs, T rhs)
{
    Matrix<T> temp = lhs;
    temp -= rhs;
    return temp;
}

} // namespace matrix










    // GetDeterminant()
    // {
    //     assert(rows_ == columns_)
        
    //     int n = rows_;
    //     long double det = 1.0;

    //     for (int i = 0; i < n; i++) {
    //         long double pivot = static_cast<long double>(matrix[i * rows_ + i]);
    //         if (std::fabs(pivot) < 1e-10) {
    //             return 0;
    //         }
            
    //         det *= pivot;
            
    //         for (int j = i + 1; j < n; j++) 
    //         {
    //             long double factor = static_cast<long double>(matrix[j * rows_ + i]) / pivot;

    //             for (int k = i; k < n; k++)
    //             {
    //                 matrix[j * rows_ + k] -= factor * static_cast<long double>(matrix[i * rows_ + k]);
    //             }
    //         }
    //     }

    //     return det;
    // }
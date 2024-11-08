#pragma once

#include <iostream>
#include <iterator>

#include "real_nums.hpp"

namespace matrix {

template <typename T>
class Row
{
public:
    Row() = default;
    Row(std::nullptr_t) {};
    
    ~Row()
    {
        if (release_)
            delete[] data_;
    }

    Row(size_t size, T *data) : size_(size), data_(data), release_(false) {}
    Row(size_t size) : size_(size), data_(new T [size] {}) {}
    
    template <typename InputIterator>
    Row(size_t size, InputIterator begin, InputIterator end) : size_(size), data_(new T [size] {})
    {
        size_t i = 0;
        for (auto it = begin; it != end; ++it, ++i)
        {
            assert(i <= size_);
            data_[i] = *it;
        }

        assert(i == size_);
    }

    Row(const Row<T> &other) : size_(other.size_), data_(new T [other.size_] {})
    {
        for (size_t i = 0; i != size_; ++i)
            data_[i] = other.data_[i];
    }

    Row &operator=(const Row<T> &other)
    {
        if (*this != other)
        {
            if (size_ != other.size_)
            {
                size_ = other.size_;
                delete[] data_;
                data_ = new T [size_];
            }
            
            for (size_t i = 0; i != size_; ++i)
                data_[i] = other.data_[i];
        }
        
        return *this;
    }

    Row(Row<T> &&other)
    {
        std::swap(data_, other.data_);
        size_ = other.size_;
    }

    Row &operator=(Row<T> &&other)
    {
        if (*this != other)
        {
            std::swap(data_, other.data_);
            size_ = other.size_;
        }

        return *this;
    }

    T& operator[](int num_elem)
    {
        return data_[num_elem];
    }

    const T& operator[](int num_elem) const
    {
        return data_[num_elem];
    }

    bool operator==(const Row<T> &other)
    {
        return (data_ == other.data_) && (size_ == other.size_);
    }

    bool operator!=(const Row<T> &other)
    {
        return !(*this == other);
    }

    Row &operator+=(const Row<T> &other)
    {
        assert(size_ == other.size_);

        for (size_t i = 0; i < size_; ++i)
            data_[i] += other.data_[i];

        return *this;
    }
    
    Row &operator-=(const Row<T> &other)
    {
        assert(size_ == other.size_);

        for (size_t i = 0; i < size_; ++i)
            data_[i] -= other.data_[i];

        return *this;
    }

    Row &operator+=(T rhs)
    {
        for (size_t i = 0; i < size_; ++i)
            data_[i] += rhs;

        return *this;
    }

    Row &operator-=(T rhs)
    {
        for (size_t i = 0; i < size_; ++i)
            data_[i] -= rhs;

        return *this;
    }

    Row &operator*=(const Row<T> &other)
    {
        assert(size_ == other.size_);

        for (size_t i = 0; i < size_; ++i)
            data_[i] *= other.data_[i];

        return *this;
    }

    Row &operator*=(T rhs)
    {
        for (size_t i = 0; i < size_; ++i)
            data_[i] *= rhs;

        return *this;
    }

    size_t GetSize() const
    {
        return size_;
    }

    T GetSum()
    {
        T temp = 0;

        for (size_t i = 0; i < size_; ++i)
            temp += data_[i];

        return temp;
    }

    template <typename InputIterator>
    Row<T> &RewriteRow(InputIterator *begin)
    {
        size_t i = 0;
        for (auto it = *begin; i < size_; ++i, ++it)
        {
            assert(it);
            data_[i] = *it;
        }

        return *this;
    }

    T *GetData()
    {
        return data_;
    }

    void print()
    {
        std::cout << "Row:" << std::endl;
        for (size_t i = 0; i < size_; ++i)
            std::cout << data_[i] << " ";

        std::cout << std::endl;
    }

private:
    T *data_ = nullptr;
    size_t size_ = 0;
    bool release_ = true;
}; // class Row

template <typename T>
static int swap_rows(matrix::Row<T>** rows, size_t from, size_t to)
{
    int mult = 1;
    for (size_t i = from + 1; i < to && real_nums::is_zero((*rows[from])[from]); i++)
    {
        mult *= -1;
        std::swap(rows[from], rows[i]);
    }

    if (real_nums::is_zero((*rows[from])[from]))
        return 0;
    return mult;
}

template <typename T> 
Row<T> operator+(Row<T> lhs, Row<T> rhs)
{
    Row<T> temp = lhs;
    temp += rhs;
    return temp;
}

template <typename T>
Row<T> operator+(Row<T> lhs, T rhs)
{
    Row<T> temp = lhs;
    temp += rhs;
    return temp;
}

template <typename T>
Row<T> operator+(T lhs, Row<T> rhs)
{
    Row<T> temp = rhs;
    temp += lhs;
    return temp;
}

template <typename T> 
Row<T> operator-(Row<T> lhs, Row<T> rhs)
{
    Row<T> temp = lhs;
    temp -= rhs;
    return temp;
}

template <typename T>
Row<T> operator-(Row<T> lhs, T rhs)
{
    Row<T> temp = lhs;
    temp -= rhs;
    return temp;
}

template <typename T> 
Row<T> operator*(Row<T> lhs, Row<T> rhs)
{
    Row<T> temp = lhs;
    temp *= rhs;
    return temp;
}

template <typename T>
Row<T> operator*(Row<T> lhs, T rhs)
{
    Row<T> temp = lhs;
    temp *= rhs;
    return temp;
}

template <typename T>
Row<T> operator*(T lhs, Row<T> rhs)
{
    Row<T> temp = rhs;
    temp *= lhs;
    return temp;
}

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

    Row<T> operator[] (int num_row)
    {
        return Row<T>{column_count_, GetData() + num_row * column_count_};
    }

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

    Matrix<T> Transpose() const
    {
        Matrix<T> transpose_matrix(column_count_, row_count_);

        T *src_matrix = GetData();
        T *dst_matrix = transpose_matrix.GetData();

        for (size_t i = 0; i < row_count_; ++i)
            for (size_t j = 0; j < column_count_; ++j)
                dst_matrix[j * row_count_ + i] = src_matrix[i * column_count_ + j];

        return transpose_matrix;
    }

    Matrix<T> &operator*=(const Matrix<T> &other)
    {
        assert(column_count_ == other.row_count_);

        Matrix<T> tother = other.Transpose();
        Matrix<T> result{row_count_, other.column_count_};

        T *data = GetData();
        T *rdata = result.GetData();
        T *tdata = tother.GetData();

        for (size_t i = 0; i < row_count_; i++)
        {
            Row<T> index_row_this(column_count_, data + i * column_count_);
            Row<T> tmp1(index_row_this);

            for (size_t j = 0; j < other.column_count_; j++)
            {
                Row<T> index_row_tother(tother.column_count_, tdata + j * tother.column_count_);
                Row<T> tmp2(index_row_tother);
             
                tmp2 *= tmp1;
                rdata[i * result.column_count_ + j] = tmp2.GetSum();
            }
        }

        *this = result;
        return *this;
    }

    Matrix<T> &operator*=(T rhs)
    {
        T *data = GetData();
        size_t size = row_count_ * column_count_;
        
        for (size_t i = 0; i < size; ++i)
            data[i] *= rhs;

        return *this;
    }

    long double GetDeterminant() const
    {
        assert(row_count_ == column_count_);

        size_t n = row_count_;
        long double det = 1;
        Matrix<long double> *tmp_matrix = new Matrix<long double>{n};
        T *data = GetData();
        long double *tmp_data = tmp_matrix->GetData();

        for (size_t i = 0; i < n * n; i++)
        {
            tmp_data[i] = static_cast<long double>(data[i]);
        }

        auto **rows = new Row<long double>*[n];
        for (size_t i = 0; i < n; i++)
        {
            rows[i] = new Row<long double>{n, tmp_data + i * n};
        }

        for (size_t i = 0; i < n - 1; i++)
        {
            for (size_t j = i + 1; j < n; j++)
            {
                det *= swap_rows(rows, i, n);
                if (det == 0)
                    return 0;

                *(rows[j]) -= (*(rows[i]) * ((*rows[j])[i] / (*rows[i])[i]));
            }
        }

        for (size_t i = 0; i < n; i++)
            det *= (*rows[i])[i];

        for (size_t i = 0; i < n; i++)
        {
            delete rows[i];
        }

        delete[] rows;
        delete tmp_matrix;
        
        return det;
    }

    void print() const
    {
        T *data = GetData();
        std::cout << "Matrix:" << std::endl;

        for (size_t i = 0; i < row_count_; ++i)
        {
            for (size_t j = 0; j < column_count_; ++j)
            {
                std::cout << data[i * column_count_ + j] << " ";
            }
            std::cout << std::endl;
        }
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
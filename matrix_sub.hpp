// José Joaquín Zubieta Rico
//
// This small library supports matrix multiplication and
#ifndef JZR_MATRIXSUB_H
#define JZR_MATRIXSUB_H 1

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace matrixlibsub {

template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
class Matrix {
private:
    std::size_t _rows;
    std::size_t _columns;
    std::vector<T> _data;

public:
    typedef T value_type;

    Matrix(std::size_t rows, std::size_t columns, T const& value = T())
        : _rows(rows)
        , _columns(columns)
        , _data(rows * columns, value)
    {
    }

    Matrix()
        : Matrix(0, 0)
    {
    }

    Matrix(Matrix<T> const& expr, T const& value)
        : _rows(expr.rows())
        , _columns(expr.columns())
        , _data(expr.size())
    {
        for (std::size_t i = 0; i < expr.rows(); ++i) {
            for (std::size_t j = 0; j < expr.columns(); ++j) {
                _data[i * _columns + j] = value;
            }
        }
    }

    std::size_t rows() const
    {
        return _rows;
    }

    std::size_t columns() const
    {
        return _columns;
    }

    std::size_t size() const
    {
        return _data.size();
    }

    T& operator()(std::size_t row, std::size_t column)
    {
        return _data[row * _columns + column];
    }

    T operator()(std::size_t row, std::size_t column) const
    {
        return _data[row * _columns + column];
    }

    T& at(std::size_t row, std::size_t column)
    {
        if (row >= _rows || column >= _columns) {
            throw std::out_of_range("Element index out of range");
        }
        return (*this)(row, column);
    }

    T at(std::size_t row, std::size_t column) const
    {
        return this->at(row, column);
    }

    Matrix<T> operator*(Matrix<T> const& expr) const
    {
        if (_columns != expr.rows()) {
            throw std::logic_error("Matrix multiplication size mismatch");
        }
        Matrix<T> result(_rows, expr.columns());
        for (std::size_t i = 0; i < _rows; ++i) {
            for (std::size_t j = 0; j < expr.columns(); ++j) {
                for (std::size_t k = 0; k < _columns; ++k) {
                    result(i, j) += _data[i * _columns + k] * expr(k, j);
                }
            }
        }
        return result;
    }

    Matrix<T> operator+(Matrix<T> const& rhs)
    {
        if (this->size() != rhs.size()) {
            throw std::logic_error("Matrix sum with different sizes");
        }
        Matrix<T> result(_rows, rhs.columns());
        for (std::size_t i = 0; i < _rows; ++i) {
            for (std::size_t j = 0; j < _columns; ++j) {
                result(i, j) = _data[i * _columns + j] + rhs(i, j);
            }
        }
        return result;
    }

    Matrix<T> operator-(Matrix<T> const& rhs)
    {
        if (this->size() != rhs.size()) {
            throw std::logic_error("Matrix sum with different sizes");
        }
        Matrix<T> result(_rows, rhs.columns());
        for (std::size_t i = 0; i < _rows; ++i) {
            for (std::size_t j = 0; j < _columns; ++j) {
                result(i, j) = _data[i * _columns + j] - rhs(i, j);
            }
        }
        return result;
    }
    
    Matrix<T> &operator+=(Matrix<T> const& rhs)
    {
        if (this->size() != rhs.size()) {
            throw std::logic_error("Matrix sum with different sizes");
        }
        for (std::size_t i = 0; i < _rows; ++i) {
            for (std::size_t j = 0; j < _columns; ++j) {
                _data[i * _columns + j] += rhs(i, j);
            }
        }
        return *this;
    }

    Matrix<T> &operator-=(Matrix<T> const& rhs)
    {
        if (this->size() != rhs.size()) {
            throw std::logic_error("Matrix sum with different sizes");
        }
        for (std::size_t i = 0; i < _rows; ++i) {
            for (std::size_t j = 0; j < _columns; ++j) {
                _data[i * _columns + j] -= rhs(i, j);
            }
        }
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& out, Matrix<T> const& rhs)
    {
        for (std::size_t i = 0; i < rhs.rows(); ++i) {
            for (std::size_t j = 0; j < rhs.columns(); ++j) {
                out << rhs(i, j);
                if (j < rhs.columns() - 1) {
                    out << " ";
                }
            }
            if (i < rhs.rows() - 1) {
                out << std::endl;
            }
        }
        return out;
    }
};

}
#endif

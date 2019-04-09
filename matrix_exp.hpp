// José Joaquín Zubieta Rico
//
// This small library supports matrix multiplication and
#ifndef JZR_MATRIX_H
#define JZR_MATRIX_H 1

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace matrixlib {

template <typename E>
class _MatrixExpr {
public:
    typedef E value_type;

    std::size_t size() const
    {
        return static_cast<E const&>(*this).size();
    }

    std::size_t rows() const
    {
        return static_cast<E const&>(*this).rows();
    }

    std::size_t columns() const
    {
        return static_cast<E const&>(*this).columns();
    }

    auto operator()(std::size_t row, std::size_t column) const
    {
        return static_cast<E const&>(*this)(row, column);
    }

    friend std::ostream& operator<<(std::ostream& out, _MatrixExpr<E> const& expr)
    {
        for (std::size_t i = 0; i < expr.rows(); ++i) {
            for (std::size_t j = 0; j < expr.columns(); ++j) {
                out << expr(i, j);
                if (j < expr.columns() - 1) {
                    out << " ";
                }
            }
            if (i < expr.rows() - 1) {
                out << std::endl;
            }
        }
        return out;
    }
};

template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
class Matrix : public _MatrixExpr<Matrix<T>> {
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

    template <typename U>
    Matrix(_MatrixExpr<U> const& expr)
        : _rows(expr.rows())
        , _columns(expr.columns())
        , _data(expr.size())
    {
        for (std::size_t i = 0; i < expr.rows(); ++i) {
            for (std::size_t j = 0; j < expr.columns(); ++j) {
                _data[i * _columns + j] = expr(i, j);
            }
        }
    }

    template <typename U>
    Matrix(_MatrixExpr<U> const& expr, T const& value)
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

    template <typename E>
    Matrix<T> operator*(_MatrixExpr<E> const& expr) const
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
    
    template <typename E>
    Matrix<T> &operator+=(_MatrixExpr<E> const& expr)
    {
        if (this->size() != expr.size()) {
            throw std::logic_error("Matrix sum with different sizes");
        }
        for (std::size_t i = 0; i < _rows; ++i) {
            for (std::size_t j = 0; j < _columns; ++j) {
                _data[i * _columns + j] += expr(i, j);
            }
        }
        return *this;
    }

    template <typename E>
    Matrix<T> &operator-=(_MatrixExpr<E> const& expr)
    {
        if (this->size() != expr.size()) {
            throw std::logic_error("Matrix sum with different sizes");
        }
        for (std::size_t i = 0; i < _rows; ++i) {
            for (std::size_t j = 0; j < _columns; ++j) {
                _data[i * _columns + j] -= expr(i, j);
            }
        }
        return *this;
    }
};

template <typename T, typename U>
class _MatrixDiff : public _MatrixExpr<_MatrixDiff<T, U>> {
private:
    T const& _first;
    U const& _second;

public:
    _MatrixDiff(T const& lhs, U const& rhs)
        : _first(lhs)
        , _second(rhs)
    {
        if (_first.size() != _second.size()) {
            throw std::logic_error("Matrix sum with different sizes");
        }
    }

    std::size_t size() const
    {
        return _first.size();
    }

    std::size_t rows() const
    {
        return _first.rows();
    }

    std::size_t columns() const
    {
        return _first.rows();
    }

    auto operator()(std::size_t row, std::size_t column) const
    {
        return _first(row, column) - _second(row, column);
    }
};

template <typename T, typename U>
_MatrixDiff<T, U>
operator-(T const& rhs, U const& lhs)
{
    return _MatrixDiff<T, U>(rhs, lhs);
}

template <typename T, typename U>
class _MatrixSum : public _MatrixExpr<_MatrixSum<T, U>> {
private:
    T const& _first;
    U const& _second;

public:
    _MatrixSum(T const& lhs, U const& rhs)
        : _first(lhs)
        , _second(rhs)
    {
        if (_first.size() != _second.size()) {
            throw std::logic_error("Matrix sum with different sizes");
        }
    }

    std::size_t size() const
    {
        return _first.size();
    }

    std::size_t rows() const
    {
        return _first.rows();
    }

    std::size_t columns() const
    {
        return _first.rows();
    }

    auto operator()(std::size_t row, std::size_t column) const
    {
        return _first(row, column) + _second(row, column);
    }
};

template <typename T, typename U>
_MatrixSum<T, U>
operator+(T const& rhs, U const& lhs)
{
    return _MatrixSum<T, U>(rhs, lhs);
}

}
#endif

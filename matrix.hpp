// Template for a matrix class.
// Very basic memory efficient matrix.  
#include <vector>

#ifndef MATRIX_HPP_INCLUDED
#define MATRIX_HPP_INCLUDED

template <typename T>
class Matrix {
    
  private:
    T* data_;
    size_t numRows_;
    size_t numCols_;

  public:
    Matrix(size_t numRows, size_t numColumns);
    ~Matrix();

    size_t numRows() const;
    size_t numCols() const;
    T &operator()(size_t row, size_t col) const;
    void print(std::ostream &os) const;
};

template <typename T>
std::ostream &operator<<(std::ostream &os, const Matrix<T> &m);

#include "matrix-private.hpp"

#endif // MATRIX_HPP_INCLUDED
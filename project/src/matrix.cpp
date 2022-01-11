#include <iomanip>
#include <istream>
#include <cmath>
#include "matrix.h"
#include "exceptions.h"

static int sign(size_t x) {
    return x % 2 == 0 ? 1 : -1;
}

namespace prep {
    Matrix::Matrix(size_t rows, size_t cols)
        :rows(rows), cols(cols), matr(rows, std::vector<double>(cols)) {}


    Matrix::Matrix(std::istream& is) {
        if (!(is >> rows >> cols)) {
            throw InvalidMatrixStream();
        }

        matr.resize(rows, std::vector<double>(cols));

        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                if (!(is >> matr[i][j])) {
                    throw InvalidMatrixStream();
                }
            }
        }
    }


    size_t Matrix::getRows() const {
        return rows;
    }


    size_t Matrix::getCols() const {
        return cols;
    }


    double Matrix::operator()(size_t i, size_t j) const {
        if (i >= rows || j >= cols) {
            throw OutOfRange(i, j, *this);
        }
        return matr[i][j];
    }


    double& Matrix::operator()(size_t i, size_t j) {
        if (i >= rows || j >= cols) {
            throw OutOfRange(i, j, *this);
        }
        return matr[i][j];
    }


    bool Matrix::operator==(const Matrix& rhs) const {
        if (this->rows != rhs.rows || this->cols != rhs.cols) {
            return false;
        }
        for (size_t i = 0; i < this->rows; ++i) {
            for (size_t j = 0; j < this->cols; ++j) {
                if (std::abs(this->matr[i][j] - rhs.matr[i][j]) > 1e-7) {
                    return false;
                }
            }
        }
        return true;
    }


    bool Matrix::operator!=(const Matrix& rhs) const {
        return !(*this == rhs);
    }


    Matrix Matrix::operator+(const Matrix& rhs) const {
        if (this->rows != rhs.rows || this->cols != rhs.cols) {
            throw DimensionMismatch(*this, rhs);
        }

        Matrix new_matrix(this->rows, this->cols);

        for (size_t i = 0; i < this->rows; ++i) {
            for (size_t j = 0; j < this->cols; ++j) {
                new_matrix.matr[i][j] = this->matr[i][j] + rhs.matr[i][j];
            }
        }
        return new_matrix;
    }


    Matrix Matrix::operator-(const Matrix& rhs) const {
        if (this->rows != rhs.rows || this->cols != rhs.cols) {
            throw DimensionMismatch(*this, rhs);
        }

        Matrix new_matrix(this->rows, this->cols);

        for (size_t i = 0; i < this->rows; ++i) {
            for (size_t j = 0; j < this->cols; ++j) {
                new_matrix.matr[i][j] = this->matr[i][j] - rhs.matr[i][j];
            }
        }
        return new_matrix;
    }


    Matrix Matrix::operator*(const Matrix& rhs) const {
        if (this->cols != rhs.rows) {
            throw DimensionMismatch(*this, rhs);
        }

        Matrix new_matrix(this->rows, rhs.cols);

        for (size_t i = 0; i < this->rows; ++i) {
            for (size_t j = 0; j < rhs.cols; ++j) {
                for (size_t k = 0; k < this->cols; ++k) {
                    new_matrix.matr[i][j] += this->matr[i][k] * rhs.matr[k][j];
                }
            }
        }
        return new_matrix;
    }


    Matrix Matrix::operator*(double val) const {
        Matrix new_matrix(this->rows, this->cols);

        for (size_t i = 0; i < this->rows; ++i) {
            for (size_t j = 0; j < this->cols; ++j) {
                new_matrix.matr[i][j] = this->matr[i][j] * val;
            }
        }
        return new_matrix;
    }


    Matrix operator*(double val, const Matrix& matrix) {
        return matrix * val;
    }


    std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
        os << matrix.rows << ' ' << matrix.cols << '\n';

        for (size_t i = 0; i < matrix.rows; ++i) {
            for (size_t j = 0; j < matrix.cols; ++j) {
                os << std::setprecision(std::numeric_limits<double>::max_digits10) << matrix.matr[i][j] << ' ';
            }
            os << '\n';
        }
        return os;
    }

    Matrix Matrix::minor_matrix(size_t i, size_t j) const {
        Matrix minor_matrix(this->rows - 1, this->cols - 1);

        for (size_t l = 0; l < this->rows - 1; ++l) {
            for (size_t k = 0; k < this->cols - 1; ++k) {
                const unsigned row_offset = l >= i;
                const unsigned col_offset = k >= j;
                minor_matrix.matr[l][k] = this->matr[l + row_offset][k + col_offset];
            }
        }
        return minor_matrix;
    }

    Matrix Matrix::transp() const {
        Matrix new_matrix(this->cols, this->rows);

        for (size_t i = 0; i < this->rows; ++i) {
            for (size_t j = 0; j < this->cols; ++j) {
                new_matrix.matr[j][i] = this->matr[i][j];
            }
        }
        return new_matrix;
    }


    double Matrix::det() const {
        if (this->cols != this->rows) {
            throw DimensionMismatch(*this);
        }

        double determinant = 0;

        switch (this->cols) {
            case 1:
                determinant = this->matr[0][0];
                break;
            case 2:
                determinant = this->matr[0][0] * this->matr[1][1] - this->matr[0][1] *this->matr[1][0];
                break;
            case 3:
                for (size_t i = 0; i < 3; ++i) {
                    determinant += this->matr[0][i] * this->matr[1][(i + 1) % 3] * this->matr[2][(i + 2) % 3];
                    determinant -= this->matr[0][i] * this->matr[1][(i + 2) % 3] * this->matr[2][(i + 1) % 3];
                }
                break;
            default:
                for (size_t k = 0; k < this->cols; ++k) {
                    double v = this->minor_matrix(0, k).det();

                    determinant += sign(k) * this->matr[0][k] * v;
                }
        }
        return determinant;
    }


    Matrix Matrix::adj() const {
        if (this->cols != this->rows) {
            throw DimensionMismatch(*this);
        }

        double determinant = this->det();
        if (determinant == 0) {
            throw SingularMatrix();
        }

        Matrix new_matrix(this->rows, this->cols);

        if (this->rows == 1) {
            new_matrix.matr[0][0] = 1;
            return new_matrix;
        }

        Matrix transp_matrix = this->transp();

        for (size_t i = 0; i < new_matrix.rows; ++i) {
            for (size_t j = 0; j < new_matrix.cols; ++j) {
                Matrix minor = transp_matrix.minor_matrix(i, j);

                double v = minor.det();

                new_matrix.matr[i][j] = sign(i + j) * v;
            }
        }

        return new_matrix;
    }


    Matrix Matrix::inv() const {
        if (this->cols != this->rows) {
            throw DimensionMismatch(*this);
        }

        double determinant = this->det();
        if (determinant == 0) {
            throw SingularMatrix();
        }

        Matrix attached = this->adj();

        double inv_det = 1 / determinant;

        return attached * inv_det;
    }
}  // namespace prep

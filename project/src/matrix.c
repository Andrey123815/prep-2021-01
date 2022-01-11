#include "matrix.h"

#include <stdlib.h>
#include <math.h>
#include <float.h>

//  Determination of the sign of a number
inline static int sign(size_t x) {
    return x % 2 == 0 ? 1 : -1;
}


static Matrix *get_minor(const Matrix *matrix, Matrix *minor, size_t row_matrix, size_t col_matrix) {
    for (size_t l = 0; l < matrix->row - 1; ++l) {
        for (size_t k = 0; k < matrix->col - 1; ++k) {
            if (l < row_matrix) {
                if (k < col_matrix) {
                    minor->matr[l][k] = matrix->matr[l][k];
                } else {
                    minor->matr[l][k] = matrix->matr[l][k + 1];
                }
            } else {
                if (k < col_matrix) {
                    minor->matr[l][k] = matrix->matr[l + 1][k];
                } else {
                    minor->matr[l][k] = matrix->matr[l + 1][k + 1];
                }
            }
        }
    }
    return minor;
}


//  Init/release operations
Matrix* create_matrix_from_file(const char* path_file) {
    if (path_file == NULL) {
        return NULL;
    }

    FILE* fmatrix = fopen(path_file, "a+");
    if (fmatrix == NULL) {
        return NULL;
    }

    size_t rows;
    size_t cols;
    if (fscanf(fmatrix, "%zu%zu", &rows, &cols) != 2) {
        fclose(fmatrix);
        return NULL;
    }

    Matrix* M = create_matrix(rows, cols);
    if (M == NULL) {
        fclose(fmatrix);
        return NULL;
    }

    M->row = rows;
    M->col = cols;

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            if (fscanf(fmatrix, "%lf", &M->matr[i][j]) != 1) {
                fclose(fmatrix);
                free_matrix(M);
                return NULL;
            }
        }
    }
    fclose(fmatrix);
    return M;
}


Matrix* create_matrix(size_t rows, size_t cols) {
    if (rows < 1 || cols < 1) {
        return NULL;
    }

    Matrix* M = malloc(sizeof(Matrix));
    if (M == NULL) {
        return NULL;
    }

    M->row = rows;
    M->col = cols;
    M->matr = calloc(rows, sizeof(double *));
    if (M->matr == NULL) {
        free(M);
        return NULL;
    }

    for (size_t i = 0; i < rows; ++i) {
        M->matr[i] = calloc(cols, sizeof(double));
        if (M->matr[i] == NULL) {
            free_matrix(M);
            return NULL;
        }
    }

    return M;
}


int free_matrix(Matrix* matrix) {
    if (matrix == NULL) {
        return -1;
    }
    for (size_t i = 0; i < matrix->row; ++i) {
        free(matrix->matr[i]);
    }
    free(matrix->matr);
    free(matrix);
    return 0;
}


//  Basic operations
int get_rows(const Matrix* matrix, size_t* rows) {
    if (matrix == NULL || rows == NULL) {
        return -1;
    }
    *rows = matrix->row;
    return 0;
}


int get_cols(const Matrix* matrix, size_t* cols) {
    if (matrix == NULL || cols == NULL) {
        return -1;
    }
    *cols = matrix->col;
    return 0;
}


int get_elem(const Matrix* matrix, size_t row, size_t col, double* val) {
    if (matrix == NULL || val == NULL || row >= matrix->row || col >= matrix->col) {
        return -1;
    }
    *val = matrix->matr[row][col];
    return 0;
}


int set_elem(Matrix* matrix, size_t row, size_t col, double val) {
    if (matrix == NULL || row >= matrix->row || col >= matrix->col) {
        return -1;
    }
    matrix->matr[row][col] = val;
    return 0;
}


//  Math operations
Matrix* mul_scalar(const Matrix* matrix, double val) {
    if (matrix == NULL) {
        return NULL;
    }

    Matrix* new_matrix = create_matrix(matrix->row, matrix->col);
    if (new_matrix == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < matrix->row; ++i) {
        for (size_t j = 0; j < matrix->col; ++j) {
            new_matrix->matr[i][j] = matrix->matr[i][j] * val;
        }
    }
    return new_matrix;
}


Matrix* transp(const Matrix* matrix) {
    if (matrix == NULL) {
        return NULL;
    }

    Matrix* new_matrix = create_matrix(matrix->col, matrix->row);
    if (new_matrix == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < matrix->row; ++i) {
        for (size_t j = 0; j < matrix->col; ++j) {
            new_matrix->matr[j][i] = matrix->matr[i][j];
        }
    }
    return new_matrix;
}


Matrix* sum(const Matrix* l, const Matrix* r) {
    if (l == NULL || r == NULL || l->row != r->row || l->col != r->col) {
        return NULL;
    }

    Matrix* new_matrix = create_matrix(l->row, l->col);
    if (new_matrix == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < l->row; ++i) {
        for (size_t j = 0; j < l->col; ++j) {
            new_matrix->matr[i][j] = l->matr[i][j] + r->matr[i][j];
        }
    }
    return new_matrix;
}


Matrix* sub(const Matrix* l, const Matrix* r) {
    if (l == NULL || r == NULL || l->row != r->row || l->col != r->col) {
        return NULL;
    }

    Matrix* new_matrix = create_matrix(l->row, l->col);
    if (new_matrix == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < l->row; ++i) {
        for (size_t j = 0; j < l->col; ++j) {
            new_matrix->matr[i][j] = l->matr[i][j] - r->matr[i][j];
        }
    }
    return new_matrix;
}


Matrix* mul(const Matrix* l, const Matrix* r) {
    if (l == NULL || r == NULL || l->col != r->row) {
        return NULL;
    }

    Matrix* new_matrix = create_matrix(l->row, r->col);
    if (new_matrix == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < l->row; ++i) {
        for (size_t j = 0; j < r->col; ++j) {
            for (size_t k = 0; k < l->col; ++k) {
                new_matrix->matr[i][j] += l->matr[i][k] * r->matr[k][j];
            }
        }
    }
    return new_matrix;
}


//  Extra operations
int det(const Matrix* matrix, double* val) {
    if (matrix == NULL || val == NULL || matrix->col != matrix->row) {
        return -1;
    }

    double** M = matrix->matr;
    *val = 0;

    switch (matrix->col) {
        case 1:
            *val = M[0][0];
            break;
        case 2:
            *val = M[0][0] * M[1][1] - M[0][1] *M[1][0];
            break;
        case 3:
            for (int i = 0; i < 3; ++i) {
                *val += M[0][i] * M[1][(i + 1) % 3] * M[2][(i + 2) % 3];
                *val -= M[0][i] * M[1][(i + 2) % 3] * M[2][(i + 1) % 3];
            }
            break;
        default:
            for (size_t k = 0; k < matrix->col; ++k) {
                Matrix* new_matrix = create_matrix(matrix->row - 1, matrix->col - 1);
                if (new_matrix == NULL) {
                    return -1;
                }

                double v;
                if (det(get_minor(matrix, new_matrix, 0, k), &v) != 0) {
                    free_matrix(new_matrix);
                    return -1;
                }

                *val += sign(k) * M[0][k] * v;

                free_matrix(new_matrix);
            }
    }
    return 0;
}


Matrix* adj(const Matrix* matrix) {
    if (matrix == NULL || matrix->col != matrix->row) {
        return NULL;
    }

    double determinant = 0;
    if (det(matrix, &determinant) != 0 || fabs(determinant) < DBL_EPSILON) {
        return NULL;
    }

    Matrix* new_matrix = create_matrix(matrix->row, matrix->col);
    if (new_matrix == NULL) {
        return NULL;
    }

    if (matrix->col == 1) {
        new_matrix->matr[0][0] = 1;
        return new_matrix;
    }

    Matrix* transp_matrix = transp(matrix);
    if (transp_matrix == NULL) {
        free_matrix(new_matrix);
        return NULL;
    }

    for (size_t i = 0; i < new_matrix->row; ++i) {
        for (size_t j = 0; j < new_matrix->col; ++j) {
            Matrix* minor = create_matrix(transp_matrix->row - 1, transp_matrix->col - 1);
            if (minor == NULL) {
                free_matrix(transp_matrix);
                free_matrix(new_matrix);
                return NULL;
            }

            double v = 0;
            if (det(get_minor(transp_matrix, minor, i, j), &v) != 0) {
                free_matrix(minor);
                free_matrix(transp_matrix);
                free_matrix(new_matrix);
                return NULL;
            }

            new_matrix->matr[i][j] = sign(i + j) * v;

            free_matrix(minor);
        }
    }

    free_matrix(transp_matrix);
    return new_matrix;
}


Matrix* inv(const Matrix* matrix) {
    if (matrix == NULL || matrix->col != matrix->row) {
        return NULL;
    }

    double determinant = 0;
    if (det(matrix, &determinant) != 0 || fabs(determinant) < DBL_EPSILON) {
        return NULL;
    }

    Matrix* attached = adj(matrix);
    if (attached == NULL) {
        return NULL;
    }

    double inv_det = 1 / determinant;
    Matrix* new_matrix = mul_scalar(attached, inv_det);
    if (new_matrix == NULL) {
        free_matrix(attached);
        return NULL;
    }

    free_matrix(attached);
    return new_matrix;
}

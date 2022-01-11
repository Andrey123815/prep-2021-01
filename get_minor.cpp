//
// Created by andrey on 17.04.2021.
//

Matrix *get_minor (Matrix *matrix, Matrix *minor, size_t row_matrix, size_t col_matrix) {
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
}
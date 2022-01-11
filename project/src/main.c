#include "matrix.h"

int main(void) {
    Matrix* a = create_matrix_from_file("Matr");
    Matrix* b = create_matrix(2, 2), *c[4];

    size_t w;
    double v = 0;

    get_cols(a, &w);
    get_rows(a, &w);
    set_elem(b, 0, 0, v);
    get_elem(b, 0, 0, &v);
    c[0] = sum(a, b);
    c[1] = sub(a, b);
    c[2] = mul(a, b);
    c[3] = inv(a);

    free_matrix(a);
    free_matrix(b);
    for (int i = 0; i < 4; i++) {
        free_matrix(c[i]);
    }
    return 0;
}


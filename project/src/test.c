#include <string.h>
#include "test.h"

#define FILE_READ 0


int compare_struct(bank_client obj1, bank_client obj2) {
    if (!(obj1.name && obj1.surname && obj1.address && obj1.tel_number &&
        obj2.name && obj2.surname && obj2.address && obj2.tel_number)) {
        puts("error: passed arguments");
        return 1;
    }
    if (obj1.number == obj2.number &&
        !strcmp(obj1.name, obj2.name) &&
        !strcmp(obj1.surname, obj2.surname) &&
        !strcmp(obj1.address, obj2.address) &&
        !strcmp(obj1.tel_number, obj2.tel_number) &&
        obj1.indebtedness == obj2.indebtedness &&
        obj1.credit_limit == obj2.credit_limit &&
        obj1.cash_payments == obj2.cash_payments) {
        return 0;
    }
    return 1;
}

int test_write_and_read() {
    const char *filename = "ftest.dat";
    bank_client test_begin[3] = {
        {1, "Sergey1", "Tit", "st.Pushkin", "89252281337", 20, 140, 73.02},
        {2, "Sergey2", "Tit", "st.Pushkin", "89252281337", 20, 140, 73.02},
        {3, "Sergey3", "Tit", "st.Pushkin", "89252281337", 20, 140, 73.02}
    };
    bank_client test_end[3];

    for (int i = 0; i < 3; i++) {
        if (write_to_file(filename, &test_begin[i])) {
            return 1;
        }
    }

    for (int num_clients = 0; num_clients < 3; num_clients++) {
        if (read_client(filename, &test_end[num_clients], num_clients, FILE_READ)) {
            return 1;
        }
    }

    for (int i = 0; i < 3; i++) {
        if (compare_struct(test_begin[i], test_end[i])) {
            return 1;
        }
    }

    return 0;
}

int main() {
    if (test_write_and_read() == 0) {
        puts("test passed");
        return 0;
    }
    puts("test failed");

    return 0;
}

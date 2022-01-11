#include "read_and_write.h"

#define STR_LEN_CLIENT 107
#define STR_LEN_TRANSACTION 10
#define NOT_SUCCESS 1

int read_data(FILE * fptr, bank_client *data) {
    if (fscanf(fptr, "%12d%11s%11s%16s",
               &data->number,
               data->name,
               data->surname,
               data->address) != 4 || fscanf(fptr, "%20s%12lf%12lf%12lf",
               data->tel_number,
               &data->indebtedness,
               &data->credit_limit,
               &data->cash_payments) != 4) {
        return 1;
    }
    return 0;
}

int read_client(const char *filename, bank_client *data, int num_client, bool is_stdin) {
    if (filename == NULL || data == NULL) {
        return 1;
    }

    FILE *fptr = stdin;
    if (!is_stdin) {
        fptr = fopen(filename, "a+");
        if (fptr == NULL) {
            return 1;
        }

        if (fseek(fptr, STR_LEN_CLIENT * num_client, 0) != 0) {
            fclose(fptr);
            return 1;
        }
    }

    if (read_data(fptr, data) == NOT_SUCCESS) {
        if (!is_stdin) {
            fclose(fptr);
        }
        return 1;
    }

    if (!is_stdin) {
        fclose(fptr);
    }
    return 0;
}

int read_transfer(const char *filename, bank_client *data, int num_transfer, bool is_stdin) {
    if (filename == NULL || data == NULL) {
        return 1;
    }

    FILE *fptr = stdin;
    if (!is_stdin) {
        fptr = fopen(filename, "a+");
        if (fptr == NULL) {
            return 1;
        }

        if (fseek(fptr, STR_LEN_TRANSACTION * num_transfer, 0) != 0) {
            fclose(fptr);
            return 1;
        }
    }

    if (fscanf(fptr, "%12d %12lf", &data->number, &data->cash_payments) != 2) {
        if (!is_stdin) {
            fclose(fptr);
        }
        return 1;
    }

    if (!is_stdin) {
        fclose(fptr);
    }
    return 0;
}

int write_to_file(const char *filename, bank_client *data) {
    if (!(data->name && data->surname && data->address && data->tel_number)) {
        return 1;
    }

    FILE *fptr = fopen(filename, "a+");
    if (fptr == NULL) {
        return 1;
    }

    fprintf(fptr, "%-12d%-11s%-11s%-16s",
            data->number,
            data->name,
            data->surname,
            data->address);
    fprintf(fptr, "%20s%12.2f%12.2f%12.2f\n",
        data->tel_number,
        data->indebtedness,
        data->credit_limit,
        data->cash_payments);

    fclose(fptr);

    return 0;
}

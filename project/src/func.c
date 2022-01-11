#include "func.h"

#define FILE_READ 0
#define STD_READ 1



static void print_form1() {
    printf("1 Number account: \n"
        "2 Client name: \n"
        "3 Surname: \n"
        "4 Address client: \n");
    printf("5 Client Telnum: \n"
        "6 Client indebtedness: \n"
        "7 Client credit limit: \n"
        "8 Client cash payments: \n");
}

static void print_form2() {
    printf("1 Number account: \n"
        "2 Client cash payments: \n");
}

void create_record(const char *record) {
    print_form1();

    bank_client client;
    int num_clients = 0;

    while (!read_client("", &client, num_clients, STD_READ)) {
        write_to_file(record, &client);
        num_clients++;
        print_form1();
    }
}

void create_transaction(const char *file_transaction) {
    FILE *transaction = fopen(file_transaction, "a+");
    if (transaction == NULL) {
        return;
    }

    print_form2();

    bank_client transfer;
    int num_transfer = 0;

    while (!read_transfer("", &transfer, num_transfer, STD_READ)) {
        fprintf(transaction, "%-3d%-6.2f\n", transfer.number, transfer.cash_payments);
        num_transfer++;
        print_form2();
    }
    fclose(transaction);
}

void update_record(const char *record, const char *transaction, const char *blackrecord) {
    bank_client client;
    bank_client transfer;
    int num_clients = 0;
    int num_transfer = 0;

    while (!read_client(record, &client, num_clients, FILE_READ)) {
        while (!read_transfer(transaction, &transfer, num_transfer, FILE_READ)) {
            if (client.number == transfer.number && transfer.cash_payments != 0) {
                client.credit_limit += transfer.cash_payments;
            }
            num_transfer++;
        }
        num_transfer = 0;
        num_clients++;

        write_to_file(blackrecord, &client);
    }
}

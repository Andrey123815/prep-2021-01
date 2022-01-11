#include "func.h"


static void print_menu() {
    printf("%s\n",
        "please enter action\n"
        "1 enter data client: \n"
        "2 enter data transaction: \n"
        "3 update base: \n");
}

int main() {
    int choice = 0;
    const char *record = "record.dat";
    const char *transaction = "transaction.dat";
    const char *blackrecord = "blackrecord.dat";

    print_menu();
    while (scanf("%d", &choice) == 1) {
        switch (choice) {
            case 1:
                create_record(record);
                break;
            case 2:
                create_transaction(transaction);
                break;
            case 3:
                update_record(record, transaction, blackrecord);
                break;
            default:
                puts("error");
                break;
        }
        print_menu();
    }
    return 0;
}

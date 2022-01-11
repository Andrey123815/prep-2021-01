<<<<<<< HEAD
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
=======
#include <stdlib.h>
#include <stdio.h>
#include "simple_number.h"
#include "utils.h"

#define ERR_ARGS_COUNT (-1)
#define ERR_WRONG_FLG (-2)

#define LENGTH_STR     1
#define NUM_IN_POWER   2
#define SIMPLE_NUMBER  3


int main(int argc, const char** argv) {
    if (argc < 3) {
        return ERR_ARGS_COUNT;
    }

    int test_case = atoi(argv[1]);
    const char* data = argv[2];

    switch (test_case) {
        case LENGTH_STR: {
            size_t res = custom_strlen(data);
            printf("%zu\n", res);
            break;
        }

        case NUM_IN_POWER: {
            if (argc != 4) {
                return ERR_ARGS_COUNT;
            }
            int base = atoi(data);
            int pow =  atoi(argv[3]);
            printf("%i\n", custom_power(base, pow));
            break;
        }

        case SIMPLE_NUMBER: {
            int num = atoi(data);
            printf("%i\n", smp_num(num));
            break;
        }

        default: {
            return ERR_WRONG_FLG;
        }
    }

>>>>>>> ac1e43c45e06145cfa76e196b6be40b3159ff192
    return 0;
}

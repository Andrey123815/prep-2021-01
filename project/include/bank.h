#ifndef PROJECT_INCLUDE_BANK_H_
#define PROJECT_INCLUDE_BANK_H_


typedef struct {
    int number;
    char name[20];
    char surname[20];
    char address[30];
    char tel_number[15];
    double indebtedness;
    double credit_limit;
    double cash_payments;
} bank_client;

#endif  //  PROJECT_INCLUDE_BANK_H_

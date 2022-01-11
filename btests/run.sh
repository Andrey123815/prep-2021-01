#!/usr/bin/env bash

PROG_PATH=${1}  # ./main.out
<<<<<<< HEAD
ROOT=$(dirname $(realpath $0))

RECORDS_F_NAME="record.dat"
TRANSACTIONS_F_NAME="transaction.dat"
BLACKRECORDS_F_NAME="blackrecord.dat"

rm -f $RECORDS_F_NAME $TRANSACTIONS_F_NAME $BLACKRECORDS_F_NAME
touch $RECORDS_F_NAME $TRANSACTIONS_F_NAME $BLACKRECORDS_F_NAME

IN="1
1
name_1
surname_1
address_1
tel_1
80
150
20
2
name_2
surname_2
address_2
tel_2
100
200
59"

echo "${IN}" | eval "${PROG_PATH}"

IN="2
2
85
1
24"

echo "${IN}" | eval "${PROG_PATH}"

IN="3"

echo "${IN}" | eval "${PROG_PATH}"

for file in $RECORDS_F_NAME $TRANSACTIONS_F_NAME $BLACKRECORDS_F_NAME; do
	diff -uN $file $ROOT/${file}.gold
	if [[ "$?" != "0" ]]; then
		echo "Files mismatches"
		exit 1
	fi
done
=======
CHECK_MEM_ARG="--memcheck"
LOCAL_ARG="--local"

VALGRIND_LOG="valgrind.log"
NO_LOST_PATTERN="All heap blocks were freed -- no leaks are possible"
NO_LOST_PATTERN_LOCAL="definitely lost: 0 bytes"
NO_ERROR_PATTERN="ERROR SUMMARY: 0 errors"

if [ -z "$1" ]; then
    echo "Usage: run_btests.sh /path/to/main.out [--memcheck [--local]]"
    exit 1
fi

for test in ./btests/*.tst; do
    source ./btests/_default
    source ${test}

    EXPECTED=${OUT}
    RECEIVED=$(echo "${IN}" | eval "${PROG_PATH} ${KEYS}")
    REC_STATUS=$?
    STATUS=${STATUS:-0}

    if [ "${REC_STATUS}" != "${STATUS}" ]; then
        echo -e "TEST ${test} FAILED. INVALID EXIT STATUS"\
                "\nEXPECTED:\n${STATUS}"\
                "\nRECEIVED:\n${REC_STATUS}"
        exit 1
    fi

    if [ -n "${EXPECTED}" ] && [ "${EXPECTED}" != "${RECEIVED}" ]; then
        echo -e "TEST ${test} FAILED"\
                "\nEXPECTED (${#EXPECTED} symbols):\n${EXPECTED}"\
                "\nRECEIVED (${#RECEIVED} symbols):\n${RECEIVED}"
        exit 1
    fi

    # TODO: Оверхед :(
    if [[ ${2} == ${CHECK_MEM_ARG} ]]; then
        if [[ ${3} == ${LOCAL_ARG} ]]; then
            NO_LOST_PATTERN=${NO_LOST_PATTERN_LOCAL}
        fi
        RECEIVED=$(echo "${IN}" | eval "valgrind --tool=memcheck --leak-check=summary --log-file=${VALGRIND_LOG} ${PROG_PATH} ${KEYS}")

        NO_LOST=$(grep "${NO_LOST_PATTERN}" ${VALGRIND_LOG})
        NO_ERROR=$(grep "${NO_ERROR_PATTERN}" ${VALGRIND_LOG})
        if [ -z "${NO_LOST}" ] || [ -z "${NO_ERROR}" ]; then
            echo -e "TEST ${test} FAILED\n"

            cat ${VALGRIND_LOG}
            echo -e "\n\nRUN: valgrind --tool=memcheck --leak-check=full ./main.out\n\n"

            rm -rf ${VALGRIND_LOG}
            exit 1
        fi
    fi

    echo -n "."
done

rm -f ${VALGRIND_LOG}
echo "SUCCESS"
>>>>>>> ac1e43c45e06145cfa76e196b6be40b3159ff192

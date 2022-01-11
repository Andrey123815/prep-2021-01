#!/usr/bin/env bash

PROG_PATH=${1}  # ./main.out
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

#!/usr/bin/env bash

PROG_PATH=${1}  # ./main.out
if [ ! -f "${PROG_PATH}" ]; then
    echo "Invalid prog path: '${PROG_PATH}'"
    exit -1
fi

shift; # remove first arg from $@

# Setup default tests path
TESTS=$(find btests/testcases -type f -name '*.tst' 2>/dev/null | tr ' ' '\n')

WITH_MEMCHECK=0
LOCAL_MEMCHECK=0
DEFAULT_FNAME="./btests/_default"

#####################################
# Parse args
#####################################

while true; do
    case "$1" in
        --memcheck )
            WITH_MEMCHECK=1
            echo "Memcheck enabled"
            shift
            ;;
        --local )
            LOCAL_MEMCHECK=1
            shift
            ;;
        --default-fname )
            DEFAULT_FNAME="$2"
            shift 2
            ;;
        -- )
            shift
            TESTS=$@
            break
            ;;
        * )
            break
            ;;
    esac
done

if [ ! -f "${DEFAULT_FNAME}" ]; then
    echo "Invalid default file: ${DEFAULT_FNAME}"
    exit -1
fi

if [ "${TESTS}" == "" ]; then
    echo "Usage: $0 /path/to/main.out [--default-fname default_test_out] [--memcheck [--local]] [ -- <Tests list> ]"
    exit -1
fi

#####################################
# End args parsing
#####################################

VALGRIND_LOG="valgrind.log"
NO_LOST_PATTERN="All heap blocks were freed -- no leaks are possible"
NO_LOST_PATTERN_LOCAL="definitely lost: 0 bytes"
NO_ERROR_PATTERN="ERROR SUMMARY: 0 errors"

for test in ${TESTS}; do
    echo "${test}"

    source ${DEFAULT_FNAME}
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
    if [[ "${WITH_MEMCHECK}" == "1" ]]; then
        if [[ "${LOCAL_MEMCHECK}" == "1" ]]; then
            NO_LOST_PATTERN=${NO_LOST_PATTERN_LOCAL}
        fi
        RECEIVED=$(echo "${IN}" | eval "valgrind --tool=memcheck --leak-check=summary --log-file=${VALGRIND_LOG} ${PROG_PATH} ${KEYS}")

        NO_LOST=$(grep "${NO_LOST_PATTERN}" ${VALGRIND_LOG})
        NO_ERROR=$(grep "${NO_ERROR_PATTERN}" ${VALGRIND_LOG})
        if [ -z "${NO_LOST}" ] || [ -z "${NO_ERROR}" ]; then
            echo -e "TEST ${test} FAILED\n"

            cat ${VALGRIND_LOG}
            echo -e "\n\nRUN: valgrind --tool=memcheck --leak-check=full ./main.out\n\n"

            rm -f ${VALGRIND_LOG}
            exit 1
        fi
    fi

    # echo -n "."
done

rm -f ${VALGRIND_LOG}
echo "SUCCESS"

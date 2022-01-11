#!/usr/bin/env bash

USAGE="Usage: $0 <executable> [args] [ -- <Tests list> ]

Args are:
    --default-fname <default_test_out>          Path to file with default test settings
    --tests-path /path/to/tests                 Path to directory with tests
    --memcheck                                  Enable memcheck
    --local                                     Enable memcheck for local machine
    --enable-hexdump                            Enable hexdmp for failed test cases
"

if [ "$1" == "-h" ] || [ "$1" == "--help" ]; then
    echo "$USAGE"
    exit 0
fi

PROG_PATH=${1}  # ./main.out
if [ ! -f "${PROG_PATH}" ]; then
    echo "Invalid prog path: '${PROG_PATH}'"
    exit -1
fi

shift; # remove first arg from $@

# Setup default tests path
TESTS=

WITH_MEMCHECK=0
LOCAL_MEMCHECK=0
SPECIFIC_TESTS=0
WITH_HEXDUMP=0
TESTS_PATH="btests/testcases"
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
        --tests-path )
            TESTS_PATH="$2"
            shift 2
            ;;
        --enable-hexdump )
            WITH_HEXDUMP=1
            shift
            ;;
        -h )
            echo "$USAGE"
            exit 0
            ;;
        -- )
            shift
            SPECIFIC_TESTS=1
            TESTS=$@
            break
            ;;
        * )
            break
            ;;
    esac
done

if [ "${SPECIFIC_TESTS}" == "0" ]; then
    TESTS=$(find ${TESTS_PATH} -type f -name '*.tst' 2>/dev/null | sort | tr ' ' '\n')
fi

if [ ! -f "${DEFAULT_FNAME}" ]; then
    echo "Invalid default file: ${DEFAULT_FNAME}"
    exit -1
fi

if [ "${TESTS}" == "" ]; then
    echo "$USAGE"
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
    # NOTE(a.telyshev): Command Substitution.
    # FIXME(a.telyshev): Костыль: добавляем в конец символ, чтобы зацепить '\n', а потом этот же символ стрипаем.
    RECEIVEDZ="$(echo -n "${IN}" | eval "${PROG_PATH} ${KEYS}"; echo z)"
    RECEIVED="${RECEIVEDZ%?}"
    REC_STATUS=$?
    STATUS=${STATUS:-0}

    if [ "${REC_STATUS}" != "${STATUS}" ]; then
        echo -e "TEST ${test} FAILED. INVALID EXIT STATUS"\
                "\nEXPECTED:\n${STATUS}"\
                "\nRECEIVED:\n${REC_STATUS}"
        exit 1
    fi

    if [ -n "${EXPECTED}" ] && [ "${EXPECTED}" != "${RECEIVED}" ]; then
        DIFF_HEX=

        if [ "$WITH_HEXDUMP" == "1" ]; then
            DIFF_HEX=$(\
                diff -uN \
                    <(echo -n "${EXPECTED}" | hexdump -C)\
                    <(echo -n "${RECEIVED}" | hexdump -C)\
                | head -20
            )
            SEPARATOR="==============================================================================="
            DIFF_HEX=$(echo "\nDIFF (HEX):\n${SEPARATOR}\n${DIFF_HEX}\n${SEPARATOR}")
        fi

        echo -e "TEST ${test} FAILED"\
                "\nEXPECTED (${#EXPECTED} symbols):\n${EXPECTED}"\
                "\nRECEIVED (${#RECEIVED} symbols):\n${RECEIVED}"\
                "${DIFF_HEX}"
        exit 1
    fi

    # TODO(a.telyshev): Оверхед :(.
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

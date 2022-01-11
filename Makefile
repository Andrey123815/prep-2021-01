TARGET = main.out
TARGET2 = test.out
HDRS_DIR = project/include

SRCS = \
	project/src/main.c \
	project/src/func.c \
	project/src/read_and_write.c

SRC = \
	project/src/test.c \
	project/src/read_and_write.c

.PHONY: all clean

all: main test

main: $(SRCS)
	$(CC) -Wall -Wextra -Werror -I $(HDRS_DIR) -o $(TARGET) $(CFLAGS) $(SRCS)

test: $(SRC)
	$(CC) -Wall -Wextra -Werror -I $(HDRS_DIR) -o $(TARGET2) $(CFLAGS) $(SRC)
clean:
	rm -rf $(TARGET) $(TARGET2) ftest.dat

TARGET = main.out
HDRS_DIR = project/include

SRCS = \
       project/src/main.c \
       project/src/utils.c \
       project/src/simple_number.c

.PHONY: all clean

all: $(SRCS)
	$(CC) -Wall -Wextra -Werror -I $(HDRS_DIR) -o $(TARGET) $(CFLAGS) $(SRCS)

clean:
	rm -rf $(TARGET)

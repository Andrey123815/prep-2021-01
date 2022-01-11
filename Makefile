TARGET = main.out

HDRS = \
       project/include

SRCS = \
       project/src/main.c \
       project/src/parser.c \
       project/src/list.c 

.PHONY: all clean

all: $(SRCS)
	$(CC) -Wall -Wextra -Werror $(addprefix -I,$(HDRS)) -o $(TARGET) $(CFLAGS) $(SRCS) -lm

clean:
	rm -rf $(TARGET)

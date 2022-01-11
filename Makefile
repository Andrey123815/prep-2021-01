TARGET = main.out

# XXX: Don't forget backslash at the end of any line except the last one
HDRS = \
	   project/include

SRCS = \
       project/src/main.cpp \
       project/src/Entity.cpp \
       project/src/Map.cpp \
       project/src/Enemy.cpp \
       project/src/Player.cpp \
       project/src/game.cpp \
       project/src/Clothes.cpp

.PHONY: all clean

all: $(SRCS)
	$(CXX) -std=gnu++17 -Wall -Wextra -Werror $(addprefix -I,$(HDRS)) -o $(TARGET) $(CFLAGS) $(SRCS)

clean:
	rm -rf $(TARGET)

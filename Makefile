SRC=src
OBJ=obj
BIN=bin

CFLAGS += -Wfatal-errors -w -pthread
CC=clang++ -std=c++11
TARGET=ftp
RM=rm -rf

$(shell mkdir -p obj)

SRCS=$(wildcard $(SRC)/*.cpp)
OBJS=$(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SRCS))

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS)

$(OBJ)/%.o: $(SRC)/%.cpp
	${CC} ${CFLAGS} -I include -c $< -o $@

clean:
	$(RM) $(OBJ)/*.o 

fclean: clean
	$(RM) $(TARGET) $(OBJ)

re: 	fclean all

.PHONY: all clean fclean re


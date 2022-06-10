SRC=src
SERVERSRC=$(SRC)/server
CLIENTSRC=$(SRC)/client
OBJ=obj
CLIENTOBJ=$(OBJ)/client
SERVEROBJ=$(OBJ)/server
BIN=bin

CFLAGS += -Wfatal-errors -w -pthread -I ./include
CC=g++ -std=c++11
SERVERTARGET=server
CLIENTTARGET=client
RM=rm -rf
MKDR=mkdir -p

OBJDIR := $(shell $(MKDR) $(OBJ) $(CLIENTOBJ) $(SERVEROBJ))

SERVERSRCS=$(wildcard $(SERVERSRC)/*.cpp)
SERVEROBJS=$(patsubst $(SERVERSRC)/%.cpp, $(SERVEROBJ)/%.o, $(SERVERSRCS))

CLIENTSRCS=$(wildcard $(CLIENTSRC)/*.cpp)
CLIENTOBJS=$(patsubst $(CLIENTSRC)/%.cpp, $(CLIENTOBJ)/%.o, $(CLIENTSRCS))


# server

all: $(SERVERTARGET) $(CLIENTTARGET)

$(SERVERTARGET): $(SERVEROBJS)
		$(CC) -o $(SERVERTARGET) $(SERVEROBJS) $(CFLAGS)

$(SERVEROBJ)/%.o: $(SERVERSRC)/%.cpp
		${CC} ${CFLAGS} -c $< -o $@

# Client
$(CLIENTTARGET): $(CLIENTOBJS)
		$(CC) -o $(CLIENTTARGET) $(CLIENTOBJS) $(CFLAGS)

$(CLIENTOBJ)/%.o: $(CLIENTSRC)/%.cpp
		${CC} ${CFLAGS} -c $< -o $@

clean:
	$(RM) $(OBJ)/*/*.o 

fclean: clean
	$(RM) $(SERVERTARGET)
	$(RM) $(CLIENTTARGET)
	$(RM) $(OBJ)

re: 	clean
	$(MAKE) all

.PHONY: all clean fclean re


ifeq ($(OS),Windows_NT)
	OUTPUT_FILE_NAME = main.exe
else
    OUTPUT_FILE_NAME = main
endif

CC = g++
CFLAGS = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -D_DEBUG -D_EJUDGE_CLIENT_SIDE
IFLAGS = -I./include/ #-I../LOG/ -I../OptionsCmdLine/ -I./../

#main
SRC_DIR = ./src/
OBJ_DIR = ./lib/

SRC = $(wildcard $(SRC_DIR)*.cpp)
OBJ = $(patsubst $(SRC_DIR)%.cpp, $(OBJ_DIR)%.o, $(SRC)) 

#LOG
LOG_SRC_DIR = ../LOG/src/
LOG_OBJ_DIR = ../LOG/lib/

LOG_SRC = $(wildcard $(LOG_SRC_DIR)*.cpp)
LOG_OBJ = $(patsubst $(LOG_SRC_DIR)%.cpp, $(LOG_OBJ_DIR)%.o, $(LOG_SRC))

#OPTIONS_CMD_LINE
OPT_SRC_DIR = ../OptionsCmdLine/src/
OPT_OBJ_DIR = ../OptionsCmdLine/lib/

OPT_SRC = $(wildcard $(OPT_SRC_DIR)*.cpp)
OPT_OBJ = $(patsubst $(OPT_OBJ_DIR)%.cpp, $(OPT_OBJ_DIR)%.o, $(OPT_SRC))

#linking
all : $(OBJ) #$(LOG_OBJ) #$(OPT_OBJ)
	$(CC) $(IFLAGS) $(CFLAGS) $^ -o $(OUTPUT_FILE_NAME)

#main
$(OBJ_DIR)%.o : $(SRC_DIR)%.cpp
	mkdir -p $(@D)
	$(CC) $(IFLAGS) $(CFLAGS) -c $^ -o $@

#LOG
$(LOG_OBJ_DIR)%.o : $(LOG_SRC_DIR)%.cpp
	mkdir -p $(@D)
	$(CC) $(IFLAGS) $(CFLAGS) -c $^ -o $@

#OPTIONS_CMD_LINE
$(OPT_OBJ_DIR)%.o : $(OPT_SRC_DIR)%.cpp
	mkdir -p $(@D)
	$(CC) $(IFLAGS) $(CFLAGS) -c $^ -o $@

clear: 
	rm -f $(OBJ_DIR)*.o
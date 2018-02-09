OBJS = qsort_struct_norec.c

CC = gcc

COMPILER_FLAGS = -g -Wall -Wextra -pedantic
#COMPILER_FLAGS = -g -Wall -Wextra -pedantic -fsanitize=address -fno-omit-frame-pointer

LINKER_FLAGS =

OBJ_NAME = prog

all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

HEADERS = common.h
SRC = common.c
CC = gcc
LIBS = -fopenmp -lm
OUT_DIR = out

default: prepare

prepare:
	mkdir $(OUT_DIR)

task1: task1.c $(HEADERS) $(SRC)
	$(CC) $(SRC) task1.c -o $(OUT_DIR)/task1 $(LIBS) 

task2: task2.c $(HEADERS) $(SRC)
	$(CC) $(SRC) task2.c -o $(OUT_DIR)/task2 $(LIBS) 

task3: task3.c $(HEADERS) $(SRC)
	$(CC) $(SRC) task3.c -o $(OUT_DIR)/task3 $(LIBS)

clean:
	rm -rf $(OUT_DIR)
HEADERS = common.h
SRC = common.c
CC = gcc
LIBS = -fopenmp -lm
OUT_DIR = out
RUN_SCRIPT = python plot.py
default: prepare

prepare:
	rm -rf $(OUT_DIR)
	mkdir $(OUT_DIR)

task1: task1.c $(HEADERS) $(SRC)
	$(CC) $(SRC) task1.c -o $(OUT_DIR)/task1 $(LIBS)
	./$(OUT_DIR)/task1
	$(RUN_SCRIPT) $(OUT_DIR)/find_min_data*
	
task2: task2.c $(HEADERS) $(SRC)
	$(CC) $(SRC) task2.c -o $(OUT_DIR)/task2 $(LIBS)
	./$(OUT_DIR)/task2
	$(RUN_SCRIPT) $(OUT_DIR)/dot_product_data*

task3: task3.c $(HEADERS) $(SRC)
	$(CC) $(SRC) task3.c -o $(OUT_DIR)/task3 $(LIBS)
	./$(OUT_DIR)/task3
	$(RUN_SCRIPT) $(OUT_DIR)/integral_data*

task4: task4.c $(HEADERS) $(SRC)
	$(CC) $(SRC) task4.c -o $(OUT_DIR)/task4 $(LIBS)
	./$(OUT_DIR)/task4
	$(RUN_SCRIPT) $(OUT_DIR)/max_min_data*

task5: task5.c $(HEADERS) $(SRC)
	$(CC) $(SRC) task5.c -o $(OUT_DIR)/task5 $(LIBS)
	./$(OUT_DIR)/task5
	$(RUN_SCRIPT) $(OUT_DIR)/max_min_triang_data*

task6: task6.c $(HEADERS) $(SRC)
	$(CC) $(SRC) task6.c -o $(OUT_DIR)/task6 $(LIBS)
	./$(OUT_DIR)/task6
	$(RUN_SCRIPT) $(OUT_DIR)/sync_cmp_data*

clean:
	rm -rf $(OUT_DIR)
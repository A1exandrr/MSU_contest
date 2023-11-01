#=========================================================
# Основные флаги для работы программы
CC     = gcc
CFLAGS = -Wall -g
RM     = rm -f

# Цель по умолчанию
default: all

# Правило для компиляции программы
all: myprogram

myprogram: 20_2_02task.cpp
	$(CC) $(CFLAGS) -o main_test 20_2_02task.cpp
	./main_test
	$(RM) main_test
#=========================================================

#Это уже второстепенные точки запуска программы, для специальных вызывов


# Цель для запуска программы
run: myprogram
	./main_test input.txt output.txt
	$(RM) main_test

deb: myprogram
	gdb ./main_test
	$(RM) main_test

run_p: myprogram
	./main_test input.txt output.txt
	$(RM) main_test


deb_p: myprogram
	gdb ./main_test input.txt output.txt
	$(RM) main_test

# Цель для очистки
clean:
	$(RM) main_test
#=========================================================
# Основные флаги для работы программы
CC     = gcc
CFLAGS = -Wall -g
RM     = rm -f

# Цель по умолчанию
default: all

# Правило для компиляции программы
all: myprogram

myprogram: 20_2_02main.cpp
	$(CC) $(CFLAGS) -o main 20_2_02main.cpp
	./main
	$(RM) main
#=========================================================

#Это уже второстепенные точки запуска программы, для специальных вызывов


# Цель для запуска программы
run: myprogram
	./main input.txt output.txt
	$(RM) main

deb: myprogram
	gdb ./main
	$(RM) main

run_p: myprogram
	./main input.txt output.txt
	$(RM) main


deb_p: myprogram
	gdb ./main input.txt output.txt
	$(RM) main

# Цель для очистки
clean:
	$(RM) main
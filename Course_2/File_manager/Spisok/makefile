#=========================================================
# Основные флаги компиляции и запуска для работы программы
CC      = g++
CONTEST = -fsanitize=address
CFLAGS  = -Wall -g
RM      = rm -f

myprogram: 20_2_02main.cpp
	$(CC) $(CFLAGS) -o main 20_2_02main.cpp $(CONTEST)
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

memory: 20_2_02main.cpp
		$(CC) $(CFLAGS) -o main 20_2_02main.cpp
		valgrind --leak-check=full ./main
		$(RM) main

# Цель для очистки
clean:
	$(RM) main
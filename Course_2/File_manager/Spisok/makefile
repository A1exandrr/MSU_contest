#=========================================================
# Основные флаги компиляции и запуска для работы программы
CC       = g++
CFLAGS   = -Wall -g
ASan	 = -fsanitize=address
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all
RM       = rm -f

# Цель по умолчанию
default: main

# Правило для компиляции программы
main: program

program: 20_2_02main.cpp
	$(CC) $(CFLAGS) -o main 20_2_02main.cpp
	./main
	$(RM) main
#=========================================================

#Второстепенные точки запуска программы для специальных вызывов

#=========================================================

#Для контеста(осуществляется проверки работы программы с памятью через -fsanitize=address)
contest: 20_2_02main.cpp
	$(CC) $(CFLAGS) $(ASan) -o main 20_2_02main.cpp
	./main
	$(RM) main

#Для проверки работы программы с памятью через valgrind
valgrind: 20_2_02main.cpp
	$(CC) $(CFLAGS) -o main 20_2_02main.cpp
	$(VALGRIND) ./main
	$(RM) main
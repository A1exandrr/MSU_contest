#=========================================================
# Основные флаги компиляции и запуска для работы программы
CC       = g++
CFLAGS   = -Wall -g
ASan	 = -fsanitize=address
VALGRIND-leak-check = valgrind --leak-check=full --show-leak-kinds=all
VALGRIND-variable-null= valgrind --leak-check=full --track-origins=yes --show-reachable=yes
RM       = rm -f

# Цель по умолчанию
default: main

main: 20_2_02main.cpp
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

#Каждая отдельная утечка будет показана подробно и
#Показать все «определенные, косвенные, возможные, достижимые» виды утечек в «полном» отчете.
leak_check: 20_2_02main.cpp
	$(CC) $(CFLAGS) -o main 20_2_02main.cpp
	$(VALGRIND-leak-check) ./main
	$(RM) main

#
variable_null: 20_2_02main.cpp
	$(CC) $(CFLAGS) -o main 20_2_02main.cpp
	$(VALGRIND-leak-check) ./main
	$(RM) main
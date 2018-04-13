CC=gcc

maptest: mapmanagement.o test_mapmanagement.c
	gcc -o test_map mapmanagement.o test_mapmanagement.c


build: test.c hashmap.c
	gcc test.c hashmap.c -o test

run: build
	./test

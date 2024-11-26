mem: memory.c malloc.c
	gcc memory.c malloc.c -o memory 

clean:
	rm -r memory
all: dye dyed

dye: dye.c
	gcc -Wall -o dye dye.c

dyed: dyed.c
	gcc -Wall -o dyed dyed.c

clean:
	rm -f dye dyed


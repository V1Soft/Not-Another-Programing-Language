
napl: napl.o
	cc napl.o mpc.o -ledit -o napl

napl.o: napl.c mpc.c
	cc -c -std=c99 -Wall napl.c mpc.c -ledit -lm

clean:
	rm mpc.o napl.o

remove:
	rm napl



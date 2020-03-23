test: echeance.o
	$(CC) -o test echeance.o -lm

echeance.o: echeance.c
	$(CC) -O -Wall -W -ansi -c echeance.c -lm

#################################################################################

clean:
	rm -f *~ *.o
	rm -f test
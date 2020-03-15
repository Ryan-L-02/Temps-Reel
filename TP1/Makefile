test: chrono.o
	$(CC) -o test prem.o

chrono.o: chronogramme.c
	$(CC) -O -Wall -W -ansi -c chronogramme.c

#################################################################################

clean:
	rm -f *~ *.o
	rm -f test
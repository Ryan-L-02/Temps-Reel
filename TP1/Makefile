test: chronogramme.o sorted_job_list.o
	$(CC) -o test chronogramme.o sorted_job_list.o

chronogramme.o: chronogramme.c
	$(CC) -O -Wall -W -ansi -c chronogramme.c

sorted_job_list.o: sorted_job_list.c
	$(CC) -O -Wall -W -ansi -c sorted_job_list.c

#################################################################################

clean:
	rm -f *~ *.o
	rm -f test
all:
	gcc -pg biophysics.c -lm 
	./a.out
	gprof ./a.out > profile_info.txt

clean:
	rm -rf *.out

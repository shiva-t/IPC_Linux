TARGET: exec

exec: publisher subscriber

publisher:
	gcc -g -c publisher.c -o publisher.o
	gcc -g publisher.o -o publisher.exe -lrt

subscriber:
	gcc -g -c subscriber.c -o subscriber.o
	gcc -g subscriber.o -o subscriber.exe -lrt

clean:
	rm *.exe *.o

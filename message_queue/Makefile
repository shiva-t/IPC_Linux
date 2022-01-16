TARGET: exe

exe: sender receiver

sender:
	gcc -g -c sender.c -o sender.o
	gcc -g sender.o -o sender.exe -lrt
receiver:
	gcc -g -c receiver.c -o receiver.o
	gcc -g receiver.o -o receiver.exe -lrt

clean:
	rm *.o *.exe

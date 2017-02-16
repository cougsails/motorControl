example: example.c motorControl.c
	gcc -lwiringPi -lpthread -o output example.c motorControl.c -I.
clean:
	rm output

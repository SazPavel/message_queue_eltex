all:
	gcc server.c -o server
	gcc client.c -o client
	./server&
	sleep 1
	./client

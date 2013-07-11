CC = g++

koopa: main.cpp
	$(CC) main.cpp -lpthread -o hls

clean: 
	rm -Rf *.o koopa

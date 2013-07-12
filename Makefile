CC = g++

hls: main.cpp
	$(CC) main.cpp -lpthread -o hls

debug: main.cpp
	$(CC) main.cpp -lpthread -g -o hls

clean: 
	rm -Rf *.o hls

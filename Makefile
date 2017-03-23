all:
	g++ -std=gnu++11 main.cpp -Wall -lcurl -o main
clean:
	rm main

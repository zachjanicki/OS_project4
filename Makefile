all: site-tester
	

site-tester: 
	g++ -std=gnu++11 -static-libstdc++ -Wall mainThreading.cpp readConfigFile.h readFileWrapper.h -o site-tester -lcurl

clean:
	rm site-tester *.csv

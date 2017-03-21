// Counts the number of occurrences of a word in a file (single string file)
//

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

int main() {

	std::string fileName = "stuff.txt";
	std::ifstream infile(fileName);
	std::string word = "Apple";	


	std::string line;
	int count = 0;
	if (std::getline(infile, line)) {
		std::size_t found = line.find(word, 0); // first occurrence
		while (found != std::string::npos) {
			count++;
			found = line.find(word, found+1); // next occurrence
		}		
	} else {
		std::cout << "Error reading in string" << std::endl;	
	}

	std::cout << count << std::endl;





}

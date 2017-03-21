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


	string line;
	if (std::getline(infile, line)) {
		int count = 0;
		std::size_t found = s.find(word, 0); // first occurrence
		while (found != string::npos) {
			count++;
			found = s.find(word, found+1); // next occurrence
		}		
	} else {
		std::cout << "Error reading in string" << std::endl;	
	}

	std::cout << count;





}

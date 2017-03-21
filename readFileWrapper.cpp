#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class readFileWrapper {
public:
  readFileWrapper(string file) {
    fileName = file;
  }

  string fileName;
  vector<string> lines;

  void readFile() {
    ifstream infile(fileName);
    string line;
    while (getline(infile, line)) {
      lines.push_back(line);
    }
  }

  vector<string> getLines() {
    return lines;
  }
};

int main() {
  readFileWrapper r("text.txt");
  r.readFile();
  vector<string> lines = r.getLines();

  for (int i = 0; i < lines.size(); i++) {
    cout << lines[i] << endl;
  }
}

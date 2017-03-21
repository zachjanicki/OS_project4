#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

class readConfigFile {
public:
  readConfigFile(string file) {
    fileName = file;
  }

  string fileName;
  vector<string> lines;
  vector<string> params;
  vector<string> values;

  void readFile() {
    ifstream infile(fileName);
    string line;
    while (getline(infile, line)) {
      lines.push_back(line);
    }
  }

  void parse() {
    vector<string> linesVec = lines;
    for (int i = 0; i < linesVec.size(); i++) {
      stringstream tempStream(linesVec[i]);
      vector<string> paramAndValue;
      string temp;
      while (getline(tempStream, temp, '=')) {
        paramAndValue.push_back(temp);
      }
      params.push_back(paramAndValue[0]);
      values.push_back(paramAndValue[1]);
    }
  }

  vector<string> getLines() {
    return lines;
  }

  vector<string> getParams() {
    return params;
  }

  vector<string> getValues() {
    return values;
  }
};

int main() {
  readConfigFile r("testConfig.txt");
  r.readFile();
  vector<string> lines = r.getLines();
  r.parse();
  vector<string> params = r.getParams();
  vector<string> values = r.getValues();
  for (int i = 0; i < params.size(); i++) {
    cout << params[i] << "====" << values[i] << endl;
  }
}

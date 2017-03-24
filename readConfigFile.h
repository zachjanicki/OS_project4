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
    for (size_t i = 0; i < linesVec.size(); i++) {
      stringstream tempStream(linesVec[i]);
      vector<string> paramAndValue;
      string temp;
      int badParamFlag = 0;
      while (getline(tempStream, temp, '=')) {
        if (paramAndValue.size() == 0 && (temp != "PERIOD_FETCH" ||
                                        temp != "NUM_FETCH" ||
                                        temp != "NUM_PARSE" ||
                                        temp != "SEARCH_FILE" ||
                                        temp != "SITE_FILE")) {
          badParamFlag = 1;
        }
        paramAndValue.push_back(temp);
      }
      if (!badParamFlag) {
        params.push_back(paramAndValue[0]);
        values.push_back(paramAndValue[1]);
      } else {
        cout << "Illegal parameter '" << paramAndValue[0] << "' given" << endl;
      }
    }
    // check to make sure SEARCH_FILE and SITE_FILE are given
    int searchFileFlag = 0;
    int siteFileFlag = 0;
    for (size_t i = 0; i < params.size(); i++) {
      if (params[i] == "SEARCH_FILE") {
        searchFileFlag = 1;
      }
      if (params[i] == "SITE_FILE") {
        siteFileFlag = 1;
      }
    }
    if (!searchFileFlag || !siteFileFlag) {
      cout << "missing either SEARCH_FILE or SITE_FILE parameter" << endl;
      exit(0);
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

/*int main() {
  readConfigFile r("testConfig.txt");
  r.readFile();
  vector<string> lines = r.getLines();
  r.parse();
  vector<string> params = r.getParams();
  vector<string> values = r.getValues();
  for (int i = 0; i < params.size(); i++) {
    cout << params[i] << "====" << values[i] << endl;
  }
}*/

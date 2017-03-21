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
      int badParamFlag = 0;
      while (getline(tempStream, temp, '=')) {
        if (paramAndValue.size() == 0 && (temp != "PERIOD_FETCH" &&
                                        temp != "NUM_FETCH" &&
                                        temp != "NUM_PARSE" &&
                                        temp != "SEARCH_FILE" &&
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
    for (int i = 0; i < params.size(); i++) {
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

void usage() {
  cout << "Please provide a configuration file" << endl;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    usage();
    exit(1);
  }
  // default values for parameters
  int PERIOD_FETCH = 180;
  int NUM_FETCH = 1;
  int NUM_PARSE = 1;
  string SEARCH_FILE = "Search.txt";
  string SITE_FILE = "Sites.txt";
  readConfigFile configFile(argv[1]);
  configFile.readFile();
  configFile.parse();
  vector<string> params = configFile.getParams();
  vector<string> values = configFile.getValues();
  // loop through params and set params that are given
  for (int i = 0; i < params.size(); i++) {
    if (params[i] == "PERIOD_FETCH") {
      PERIOD_FETCH = stoi(values[i]);
    } else if (params[i] == "NUM_FETCH") {
      NUM_FETCH = stoi(values[i]);
    } else if (params[i] == "NUM_PARSE") {
      NUM_PARSE = stoi(values[i]);
    } else if (params[i] == "SEARCH_FILE") {
      SEARCH_FILE = values[i];
    } else if (params[i] == "SITE_FILE") {
      SITE_FILE = values[i];
    }
  }
  // read the site file to get list of sites to ping
  readFileWrapper siteList(SITE_FILE);
  siteList.readFile();
  vector<string> sites = siteList.getLines();
  // read search file to get list of search terms
  readFileWrapper searchTermList(SEARCH_FILE);
  searchTermList.readFile();
  vector<string> searchTerms = searchTermList.getLines();

  // debug output
  cout << PERIOD_FETCH << endl;
  cout << NUM_FETCH << endl;
  cout << NUM_PARSE << endl;
  cout << SEARCH_FILE << endl;
  cout << SITE_FILE << endl;
  for (int i = 0; i < searchTerms.size(); i++) {
    cout << searchTerms[i] << endl;
  }
  for (int i = 0; i < sites.size(); i++) {
    cout << sites[i] << endl;
  }

}

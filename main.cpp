#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <curl/curl.h>

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

class writeCSVWrapper {
public:
  writeCSVWrapper(string file) {
    fileName = file;
  }

  string fileName;
  void init() {
    ofstream outfile;
    outfile.open(fileName, std::ios_base::app);
    outfile << "Time,Phrase,Site,Count" << endl;
  }

  void writeLine(string time, string phrase, string site, int count) {
    ofstream outfile;
    outfile.open(fileName, std::ios_base::app);
    outfile << time << "," << phrase << "," << site << "," << count << endl;
  }
};


//libcurl struct and functions
struct MemoryStruct {
  char *memory;
  size_t size;
};

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  mem->memory = (char *) realloc(mem->memory, mem->size + realsize + 1);
  if(mem->memory == NULL) {
    /* out of memory! */
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }

  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

struct MemoryStruct runCurl(string url) {
  CURL *curl_handle;
  CURLcode res;

  struct MemoryStruct chunk;

  chunk.memory = (char *) malloc(1);  /* will be grown as needed by the realloc above */
  chunk.size = 0;    /* no data at this point */

  curl_global_init(CURL_GLOBAL_ALL);

  /* init the curl session */
  curl_handle = curl_easy_init();

  /* specify URL to get */
  curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());

  /* send all data to this function  */
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

  /* we pass our 'chunk' struct to the callback function */
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

  /* some servers don't like requests that are made without a user-agent
     field, so we provide one */
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

  /* get it! */
  res = curl_easy_perform(curl_handle);

  /* check for errors */
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
  }
  else {
    /*
     * Now, our chunk.memory points to a memory block that is chunk.size
     * bytes big and contains the remote file.
     *
     * Do something nice with it!
     */
    curl_easy_cleanup(curl_handle);
    curl_global_cleanup();
    printf("%lu bytes retrieved\n", (long)chunk.size);
    return chunk;
  }

  /* cleanup curl stuff */


  //free(chunk.memory);

  /* we're done with libcurl, so clean it up */

  //return 0;
}


void usage() {
  cout << "Please provide a configuration file" << endl;
}

// function to count the words of a given keyword after a curl request
int wordCount(string curlResult, string word) {
  string line;
  int count = 0;
  istringstream curlResultStream(curlResult);
  while (std::getline(curlResultStream, line)) {
    size_t found = line.find(word, 0); // first occurrence
    while (found != string::npos) {
      count++;
      found = line.find(word, found + 1); // next occurrence and all others
    }
  }
  return count;
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
  writeCSVWrapper outputFile("example.csv");
  outputFile.init();
  vector<MemoryStruct> curlResults;
  int running = 1;

  // main program loop
  while (running) {
    vector<int> countResults;
    vector<string> curlResultsAsString;
    for (int i = 0; i < sites.size(); i++) {
      // put one curling job into a thread and output to vector
      cout << sites[i] << endl;
      curlResults.push_back(runCurl(sites[i]));
      curlResultsAsString.push_back(curlResults[i].memory);
    }
    for (int i = 0; i < curlResultsAsString.size(); i++) {
      for (int j = 0; j < searchTerms.size(); j++) {
        int c = wordCount(curlResultsAsString[i], searchTerms[j]);
        countResults.push_back(c);
        // every n search terms (# of total search terms, there is a new site)
      }
    }
    cout << "count results.size " << countResults.size() << endl;
    for (int j = 0; j < sites.size(); j++) {
      for (int k = 0; k < searchTerms.size(); k++) {
        outputFile.writeLine("time", searchTerms[k], sites[j], countResults[j + k]);
      }
    }
    running = 0;
    sleep(PERIOD_FETCH);
  }

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
  for (int i = 0; i < curlResults.size(); i++) {
    free(curlResults[i].memory);
  }
}

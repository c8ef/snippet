#include <string.h>

#include <iostream>
#include <regex>
#include <string>
#include <vector>

using namespace std;

void split(const string& s, vector<string>& tokens, const string& delim = " ") {
  string::size_type lastPos = s.find_first_not_of(delim, 0);
  string::size_type pos = s.find_first_of(delim, lastPos);
  while (string::npos != pos || string::npos != lastPos) {
    tokens.emplace_back(s.substr(lastPos, pos - lastPos));
    lastPos = s.find_first_not_of(delim, pos);
    pos = s.find_first_of(delim, lastPos);
  }
}

int main() {
  cout << ">>>> 1" << endl;
  string str = "one two three";
  char* token = strtok(str.data(), " ");
  while (token != NULL) {
    cout << token << endl;
    token = strtok(NULL, " ");
  }

  cout << ">>>> 2" << endl;
  str = "one two three";
  vector<string> s;
  split(str, s);
  for (auto v : s) {
    cout << v << endl;
  }

  cout << ">>>> 3" << endl;
  str = "one two three";
  regex ws_re{"\\s+"};
  vector<string> v(sregex_token_iterator(str.begin(), str.end(), ws_re, -1),
                   sregex_token_iterator());
  for (auto&& p : v) {
    cout << p << endl;
  }
}
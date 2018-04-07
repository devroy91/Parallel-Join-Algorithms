#include <bits/stdc++.h>
using namespace std;
typedef struct entry{
	int idx;
	string value;
}entry;

typedef map<string, vector< entry > > Map;


void write_csv(Map d, string filename, char delim = ',');
vector<string> getVal(Map d, string key);
string getVal(Map d, string key, int id);
Map read_csv(string filename, bool head = true);
vector<string > ssplit(string s);


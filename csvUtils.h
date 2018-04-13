#include <bits/stdc++.h>
using namespace std;
typedef class entry{
	public:
	int idx;
	string value;
	int ival;
	entry(int idx, string value){
	    this->idx = idx;
	    this->value = value;
	    try{
                this->ival = stoi(value);
            }
            catch(exception e){
	        this->ival = -9999;
	    }
	}
	
}entry;

typedef map<string, vector< entry > > Map;
typedef class csvstream{
        public:
                string filename;
                string mode;
		
                csvstream(string name){
                        this->filename = name;
                        
                }
                int ipos = 0;
		int lineNo = 0;
		int len = 0;
		Map read_csv(int chunk, bool head, bool contiguous);
                Map read_csv(int chunk, bool head);
                Map read_csv(bool head);
		Map read_csv(vector<string> &s, bool head);
		vector< string > header;
}csvstream;

typedef enum mode{out, app} Mode;
void write_csv(Map d, string filename, Mode m, char delim = ',');
vector<string> getVal(Map d, string key);
string getVal(Map d, string key, int id);
/*Map read_csv(string filename, int chunk, bool head = true);
Map read_csv(string filename, bool head = true);
*/
vector<string > ssplit(string s);


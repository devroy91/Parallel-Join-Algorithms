/****************************************
 * reads a csv file and returns a dict  *
 * where each dict contains a row of csv*
 * **************************************/
#define pb push_back
#include<bits/stdc++.h>
using namespace std;
/*
        dict
 _____________________________________________
 |       key1                   key2          |
 | ________________________   ______________  |
 | |       ______________ |   |  ________  |  |
 | |entry1 | idx | value ||   | | entry1 | |  |
 | |       |-------------||   | |--------| |  |
 | |entry2 | idx | value ||   | | entry2 | |  |
 | |       |-------------||   | |--------| |  |
 | |______________________|   |____________|  |
 |____________________________________________|
*/


/*<----defines structure of entry--->*/ 
typedef struct entry{
        int idx;
        string value;
}entry;

typedef map< string, vector< entry> > Map;

/*<-----takes a string of words separated by comma
 *  and splits it into vector of words -------->*/

vector<string> ssplit(string s){
        
        vector< string > words;
        string word = "";
        for(int i = 0; i<s.size();i++){
                if(s[i] == ','){
                        words.pb(word);
                        word = "";
                }
                else{
                        word += s[i];
                }
        
        }
        words.pb(word);
        return words;
}

/*<-----takes a filename and returns a dict------->*/
Map read_csv(string filename, bool head = true){
        Map d;
        ifstream inp(filename);
        string line;
        inp >> line;
        //cout << line << endl;
        vector < string > header = ssplit(line);
        int len = header.size();
        int lineNo = 0;

        while(inp >> line){
                ++lineNo;
                vector < string > values = ssplit(line);
                for(int i = 0; i < len;i++){
                        entry e = {lineNo, values[i]};
                        d[header[i]].pb(e);
                }
        }
        cin.clear();
        inp.clear();
        /*for(auto it = header.begin(); it!= header.end(); it++){
                cout << *it << endl;
        }
        */
        inp.close();
        return d;
}

string getVal(Map d, string key, int id){
        return d[key][id].value;
}

vector < string > getVal(Map d, string key){
        vector< string > val;
        for(int i = 0; i < d[key].size(); i++){
                val.pb(getVal(d, key, i));
        }
        return val;
}

int main(){
    Map d = read_csv("ManagerS.csv");
    vector< string > values = getVal(d, "Name");
    cout << getVal(d, "Name", 0);
    for(int i = 0; i < values.size(); i++){
            cout << values[i] << endl;
    }
}

/****************************************
 * reads a csv file and returns a dict  *
 * where each dict contains a row of csv*
 * **************************************/
#define pb push_back
#include "csvUtils.h"

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
Map read_csv(string filename, int chunk, bool head){
        Map d;
        static int pos = 0;
        ifstream inp(filename);
        string line;
        inp.seekg(pos, ios::beg);
        static vector < string > header;
        static int len;
        if (pos == 0){
            inp >> line;
            //cout << line << endl;
            header = ssplit(line);
            len = header.size();
        }
        static int lineNo = 0;
        int  alreadyRead = 0;
        while(inp >> line && alreadyRead < chunk){
                ++alreadyRead;
                ++lineNo;
                pos = inp.tellg();
                //pos += (line.size() * 8);
                vector < string > values = ssplit(line);
                for(int i = 0; i < len;i++){
                        entry e = {lineNo, values[i]};
                        d[header[i]].pb(e);
                }
        }
        inp.close();
        return d;
}

        

Map read_csv(string filename, bool head){
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
void write_csv(Map d, string filename, int mode, char delim){
        vector< string > keys;
        ofstream out;
        for(auto const& key:d){
                keys.pb(key.first);
        }
        if (mode == 0){
            out.open(filename, ios::out);
        }
        else{
                out.open(filename, ios::app);
        }
        
        int len = d[keys[0]].size();
        for(int i=0; i < len;i++){
                string word = "";
                for(auto const& key:keys){
                        word+=getVal(d, key, i);
                        word+=',';
                }
                word.pop_back();
                word.pb('\n');
                out<<word;
        }
        out.close();
}



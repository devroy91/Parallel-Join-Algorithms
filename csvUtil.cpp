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
Map csvstream::read_csv(int chunk, bool head){
        Map d;
        string line;
	ifstream inp(filename);
        inp.seekg(ipos, ios::beg);
        if (ipos == 0){
            inp >> line;
            //cout << line << endl;
            header = ssplit(line);
            len = header.size();
        }
	int lineNo = 0; 
        int  alreadyRead = 0;
        while(inp >> line && alreadyRead < chunk){
                ++alreadyRead;
                
                ipos = inp.tellg();
                //pos += (line.size() * 8);
                vector < string > values = ssplit(line);
                for(int i = 0; i < len;i++){
                        entry e = {lineNo, values[i]};
                        d[header[i]].pb(e);
                }
		++lineNo;
        }
        inp.close();
        return d;
}
Map csvstream::read_csv(int chunk, bool head, bool contiguous){
        Map d;
        string line;
	ifstream inp(filename);
        inp.seekg(ipos, ios::beg);
        if (ipos == 0){
            inp >> line;
            //cout << line << endl;
            header = ssplit(line);
            len = header.size();
        }
	static int lineNo = 0; 
        int  alreadyRead = 0;
        while(inp >> line && alreadyRead < chunk){
                ++alreadyRead;
                
                ipos = inp.tellg();
                //pos += (line.size() * 8);
                vector < string > values = ssplit(line);
                for(int i = 0; i < len;i++){
                        entry e = {lineNo, values[i]};
                        d[header[i]].pb(e);
                }
		++lineNo;
        }
        inp.close();
        return d;
}
        

Map csvstream::read_csv(vector<string> &s, bool head){
        Map d;
	ipos = 0;
        ifstream inp(filename);
        string line;
        lineNo = 0;
        inp >> line;
        //cout << line << endl;
	s.pb(line);
        header = ssplit(line);
        int len = header.size();
        int lineNo = 0;
        while(inp >> line){
		++lineNo;
                s.pb(line);
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
Map csvstream::read_csv(bool head){
        Map d;
	ipos = 0;
        ifstream inp(filename);
        string line;
        lineNo = 0;
        inp >> line;
        //cout << line << endl;
        header = ssplit(line);
        int len = header.size();
        int lineNo = 0;
        while(inp >> line){
                
                vector < string > values = ssplit(line);
                for(int i = 0; i < len;i++){
                        entry e = {lineNo, values[i]};
                        d[header[i]].pb(e);
                }
		++lineNo;
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
void write_csv(Map d, string filename, Mode m, char delim){
        vector< string > keys;
        ofstream out;
        string str = "";
        for(auto const& key:d){
                keys.pb(key.first);
        }
        if (m == Mode::out){
            out.open(filename, ios::out);
	    for(auto const& k:keys){
		str += k;
		str += ",";
	    }
	    str.pop_back();
	    str.pb('\n');
	    out<<str;
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



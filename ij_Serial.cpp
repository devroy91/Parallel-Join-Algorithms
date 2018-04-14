#include"csvUtils.h"
#include<bits/stdc++.h>
#define pb push_back
using namespace std;
typedef struct ind{
     int idx;
     int value;
}ind;

void process(ind key[], Map &tab, string col){
    key[0].idx = 0;  //for header
    key[0].value = 0;
    int i = 1;
    for(auto it:tab[col]){
	key[i].idx = it.ival;
	key[i].value = i;
	i++;
    }
}
bool comp(ind key1, ind key2){
    return key1.idx < key2.idx;
}
int countSort(ind *key, int start[], int range[], int len){
    int maxValCol = 0;
    for(int i=0;i < len;i++){
	int idx = key[i].idx;
	range[idx] += 1;
        maxValCol = max(maxValCol, idx);
    }
    partial_sum(range, range + len, start);
    return maxValCol;
}
void join(ind *key1, ind *key2, vector<string> &tab1, vector<string> &tab2, int *start, int *range, int lenTab2, string col){
	
    string lout="";
    lout +=tab1[0]+","+tab2[0]+"\n";
    for(int i = 1; i <= lenTab2; i++){
	int val = key2[i].idx;
        int pos = key2[i].value;
	int s = start[val-1];
	int r = start[val];
	for(int j = s; j < r; j++){
            int idx = key1[j].value;
	    lout +=tab1[idx]+","+tab2[pos]+"\n";
	}
    }
    ofstream outp("ij_Serial.csv");
    outp << lout;
}
int main(){
    csvstream tab1("Trainee1.csv"), tab2("ManagerO.csv");
    Map trainee, mgr;
    vector<string> strainee, smgr;

    //read the tables
    trainee = tab1.read_csv(strainee, true);
    mgr = tab2.read_csv(smgr, true);

    //calculate size of tables
    int lenTab1 = strainee.size();
    int lenTab2 = smgr.size();

    //create index
    ind key1[lenTab1];
    ind key2[lenTab2];
    process(key1, trainee, "managerID");    
    process(key2, mgr, "managerID");

    sort(key1, key1+lenTab1, comp);
 
    int start1[lenTab1], range1[lenTab1];
    int start2[lenTab2], range2[lenTab2];
    int maxValCol = countSort(key1, start1, range1, lenTab1);
    countSort(key2, start2, range2, lenTab2);
    
    join(key1, key2, strainee, smgr, start1, range1, lenTab2, "managerID");
    return 0;
}

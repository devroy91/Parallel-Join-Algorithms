#include <bits/stdc++.h>
#include "csvUtils.h"
using namespace std;
#define pb push_back 
vector< string > header1, header2;

void join(vector< int > const& count, Map & a, Map &b, string col){
	vector< entry > valueA = a[col];
	vector< entry > valueB = b[col];
	Map d;
	static mode m = Mode::out;
	int line = 0;
	for( entry const& i:valueA){
		for(entry const& j:valueB){
			if(i.value == j.value){
				++line;
				int idx1 = i.idx;
				int idx2 = j.idx;
				for(auto it:header1){
					if(it != col){
						
						string s = getVal(a, it, idx1);
						entry e = {line, s};
						d["t1"+it].pb(e);
					}
				}
				
				entry e = {line, i.value};
				d[col].pb(e);
				for(auto it:header2){
					if(it!=col){
						string s = getVal(b, it, idx2);
						entry e = {line, s};
						d["t2"+it].pb(e);
					}
				}
			}
		}
	}
	if(!d.empty()){
		write_csv(d, "out.csv", m, ',');
		m = Mode::app;
	}
}

void joinMatches(vector< int > const& count, string file1, string file2, string col){
	csvstream tinp(file1);
	//read from file1 in chunks
	do{
		Map tab1 = tinp.read_csv(10, true);

		if(tab1 .empty())
			break;
		
		//read from file2 in chunks
		csvstream minp(file2);
		
		do{	
			Map tab2 = minp.read_csv(5, true);
			if(tab2 .empty())
				break;
			//counting sort to find positions in output array
			join(count, tab1, tab2, col);
		}while(true);
	}while(true);


}
int countSort(Map a, Map b, string col){
	int count = 0;
	vector< entry > valueA = a[col];
	vector< entry > valueB = b[col];
	for( entry const& i:valueA){
		for(entry const& j:valueB){
			if(i.value == j.value)
				++count;
		}
	}
	return count;
}

void countMatches(vector< int > &count, string file1, string file2, string col){
	csvstream tinp(file1);
	count.pb(0);
	bool collect = false; 
	//read from file1 in chunks
	do{
		Map tab1 = tinp.read_csv(10, true);
		if(tab1 .empty())
			break;
		
		//read from file2 in chunks
		csvstream minp(file2);
		
		do{
			Map tab2 = minp.read_csv(5, true);
			if(tab2 .empty())
				break;
			//counting sort to find positions in output array
			int res = countSort(tab1, tab2, col);
			count.pb(res);
		}while(true);
		if(!collect){
			header2 = minp.header;
			collect = true;
		}

	}while(true);
	header1 = tinp.header;
	

}
int main(){
	Map trainee, mgr;
	vector< int > count;
	//counting sort
	countMatches(count, "Trainees.csv", "ManagerO.csv", "managerID");
	//prefix sum of count
	partial_sum(count.begin(), count.end(), count.begin());
	//actual join operation
	joinMatches(count, "Trainees.csv", "ManagerO.csv", "managerID");
}

#include"csvUtils.h"
#include<bits/stdc++.h>
#include<thrust/sort.h>
#include<thrust/host_vector.h>
#include<thrust/device_vector.h>
#include<thrust/execution_policy.h>
#define pb push_back
#include<stdio.h>
#include<thrust/binary_search.h>
#include<thrust/sequence.h>

using namespace std;

void process(int key[], Map &tab, string col){
    key[0] = 0;  //for header
    
    int i = 1;
    for(auto it:tab[col]){
        
	key[i] = it.ival;
	i++;
    }
}

__global__
void countSort(int *key, int *start, int len){
    int idx = blockDim.x*blockIdx.x + threadIdx.x;
    if(idx < len){
	int index = key[idx];
        atomicAdd(&start[index], 1);
    }
    
}
void join(int *key1, int *key2, string tab1[], vector<string> &tab2, thrust::host_vector<int>start, int lenTab2, string col){
	
    string lout="";
    lout +=tab1[0]+","+tab2[0]+"\n";
    for(int i = 1; i < lenTab2; i++){
	int val = key2[i];
        int pos = i;
	int s = start[val-1];
	int r = start[val];
	for(int j = s; j < r; j++){
            
	    lout +=tab1[j]+","+tab2[pos]+"\n";
	}
    }
    ofstream outp("ij_Parallel.csv");
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
    int key1[lenTab1];
    int key2[lenTab2];
    string strain[lenTab1];
    
    std::copy(strainee.begin(), strainee.end(), strain);
    
    process(key1, trainee, "managerID");    
    process(key2, mgr, "managerID");
    
    thrust::sort_by_key(key1, key1+lenTab1, (char**)strain);
   
    int maxVal = key1[lenTab1 - 1];
    thrust::host_vector<int> values(maxVal + 1);
    thrust::sequence(values.begin(), values.end());
    thrust::host_vector<int> hist(maxVal + 1);
    thrust::upper_bound(key1, key1 + lenTab1, values.begin(), values.end(), hist.begin());
    
    join(key1, key2, strain, smgr, hist, lenTab2, "managerID");

    return 0;
}

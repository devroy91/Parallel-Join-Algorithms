#include <bits/stdc++.h>
#include "csvUtils.h"
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/scan.h>
#include <stdio.h>
using namespace std;
#define pb push_back 
vector< string > header1, header2;
#define CHUNK_SIZE 1000

//entry structure for cuda
typedef struct cudaEntry{
     int idx;
     int value;
}cudaEntry;


Map read_file(string file1, vector<string> &s, vector<string> &header){
	csvstream tinp(file1);
	
	Map tab1 = tinp.read_csv(s, true);
		
	header = tinp.header;
	return tab1;	

}
thrust::host_vector< cudaEntry > process(Map a, string col){
	
	thrust::host_vector< cudaEntry > intCol;
	for(auto row:a[col]){
		cudaEntry e = {row.idx, row.ival};
		intCol.pb(e);
	}
		
	return intCol;
}
		
__global__
void join(cudaEntry *tab1, cudaEntry *tab2, cudaEntry *out, int *count, int lenTab1, int lenTab2){
	int tx = threadIdx.x;
	int bx = blockIdx.x;
	int bd = blockDim.x;
	int idx = bx * bd + tx;
	if(idx < lenTab1){
	    int matches = 0;
	    cudaEntry e1 = *(tab1 + idx);
	    int val1 = e1.value;
	    for(int i = 0; i < lenTab2; i++){
		cudaEntry e2 = *(tab2 + i);
		int val2 = e2.value;
		if(val1 == val2){
		    cudaEntry *temp = out+idx+matches;
		    temp->idx = e1.idx;
		    temp->value = e2.idx;
		    ++matches;
		}
	    }
	}
}

__global__
void countSort(cudaEntry *tab1, cudaEntry *tab2, int *count, int lenTab1, int lenTab2){
	int tx = threadIdx.x;
	int bx = blockIdx.x;
	int bd = blockDim.x;
	int idx = bx * bd + tx;
	if(idx < lenTab1){
	    int countVal = 0;
	    cudaEntry e1 = *(tab1 + idx);
	    int val1 = e1.value;
	    for(int i = 0; i < lenTab2; i++){
		cudaEntry e2 = *(tab2 + i);
		int val2 = e2.value;
		if(val1 == val2){
		    countVal++;
		}
	    }
	    count[idx] = countVal;
	}
}
void write_file(string filename, cudaEntry *out, vector<string> &trainee, vector<string> &mgr, int totCount, string col){
	Map d;
	ofstream outp(filename);
	string lout = trainee[0];
	lout.pb(',');
	lout+=mgr[0]+'\n';
	for(int i = 0;i<totCount;i++){
		int idx1 = out[i].idx;
		lout+=trainee[idx1];
		lout.pb(',');	
		
		int idx2 = out[i].value;
		lout +=mgr[idx2]+'\n';
	}
	outp<<lout<<endl;
	outp.close();

}

int main(){
	vector<string> trainee, mgr;
	Map h_tab1, h_tab2;
	h_tab1 = read_file("Trainee1.csv", trainee, header1);
	h_tab2 = read_file("ManagerO.csv", mgr, header2);
	thrust::host_vector< cudaEntry > h_col1, h_col2;
	h_col1 = process(h_tab1, "managerID");
	h_col2 = process(h_tab2, "managerID");
	
	int lenTab1 = h_col1.size();
	int lenTab2 = h_col2.size();
	thrust::device_vector< cudaEntry > d_col1, d_col2;
	d_col1 = h_col1;
	d_col2 = h_col2;
	cudaEntry *pd_col1 = thrust::raw_pointer_cast(d_col1.data());
	cudaEntry *pd_col2 = thrust::raw_pointer_cast(d_col2.data());
	dim3 gridDim = (lenTab1 - 1)/CHUNK_SIZE + 1;
	int *d_count, *h_count;
	cudaMalloc((void**)&d_count, lenTab1 * sizeof(int));
	h_count = new int[lenTab1];	

	countSort<<< gridDim, CHUNK_SIZE >>>(pd_col1, pd_col2, d_count, lenTab1,  lenTab2);
	cudaMemcpy(h_count, d_count, sizeof(int) * lenTab1, cudaMemcpyDeviceToHost);
	int lastThreadCount = h_count[lenTab1 - 1];
	//prefix sum
	thrust::exclusive_scan(h_count, h_count+lenTab1, h_count);
	cudaMemcpy(d_count, h_count, sizeof(int) * lenTab1, cudaMemcpyHostToDevice);
	int totCount = lastThreadCount + h_count[lenTab1 - 1];
	cudaEntry *d_out, *h_out;
	cudaMalloc((void **)&d_out, sizeof(cudaEntry) * totCount);
	h_out = new cudaEntry[totCount]; 
	join<<< gridDim, CHUNK_SIZE >>>(pd_col1, pd_col2, d_out, d_count, lenTab1,  lenTab2);
	cudaMemcpy(h_out, d_out, sizeof(cudaEntry) * totCount, cudaMemcpyDeviceToHost);

	write_file("nij_Parallel.csv", h_out, trainee, mgr, totCount, "managerID");	
		
	cudaFree(d_out);
	cudaFree(d_count);
}

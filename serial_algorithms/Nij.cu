# include "csv.h"
#include<iostream>
#include<cuda.h>

using namespace std;

int main(){

  io::CSVReader<3> in1("Trainees.csv");
  in1.read_header(io::ignore_extra_column, "empID", "Name", "managerID");

  string name;
  int empid; 
  int managerid;

  vector<string>table1_name;
  vector<int>table1_empid;
  vector<int>table1_managerid;
 
  while(in1.read_row(empid,name,managerid)){
  	table1_empid.push_back(empid);
  	table1_name.push_back(name);
  	table1_managerid.push_back(managerid);
  }

  io::CSVReader<2> in2("Manager.csv");
  in2.read_header(io::ignore_extra_column, "managerID" , "Name");

  vector<string>table2_name;
  vector<int>table2_managerid;
 
  while(in2.read_row(managerid,name)){
  	table2_managerid.push_back(managerid);
  	table2_name.push_back(name);
  }

  return 0;
}
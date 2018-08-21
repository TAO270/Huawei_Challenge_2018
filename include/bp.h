#ifndef BP_H
#define BP_H
#include "lib_io.h"
#include <stdio.h>
#include<iostream>
#include<string>   
#include<fstream>  
#include<sstream>  

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <sys/timeb.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <vector>  

#include <string.h> 
#include <cmath>

#include <cstdlib>
#include <random>
using namespace std;

#define Layer    3      
#define MaxNode     25    

#define A        2.5  
#define B        4    
#define ITERS    1000     

#define ETA_W    0.40  
#define ETA_B    0.40   
#define ERROR    0.01    
#define ACCU     1e-6      
#define mom    0.35   

const int discard_day=360;

struct Data
{
	vector<double> x;       
	vector<double> y;      
};

class BP {

private:

	void InitNet();           
	void InitParameter();             
	void ForwardPropagation();      
    void BackwardPropagation(int cnt);
	void ComputeDelta(int);        
	void UpdateNet();      
	double GetError();             
	double Sigmoid(const double);   
	
private:
	int input_node;               
	int out_node;                   
	int hidden_node;                 
	double acc_last=0;
	vector<Data> data;           

	double w[Layer][MaxNode][MaxNode];     
	double b[Layer][MaxNode];          

	double x[Layer][MaxNode];        
	double d[Layer][MaxNode];         
    double last_d[Layer][MaxNode][MaxNode];  
    double last_d_b[Layer][MaxNode];       
public:
	void GetData(const vector<Data>  _data);
	void Train();
	vector<double> Predicting(const vector<double>);    
};

#endif

#include "predict.h"
#include <stdio.h>

#include<iostream>
#include<string>   
#include<fstream>  
#include<sstream>  
#include <numeric> 
#include<algorithm>
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
#include<memory>


/**********数据处理参数*******/
#define MAX_DATA_NUM    50000 
#define MAX_LINE_LEN 55000
#define MAX_INFO_NUM    50 
#define Individual_num  500//种群初始值

using namespace std;

//你要完成的功能总入口
void predict_server(char * info[MAX_INFO_NUM], char * data[MAX_DATA_NUM], int data_line_num,int info_line_num, char * filename)
{
    const double StartTime=clock();
    int N=computedaynum(data,data_line_num);
    if(N>discard_day) N=discard_day;
    
    int **ReadResult=readdata(data,data_line_num,N);
    
    INFO input_info;
     readinfo(info,info_line_num,input_info);
   
    //obtain information from input.txt
    const int gap=input_info.gap;
    const int cpu_num=input_info.CPU_number;
    const int mem_num=input_info.MEM_number;
    const string cpu_or_mem=input_info.CPUorMEM;
    vector<int> ForecastFlavor=input_info.ForecastFlavor;
    
    //create training data
    int rows=int(floor(N/gap));
    //use last 3 weeks to predict next week
    
    
	int week_num=4; 
	if(week_num>MaxNode)
        week_num=MaxNode;
   
	int **reshape_data;
    reshape_data=(int **) new int*[rows];
    for(int i=0;i<rows;i++)
        reshape_data[i]=new int[15];
    int **data_max;
    data_max=(int **) new int*[rows];
    for(int i=0;i<rows;i++)
        data_max[i]=new int[15];
    
    int **data_zeros;
    data_zeros=(int **) new int*[rows];
    for(int i=0;i<rows;i++)
        data_zeros[i]=new int[15];

    //record min and max in data	
	int min[15],max[15];
    NewReshapedata(ReadResult,reshape_data,data_max,data_zeros, gap,N,min,max);
/***********predicting***************/
    int PredictionResultsFlavor[15]={0};
    
    shared_ptr<BP> bp =shared_ptr<BP> (new BP());
    
for(size_t i=0;i<ForecastFlavor.size();i++)
{
    int flavor=ForecastFlavor[i]-1;
    vector<Data> _data_=GetNewTrainData( reshape_data,data_max,data_zeros,flavor,week_num,rows,max);
    
    bp->GetData(_data_);
    bp->Train();
    
    //ForeCast
    vector<double> data_for_predict;
    double data_normalize;
	for(int id=0;id<week_num;id++)
	{
	  data_normalize=double(reshape_data[rows-week_num+id][flavor])/double(max[flavor]);
	  data_for_predict.push_back(data_normalize);
      
      data_normalize=double(data_max[rows-week_num+id][flavor])/double(max[flavor]);
	  data_for_predict.push_back(data_normalize);
      
        data_normalize=double(data_zeros[rows-week_num+id][flavor])/double(max[flavor]);
	  data_for_predict.push_back(data_normalize);
	}
	vector<double> predict_next_week;
	predict_next_week=bp->Predicting(data_for_predict);
	double predict_value=predict_next_week[0];
    
	predict_value=predict_value*double(max[flavor]);
    PredictionResultsFlavor[flavor]=int(predict_value+0.5);
  //reduce to fitness
  //if(flavor>10) 
    //  PredictionResultsFlavor[flavor]=(PredictionResultsFlavor[flavor]-1)?PredictionResultsFlavor[flavor]-1:0;
}

/***********Place***************************************************************/
vector<box>GA=GAPacking(PredictionResultsFlavor,80000,1000,10,Individual_num,cpu_num,mem_num,StartTime);
vector<box> Bestfit=binpack( cpu_or_mem,cpu_num,mem_num,PredictionResultsFlavor);

//check packing 
   bool GA_ok= CheckPacking(PredictionResultsFlavor,Bestfit,cpu_or_mem,cpu_num,mem_num);
   
  vector<box> BoxOpt; 
   if(GA_ok&&(GA.size()<Bestfit.size()))
   {BoxOpt=GA;cout<<"Using GAPacking algorithm."<<endl;}
  else
 {BoxOpt=Bestfit;cout<<"Using binPacking algorithm."<<endl;}

/*********print to result.txt*********************************************************/	
    string temp=print2string(PredictionResultsFlavor,ForecastFlavor,BoxOpt);
        cout<<temp;
	const char *result_file=temp.c_str();
	write_result(result_file, filename);
/****************delete  space*********/

    for(int i=0;i<rows;i++)
       delete [] reshape_data[i];
    delete [] reshape_data;

}

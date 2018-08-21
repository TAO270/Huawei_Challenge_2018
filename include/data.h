#ifndef DATA_H
#define DATA_H
#include <stdio.h>
#include"bp.h"

//new adding
#include<iostream>
#include<string>   //字符串头文件
#include<fstream>  //文件流操作头文件
#include<sstream>  //串流操作头文件
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

using namespace std;

struct INFO
{
    string CPUorMEM;
    int CPU_number;
    int MEM_number;
    int gap;
   vector<int> ForecastFlavor;
};

bool isLeap(int year);
bool string2date(string date, int& year, int& month, int& day);

int DayInYear(int year, int month, int day);
int DaysGap(string date1, string date2);

int computedaynum(char **const data,int const data_line_num);
int **readdata( char ** const data, int const data_line_num,int const N );
//get new data accordong to gap
void reshapedata(int **const ReadResult,int **const reshape_data,const int gap,int const N,int *min,int *max);
void readinfo(char * info[],const int info_line_num, INFO &input_data);
vector<Data> GetTraindata(int ** const reshape_data, int const flavor,const int week_num,const int rows,int *min,int *max);
void NewReshapedata(int **const ReadResult,int  **const reshape_data1, int  **const reshape_data2, int  **const reshape_data3, 
	const int gap,int const N,int *min,int *max);
vector<Data> GetNewTrainData(int ** const reshape_data, int ** const data_max,int ** const data_zeros,int const flavor,const int week_num,const int rows,int *max);

#endif

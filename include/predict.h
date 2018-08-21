#ifndef __ROUTE_H__
#define __ROUTE_H__

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
#define Vector std::vector
#include <string.h> 
#include <cmath>

#include <cstdlib>
using namespace std;

#include"data.h"
#include"bp.h"
#include"packing.h"
#include"print2string.h"


void predict_server(char * info[MAX_INFO_NUM], char * data[MAX_DATA_NUM], int data_line_num,int info_line_num,char * filename);
	

#endif

#ifndef PACKING_H
#define PACKING_H
#include<string>   //字符串头文件
#include<fstream>  //文件流操作头文件
#include<sstream>  //串流操作头文件
#include <numeric> 
#include<algorithm>
#include<iostream>
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
//#include <math.h>
#include <cstdlib>

using namespace std;

struct FlavorType                    
{
	int x;   //CPU number
	int y;   //MEM number
	int z;   //flavor type
};


struct box
{
	int x;//place cpu
	int y;//uints MB not G MEM
	int NumberOfVM[15] = { 0 };         //15位置代表15种类型，数值代表该种类型的数量，初始化为0
};
//new adding 
struct newFlavorType                  
{
	int x = 0;   //CPU数量
	int y = 0;   //MEM数量
	int z = 0;   //第多少种类型
	int ID = 0;  //序号
};
struct newbox
{
	int x = 0;          //单个物理机里已经使用的CPU
	int y = 0;          //单个物理机里已经使用的MEM
	vector <int> ThisboxVMID;         // 谁被放到这个箱子，拿就记录它的ID，
	int xratio = 0;
	int yratio = 0;
};
struct  Population
{
	vector<int> OrderofID;             //乱序ID   
	vector <newbox> Box;
	double Fitness;               //适应度，计算法则为箱子个数的倒数
};

vector<Population> PrimaryProduction(const int PredictionResultsFlavor[], const int individual_num, const int CPU, const int MEM) ;
void cross_over(vector <Population> &population, int n1, int n2, int m1, int m2,
	const int PredictionResultsFlavor[], const int CPU, const int MEM);
void mutation(const int  PredictionResultsFlavor[], Population &population, int n, vector <newFlavorType> _flavortype_, const int CPU, const int MEM);
void Choice(vector <Population> &population);
vector<box> MutationPacking(const int PredictionResultsFlavor[], const int NumOfMuta, vector <Population>population, int NumOFInitialpop, const int CPU, const int MEM);
vector<box> binpack(const string inputAimCPUOrMEM, const int inputPhysicalMachineCPU, const int inputPhysicalMachineMEM, const int PredictionResultsFlavor[]);
vector<box> GAPacking(const int PredictionResultsFlavor[], const int NumOfEvolution,
	                  const int NumOfmeta, const int NumOfcross, int NumOFInitialpop, const int CPU, const int MEM,const double StartTime);
bool CheckPacking(const int PredictionResultsFlavor[],const vector<box> BoxOpt,string cpu_or_mem,const int cpu_num,const int mem_num);
#endif

#include"data.h"

bool isLeap(int year)
{
   return (year % 4 ==0 || year % 400 ==0) && (year % 100 !=0);
}

bool string2date(string date, int& year, int& month, int& day)
{
    year = atoi((date.substr(0,4)).c_str());
    month = atoi((date.substr(5,2)).c_str());
    day = atoi((date.substr(8,2)).c_str());
   
    int DAY[12]={31,28,31,30,31,30,31,31,30,31,30,31};
    if(isLeap(year)){
         DAY[1] = 29;
    }
    return year >= 0 && month<=12 && month>0 && day<=DAY[month-1] && day>0;
}
 
int DayInYear(int year, int month, int day)
{
    //int _day = 0;
    int DAY[12]={31,28,31,30,31,30,31,31,30,31,30,31};
    if(isLeap(year))
        DAY[1] = 29;
    for(int i=0; i<month - 1; ++i)
    {
        day += DAY[i];
    }
    return day;
}
 
int DaysGap(string date1, string date2)
{
 
     int year1, month1, day1;
     int year2, month2, day2;
     if(!string2date(date1, year1, month1, day1) || !string2date(date2, year2,month2,day2))
     {
        cout<<"date format is error"<<endl;
        return -1;
     }
     if(year1 == year2 && month1 == month2)
     {
          return day1 > day2 ? day1 - day2 : day2 - day1;
      

     }else if(year1 == year2)
     {
         int d1, d2;
         d1 = DayInYear(year1, month1, day1);
         d2 = DayInYear(year2, month2, day2);
         return d1 > d2 ? d1 - d2 : d2 - d1;
      

     }else{
         
         if(year1 > year2)
         {
             
             swap(year1, year2);
             swap(month1, month2);
             swap(day1, day2);
         }
         int d1,d2,d3;
         if(isLeap(year1))
             d1 = 366 - DayInYear(year1,month1, day1); 
         else
             d1 = 365 - DayInYear(year1,month1, day1);
         d2 = DayInYear(year2,month2,day2); 
         cout<<"d1:"<<d1<<", d2:"<<d2;
          
         d3 = 0;
         for(int year = year1 + 1; year < year2; year++)
         {
             if(isLeap(year))
                 d3 += 366;
             else
                 d3 += 365;
         }
         return d1 + d2 + d3;
     }
}

int computedaynum(char **const data,int const data_line_num)
{
	istringstream line_first(data[0]);
	istringstream line_last(data[data_line_num-1]);
	int N=0;
	for(int i=1;i<4; i++)
	    {
	      string starttime,endtime;
	      line_first>>starttime;
	      line_last>>endtime;
	      if(i==3){
	      
		if(endtime.empty()||starttime.empty())
		{ 
		  cout<<"error: nothing in last line or first line"<<endl;
		  return -1;
		}
	    //    cout<<starttime<<" "<<endtime<<endl;
		N=DaysGap(starttime,endtime);
		N=N+1;
	      //  cout<<N<<endl;
		if(N==0)
		{
		  cout<<"error: N=0"<<endl;
		  return -1;
		}
		break;
	      }
	    }
      return N;
}

int **readdata( char ** const data, int const data_line_num,int const N )
{
#define M 15
	int ** ReadResult=0;    

	ReadResult=(int **)malloc(sizeof(int *) *N);
	for(int i = 0; i < N; i ++)
	  ReadResult[i] = (int *)malloc(sizeof(int)*M);

	for (int i = 0; i < N; i++)   
	{
		for (int j = 0; j < M; j++)
			ReadResult[i][j] = 0;
	}     
    	istringstream line_frist(data[0]);
        string starttime;
    for(int i=1;i<4;i++)
    {
        line_frist>>starttime;
        if(i==1||i==2)
            starttime.clear();
    }
	string nowtime;
	string s1, s2;
	for (int k = 0; k < data_line_num; k++)
	{
		istringstream is(data[k]);
		int m =0;               
		for (int i = 1; i <= 3; i++)
		{
			is >> s1;
			if (i == 2)
			{
				s2.assign(s1, 6, s1.size() - 6);
				m = atoi(s2.c_str());
				if (m > 15)
					break;
			}
			if (i == 3)
			{
				nowtime=s1;
                int id=DaysGap(starttime,nowtime);
				ReadResult[id][m - 1]++;   
				
			}
		}
	} 
	return ReadResult;
}

void NewReshapedata(int **const ReadResult,int  **const reshape_data1, int  **const reshape_data2, int  **const reshape_data3, 
	const int gap,int const N,int *min,int *max)
{
    	int rows=int(floor(N/gap));
     for(int i=0;i<15;i++)
	{
	  min[i]=999;
	  max[i]=0;
	}
	
	for(int i=0;i<15;i++)
	{	  
	  int row=rows;
	  
	  for(int j=N-1;j>=0;j=j-gap)   
	  {
	      if(row>=1)
	      {
	      int sum=0;
		  int MaxOfEveryGap = 0;       //临时记录每个gap里最大值
		  int NumberOfZeroEveryGap=0;   //临时记录每个gap里0的数量
	      for(int k=0;k<gap;k++)
		  {
			  if (MaxOfEveryGap < ReadResult[j - k][i])   
			  {
				  MaxOfEveryGap = ReadResult[j - k][i];
			  }
			  if (ReadResult[j - k][i] == 0)
			  {
				  NumberOfZeroEveryGap++;
			  }
			  sum=sum+ReadResult[j-k][i];	      
		  }
	      reshape_data1[row - 1][i] = sum;                   //记录每个gap里总的虚拟机数量
		  reshape_data2[row - 1][i] = MaxOfEveryGap;         //记录每个gap里最大的虚拟机数量 
		  reshape_data3[row - 1][i] = NumberOfZeroEveryGap;  //记录每个gap里虚拟机数量为0的天数
	      //判断最大值和最小值
	      if(sum>max[i])
		max[i]=sum;
	      if(sum<min[i])
		min[i]=sum;
	      row=row-1;
	      }	    
	  }
	}
}


void reshapedata(int **const ReadResult,int  **const reshape_data,const int gap,int const N,int *min,int *max)
{
    	int rows=int(floor(N/gap));
     for(int i=0;i<15;i++)
	{
	  min[i]=999;
	  max[i]=0;
	}
	
	for(int i=0;i<15;i++)
	{	  
	  int row=rows;
	  
	  for(int j=N-1;j>=0;j=j-gap)
	  {
	      if(row>=1)
	      {
	      int sum=0;
	      for(int k=0;k<gap;k++)
		{
		sum=sum+ReadResult[j-k][i];	      
		}
	      reshape_data[row-1][i]=sum;
	      
	      if(sum>max[i])
		max[i]=sum;
	      if(sum<min[i])
		min[i]=sum;
	      row=row-1;
	      }	    
	  }
	}

}

void readinfo(char * info[],const int info_line_num, INFO &input_data)
{
    string temps1, temps2, temps3;
    istringstream inputPhysicalMachine(info[0]);              
    //CPU_number
    inputPhysicalMachine >> temps1;
    input_data.CPU_number=atoi(temps1.c_str()); temps1.clear();
    //MEM_number
    inputPhysicalMachine >> temps1;
    input_data.MEM_number=atoi(temps1.c_str()); temps1.clear();
    
    //输入文件第info_line_num-4行一定是需要优化的量（CPU/MEM）
    istringstream inputCPUOrMEM(info[info_line_num-4]); 
     inputCPUOrMEM >> temps1;
	 input_data.CPUorMEM= temps1;temps1.clear();
     
       //输入文件倒数第二行一定是预测开始的时间
    istringstream inputBeginDay(info[info_line_num - 2]);    
    inputBeginDay >> temps1;
	string pre_starttime=temps1;temps1.clear();
    
    istringstream inputEndDay(info[info_line_num - 1]);         
	inputEndDay >> temps1;
    string pre_endtime=temps1;temps1.clear();
   input_data.gap=DaysGap(pre_starttime,pre_endtime)+1;
    //需要预测的虚拟机种类
   for (int i = 3; i < info_line_num-5; i++)                     
	{
		int templeixing = 0;
		istringstream inputflavor(info[i]);
		inputflavor >> temps1;
		temps2.assign(temps1, 6, temps1.size() - 6); 
		templeixing = atoi(temps2.c_str()); temps1.clear(); temps2.clear();
		input_data.ForecastFlavor.push_back(templeixing);
	}
   
}
vector<Data> GetTraindata(int ** const reshape_data, int const flavor,const int week_num,const int rows,int *min,int *max)
{
    vector<Data> _data_;
    _data_.reserve(rows-week_num);
    double data_normalize;
	for(int i=0;i<rows-week_num;i++)
	{
	  Data tempstructdata ;
	  for(int j=0;j<week_num;j++)
	  {
	     //没有考虑所有值均为零，max=min=0 ,归一化失败 (x-min)/(max-min)
	  data_normalize=double(reshape_data[i+j][flavor]-min[flavor])/double(max[flavor]-min[flavor]);
	  tempstructdata.x.push_back(data_normalize);

	  }
	  //normalize
	  data_normalize=double(reshape_data[i+week_num][flavor]-min[flavor])/double(max[flavor]-min[flavor]);
	  tempstructdata.y.push_back(data_normalize);
	  _data_.push_back(tempstructdata);	  
	}
    return _data_;
}
vector<Data> traindata(int **const ReadResult,int const flavor,const int N,int &days,int &max,int &min)
{
    
    max=0;
    min=999;
    for(int i=0;i<N;i++)
    {
        if(ReadResult[i][flavor]<min) 
           min=ReadResult[i][flavor];
        if(ReadResult[i][flavor]>max)
            max=ReadResult[i][flavor];
    }    
   
    vector<Data> _data_;

    if(N-1<days)
        days=N-2;
    
     _data_.reserve(N-days);
    double normalize=0;
    for(int i=0;i<N-days;i++)
    {
         Data tempdata;
     
        for(int j=i;j<i+days;j++)
        {
            normalize=double(ReadResult[j][flavor]-min)/double(max-min);
            tempdata.x.push_back(normalize);
        }
        normalize=double(ReadResult[i+days][flavor]-min)/double(max-min);
        tempdata.y.push_back(normalize);
         _data_.push_back(tempdata);
    }
    
    return _data_;
}
vector<Data> GetNewTrainData(int ** const reshape_data, int ** const data_max,int ** const data_zeros,int const flavor,const int week_num,const int rows,int *max)
{
    vector<Data> _data_;
    _data_.reserve(rows-week_num);
    double data_normalize;
	for(int i=0;i<rows-week_num;i++)
	{
	  Data tempstructdata ;
	  for(int j=0;j<week_num;j++)
	  {
	     //没有考虑所有值均为零，max=min=0 ,归一化失败 (x-min)/(max-min)
	  data_normalize=double(reshape_data[i+j][flavor])/double(max[flavor]);
	  tempstructdata.x.push_back(data_normalize);
      //Max
       data_normalize=double(data_max[i+j][flavor])/double(max[flavor]);
	  tempstructdata.x.push_back(data_normalize);
      //data_zeros
    data_normalize=double(data_zeros[i+j][flavor])/double(max[flavor]);
	  tempstructdata.x.push_back(data_normalize);
	  }
	  //normalize
	  data_normalize=double(reshape_data[i+week_num][flavor])/double(max[flavor]);
	  tempstructdata.y.push_back(data_normalize);
	  _data_.push_back(tempstructdata);	  
	}
    return _data_;
}




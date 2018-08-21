#include"print2string.h"

string int2str(int num)
{
  stringstream ss;
  string temp;
  ss<<num;
  ss>>temp;
  return temp;
}
string print2string(const int PredictionResultsFlavor[],const vector<int> ForecastFlavor,const vector<box> BoxOpt)
{
    int vm_sum=0;
	string temp;
	
	for(int i=0;i<15;i++)
	{
	      vm_sum=vm_sum+PredictionResultsFlavor[i];
	}
	//cout<<vm_sum<<endl;
	temp=int2str(vm_sum);
	//write flavor
    
for(size_t i=0;i<ForecastFlavor.size();i++)
{
  int flavor=ForecastFlavor[i];
  temp=temp+"\n"+"flavor";
   string tt;
   tt=int2str(flavor);
    temp=temp+tt+" ";
    
    int value=0;
    value=PredictionResultsFlavor[flavor-1];

    tt=int2str(value);
    temp=temp+tt;
}	
//place print the number of VM
{
	temp=temp+"\n"+"\n";
	string ww;
	ww=int2str(BoxOpt.size());
	temp=temp+ww+"\n";
}	
// print detailed place	
	for(size_t i=0;i<BoxOpt.size();i++)
	{
	  
	  temp=temp+int2str(i+1)+" ";
	  for(size_t j=0;j<ForecastFlavor.size();j++)
	  {
	  int flavor=ForecastFlavor[j];
	  int number=BoxOpt.at(i).NumberOfVM[flavor-1];
	  if(number==0)
	    continue;
	  temp=temp+"flavor"+int2str(flavor)+' '+int2str(number)+" ";
	  }	
	  temp=temp+"\n";
	}
	
	return temp;
}

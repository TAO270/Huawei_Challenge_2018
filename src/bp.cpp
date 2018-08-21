#include"bp.h"

void BP::GetData(const vector<Data> _data)
{
	data = _data;
}

void BP::Train()
{
	InitParameter();
	InitNet();
	int num = data.size();   
	
	for (int iter = 0; iter <= ITERS; iter++)              
	{
		for (int cnt = 0; cnt < num; cnt++)
		{
			
			for (int i = 0; i < input_node; i++)
				x[0][i] = data.at(cnt).x[i];
				ForwardPropagation();
				BackwardPropagation(cnt);
		}
		double accu = GetError();           
		if(std::abs(acc_last-accu)<ACCU) break;
		acc_last=accu;
	}
	printf("All Samples Accuracy is %lf\n", acc_last);
}

vector<double> BP::Predicting(const vector<double> data)
{
	int n = data.size();
	assert(n == input_node);
	for (int i = 0; i < input_node; i++)
		x[0][i] = data[i];

	ForwardPropagation();
	vector<double> v;
	for (int i = 0; i < out_node; i++)
		v.push_back(x[2][i]);
	return v;
}

  
void BP::InitParameter()
{
	input_node= data[0].x.size();                        
	out_node = data[0].y.size();                       
	hidden_node = (int)sqrt((input_node+ out_node) * 1.0) + 14;   
	if (hidden_node > MaxNode) hidden_node = MaxNode;                    
}


void BP::InitNet()
{
//set 0  
	memset(last_d,0,sizeof(last_d));
	memset(last_d_b,0,sizeof(last_d_b));
    memset(x,0,sizeof(x));
    memset(d,0,sizeof(d));
    //random mormal
    default_random_engine g;
    normal_distribution<double> distribution(0.0,1/sqrt(input_node));
for(int i=0; i<Layer; i++)
	for(int j=0;j<MaxNode;j++)
	for(int k=0;k<MaxNode;k++)
        	w[i][j][k] =  distribution(g);  //normal_distribution
	for(int i=0; i<Layer; i++)
	for(int j=0;j<MaxNode;j++)
        	b[i][j] = distribution(g);  //normal_distribution
}

void BP::ForwardPropagation()
{
	
	for (int j = 0; j < hidden_node; j++)
	{
		double t = 0;
		for (int i = 0; i < input_node; i++)
			t += w[1][i][j] * x[0][i];
		t += b[1][j];
		x[1][j] = Sigmoid(t);
	}

	for (int j = 0; j < out_node; j++)
	{
		double t = 0;
		for (int i = 0; i < hidden_node; i++)
			t += w[2][i][j] * x[1][i];
		t += b[2][j];
		x[2][j] = Sigmoid(t);
	}
}

void BP::BackwardPropagation(int cnt)
{
	ComputeDelta(cnt);
	UpdateNet();
}

double BP::GetError()
{
	double ans = 0;
	int num = data.size();
	for (int i = 0; i < num; i++)
	{
		int m = data.at(i).x.size();

		for (int j = 0; j < m; j++)
			x[0][j] = data.at(i).x[j];
		ForwardPropagation();
		int n = data.at(i).y.size(); 
		for (int j = 0; j < n; j++)
			ans += 0.5 * (x[2][j] - data.at(i).y[j]) * (x[2][j] - data.at(i).y[j]);
	}
	return ans / num;
}


void BP::ComputeDelta(int cnt)
{
	
	for (int i = 0; i < out_node; i++)
		d[2][i] = (x[2][i] - data.at(cnt).y[i]) * x[2][i] * (A - x[2][i]) / (A * B);
	
	for (int i = 0; i < hidden_node; i++)
	{
		double t = 0;
		for (int j = 0; j < out_node; j++)
			t += w[2][i][j] * d[2][j];
		d[1][i] = t * x[1][i] * (A - x[1][i]) / (A * B);
	}
}


void BP::UpdateNet()
{
	
	for (int i = 0; i < hidden_node; i++)
	{
		for (int j = 0; j < out_node; j++)
		{
		  w[2][i][j] -= ETA_W * d[2][j] * x[1][i];
	
		w[2][i][j]+=mom*last_d[2][i][j];
		last_d[2][i][j]=-ETA_W * d[2][j] * x[1][i];
		}
	}
	for (int i = 0; i < out_node; i++)
	{
	  b[2][i] -= ETA_B * d[2][i];
	  b[2][i] +=mom*last_d_b[2][i];
	  last_d_b[2][i]=-ETA_B * d[2][i];
	}
	 
	for (int i = 0; i < input_node; i++)
	{
		for (int j = 0; j < hidden_node; j++)
		{
		  w[1][i][j] -= ETA_W * d[1][j] * x[0][i];
		  w[1][i][j] +=mom*last_d[1][i][j];
		  last_d[1][i][j]=-ETA_W * d[1][j] * x[0][i];
		}
	}
	for (int i = 0; i < hidden_node; i++)
	{
	  b[1][i] -= ETA_B * d[1][i];
	  b[1][i] +=mom*last_d_b[1][i];
	   last_d_b[1][i]=-ETA_B * d[1][i];
	}
}

double BP::Sigmoid(const double x)
{
	return A / (1 + exp(-x / B));
}

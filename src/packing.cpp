#include"packing.h"
vector<Population> PrimaryProduction(const int PredictionResultsFlavor[], const int individual_num, const int CPU, const int MEM)  //输入为预测结果 和初代种群大小
{

	vector <newFlavorType> _flavortype_;                 //第z种类型的虚拟机，有x个cpu，yMEM
	newFlavorType tempflavortype;
	tempflavortype.z = 1;  tempflavortype.x = 1;  tempflavortype.y = 1024; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 2;  tempflavortype.x = 1;  tempflavortype.y = 2048; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 3;  tempflavortype.x = 1;  tempflavortype.y = 4096; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 4;  tempflavortype.x = 2;  tempflavortype.y = 2048; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 5;  tempflavortype.x = 2;  tempflavortype.y = 4096; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 6;  tempflavortype.x = 2;  tempflavortype.y = 8192; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 7;  tempflavortype.x = 4;  tempflavortype.y = 4096; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 8;  tempflavortype.x = 4;  tempflavortype.y = 8192; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 9;  tempflavortype.x = 4;  tempflavortype.y = 16384; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 10; tempflavortype.x = 8;  tempflavortype.y = 8192; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 11; tempflavortype.x = 8;  tempflavortype.y = 16384; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 12; tempflavortype.x = 8;  tempflavortype.y = 32768; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 13; tempflavortype.x = 16; tempflavortype.y = 16384; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 14; tempflavortype.x = 16; tempflavortype.y = 32768; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 15; tempflavortype.x = 16; tempflavortype.y = 65536; _flavortype_.push_back(tempflavortype);

	int sumVM = 0;       //预测到的虚拟机的总的数量，给每个VM一个序号
	newFlavorType temptype1;
	vector <newFlavorType> OrWarehouse;
	for (int i = 0; i <15; i++) {                 //对虚拟机进行编号，并且放入仓库                
		for (int j = 0; j <PredictionResultsFlavor[i]; j++)
		{

			temptype1.ID = sumVM + 1;                 //ID是从1开始的
			temptype1.x = _flavortype_.at(i).x;
			temptype1.y = _flavortype_.at(i).y;
			temptype1.z = _flavortype_.at(i).z;
			OrWarehouse.push_back(temptype1);
			sumVM++;
		}
	}
	//生成1到sumVM的随机序列  randp数组从1开始
	vector<int> randp;               //记录乱序数组
	vector<int> tempDecisionID;      //记录的是怎么分割装箱
	unsigned int tseed = time(NULL);

	vector <Population> population;
    population.reserve(individual_num);

	newbox tempboxP;

	for (int i1 = 0; i1 < individual_num; i1++)
	{
		//生成乱序ID
		tseed = i1 + 1;
		srand(tseed);
		//生成1到sumVM的随机序列  randp数组从1开始到sumVM
		for (int i = 0; i <= sumVM; ++i)
		{
			randp.push_back(i);           //正序排列
		}
		for (int i = 1; i <= sumVM; ++i)
		{
			int j = rand() % (sumVM + 1);
			if (j != 0)
				swap(randp[i], randp[j]);  //混乱一下
		}
		for (int i = 1; i <= sumVM; ++i)
		{
			int j = rand() % (sumVM + 1);
			if (j != 0)
				swap(randp[i], randp[j]);  //再混乱一下  int randp[city_num + 1];                    随机数序列
		}
		//对产生的数据数列进行划分
		int tempx = 0, tempy = 0;
		//tempDec = 1;
		bool flag1 = true, flag2 = true;
		//flag3 = true;
		Population Mtemppopulatiom;
		for (int i = 1; i <= sumVM; i++)
		{
			tempx = OrWarehouse.at(randp[i] - 1).x + tempx;
			tempy = OrWarehouse.at(randp[i] - 1).y + tempy;

			flag1 = (tempx <= CPU);
			flag2 = (tempy <= MEM * 1024);

			if (flag1&&flag2)  //零时箱子可以放下的时候，push ID 进去，并更新临时箱子的cpu一共用了多少，MEM一共用了多少
			{
				tempboxP.ThisboxVMID.push_back(randp[i]);
				tempboxP.x = tempx;
				tempboxP.y = tempy;
				//tempDec++;
			}
			else     //当临时箱子放不下的时候，把临时箱子放到仓库，并且记录当前物品的x和y，将其放入新开的箱子
			{
				//tempDecisionID.push_back(tempDec);
				Mtemppopulatiom.Box.push_back(tempboxP);

				tempx = OrWarehouse.at(randp[i] - 1).x;
				tempy = OrWarehouse.at(randp[i] - 1).y;

				tempboxP.x = tempx;
				tempboxP.y = tempy;
				tempboxP.ThisboxVMID.clear();
				tempboxP.ThisboxVMID.push_back(randp[i]);
			}
		}
		Mtemppopulatiom.Box.push_back(tempboxP);       
        
		Mtemppopulatiom.OrderofID.assign(randp.begin() + 1, randp.end());
		Mtemppopulatiom.Fitness = Mtemppopulatiom.Box.size();
		population.push_back(Mtemppopulatiom);
		tempboxP.ThisboxVMID.clear();
		randp.clear();
	}
	return population;
}
//crosss_over
void cross_over(vector <Population> &population, int n1, int n2, int m1, int m2,
	const int PredictionResultsFlavor[], const int CPU, const int MEM)
{
	vector <newFlavorType> _flavortype_;                 //第z种类型的虚拟机，有x个cpu，yMEM
	newFlavorType tempflavortype;
	tempflavortype.z = 1;  tempflavortype.x = 1;  tempflavortype.y = 1024; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 2;  tempflavortype.x = 1;  tempflavortype.y = 2048; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 3;  tempflavortype.x = 1;  tempflavortype.y = 4096; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 4;  tempflavortype.x = 2;  tempflavortype.y = 2048; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 5;  tempflavortype.x = 2;  tempflavortype.y = 4096; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 6;  tempflavortype.x = 2;  tempflavortype.y = 8192; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 7;  tempflavortype.x = 4;  tempflavortype.y = 4096; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 8;  tempflavortype.x = 4;  tempflavortype.y = 8192; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 9;  tempflavortype.x = 4;  tempflavortype.y = 16384; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 10; tempflavortype.x = 8;  tempflavortype.y = 8192; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 11; tempflavortype.x = 8;  tempflavortype.y = 16384; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 12; tempflavortype.x = 8;  tempflavortype.y = 32768; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 13; tempflavortype.x = 16; tempflavortype.y = 16384; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 14; tempflavortype.x = 16; tempflavortype.y = 32768; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 15; tempflavortype.x = 16; tempflavortype.y = 65536; _flavortype_.push_back(tempflavortype);
	

	//******交叉两个个体中选中的箱子********
	newbox tempbox1 = population[n2].Box[m2 - 1];  //取出n2个体中第m2-1个箱子，为了替换n1个体序号为m1-1的箱子
	newbox tempbox2 = population[n1].Box[m1 - 1];  //取出n1个体中第m1-1个箱子，为了替换n2个体序号为m2-1的箱子
	population[n1].Box[m1 - 1] = tempbox1;
	population[n2].Box[m2 - 1] = tempbox2;

	//****************对选中的n1个体进行更新********
	//******第一步：去除n1个体中其他箱子含有进来的ID，同时注意其他箱子cpu、MEM的变化
	for (int i = 0; i < int(tempbox1.ThisboxVMID.size()); i++)//循环1----- i代表循环次数一直到交叉进来ID数目，依次去除其中ID
	{
		int ID = tempbox1.ThisboxVMID[i];  //取出交叉进来箱子中的ID; ID 最小值为1
		int sum = 0;    //用于判断虚拟机类型；虚拟机ID是根据类型和台数顺序编制
		int vm_type = 0;   //代表虚拟机类型
		for (; vm_type < 15; vm_type++)  //判断虚拟机ID属于哪一种类型,vm_type代表虚拟机类型
		{
			sum += PredictionResultsFlavor[vm_type];
			if (ID <= sum)
				break;
		}
		int tempx = _flavortype_[vm_type].x;   //代表虚拟机该ID的cpu量
		int tempy = _flavortype_[vm_type].y;   //代表虚拟机该ID的MEM量


		for (int j = 0; j < int(population[n1].Box.size()); j++)  //循环2----j表示n1个体中箱子个数,依次查找箱子
		{
			if (j == (m1 - 1)) //判断箱子号是否等于交叉进来箱子号
				continue;

			bool flag1 = false;  //表示j号箱子是否有该ID
			int  box_id_pos = 0; //j号箱子中有该ID，记录下位置

			for (int m = 0; m < int(population[n1].Box[j].ThisboxVMID.size()); m++)//循环3--m表示箱子中的ID，依次查找ID
			{
				if (population[n1].Box[j].ThisboxVMID[m] == ID)
				{
					flag1 = true;
					box_id_pos = m;
					break;
				}
			}//循环3---结束

			if (flag1) //j号箱子找到此ID，需要去掉箱子中的此ID，箱子中的cpu、MEM需要修改
			{
				vector<int>::iterator it = population[n1].Box[j].ThisboxVMID.begin() + box_id_pos;
				population[n1].Box[j].ThisboxVMID.erase(it);  //去除ID

				population[n1].Box[j].x = population[n1].Box[j].x - tempx; //箱子中cpu、MEM更改
				population[n1].Box[j].y = population[n1].Box[j].y - tempy;
				break;
			}

		}//循环2---结束

	}//循环1--结束

	 //******第二步：判断n1个体中箱子是否为空；是---需要去掉该箱子
	for (int j = 0; j <int(population[n1].Box.size()); j++)
	{
		if (population[n1].Box[j].ThisboxVMID.size() == 0)
		{
			vector<newbox>::iterator it = population[n1].Box.begin() + j;
			population[n1].Box.erase(it);  //去除空箱子
			j--;
		}
	}

	//******第三步：插入操作之前，要判断temp1与temp2要去掉重复的ID号
	newbox tempbox5 = tempbox2; //表示存储要插入里的ID
	for (int i = 0; i < int(tempbox2.ThisboxVMID.size()); i++)
	{
		int ID = tempbox2.ThisboxVMID[i];  //取出箱子中的ID
		int sum = 0;    //用于判断虚拟机类型；虚拟机ID是根据类型和台数顺序编制
		int vm_type = 0;   //代表虚拟机类型

		for (; vm_type < 15; vm_type++)  //判断虚拟机ID属于哪一种类型,vm_type代表虚拟机类型
		{
			sum += PredictionResultsFlavor[vm_type];
			if (ID <= sum)
				break;
		}

		int tempx = _flavortype_[vm_type].x;   //表示虚拟机ID的cpu量
		int tempy = _flavortype_[vm_type].y;   //表示虚拟机ID的MEM量

		for (int j = 0; j < int(tempbox1.ThisboxVMID.size()); j++)
		{
			if (ID == tempbox1.ThisboxVMID[j])
			{
				tempbox5.ThisboxVMID[i] = 0; //如果ID相同，把tempbox5的ID置0,CPU、MEM变化
				tempbox5.x -= tempx;
				tempbox5.y -= tempy;
				break;
			}
		}

	}
	//去除ID 号为0 的ID
	for (int i = 0; i <int(tempbox5.ThisboxVMID.size()); i++)
	{
		if (tempbox5.ThisboxVMID[i] == 0)
		{
			vector<int>::iterator it = tempbox5.ThisboxVMID.begin() + i;
			tempbox5.ThisboxVMID.erase(it);  //去除空箱子
			i--;
		}
	}

	//******第四步：由于交叉操作，n1个体必然缺少交叉所用箱子中物品，所以需要对缺少物品进行插入操作
	newbox tempbox3 = tempbox5; //表示tempbox2剩余量
	for (int i = 0; i < int(tempbox5.ThisboxVMID.size()); i++) //循环3---依次查找交叉进入n2个体箱子的ID
	{
		int ID = tempbox5.ThisboxVMID[i];  //取出箱子中的ID
		int sum = 0;    //用于判断虚拟机类型；虚拟机ID是根据类型和台数顺序编制
		int vm_type = 0;   //代表虚拟机类型

		for (; vm_type < 15; vm_type++)  //判断虚拟机ID属于哪一种类型,vm_type代表虚拟机类型
		{
			sum += PredictionResultsFlavor[vm_type];
			if (ID <= sum)
				break;
		}

		int tempx = _flavortype_[vm_type].x;   //表示虚拟机ID的cpu量
		int tempy = _flavortype_[vm_type].y;   //表示虚拟机ID的MEM量

		for (int j = 0; j < int(population[n1].Box.size()); j++) //循环4--箱子中ID号的虚拟机能否放入个体中其他箱子
		{
			population[n1].Box[j].x += tempx; //用于判断cpu是否超量
			population[n1].Box[j].y += tempy; //用于判断MEM是否超量
			if ((population[n1].Box[j].x < CPU) && (population[n1].Box[j].y < MEM * 1024)) //两者均不超量
			{
				population[n1].Box[j].ThisboxVMID.push_back(ID);  //ID 添加进去

				tempbox3.x = tempbox3.x - tempx; // tempbox3箱子里去除该ID以及CPu、MEM进行变化
				tempbox3.y = tempbox3.y - tempy;
				tempbox3.ThisboxVMID[i] = 0;

				break;   //跳出循环4，进入循环3
			}
			else //个体中第j个箱子放不进去,箱子cpu、MEM需要去掉前面已加的量
			{
				population[n1].Box[j].x -= tempx; //用于判断cpu是否超量
				population[n1].Box[j].y -= tempy; //用于判断MEM是否超量
			}
		}//循环4---结束
	} //循环3结束

	for (int i = 0; i < int(tempbox3.ThisboxVMID.size()); i++)
	{
		if (tempbox3.ThisboxVMID[i] == 0)
		{
			vector<int>::iterator it = tempbox3.ThisboxVMID.begin() + i;
			tempbox3.ThisboxVMID.erase(it);
			i--;
		}
	}
	if (tempbox3.ThisboxVMID.size() != 0) //表示交叉进入n2个体的物品不能完全插入到n1个体其他箱子，n1个体需要重开一个箱子
		population[n1].Box.push_back(tempbox3);

	//*************************** n1个体进行更新结束********

	//***************************************************************
	//****************对选中的n2个体进行更新********
	//******第一步：去除n1个体中其他箱子含有进来的ID，同时注意其他箱子cpu、MEM的变化
	for (int i = 0; i < int(tempbox2.ThisboxVMID.size()); i++)//循环1----- i代表循环次数一直到交叉进来ID数目，依次去除其中ID
	{
		int ID = tempbox2.ThisboxVMID[i];  //取出交叉进来箱子中的ID; ID 最小值为1
		int sum = 0;    //用于判断虚拟机类型；虚拟机ID是根据类型和台数顺序编制
		int vm_type = 0;   //代表虚拟机类型
		for (; vm_type < 15; vm_type++)  //判断虚拟机ID属于哪一种类型,vm_type代表虚拟机类型
		{
			sum += PredictionResultsFlavor[vm_type];
			if (ID <= sum)
				break;
		}
		int tempx = _flavortype_[vm_type].x;   //代表虚拟机该ID的cpu量
		int tempy = _flavortype_[vm_type].y;   //代表虚拟机该ID的MEM量


		for (int j = 0; j < int(population[n2].Box.size()); j++)  //循环2----j表示n2个体中箱子个数,依次查找箱子
		{
			if (j == (m2 - 1)) //判断箱子号是否等于交叉进来箱子号
				continue;

			bool flag1 = false;  //表示j号箱子是否有该ID
			int  box_id_pos = 0; //j号箱子中有该ID，记录下位置

			for (int m = 0; m < int(population[n2].Box[j].ThisboxVMID.size()); m++)//循环3--m表示箱子中的ID，依次查找ID
			{
				if (population[n2].Box[j].ThisboxVMID[m] == ID)
				{
					flag1 = true;
					box_id_pos = m;
					break;
				}
			}//循环3---结束

			if (flag1) //j号箱子找到此ID，需要去掉箱子中的此ID，箱子中的cpu、MEM需要修改
			{
				vector<int>::iterator it = population[n2].Box[j].ThisboxVMID.begin() + box_id_pos;
				population[n2].Box[j].ThisboxVMID.erase(it);  //去除ID

				population[n2].Box[j].x = population[n2].Box[j].x - tempx; //箱子中cpu、MEM更改
				population[n2].Box[j].y = population[n2].Box[j].y - tempy;
				break;
			}

		}//循环2---结束

	}//循环1--结束

	 //******第二步：判断n2个体中箱子是否为空；是---需要去掉该箱子
	for (int j = 0; j <int(population[n2].Box.size()); j++)
	{
		if (population[n2].Box[j].ThisboxVMID.size() == 0)
		{
			vector<newbox>::iterator it = population[n2].Box.begin() + j;
			population[n2].Box.erase(it);  //去除空箱子
			j--;
		}
	}

	//******第三步：插入操作之前，要判断temp1与temp2要去掉重复的ID号
	newbox tempbox6 = tempbox1; //表示存储要插入里的ID
	for (int i = 0; i < int(tempbox1.ThisboxVMID.size()); i++)
	{
		int ID = tempbox1.ThisboxVMID[i];  //取出箱子中的ID
		int sum = 0;    //用于判断虚拟机类型；虚拟机ID是根据类型和台数顺序编制
		int vm_type = 0;   //代表虚拟机类型

		for (; vm_type < 15; vm_type++)  //判断虚拟机ID属于哪一种类型,vm_type代表虚拟机类型
		{
			sum += PredictionResultsFlavor[vm_type];
			if (ID <= sum)
				break;
		}

		int tempx = _flavortype_[vm_type].x;   //表示虚拟机ID的cpu量
		int tempy = _flavortype_[vm_type].y;   //表示虚拟机ID的MEM量

		for (int j = 0; j < int(tempbox2.ThisboxVMID.size()); j++)
		{
			if (ID == tempbox2.ThisboxVMID[j])
			{
				tempbox6.ThisboxVMID[i] = 0; //如果ID相同，把tempbox5的ID置0,CPU、MEM变化
				tempbox6.x -= tempx;
				tempbox6.y -= tempy;
				break;
			}
		}

	}
	//去除ID 号为0 的ID
	for (int i = 0; i <int(tempbox6.ThisboxVMID.size()); i++)
	{
		if (tempbox6.ThisboxVMID[i] == 0)
		{
			vector<int>::iterator it = tempbox6.ThisboxVMID.begin() + i;
			tempbox6.ThisboxVMID.erase(it);  //去除空箱子
			i--;
		}
	}

	//******第四步：由于交叉操作，n2个体必然缺少交叉所用箱子中物品，所以需要对缺少物品进行插入操作
	newbox tempbox4 = tempbox6; //表示tempbox1剩余量
	for (int i = 0; i <int (tempbox6.ThisboxVMID.size()); i++) //循环3---依次查找交叉进入n1个体箱子的ID
	{
		int ID = tempbox6.ThisboxVMID[i];  //取出箱子中的ID
		int sum = 0;    //用于判断虚拟机类型；虚拟机ID是根据类型和台数顺序编制
		int vm_type = 0;   //代表虚拟机类型

		for (; vm_type < 15; vm_type++)  //判断虚拟机ID属于哪一种类型,vm_type代表虚拟机类型
		{
			sum += PredictionResultsFlavor[vm_type];
			if (ID <= sum)
				break;
		}

		int tempx = _flavortype_[vm_type].x;   //表示虚拟机ID的cpu量
		int tempy = _flavortype_[vm_type].y;   //表示虚拟机ID的MEM量

		for (int j = 0; j < population[n2].Box.size(); j++) //循环4--箱子中ID号的虚拟机能否放入个体中其他箱子
		{
			population[n2].Box[j].x += tempx; //用于判断cpu是否超量
			population[n2].Box[j].y += tempy; //用于判断MEM是否超量
			if ((population[n2].Box[j].x < CPU) && (population[n2].Box[j].y < MEM * 1024)) //两者均不超量
			{
				population[n2].Box[j].ThisboxVMID.push_back(ID);  //ID 添加进去

				tempbox4.x = tempbox4.x - tempx; // tempbox3箱子里去除该ID以及CPu、MEM进行变化
				tempbox4.y = tempbox4.y - tempy;
				tempbox4.ThisboxVMID[i] = 0;

				break;   //跳出循环4，进入循环3
			}
			else //个体中第j个箱子放不进去,箱子cpu、MEM需要去掉前面已加的量
			{
				population[n2].Box[j].x -= tempx; //用于判断cpu是否超量
				population[n2].Box[j].y -= tempy; //用于判断MEM是否超量
			}
		}//循环4---结束
	} //循环3结束
	for (int i = 0; i < tempbox4.ThisboxVMID.size(); i++)
	{
		if (tempbox4.ThisboxVMID[i] == 0)
		{
			vector<int>::iterator it = tempbox4.ThisboxVMID.begin() + i;
			tempbox4.ThisboxVMID.erase(it);
			i--;
		}
	}
	if (tempbox4.ThisboxVMID.size() != 0) //表示交叉进入n1个体的物品不能完全插入到n2个体其他箱子，n2个体需要重开一个箱子
		population[n2].Box.push_back(tempbox4);
	//*************************** n2个体进行更新结束********

}
void mutation(const int  PredictionResultsFlavor[], Population &population, int n, vector <newFlavorType> _flavortype_, const int CPU, const int MEM)
{

	//int num = 0;  //代表n号箱子中虚拟机可以放入其他箱子中的数目

	newbox tempbox = population.Box[n - 1]; //代表 n-1 号箱子的剩余量

	for (int i = 0; i < population.Box[n - 1].ThisboxVMID.size(); i++) //大循环---依次查找抽中箱子的ID

	{
		int ID = population.Box[n - 1].ThisboxVMID[i];  //取出抽中箱子中的ID; ID 最小值为1，初始化种群时注意

		int sum = 0;    //用于判断虚拟机类型；虚拟机ID是根据类型和台数顺序编制

		int vm_type = 0;   //代表虚拟机类型

		for (; vm_type < 15; vm_type++)  //判断虚拟机ID属于哪一种类型,vm_type代表虚拟机类型

		{
			sum += PredictionResultsFlavor[vm_type];

			if (ID <= sum)

				break;
		}

		int tempx = _flavortype_[vm_type].x;   //临时变量存储虚拟机类型的cpu量

		int tempy = _flavortype_[vm_type].y;   //临时变量存储虚拟机类型的MEM量

		for (int j = 0; j < population.Box.size(); j++) //循环1--判断抽中箱子中ID号的虚拟机能否放入个体中其他箱子里

		{
			if (j == (n - 1))  //n 为抽中箱子，应排除
				continue;

			population.Box[j].x += tempx; //用于判断cpu是否超量

			population.Box[j].y += tempy; //用于判断MEM是否超量

			if ((population.Box[j].x < CPU) && (population.Box[j].y < MEM * 1024)) //两者均不超量

			{

				population.Box[j].ThisboxVMID.push_back(ID);  //ID 添加进去

				tempbox.x = tempbox.x - tempx; // tempbox箱子里去除该ID以及CPu、MEM进行变化

				tempbox.y = tempbox.y - tempy;

				tempbox.ThisboxVMID[i] = 0;
				//cpu、MEM 利用率 需要时添加进去

				break;   //跳出循环1，进入大循环

			}

			else //个体中第j个箱子放不进去,箱子cpu、MEM需要去掉前面已加的量

			{
				population.Box[j].x -= tempx; //用于判断cpu是否超量
				population.Box[j].y -= tempy; //用于判断MEM是否超量
			}

		}//循环1---结束

	} //大循环结束

	  //***** 需要去除 n-1号箱子，并且判断tempbox是否为空，不为空需要把tempbox添加进去
	vector<newbox>::iterator it = population.Box.begin() + n - 1;

	population.Box.erase(it); //去掉n-1号箱子

	for (int i = 0; i < tempbox.ThisboxVMID.size(); i++)
	{
		if (tempbox.ThisboxVMID[i] == 0)
		{
			vector<int>::iterator it = tempbox.ThisboxVMID.begin() + i;
			tempbox.ThisboxVMID.erase(it);
			i--;
		}
	}

	if (tempbox.ThisboxVMID.size() != 0)
		population.Box.push_back(tempbox);

}
/***********选择操作*************/
void Choice(vector <Population> &population)
{

	int SizeOfPo=population.size();       // 种群的大小
	
	//cout << SizeOfPo << "%%%%";
	int TempMinSize = population[0].Box.size();                    //记录该种群中的个体所需箱子最多数量
	//int FlagMax = 0;                                               //记录是哪一个个体
	for (int i = 1; i <SizeOfPo; i++)
	{
		//Fitness.push_back(population[i].Box.size());

		if (TempMinSize > population[i].Box.size())
		{
			TempMinSize = population[i].Box.size();
			//FlagMax = i;
		}
	}

	vector <Population> TempPopulation;
    TempPopulation.reserve(SizeOfPo);
	for (int i = 0; i < SizeOfPo; i++)
	{
		if (population[i].Box.size() == TempMinSize)
		{
			TempPopulation.push_back(population[i]);
		}
	}
    population=TempPopulation;
}


vector<box> MutationPacking(const int PredictionResultsFlavor[], const int NumOfMuta, vector <Population>population, int NumOFInitialpop, const int CPU, const int MEM)      //变异次数 /初代/初代种群大小
{
	vector <newFlavorType> _flavortype_;                 //第z种类型的虚拟机，有x个cpu，yMEM
	newFlavorType tempflavortype;
	tempflavortype.z = 1;  tempflavortype.x = 1;  tempflavortype.y = 1024; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 2;  tempflavortype.x = 1;  tempflavortype.y = 2048; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 3;  tempflavortype.x = 1;  tempflavortype.y = 4096; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 4;  tempflavortype.x = 2;  tempflavortype.y = 2048; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 5;  tempflavortype.x = 2;  tempflavortype.y = 4096; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 6;  tempflavortype.x = 2;  tempflavortype.y = 8192; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 7;  tempflavortype.x = 4;  tempflavortype.y = 4096; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 8;  tempflavortype.x = 4;  tempflavortype.y = 8192; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 9;  tempflavortype.x = 4;  tempflavortype.y = 16384; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 10; tempflavortype.x = 8;  tempflavortype.y = 8192; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 11; tempflavortype.x = 8;  tempflavortype.y = 16384; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 12; tempflavortype.x = 8;  tempflavortype.y = 32768; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 13; tempflavortype.x = 16; tempflavortype.y = 16384; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 14; tempflavortype.x = 16; tempflavortype.y = 32768; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 15; tempflavortype.x = 16; tempflavortype.y = 65536; _flavortype_.push_back(tempflavortype);

	int sumVM = 0;       //预测到的虚拟机的总的数量，给每个VM一个序号
	newFlavorType temptype1;
	vector <newFlavorType> OrWarehouse;
	for (int i = 0; i <15; i++) {                 //对虚拟机进行编号，并且放入仓库                
		for (int j = 0; j <PredictionResultsFlavor[i]; j++)
		{

			temptype1.ID = sumVM + 1;                 //ID是从1开始的
			temptype1.x = _flavortype_.at(i).x;
			temptype1.y = _flavortype_.at(i).y;
			temptype1.z = _flavortype_.at(i).z;
			OrWarehouse.push_back(temptype1);
			sumVM++;
		}
	}

	//计算初代中最好的个体
	int tempmin = population[0].Box.size();
	for (int i = 1; i < NumOFInitialpop; i++)
	{
		if (tempmin  > population[i].Box.size())
		{
			tempmin = population[i].Box.size();
		}
	}
	//cout << "At the begin： " << tempmin << endl;

	unsigned int tseed = time(NULL);
	tseed = time(NULL);
	srand(tseed);

	for (int i = 0; i <NumOfMuta; i++)                       //进行变异，变异NumOfMuta次
	{
		int n1 = rand() % NumOFInitialpop + 1;          //生产的随机数n1是1到NumOFInitialpop，即选择初始种群中的任意一个个体
		int minBox = population[n1 - 1].Box.size();


		int n2 = rand() % minBox + 1;                    //选中的个体箱子数量是多少
		mutation(PredictionResultsFlavor, population[n1 - 1], n2, _flavortype_, CPU, MEM);
		/*能否每次变异之后去除比较痴呆的，留下比较好的*/
	}


	Population temppopulation = population[0];
	int tempmin1 = population[0].Box.size();
	for (int i = 1; i < NumOFInitialpop; i++)
	{

		if (tempmin1  > population[i].Box.size())
		{
			temppopulation = population[i];
			tempmin1 = population[i].Box.size();
		}
	}

	/*********将最好的那个个体反向编码,放入BoxOpt***********/
	vector<box> BoxOpt;
	box tempBoxOpt;
	int templeixing;
	for (int i = 0; i <temppopulation.Box.size(); i++)   //
	{
		tempBoxOpt.x = temppopulation.Box.at(i).x;
		tempBoxOpt.y = temppopulation.Box.at(i).y;

		for (int j = 0; j < temppopulation.Box.at(i).ThisboxVMID.size(); j++)
		{
			templeixing = OrWarehouse[temppopulation.Box.at(i).ThisboxVMID[j] - 1].z;                        //读出它的ID,根据
			tempBoxOpt.NumberOfVM[templeixing - 1]++;
		}
		BoxOpt.push_back(tempBoxOpt);
		for (int k = 0; k < 15; k++)
		{
			tempBoxOpt.NumberOfVM[k] = 0;
		}
	}


	return BoxOpt;
}
vector<box> binpack(const string inputAimCPUOrMEM, const int inputPhysicalMachineCPU, const int inputPhysicalMachineMEM, const int PredictionResultsFlavor[])
{
	const int CPU = inputPhysicalMachineCPU;           //每台物理机56个CPU
	const int MEM = inputPhysicalMachineMEM;        //每台物理机内存128G
													//const int sizeOfdisk = inputPhysicalMachineDisk;   //每台物理机硬盘大小1200G
	string CPUorMEM = inputAimCPUOrMEM;
	std::vector <FlavorType> _flavortype_;                 //第z种类型的虚拟机，有x个cpu，yMEM
	FlavorType tempflavortype;
	tempflavortype.z = 1;  tempflavortype.x = 1;  tempflavortype.y = 1024; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 2;  tempflavortype.x = 1;  tempflavortype.y = 2048; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 3;  tempflavortype.x = 1;  tempflavortype.y = 4096; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 4;  tempflavortype.x = 2;  tempflavortype.y = 2048; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 5;  tempflavortype.x = 2;  tempflavortype.y = 4096; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 6;  tempflavortype.x = 2;  tempflavortype.y = 8192; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 7;  tempflavortype.x = 4;  tempflavortype.y = 4096; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 8;  tempflavortype.x = 4;  tempflavortype.y = 8192; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 9;  tempflavortype.x = 4;  tempflavortype.y = 16384; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 10; tempflavortype.x = 8;  tempflavortype.y = 8192; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 11; tempflavortype.x = 8;  tempflavortype.y = 16384; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 12; tempflavortype.x = 8;  tempflavortype.y = 32768; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 13; tempflavortype.x = 16; tempflavortype.y = 16384; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 14; tempflavortype.x = 16; tempflavortype.y = 32768; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 15; tempflavortype.x = 16; tempflavortype.y = 65536; _flavortype_.push_back(tempflavortype);

	vector<box> BoxOpt;
	box tempbox;

	int TempVNnumber = 0;
	int TempX = 0;
	int TempY = 0;
	//int TempZ = 0;   //当前虚拟机类型
	bool flagX = true, flagY = true, flagBox = true;
	//对CPU进行优化装箱，在装箱过程中保证MEM正确
	//先对需要优化的箱子排序
	std::vector <FlavorType> decreasingflavort;
	int subscript[15] = { 0 };
	if (CPUorMEM == "CPU")
	{
		decreasingflavort = _flavortype_;
	}
	else
	{
		//手动排序
		/*
		FlavorType tempflavortype;
		tempflavortype.z = 1;  tempflavortype.x = 1;  tempflavortype.y = 1024; decreasingflavort.push_back(tempflavortype);
		tempflavortype.z = 2;  tempflavortype.x = 1;  tempflavortype.y = 2048; decreasingflavort.push_back(tempflavortype);
		tempflavortype.z = 4;  tempflavortype.x = 2;  tempflavortype.y = 2048; decreasingflavort.push_back(tempflavortype);
		tempflavortype.z = 3;  tempflavortype.x = 1;  tempflavortype.y = 4096; decreasingflavort.push_back(tempflavortype);
		tempflavortype.z = 5;  tempflavortype.x = 2;  tempflavortype.y = 4096; decreasingflavort.push_back(tempflavortype);
		tempflavortype.z = 7;  tempflavortype.x = 4;  tempflavortype.y = 4096; decreasingflavort.push_back(tempflavortype);
		tempflavortype.z = 6;  tempflavortype.x = 2;  tempflavortype.y = 8192; decreasingflavort.push_back(tempflavortype);
		tempflavortype.z = 10; tempflavortype.x = 8;  tempflavortype.y = 8192; decreasingflavort.push_back(tempflavortype);
		tempflavortype.z = 8;  tempflavortype.x = 4;  tempflavortype.y = 8192; decreasingflavort.push_back(tempflavortype);
		tempflavortype.z = 9;  tempflavortype.x = 4;  tempflavortype.y = 16384; decreasingflavort.push_back(tempflavortype);
		tempflavortype.z = 11; tempflavortype.x = 8;  tempflavortype.y = 16384; decreasingflavort.push_back(tempflavortype);
		tempflavortype.z = 12; tempflavortype.x = 8;  tempflavortype.y = 32768; decreasingflavort.push_back(tempflavortype);
		tempflavortype.z = 13; tempflavortype.x = 16; tempflavortype.y = 16384; decreasingflavort.push_back(tempflavortype);
		tempflavortype.z = 14; tempflavortype.x = 16; tempflavortype.y = 32768; decreasingflavort.push_back(tempflavortype);
		tempflavortype.z = 15; tempflavortype.x = 16; tempflavortype.y = 65536; decreasingflavort.push_back(tempflavortype);
		*/
		decreasingflavort = _flavortype_;
	}
	for (size_t i = 0; i<decreasingflavort.size(); i++)
	{
		subscript[i] = decreasingflavort.at(i).z;
	}


	for (int i = 14; i >= 0; i--)
	{
		int script = subscript[i];
		TempVNnumber = PredictionResultsFlavor[script - 1];   //该种类型虚拟机预测得到的数量

															  //TempZ = _flavortype_.at(i).z;
		for (int j = 0; j < TempVNnumber; j++)
		{
			if (flagBox)       //开始装箱的时候第一个箱子一定是空的，且第一个虚拟机一定能装进去 只执行一次
			{
				TempX = decreasingflavort.at(i).x;//cpu
				TempY = decreasingflavort.at(i).y;//mem

				tempbox.x = TempX;
				tempbox.y = TempY;
				tempbox.NumberOfVM[i]++;
				BoxOpt.push_back(tempbox);
				flagBox = false;
				continue;
			}

			//bool flag1 = true;
			// 每次从第一个箱子开始放，如果放不下，重新开一个箱子（Push）
			//修改策略，保证箱子的数目最小。best fit decreasing
			vector<size_t> okboxscript;
			for (size_t k = 0; k < BoxOpt.size(); k++)
			{
				TempX = BoxOpt.at(k).x + decreasingflavort.at(i).x;
				TempY = BoxOpt.at(k).y + decreasingflavort.at(i).y;

				flagX = (TempX <= CPU);
				flagY = (TempY <= (MEM * 1024));

				if (flagX && flagY)
				{
					okboxscript.push_back(k);
				}
			}
			//处理多个box均可以装进去的情况

			if (okboxscript.size()>0)
			{
				int minscript = 0; int min = 0; bool flag = true;
				for (size_t i = 0; i<okboxscript.size(); i++)
				{
					int kk = okboxscript[i];
					if (CPUorMEM == "CPU")//优化CPU
					{
						int temp = CPU - BoxOpt.at(kk).x;
						if (flag == true)
						{
							min = temp;
							flag = false;
						}
						if (temp <= min)
						{
							min = temp;
							minscript = kk;//minscript 保存的是剩余资源最小的箱子序号
						}
					}
					else //优化MEM
					{
						int temp = (MEM * 1024) - BoxOpt.at(kk).y;
						if (flag == true)//置flag
						{
							min = temp;
							flag = false;
						}
						if (temp <= min)
						{
							min = temp;
							minscript = kk;//minscript 保存的是剩余资源最小的箱子序号
						}
					}

				}
				//处理最小箱子序号
				TempX = BoxOpt.at(minscript).x + decreasingflavort.at(i).x;
				TempY = BoxOpt.at(minscript).y + decreasingflavort.at(i).y;
				tempbox.x = TempX;
				tempbox.y = TempY;
				BoxOpt.at(minscript).x = tempbox.x;
				BoxOpt.at(minscript).y = tempbox.y;
				BoxOpt.at(minscript).NumberOfVM[script - 1] = BoxOpt.at(minscript).NumberOfVM[script - 1] + 1;

			}
			if (okboxscript.size() == 0)//前面所有的箱子都不能满足要求，只能新开，直接放进去。	
			{
				tempbox.x = decreasingflavort.at(i).x;
				tempbox.y = decreasingflavort.at(i).y;
				for (int i1 = 0; i1 <15; i1++)
				{
					tempbox.NumberOfVM[i1] = 0;
				}
				tempbox.NumberOfVM[script - 1] = tempbox.NumberOfVM[script - 1] + 1;
				BoxOpt.push_back(tempbox);
			}
		}
	}
	return BoxOpt;
}
/***********遗传算法装箱*******************/
//预测结果、进化次数 、每次进化变异次数、初代种群大小、每次进化次数
vector<box> GAPacking(const int PredictionResultsFlavor[], const int NumOfEvolution,
	                  const int NumOfmeta, const int NumOfcross, int NumOFInitialpop, const int CPU, const int MEM,const double StartTime)
{                     
	vector <newFlavorType> _flavortype_;                 //第z种类型的虚拟机，有x个cpu，yMEM
	newFlavorType tempflavortype;
	tempflavortype.z = 1;  tempflavortype.x = 1;  tempflavortype.y = 1024; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 2;  tempflavortype.x = 1;  tempflavortype.y = 2048; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 3;  tempflavortype.x = 1;  tempflavortype.y = 4096; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 4;  tempflavortype.x = 2;  tempflavortype.y = 2048; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 5;  tempflavortype.x = 2;  tempflavortype.y = 4096; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 6;  tempflavortype.x = 2;  tempflavortype.y = 8192; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 7;  tempflavortype.x = 4;  tempflavortype.y = 4096; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 8;  tempflavortype.x = 4;  tempflavortype.y = 8192; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 9;  tempflavortype.x = 4;  tempflavortype.y = 16384; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 10; tempflavortype.x = 8;  tempflavortype.y = 8192; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 11; tempflavortype.x = 8;  tempflavortype.y = 16384; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 12; tempflavortype.x = 8;  tempflavortype.y = 32768; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 13; tempflavortype.x = 16; tempflavortype.y = 16384; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 14; tempflavortype.x = 16; tempflavortype.y = 32768; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 15; tempflavortype.x = 16; tempflavortype.y = 65536; _flavortype_.push_back(tempflavortype);


	/************对预测的结果进行编码*************/
	int sumVM = 0;       //预测到的虚拟机的总的数量，给每个VM一个序号
	newFlavorType temptype1;
	vector <newFlavorType> OrWarehouse;
	for (int i = 0; i <15; i++) {                 //对虚拟机进行编号，并且放入仓库                
		for (int j = 0; j <PredictionResultsFlavor[i]; j++)
		{

			temptype1.ID = sumVM + 1;                 //ID是从1开始的
			temptype1.x = _flavortype_.at(i).x;
			temptype1.y = _flavortype_.at(i).y;
			temptype1.z = _flavortype_.at(i).z;
			OrWarehouse.push_back(temptype1);
			sumVM++;
		}
	}
	/*********生产初代**************/
	vector<Population> population = PrimaryProduction(PredictionResultsFlavor, NumOFInitialpop, CPU, MEM);

	int TempNumOfEvolution = NumOfEvolution;
	while (TempNumOfEvolution--)
	{
		//选择
        Choice(population);
      NumOFInitialpop=int(population.size());
		//交叉
		unsigned int tseed = time(NULL);
		int m1 = 0, m2 = 0;
		for(int j=0;j<NumOfcross;j++)
		{
			for (int i = 0; i < population.size()/2; i++)
			{
				tseed = i + 1;
				srand(tseed);
				m1 = rand() % population.at(i).Box.size() + 1;
				m2 = rand() % population.at(i + population.size() / 2).Box.size() + 1;
				cross_over(population, i, i + population.size() / 2, m1, m2, PredictionResultsFlavor, CPU, MEM);
			}
		}
		//变异
		for (int i = 0; i <NumOfmeta; i++)                       //进行变异，变异NumOfEvolution次
		{
			int n1 = rand() % int(population.size())+ 1;          //生产的随机数n1是1到NumOFInitialpop，即选择初始种群中的任意一个个体
			int minBox = population[n1 - 1].Box.size();
			int n2 = rand() % minBox + 1;                    //选中的个体箱子数量是多少

			mutation(PredictionResultsFlavor, population[n1 - 1], n2, _flavortype_, CPU, MEM);
			/*能否每次变异之后去除比较痴呆的，留下比较好的*/
		}

		double NowTime=clock();
        if((NowTime-StartTime)>50*1e3)
            break;
	}
	 
	/************寻找进化结束后最好的那个个体******************/
	Population temppopulation = population[0];
	int tempmin1 = population[0].Box.size();
	for (int i = 1; i < NumOFInitialpop; i++)
	{
		if (tempmin1  > population[i].Box.size())
		{
			temppopulation = population[i];
			tempmin1 = population[i].Box.size();
		}
	}
	/*********将最好的那个个体反向编码,放入BoxOpt***********/
	vector<box> BoxOpt;
	box tempBoxOpt;
	int templeixing;
	for (int i = 0; i <temppopulation.Box.size(); i++)   //
	{
		tempBoxOpt.x = temppopulation.Box.at(i).x;
		tempBoxOpt.y = temppopulation.Box.at(i).y;

		for (int j = 0; j < temppopulation.Box.at(i).ThisboxVMID.size(); j++)
		{
			templeixing = OrWarehouse[temppopulation.Box.at(i).ThisboxVMID[j] - 1].z;                        //读出它的ID,根据
			tempBoxOpt.NumberOfVM[templeixing - 1]++;
		}
		BoxOpt.push_back(tempBoxOpt);
		for (int k = 0; k < 15; k++)
		{
			tempBoxOpt.NumberOfVM[k] = 0;
		}
	}
	return BoxOpt;
}
bool CheckPacking(const int PredictionResultsFlavor[],const vector<box> BoxOpt,string cpu_or_mem,const int cpu_num,const int mem_num)
{
      //check box size 
    vector <newFlavorType> _flavortype_;                 //第z种类型的虚拟机，有x个cpu，yMEM
	newFlavorType tempflavortype;
	tempflavortype.z = 1;  tempflavortype.x = 1;  tempflavortype.y = 1024; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 2;  tempflavortype.x = 1;  tempflavortype.y = 2048; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 3;  tempflavortype.x = 1;  tempflavortype.y = 4096; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 4;  tempflavortype.x = 2;  tempflavortype.y = 2048; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 5;  tempflavortype.x = 2;  tempflavortype.y = 4096; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 6;  tempflavortype.x = 2;  tempflavortype.y = 8192; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 7;  tempflavortype.x = 4;  tempflavortype.y = 4096; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 8;  tempflavortype.x = 4;  tempflavortype.y = 8192; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 9;  tempflavortype.x = 4;  tempflavortype.y = 16384; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 10; tempflavortype.x = 8;  tempflavortype.y = 8192; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 11; tempflavortype.x = 8;  tempflavortype.y = 16384; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 12; tempflavortype.x = 8;  tempflavortype.y = 32768; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 13; tempflavortype.x = 16; tempflavortype.y = 16384; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 14; tempflavortype.x = 16; tempflavortype.y = 32768; _flavortype_.push_back(tempflavortype);
	tempflavortype.z = 15; tempflavortype.x = 16; tempflavortype.y = 65536; _flavortype_.push_back(tempflavortype);
       
       cout<<"Checking Packing......."<<endl;
       int count[15]={0};
      int cpu_sum=0,mem_sum=0;
    //  int idx=0;
     for(auto each:BoxOpt)  
     {
         
           int cpu=0,mem=0;
        for(int i=0;i<15;i++)  
        {
        cpu=cpu+each.NumberOfVM[i]*_flavortype_.at(i).x;
         mem= mem+each.NumberOfVM[i]*_flavortype_.at(i).y;
         count[i]=count[i]+each.NumberOfVM[i];
        }
     //   cout<<"box="<<idx<<":  "<<double((cpu*1.0)/cpu_num)*100<<"  "<<double((mem*1.0)/(mem_num*1024))*100<<endl;
      //              idx=idx+1;
                    
        if(cpu>cpu_num)
        {
             cout<<"box cpu: "<<cpu <<" > "<<cpu_num<<endl;
             return false;
        }
        if(mem>mem_num*1024)
        {    cout<<"box mem: "<<mem/1024<<" > "<<mem_num<<endl;
            return false;
        }
            cpu_sum=cpu_sum+cpu;
            mem_sum=mem_sum+mem;
     }
     //show the number of each flavor in all box 
       for(int i=0;i<15;i++)
       {
         if(count[i]!=PredictionResultsFlavor[i])
             return false;
       }
    //compute using ratio
    double ratio=0;
     if(cpu_or_mem=="CPU")
            ratio=(double(cpu_sum)/double(BoxOpt.size()*cpu_num))*100;
    else
    {
             ratio=(double(mem_sum/1024)/double(BoxOpt.size()*mem_num))*100;
        
    }
       cout<<cpu_or_mem<<" using ration: "<<ratio<<endl;
       return true;
}





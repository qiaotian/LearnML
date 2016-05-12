#include <iostream>
#include <string>
#include <cmath>
#include <ctime>
#include <vector>

#define GAS_STATION_NUM 10	//上油点数量
#define TRAIN_NUM 5		//火车数量
#define MAX_GENERATION 200
#define PC 0.0			//交配率
#define PM 0.05			//变异率
#define POPULATION 100 		//种群大小
#define MAX_DISTANCE 800	//最大里程数
#define GAS_NUM 3           	//燃油种类

using namespace std;

//int N=7; 用 GAS_STATION_NUM 代替
int g=1200;
int x;          //批次

int C=12000;
int Q=1000;
int D=775;
int M=3000;

int bestSln;	//种群中最优染色体的序号

/***********************
 * 距离矩阵
 * 需手动输入
 ***********************/
int dist[GAS_STATION_NUM+1][GAS_STATION_NUM+1] = {
    {  0,  50,  60,  80,  90,  60,  70, 150, 200, 150, 200},
    { 50,   0, 110, 100,  40,  50, 120, 140, 200, 130, 140},
    { 60, 110,   0,  50, 150, 120,  40, 210, 200, 140, 130},
    { 80, 100,  50,   0, 140, 140,  70, 120, 200, 130, 140},
    { 90,  40, 150, 140,   0,  30, 160, 120, 200, 140, 160},
    { 60,  50, 120, 140,  30,   0, 130,  90, 200, 170, 180},
    { 70, 120,  40,  70, 160, 130,   0, 220, 200, 160, 190},
    {150, 140, 210, 120, 120,  90, 220,   0, 200, 190, 210},
    {200, 200, 200, 200, 200, 200, 200, 200,   0, 200, 140},
    {150, 130, 140, 130, 140, 170, 160, 180, 190,   0, 200},
    {200, 140, 130, 140, 160, 180, 190, 210, 140, 200,   0}
};
/***********************
 * 需求矩阵
 * 单位吨，需手动输入
 ***********************/
int demand[GAS_STATION_NUM][GAS_NUM] = {
    {820, 430, 1470},
    {730, 390, 1130},
    {640, 340,  990},
    {620, 260,  750},
    {340, 230,  750},
    {560, 310,  930},
    {490, 250,  760},
    {350, 640,  760},
    {130, 140, 1400},
    {340, 650,  760}
};
/***********************
 * 需求矩阵
 * 单位罐，由demand换算得到
 ***********************/
/*int demand_cans[GAS_STATION_NUM][TRAIN_NUM] = {
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0}
 };*/
/**********************
 * 采购矩阵
 * 单位吨，需手动输入
 ***********************/
int purchase[GAS_STATION_NUM][GAS_NUM] = {
    {1000, 1000, 1000},
    {1000, 1000, 1000},
    {1000, 1000, 1000},
    {1000, 1000, 1000},
    {1000, 1000, 1000},
    {1000, 1000, 1000},
    {1000, 1000, 1000},
    {1000, 1000, 1000},
    {1000, 1000, 1000},
    {1000, 1000, 1000}
};

/**********************
 * 采购矩阵
 * 单位罐，由purchase换算得到
 ***********************/
int purchase_cans[GAS_STATION_NUM][GAS_NUM] = {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}
};

/************
 * 染色体结构
 *************/
typedef struct struct_group {
    int dir[GAS_STATION_NUM];	//上油点顺序
    double adapt;				//适应度(费用)
    double p;				    //幸存概率
} struct_group;

struct_group group[POPULATION], group_temp[POPULATION];
struct_group group_obj;
/*********************
 * 计算订货成本
 **********************/
double cal_G() {
    return GAS_STATION_NUM*g*x;
}
/*********************
 * 计算库存成本
 **********************/
double cal_K() {
    double temp = 0;
    for (int i=0; i<x; i++) {
        temp += D/x*((C-Q)*i/x+(2*C-Q)/(2*x)+M);
    }
    return temp;
}
/*********************
 * 计算运输成本
 * param: dir 输入的一条路径
 **********************/
double cal_T(int *dir) {
    int train_dir[TRAIN_NUM][GAS_STATION_NUM+1];	//三辆火车，第一个数字表示负责运送的站点数，初始为0个
    for(int i=0; i<TRAIN_NUM; ++i) {
        for(int j=0; j<GAS_STATION_NUM+1; ++j){
            train_dir[i][j]=0;
        }
    }
    
    int curr_distance = 0; //当前任务下的里程数
    int curr_train = 0;		   //当前为第train辆车分配任务
    int index=1;			   //火车路径上第index个上油点
    int curr_cans_count=0;	   //当前车的罐子需求量
    for(int i=0; i<GAS_STATION_NUM-1; ++i) {  //分配运送任务
        for(int j=0; j<train_dir[curr_train][0]; ++j){
            curr_cans_count += train_dir[curr_train][j+1];
        }
        if(curr_distance < MAX_DISTANCE) {
            train_dir[curr_train][0]++;
            train_dir[curr_train][index++]=dir[i];
            curr_distance+=dist[dir[i]][dir[i+1]];
        } else {
            index = 1;
            curr_train++;
            curr_distance = dist[0][dir[i+1]];
        }
    }
    
    //错误检测
    int all_length = 0;
    for (int i=0; i<3; ++i) {
        for (int j=0; j<train_dir[i][0]; ++j) {
            if (j==0) {
                all_length+=dist[0][train_dir[i][j+1]];
            } else {
                all_length+=dist[train_dir[i][j]][train_dir[i][j+1]];
            }
        }
    }
    //单批总罐数
    if(all_length>800) {
        return -1;
    }
    
    //上油点n对油品m的需求罐数
    int purchase_total[TRAIN_NUM];
    int demand_total[TRAIN_NUM];
    for (int i=0; i<GAS_STATION_NUM; ++i) {
        for (int j=0; j<TRAIN_NUM; j++) {
            purchase_total[j]+=purchase[i][j];
            demand_total[j]+=demand[i][j];
        }
    }
    for (int i=0; i<GAS_STATION_NUM; ++i) {
        for (int j=0; j<TRAIN_NUM; ++j) {
            if (demand_total[j]==0) {
                purchase_cans[i][j]=0;
            } else {
                purchase_cans[i][j] = ceil(purchase_total[j] * demand[i][j] / (50*demand_total[j]*x));
            }
        }
    }
    //上油点n对所有油品的需求罐数
    int purchase_station[GAS_STATION_NUM];
    for(int i=0; i<GAS_STATION_NUM; ++i) {
        purchase_station[i]=0;
        for(int j=0; j<TRAIN_NUM; ++j) {
            purchase_station[i]+=purchase_cans[i][j];
        }
    }
    
    //单批运输成本
    double cost = 0;
    for(int i=0;i<TRAIN_NUM;i++) {
        for(int j=0; j<train_dir[i][0]; ++j) {
            int bq = 0;
            for(int k=j; k<train_dir[i][0]; ++k){
                bq += purchase_station[k];
            }
            if(j==0){
                cost += 50*(19.6+0.0989*dist[0][train_dir[i][1]])*bq;
            }
            cost += 50*(19.6+0.0989*dist[train_dir[i][j]][train_dir[i][j+1]])*bq;
        }
    }
    return cost*x;
}
/*****************************************************************************************
 * 产生初始种群
 * function: 产生初始种群，若不符合条件则重新生成
 *****************************************************************************************/
void generate_init_group() {
    int i,j,t,k,flag;
    for(i=0;i<POPULATION;i++)  //初始化
        for(j=0;j<GAS_STATION_NUM;j++)
            group[i].dir[j] = 0;
    srand((unsigned)time(NULL));
    for(i=0; i<POPULATION; i++)
    {
        //产生GAS_STATION_NUM个不相同的数字
        for(j=0;j<GAS_STATION_NUM;)
        {
            t=rand()%GAS_STATION_NUM+1; //随机生成取值范围1到GAS_STATION_NUM的GAS_STATION_NUM个数
            flag=1;
            for(k=0;k<j;k++)
            {
                if(group[i].dir[k]==t)
                {
                    flag=0;
                    break;
                }
            }
            if(flag)
            {
                group[i].dir[j]=t;
                j++;
            }
        }
        //判定是否符合条件
        while(cal_T(group[i].dir)<0) {
            for(j=0;j<GAS_STATION_NUM;)
            {
                t=rand()%GAS_STATION_NUM+1; //随机生成取值范围1到GAS_STATION_NUM的GAS_STATION_NUM个数
                flag=1;
                for(k=0;k<j;k++)
                {
                    if(group[i].dir[k]==t)
                    {
                        flag=0;
                        break;
                    }
                }
                if(flag)
                {
                    group[i].dir[j]=t;
                    j++;
                }
            }
        }
    }
    //打印种群基因
    /*printf("初始的种群\n");
     for(i=0;i<POPULATION;i++)
     {
     for(j=0;j<GAS_STATION_NUM;j++)
     printf("%4d",group[i].dir[j]);
     printf("\n");
     }*/
}
/*****************************************************************************************
 * 选择函数
 * function: 适应度越高，繁殖能力越强
 *****************************************************************************************/
void select() {
    int i,j,temp;
    double gradient[POPULATION];	//梯度概率
    double random[POPULATION];		//生成选择染色体的随机概率
    int selected[POPULATION];	    //选择了的染色体
    //初始化梯度概率
    for(i=0;i<POPULATION;i++)
    {
        gradient[i]=0.0;
        random[i]=0.0;
    }
    gradient[0]=group[0].p;
    for(i=1;i<POPULATION;i++)
        gradient[i]=gradient[i-1]+group[i].p;
    srand((unsigned)time(NULL));
    //随机产生染色体的存活概率
    for(i=0;i<POPULATION;i++) {
        random[i]=(rand()%100)/100.0;
    }
    //选择能生存的染色体
    for(i=0;i<POPULATION;i++)
    {
        for(j=0;j<POPULATION;j++)
        {
            if(random[i]<gradient[j])
            {
                selected[i]=j; //第i个位置存放第j个染色体
                break;
            }
        }
    }
    //拷贝种群
    for(i=0;i<POPULATION;i++) {
        group_temp[i].adapt=group[i].adapt;
        group_temp[i].p=group[i].p;
        for(j=0;j<GAS_STATION_NUM;j++)
            group_temp[i].dir[j]=group[i].dir[j];
    }
    //数据更新
    for(i=0;i<POPULATION;i++) {
        temp=selected[i];
        group[i].adapt=group_temp[temp].adapt;
        group[i].p=group_temp[temp].p;
        for(j=0;j<GAS_STATION_NUM;j++)
            group[i].dir[j]=group_temp[temp].dir[j];
    }
    //用于测试
    /*
     printf("<------------------------------->\n");
     for(i=0;i<POPULATION;i++) {
     for(j=0;j<GAS_STATION_NUM;j++)
     printf("%4d",group[i].dir[j]);
     printf("\n");
     printf("染色体%d的路径之和与生存概率分别为%4lf  %.4f\n",i,group[i].adapt,group[i].p);
     }
     */
}
/*****************************************************************************************
 * 变异函数
 * function: 随机概率小于PM的发生变异
 *****************************************************************************************/
void variate() {
    //int i,j;
    //int t;
    int temp1,temp2,point;
    double random[POPULATION];  //染色体的变异概率
    int flag[POPULATION];       //染色体的变异情况
    for(int i=0;i<POPULATION;i++) {
        flag[i]=0;
    }
    //随机产生变异概率
    srand((unsigned)time(NULL));
    for(int i=0;i<POPULATION;i++)
    {
        random[i]=(rand()%100)/100.0;
    }
    //确定可以变异的染色体
    for(int i=0;i<POPULATION;i++)
    {
        if(random[i]<PM)
        {
            flag[i]=1;
        }
    }
    /*
     //适应度最低的路径发生变异
     int min_index = 0;
     for(int i=1;i<POPULATION;i++)
     {
     min_index = group[min_index].p>group[i].p?i:min_index;
     }
     flag[min_index]=1;
     */
    //变异操作,即交换染色体的两个节点
    srand((unsigned)time(NULL));
    for(int i=0;i<POPULATION;i++)
    {
        if(flag[i]==1)
        {
            temp1=rand()%GAS_STATION_NUM;
            temp2=rand()%GAS_STATION_NUM;
            point=group[i].dir[temp1];
            group[i].dir[temp1]=group[i].dir[temp2];
            group[i].dir[temp2]=point;
        }
        //判定是否符合条件，否则继续随机生成
        while(cal_T(group[i].dir)<0) {
            temp1=rand()%GAS_STATION_NUM;
            temp2=rand()%GAS_STATION_NUM;
            point=group[i].dir[temp1];
            group[i].dir[temp1]=group[i].dir[temp2];
            group[i].dir[temp2]=point;
        }
    }
}
/*****************************************************************************************
 * 评价函数
 * function:
	更新适应度、生存概率
	找出最优路径
 *****************************************************************************************/
void evaluate() {
    int i,j;
    //int n1,n2;
    double sumExpense, biggestsum=0;
    double biggestp=0;
    for(i=0; i<POPULATION; i++)
    {
        double T = cal_T(group[i].dir);
        while(T<0) {
            //当前个体里程超过800公理
            //随机生成一个方案填充于此并重新计算直至符合条件
            //产生GAS_STATION_NUM个不相同的数字
            
            for(j=0;j<GAS_STATION_NUM;) {
                int t=rand()%GAS_STATION_NUM+1; //随机生成取值范围1到GAS_STATION_NUM的GAS_STATION_NUM个数
                int flag=1;
                for(int k=0; k<j; k++) {
                    if(group[i].dir[k]==t) {
                        flag=0;
                        break;
                    }
                }
                if(flag) {
                    group[i].dir[j]=t;
                    j++;
                }
            }
            T = cal_T(group[i].dir);
        }
        double G = cal_G();
        double K = cal_K();
        
        sumExpense = G+K+T;
        group[i].adapt=sumExpense; //每条染色体的费用总和
        biggestsum+=sumExpense;    //种群的总费用
    }
    //计算染色体的幸存能力,费用越低生存概率越大
    for(i=0;i<POPULATION;i++)
    {
        group[i].p=1-(double)group[i].adapt/(double)biggestsum;
        biggestp+=group[i].p;
    }
    for(i=0;i<POPULATION;i++)
        group[i].p=group[i].p/biggestp;  //在种群中的幸存概率,总和为1
    //求最佳路径
    bestSln=0;
    for(i=0;i<POPULATION;i++)
        if(group[i].p>group[bestSln].p)
            bestSln=i;
    //打印适应度
    /*for(i=0;i<POPULATION;i++) {
    	for(int j=0; j<GAS_STATION_NUM; ++j){
     printf("%d ", group[i].dir[j]);
    	}
    	printf("染色体%d的费用与生存概率分别为%4lf  %.4f\n", i, group[i].adapt, group[i].p);
     }
     printf("当前种群的最优染色体是%d号染色体\n",bestSln);
     */
}
/*********************
 * 打印函数
 **********************/
void print_group() {
    for(int i=0; i<POPULATION; ++i) {
        for(int j=0; j<GAS_STATION_NUM; ++j){
            printf("%4d", group[i].dir[j]);
        }
        printf("  adapt:%4lf, p:%.4f\n", group[i].adapt, group[i].p);
    }
}

int main() {
    vector<struct_group> result;         //盛放不同x对应的最优解
    result.clear();
    for(x=3; x<31; ++x){
        generate_init_group();	//生成初始群落
        evaluate();				//评价初始种群
        int current_generation = 0;
        while(current_generation++ < MAX_GENERATION) {
            select();
            variate();
            evaluate();
        }
        struct_group temp;
        for (int i=0; i<GAS_STATION_NUM; ++i) {
            temp.dir[i]=group[bestSln].dir[i];
        }
        temp.p = group[bestSln].p;
        temp.adapt = group[bestSln].adapt;
        result.push_back(temp);
        
    }
    
    int min_index = 0;
    int count = (int)result.size();
    for(int i=0; i<count; ++i) {
        if (result[min_index].adapt > result[i].adapt) {
            min_index = i;
        }
    }
    
    //打印
    for(int i=0; i<count; ++i) {
        printf("x=%d时 ", i+3);
        int total_length = 0;
        for(int j=0; j<GAS_STATION_NUM; ++j){
            if (total_length==0) {
                total_length+=dist[0][result[i].dir[j]];
            } else {
                total_length+=dist[result[i].dir[j-1]][result[i].dir[j]];
            }
            if (total_length > MAX_DISTANCE) {
                printf("   %4d", result[i].dir[j]);
                total_length =0;
            }
            
            printf("%4d", result[i].dir[j]);
        }
        printf(" adapt:%4lf, p:%.4f\n", result[i].adapt, result[i].p);
    }
    
    cout << "最优批次方案为" << min_index+3 << " 批" << endl;
    
    //int arr[7] = {6,2,3,1,5,7,4};
    //cout << cal_T(arr) << " " << cal_K() << " " << cal_G() << " " << cal_T(arr)+cal_K()+cal_G() << endl;
    
    return 0;
}

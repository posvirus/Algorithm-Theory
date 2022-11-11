#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <math.h>
#include <chrono>
#include <iomanip>
using namespace std;
using namespace chrono;

int best_quantify_value(int &begin_of_data, int &end_of_data,int length_of_data);
int min_error(int &begin_of_data, int &end_of_data,int best_value);

int main()
{
    /********数据集的读入********/
    ifstream infile;
    infile.open("DATA.txt");
    vector<int> data;
    int temp_data;
    while(!infile.eof())
    {
        infile>>temp_data;
        data.push_back(temp_data);
    }
    infile.close();
    data.pop_back();
    /********数据预处理********/
    int s;//设定量化值
    cout<<"Please input the number of types of quantization: ";
    cin>>s;
    auto start=steady_clock::now();//开始计算程序运行时间
    sort(data.begin(),data.end());//对data[0:n-1]进行排序
    vector<vector<vector<int>>> result_of_quantization;//使用向量存储子问题的解，result_of_quantization[i][j]代表i个量化值，data[0:j-1]内的最优量化结果
    result_of_quantization.resize(s+1);//为子问题的解分配空间
    for(int i=1; i<=s; i++)
    {
        result_of_quantization[i].resize(data.size()+1);
    }
    /********子问题初始化********/
    for(int i=1; i<=s; i++)//初始化result_of_quantization[i][i]，i=1,2,3...s
    {
        result_of_quantization[i][i].assign(data.begin(),data.begin()+i);
        result_of_quantization[i][i].push_back(0);//将子问题解对应的最小误差值存储于向量末尾
    }
    for(int i=1; i<=(int)data.size(); i++)//初始化result_of_quantization[1][k]，k=1,2,3...n
    {
        temp_data=best_quantify_value(data.at(0),data.at(i-1),i);
        result_of_quantization[1][i].assign(i,temp_data);
        result_of_quantization[1][i].push_back(min_error(data.at(0),data.at(i-1),temp_data));//将子问题解对应的最小误差值存储于向量末尾
    }
    /********动态规划********/
    for(int value_of_s=2; value_of_s<=s; value_of_s++)//迭代求解子问题
    {
        for(int value_of_size=value_of_s+1; value_of_size<=(int)data.size(); value_of_size++)
        {
            int best_data=data[value_of_size-1];
            int min_value=result_of_quantization[value_of_s-1][value_of_size-1][value_of_size-1]+min_error(data.at(value_of_size-1),data.at(value_of_size-1),best_data);
            int target_size=value_of_size-1;
            for(int i=value_of_size-2; i>=(value_of_s-1); i--)
            {
                int temp=best_quantify_value(data.at(i),data.at(value_of_size-1),value_of_size-i);
                temp_data=result_of_quantization[value_of_s-1][i][i]+min_error(data.at(i),data.at(value_of_size-1),temp);
                if(min_value>temp_data)
                {
                    min_value=temp_data;
                    target_size=i;
                    best_data=temp;
                }
            }
            result_of_quantization[value_of_s][value_of_size]=result_of_quantization[value_of_s-1][target_size];//得出子问题的解
            result_of_quantization[value_of_s][value_of_size].pop_back();
            for(int i=1; i<=value_of_size-target_size; i++)
            {
                result_of_quantization[value_of_s][value_of_size].push_back(best_data);
            }
            result_of_quantization[value_of_s][value_of_size].push_back(min_value);//将子问题解对应的最小误差值存储于向量末尾
        }
    }
    /********计算程序运行时间********/
    auto finish=steady_clock::now();//终止计时
    auto duration=duration_cast<nanoseconds>(finish-start);
    cout<<"duration="<<double(duration.count())/1E9<<"s"<<endl;
    /********结果输出********/
    ofstream outfile;
    outfile.open("RESULT.txt",ios::out);

    for(int i=0; i<(int)data.size(); i++)
    {
        outfile<<result_of_quantization[s][data.size()][i]<<endl;
    }

    /********更详细的输出******/
    /*
        outfile<<"The number of types of quantization: "<<s<<endl;
        for(int i=0; i<(int)data.size(); i++)
        {
            outfile<<data[i]<<" is quantified to "<<result_of_quantization[s][data.size()][i]<<endl;
        }
        outfile<<"The min error is "<<result_of_quantization[s][data.size()][data.size()];
    */
    cout<<"The min error is "<<result_of_quantization[s][data.size()][data.size()];
    return 0;
}

int best_quantify_value(int &begin_of_data, int &end_of_data, int length_of_data)//用于求解对data[i:j]进行单值量化的最优解
{

    int *ptr=&begin_of_data;
    int best_value;
    double sum=0;
    double min_value;
    while(ptr!=&end_of_data)
    {
        sum+=*ptr;
        ptr++;
    }
    sum+=*ptr;
    sum=sum/length_of_data;//求解平均值
    ptr=&begin_of_data;
    min_value=abs(*ptr-sum);
    while(ptr!=&end_of_data)//判定与平均值相差最小的元素
    {
        if(abs(*ptr-sum)<=min_value)
        {
            min_value=abs(*ptr-sum);
            best_value=*ptr;
        }
        ptr++;
    }
    if(abs(*ptr-sum)<=min_value)
    {
        min_value=abs(*ptr-sum);
        best_value=*ptr;
    }
    return best_value;
}

int min_error(int &begin_of_data, int &end_of_data,int best_value)//用于求解对data[i:j]进行单值量化后的最小误差
{
    int min_e=0;
    int *ptr=&begin_of_data;
    while(ptr!=&end_of_data)
    {
        min_e=min_e+(*ptr-best_value)*(*ptr-best_value);
        ptr++;
    }
    min_e=min_e+(*ptr-best_value)*(*ptr-best_value);
    return min_e;
}

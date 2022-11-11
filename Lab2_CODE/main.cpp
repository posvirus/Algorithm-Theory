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
    /********���ݼ��Ķ���********/
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
    /********����Ԥ����********/
    int s;//�趨����ֵ
    cout<<"Please input the number of types of quantization: ";
    cin>>s;
    auto start=steady_clock::now();//��ʼ�����������ʱ��
    sort(data.begin(),data.end());//��data[0:n-1]��������
    vector<vector<vector<int>>> result_of_quantization;//ʹ�������洢������Ľ⣬result_of_quantization[i][j]����i������ֵ��data[0:j-1]�ڵ������������
    result_of_quantization.resize(s+1);//Ϊ������Ľ����ռ�
    for(int i=1; i<=s; i++)
    {
        result_of_quantization[i].resize(data.size()+1);
    }
    /********�������ʼ��********/
    for(int i=1; i<=s; i++)//��ʼ��result_of_quantization[i][i]��i=1,2,3...s
    {
        result_of_quantization[i][i].assign(data.begin(),data.begin()+i);
        result_of_quantization[i][i].push_back(0);//����������Ӧ����С���ֵ�洢������ĩβ
    }
    for(int i=1; i<=(int)data.size(); i++)//��ʼ��result_of_quantization[1][k]��k=1,2,3...n
    {
        temp_data=best_quantify_value(data.at(0),data.at(i-1),i);
        result_of_quantization[1][i].assign(i,temp_data);
        result_of_quantization[1][i].push_back(min_error(data.at(0),data.at(i-1),temp_data));//����������Ӧ����С���ֵ�洢������ĩβ
    }
    /********��̬�滮********/
    for(int value_of_s=2; value_of_s<=s; value_of_s++)//�������������
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
            result_of_quantization[value_of_s][value_of_size]=result_of_quantization[value_of_s-1][target_size];//�ó�������Ľ�
            result_of_quantization[value_of_s][value_of_size].pop_back();
            for(int i=1; i<=value_of_size-target_size; i++)
            {
                result_of_quantization[value_of_s][value_of_size].push_back(best_data);
            }
            result_of_quantization[value_of_s][value_of_size].push_back(min_value);//����������Ӧ����С���ֵ�洢������ĩβ
        }
    }
    /********�����������ʱ��********/
    auto finish=steady_clock::now();//��ֹ��ʱ
    auto duration=duration_cast<nanoseconds>(finish-start);
    cout<<"duration="<<double(duration.count())/1E9<<"s"<<endl;
    /********������********/
    ofstream outfile;
    outfile.open("RESULT.txt",ios::out);

    for(int i=0; i<(int)data.size(); i++)
    {
        outfile<<result_of_quantization[s][data.size()][i]<<endl;
    }

    /********����ϸ�����******/
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

int best_quantify_value(int &begin_of_data, int &end_of_data, int length_of_data)//��������data[i:j]���е�ֵ���������Ž�
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
    sum=sum/length_of_data;//���ƽ��ֵ
    ptr=&begin_of_data;
    min_value=abs(*ptr-sum);
    while(ptr!=&end_of_data)//�ж���ƽ��ֵ�����С��Ԫ��
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

int min_error(int &begin_of_data, int &end_of_data,int best_value)//��������data[i:j]���е�ֵ���������С���
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

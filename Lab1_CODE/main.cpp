#include <iostream>
#include <iomanip>
#include <fstream>
#include <chrono>

using namespace std;
using namespace chrono;

void Max_Sub_Array_cyc(int data[],int n);
long long int Max_Sub_Array_div(int num[],int sup,int inf,int &target_sup,int &target_inf);
long long int Max_Sub_Array_div_Cross(int num[],int sup,int inf,int mid,int &target_inf_mid,int &target_sup_mid);
void Max_Sub_Array_div_output(int data[],int n);

int main()
{
    static int data[1000000];//���ݼ�����ģ��ֱ�ӵ���
    int N=sizeof(data)/sizeof(int);
/********���ݼ��Ķ���********/
    ifstream infile;
    infile.open("DATA.txt");
    int *ptr=&data[0];
    while(!infile.eof())
    {
        infile>>*ptr;
        ptr++;
    }
    infile.close();
/********������������ȡ********/
    //Max_Sub_Array_cyc(data,N);//ʹ�ö���ѭ���㷨��ȡ
    Max_Sub_Array_div_output(data,N);//ʹ�÷����㷨��ȡ

    return 0;
}

void Max_Sub_Array_cyc(int num[],int n)//����ѭ���㷨
{
/********��������********/
    auto start=steady_clock::now();//��ʼ�����������ʱ��
    int sup=0;
    int inf=0;//ѭ����������Ͻ�inf���½�sup
    int target_sup=0;
    int target_inf=0;//�����������Ͻ�target_inf���½�target_sup
    long long int target_sum=0;//����������Ԫ�غ�
    long long int sum_temp=0;//�м����
/********ѭ�����********/
    for(sup=0;sup<n;sup++)
    {
        sum_temp=0;
        for(inf=sup;inf<n;inf++)
        {
            sum_temp+=num[inf];
            if(sum_temp>=target_sum)//���µ�ǰ����������
            {
                target_sum=sum_temp;
                target_inf=inf;
                target_sup=sup;
            }
        }
    }
/********�����������ʱ��********/
    auto finish=steady_clock::now();//��ֹ��ʱ
    auto duration=duration_cast<nanoseconds>(finish-start);
    cout<<"duration_cyc="<<double(duration.count())/1E9<<"s"<<endl;
/********������********/
    ofstream outfile;
    outfile.open("RESULT_cyc.txt",ios::out);
    outfile<<"target_sup="<<target_sup<<";target_inf="<<target_inf<<endl;
    outfile<<"Max_sum="<<target_sum<<endl;
    for(int j=target_sup;j<=target_inf;j++)
    {
        outfile<<"data["<<j<<"]="<<num[j]<<endl;
    }
}

long long int Max_Sub_Array_div(int num[],int sup,int inf,int &target_sup,int &target_inf)//�����㷨
{
    if(sup>=inf)//�㷨�ݹ���ֹ����
    {
        if(num[inf]<0)//���ֻʣһ��С��0��Ԫ��
        {
            target_sup=sup+1;
            target_inf=inf;
            return 0;
        }
        else
        {
            target_sup=sup;
            target_inf=inf;
            return num[inf];
        }
    }
/********��������********/
     int target_inf_left,target_sup_left,target_inf_right,target_sup_right,target_inf_mid,target_sup_mid;
     long long int target_sum_left=0;
     long long int target_sum_right=0;
     long long int target_sum_mid=0;
/********�ݹ����********/
    target_sum_left=Max_Sub_Array_div(num,sup,(sup+inf)/2,target_sup_left,target_inf_left);//�������
    target_sum_right=Max_Sub_Array_div(num,(sup+inf)/2+1,inf,target_sup_right,target_inf_right);//�Ұ�����
/********����ϲ�********/
    target_sum_mid=Max_Sub_Array_div_Cross(num,sup,inf,(sup+inf)/2,target_inf_mid,target_sup_mid);//��Խdata[mid]�����������
    long long int temp_result=max(max(target_sum_left,target_sum_right),target_sum_mid);//ȡ���ߵ����ֵ
    if(temp_result==target_sum_left)
    {
        target_inf=target_inf_left;
        target_sup=target_sup_left;
    }
    else if(temp_result==target_sum_right)
    {
        target_inf=target_inf_right;
        target_sup=target_sup_right;
    }
    else
    {
        target_inf=target_inf_mid;
        target_sup=target_sup_mid;
    }//ȷ����������е��±�

    return temp_result;
}

long long int Max_Sub_Array_div_Cross(int num[],int sup,int inf,int mid,int &target_inf_mid,int &target_sup_mid)//����Խdata[mid]�����������
{
    long long int target_sum_mid_l=0;
    long long int target_sum_mid_r=0;
    long long int sum_temp=0;
    target_sup_mid=mid+1;
    for(int i=mid;i>=sup;i--)
    {
        sum_temp+=num[i];
        if(sum_temp>target_sum_mid_l)
        {
            target_sup_mid=i;
            target_sum_mid_l=sum_temp;
        }
    }
    sum_temp=0;
    target_inf_mid=mid;
    for(int i=mid+1;i<=inf;i++)
    {
        sum_temp+=num[i];
        if(sum_temp>target_sum_mid_r)
        {
            target_inf_mid=i;
            target_sum_mid_r=sum_temp;
        }
    }
    return target_sum_mid_l+target_sum_mid_r;
}

void Max_Sub_Array_div_output(int data[],int n)//�����㷨�İ�װ�����
{
    auto start=steady_clock::now();//��ʼ�����������ʱ��
    long long int target_sum_div;
    int target_sup_div,target_inf_div;
    target_sum_div=Max_Sub_Array_div(data,0,n-1,target_sup_div,target_inf_div);
/********�����������ʱ��********/
    auto finish=steady_clock::now();//��ֹ��ʱ
    auto duration=duration_cast<nanoseconds>(finish-start);
    cout<<"duration_div="<<double(duration.count())/1E9<<"s"<<endl;
/********������********/
    ofstream outfile;
    outfile.open("RESULT_div.txt",ios::out);
    outfile<<"target_sup="<<target_sup_div<<";target_inf="<<target_inf_div<<endl;
    outfile<<"Max_sum="<<target_sum_div<<endl;
    for(int j=target_sup_div;j<=target_inf_div;j++)
    {
        outfile<<"data["<<j<<"]="<<data[j]<<endl;
    }
}

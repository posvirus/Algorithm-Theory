clear all % 清空工作区内存中的变量
close all % 关闭之前程序绘制的图像
clc % 清空之前程序在终端的输出
N=1000000;%设定序列规模
x=randi([-10*N,N*10],1,N);%生成范围为[-10N,10N]的均匀分布的随机整型序列
fileID=fopen('XLONG/DATA.txt','w');
fprintf(fileID,'%d\n',x);
fclose(fileID)%将生成的序列写入文本文件DATA.txt中
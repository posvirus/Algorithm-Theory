clear all % 清空工作区内存中的变量
close all % 关闭之前程序绘制的图像
clc % 清空之前程序在终端的输出
N=randi([50,100],1,1);%随机生成序列长度N，范围为[50,100]
x=randi([1,999],1,N);%生成范围为[1,999]的均匀分布的随机整型序列
fileID=fopen('C:\Users\86181\Desktop\Algotithm_Code\Lab2\Quantization\DATA.txt','w');
fprintf(fileID,'%d\n',x);
fclose(fileID)%将生成的序列写入文本文件DATA.txt中
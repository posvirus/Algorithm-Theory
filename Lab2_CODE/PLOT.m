clear all % 清空工作区内存中的变量
close all % 关闭之前程序绘制的图像
clc % 清空之前程序在终端的输出
x=load('Quantization\DATA.txt');x=x';
n=0:(length(x)-1);
subplot(3,1,1);%绘制原始序列图像
stem(n,x,'.');
title('原始序列data[1:N]');xlabel('n');ylabel('data[n]');
y=sort(x);
subplot(3,1,2);%绘制排序后序列图像
stem(n,y,'.');
title('排序后序列data[1:N]');xlabel('n');ylabel('data[n]');
subplot(3,1,3);%绘制量化序列图像
y=load('Quantization\RESULT.txt');y=y';
stem(n,y,'.');
title('量化后序列data[1:N]');xlabel('n');ylabel('data[n]');
clear all
clc
commandwindow
warning on verbose

x = -pi:.4:pi;
y = sin(x);

y = y';
y = (y*45)+75;
x = (x'*10)+50;

desired = [x,y,y,ones(length(x),1)*90]

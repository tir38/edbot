% function [] = FK()
% Foward Kinematics of the Barlett WAM and Wrist
% by Jason Atwood
% 2/3/2012
% requires:
% - geom3d library: http://www.mathworks.com/matlabcentral/fileexchange/24484
% - geom2d library: http://www.mathworks.com/matlabcentral/fileexchange/7844-geom2d
% - lsplane.m by I M Smith

clear all
close all
clc
commandwindow
warning on verbose

% read in text file
joint_space = textread('qdata2.txt');
joint_space(:,1) = joint_space(:,1) - 40;
joint_space(:,2) = joint_space(:,2) - 120;
joint_space(:,3) = joint_space(:,3) - 85;
joint_space(:,4) = joint_space(:,4) - 65
joint_space = deg2rad(joint_space)
[a,b] = size(joint_space);

% robot parameters
l1 = 15;    % mm
l2 = 95;
l3 = 100;
l4 = 90;
l5 = 35;

% input g_bt(0), calculation done by hand
g_bt_0 = [0 0 -1 0; 0 1 0 l1; 1 0 0 (l2+l3+l4+l5); 0 0 0 1];

% construct the twists (all twists are in matrix form. i.e. twist_hat)
w1 = [0, 0, 1]'
q1 = [0, l1, 0]'
w1hat = hatthis(w1)
v1 = -1*cross(w1,q1)
twist1 = [w1hat,v1;0,0,0,0]

w2 = [0 -1 0]';
q2 = [0 l1, l2]';
w2hat = hatthis(w2)
v2 = -1*cross(w2,q2)
twist2 = [w2hat,v2;0,0,0,0]

w3 = w2
q3 = [0,l1, l2+l3]'
w3hat = hatthis(w3)
v3 = -1*cross(w3,q3)
twist3 = [w3hat,v3;0,0,0,0]

w4 = w2
q4 = [0,l1, l2+l3+l4]'
w4hat = hatthis(w4)
v4 = -1*cross(w4,q4)
twist4 = [w4hat,v4;0,0,0,0]

% iteratively calculate g_bt(theta_i)
% For each iteration, I could pull the orientation values out of g_st(i),
% but I don't need them for this application.
for i=1:a
    g_bt_i = expm(twist1*joint_space(i,1))*expm(twist2*joint_space(i,2))*expm(twist3*joint_space(i,3))*expm(twist4*joint_space(i,4))*g_bt_0;
    
    p_st(i,1) = g_bt_i(1,4);
    p_st(i,2) = g_bt_i(2,4);
    p_st(i,3) = g_bt_i(3,4);
end

%plot movement
plot3(p_st(:,1),p_st(:,2),p_st(:,3));

% save tool path
matrix2txt(p_st,'tool_path');

% calculate plane of the white board
[x0, a, d, normd] = lsplane(p_st);
centroid_of_whiteboard = x0
normal_vector_of_whiteboard = a
error = normd
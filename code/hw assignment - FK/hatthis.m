function [output] = hatthis(input)
% Jason Atwood
% 2/17/2012
% takes in 3x1 vector and generates 3x3 cross product representation

output = [0, -input(3), input(2); input(3), 0, -input(1); -input(2), input(1), 0];

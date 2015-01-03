function [] = matrix2txt (matrix, filename)
filename = strcat(filename,'.txt')
fid = fopen(filename,'wt');
for ii = 1:size(matrix,1)
    fprintf(fid,'%g\t',matrix(ii,:));
    fprintf(fid,'\n');
end
fclose(fid)
% Jason Atwood
% 04/25/2012
clc
clear all
commandwindow
warning on verbose

% ===== OPEN SERIAL PORT ===============
s1 = serial('COM5','Baudrate',19200,'Parity','none','DataBits',8,'StopBits',1,'Terminator','LF');
fopen(s1); %open serial port
desired = textread('qdata2.txt') % desired joint angles
[a,b] = size(desired);
theta = zeros(a,b);
desired_speed = 20;
fprintf('Matlab says: Serial communication established.\n');
pause(1);

% ===== FLUSH SERIAL PORT ===============
fprintf('Matlab says: Flushing serial.\n');
while s1.BytesAvailable ~= 0
    warning off MATLAB:serial:fscanf:unsuccessfulRead % temporarily suppress timeout warning
    % fscanf probably WILL timeout because there might not be a terminating
    % character in the gibberish
    trash = fscanf(s1); % read in stray buffer and trash it
    warning on MATLAB:serial:fscanf:unsuccessfulRead; % unsupress warning
end
pause(1);
fprintf('==========================================\n');


%% ==== MAIN LOOP =======================
for j = 1:a;

hold = 0;
% ===== WRITE TO SERIAL PORT ===============
% remember, Arduino wants values as 5 numbers with commas in between. no
% brakets on beginning or end of string
joint_angles_str =  strcat(num2str(desired(j,1)),',',num2str(desired(j,2)),',',num2str(desired(j,3)),',',num2str(desired(j,4)),',',num2str(desired_speed));
fprintf(s1,joint_angles_str); % write serial
fprintf('Matlab says: Data sent to Arduino.\n');
pause(0.25);

while hold ~= 1
% ===== READ FROM SERIAL PORT ===============
while s1.BytesAvailable ~= 0 % infinite loop
    [data,~] = fscanf(s1); % read serial
    incomingString = str2num(data); % tries to convert all incoming strings to numbers
    if isempty(incomingString) == 0 % if that string really is a number, then will not be NULL
        fprintf('Matlab says: Current joint angles read in from Arduino:\n');
        fprintf(data)
        theta(j,:) = [incomingString]; % insert into row vector   
    end
end

if theta(j,:) == desired(j,:)
    fprintf('***** Matlaby says: Current joint angles match desired. *****\n\n');
    hold = 1;    
end

end

end
%%
fprintf('Matlab says: Waiting for Arduino to time out.\n');
pause(8);

% ===== CLOSE SERIAL PORT ===============
fprintf('==========================================\n');
fprintf('Matlab says: Done reading data.\n');
pause(1)
fclose(s1); % close serial port
fprintf('Matlab says: Serial communication closed.\n');
delete(s1);
  
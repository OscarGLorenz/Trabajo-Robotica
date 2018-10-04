function [A] = readArchivo (fileID)
fileID = fopen ('datos.txt', 'r');
A = fscanf(fileID,'%f');
fclose(fileID);
end


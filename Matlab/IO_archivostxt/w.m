function [A] = writeArchivo (fileID)

fileID = fopen ('salida.txt', 'w');

A = rand (1, 5); % PRUEBA
t=0; time = 0; % Inicializacion

for k=1:5
tic % Comienza a contar el tiempo
t = time + t; % Contador tiempo total en milisegundos
%HACE LO QUE QUIERAS 
fprintf(fileID,'%f %f \n',t, A(1, k)); % Guarda en el archivo el dato
pause(0.1) % Espera 100 milisegundos
time = toc * 1000; % Cuenta el tiempo en milisegundos 
end 

fclose(fileID)
global arduino;
arduino=serial('/dev/ttyACM2','BaudRate',9600);
fopen(arduino);
pause(2);

v = 10; %velocidad cartesiana
origen = [0; 600/sqrt(2); 0];
destino = [200; 300; 300];

% INICIO
ahora = origen;
[q1,q2,q3] = inversa(ahora(1), ahora(2), ahora(3));

clear pintar
close all

while norm(ahora-destino) > 1
    pintar(q1,q2,q3); plot3(destino(1),destino(2),destino(3),'ok');
    
    dir = (destino-ahora)/norm(destino-ahora) * v; % Velocidades articulares
    hold on
    qV = jacobiana(q1,q2,q3) \ dir;
    
    mandarComando(sprintf('J2 M%d %.3f',1,qV(1)));
    mandarComando(sprintf('J2 M%d %.3f',2,qV(2)));
    mandarComando(sprintf('J2 M%d %.3f',3,qV(3)));
    
    q1 = consultarPosicion(1);  
    q2 = consultarPosicion(2);
    q3 = consultarPosicion(3);

try
    [ahora(1), ahora(2), ahora(3)] = directa(q1,q2,q3);
catch
    fclose(arduino);
end

end


fclose(arduino);

function q = consultarPosicion(motor)
global arduino;
while arduino.TransferStatus ~= "idle"
   pause(0.001); 
end
fprintf(arduino,"J20 M%d\n",motor,'async');
fprintf("J20 M%d\n",motor);
pause(0.05);
q = fscanf(arduino,"D1 M%*d %f\n");
fprintf("D1 M%d %.3f\n",motor, q);    
end

function mandarComando(str)
global arduino;
while arduino.TransferStatus ~= "idle"
   pause(0.001); 
end
fprintf(arduino,"%s\n",str,'async');
fprintf("%s\n",str);
end

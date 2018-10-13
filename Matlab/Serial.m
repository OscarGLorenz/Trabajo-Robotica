global arduino;
arduino=serial('/dev/ttyACM11','BaudRate',9600);
fopen(arduino);
pause(2);

origen = [0; 600; 0];
destino = [200; 300; 300];

v = 50;

ahora = origen;
[q1,q2,q3] = inversa(ahora(1), ahora(2), ahora(3));
a = q1;b = q2; c = q3;
clear pintar
close all

for k=1:20
    pintar(a,b,c); plot3(destino(1),destino(2),destino(3),'ok');
    siguiente = (destino-origen)*k/20+origen;
    [q1,q2,q3] = inversa(siguiente(1), siguiente(2), siguiente(3));
    
    
    mandarComando(sprintf('J1 M%d %.3f',1,q1)); 
    mandarComando(sprintf('J1 M%d %.3f',2,q2));
    mandarComando(sprintf('J1 M%d %.3f',3,q3));

    a = consultarPosicion(1); 
    b = consultarPosicion(2);     
    c = consultarPosicion(3);   

try
    [ahora(1), ahora(2), ahora(3)] = inversa(a,b,c);
catch
    fclose(arduino);
    fwrite(2,'\nERROR\n');
end
    
end

pintar(a,b,c); plot3(destino(1),destino(2),destino(3),'ok');

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


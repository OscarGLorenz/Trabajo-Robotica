global arduino;
arduino=serial('/dev/ttyACM5','BaudRate',115200);
fopen(arduino);
pause(2);

mandarComando(sprintf('J1 M%d %f\n',1,20.0));
posicion = consultarPosicion(1)


fclose(arduino);


function q = consultarPosicion(motor)
global arduino;
fprintf(arduino,'J20 M%d\n',motor);
pause(0.01);
q = fscanf(arduino,sprintf('D1 M%d %%f\n',motor));
end

function mandarComando(str)
global arduino;
fprintf(arduino,'%s\n',str);
end


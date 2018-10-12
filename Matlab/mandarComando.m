function mandarComando(app,str)
global arduino;
%arduino=serial("COM5",'BaudRate',115200);
fprintf(arduino,'%s\n',str);
end


global ardu;
global x;
global y;
global z;
global pos;

ardu = serial('COM22','BaudRate',115200);
ardu.Terminator = 'LF';
ardu.OutputBufferSize = 1024;
ardu.BytesAvailableFcnMode =  'terminator';
ardu.BytesAvailableFcn = @(~,~) serialEvent;

fopen(ardu);

disp("PULSA HOME");
pause;
fprintf(ardu,"J0");

% %%Calibracion
sss = 5;
% pos = zeros(sss,sss,3);
% disp("VAMOS A 25");
% pause;
% fprintf(ardu,"J1 M1 25");
% 
% disp("PULSA MODO GÜIDADO");
% pause;
% fprintf(ardu,"J6");
% 
% disp("GUARDA PUNTO 1");
% pos(1,1,:) = [x-20 y-5 z];
% pause;
% 
% 
% disp("GUARDA PUNTO 2");
% pos(1,sss,:) = [x-20 y z];
% pause;
% 
% 
% disp("VAMOS A 225");
% pause;
% fprintf(ardu,"J6");
% pause(1);
% fprintf(ardu,"J1 M1 225");
% 
% disp("PULSA MODO GÜIDADO");
% pause;
% fprintf(ardu,"J6");
% 
% disp("GUARDA PUNTO 3");
% pos(sss,1,:) = [x+10 y+10 z];
% pause;
% 
% fprintf(ardu,"J6");
% %%Fin de calibracion
pos(1,1,1) = 237; pos(1,2,1) = 174; pos(1,3,1) = 121; pos(1,4,1) = 63; pos(1,5,1) = 5;
pos(2,1,1) = 237; pos(2,2,1) = 174; pos(2,3,1) = 121; pos(2,4,1) = 63; pos(2,5,1) = 5;
pos(3,1,1) = 237; pos(3,2,1) = 174; pos(3,3,1) = 121; pos(3,4,1) = 63; pos(3,5,1) = 5;
pos(4,1,1) = 237; pos(4,2,1) = 174; pos(4,3,1) = 121; pos(4,4,1) = 63; pos(4,5,1) = 5;
pos(5,1,1) = 237; pos(5,2,1) = 174; pos(5,3,1) = 121; pos(5,4,1) = 63; pos(5,5,1) = 5;

pos(1,1,2) = 232.0425; pos(1,2,2) = 232.0425; pos(1,3,2) = 232.0425; pos(1,4,2) = 232.0425; pos(1,5,2) = 232.0425;
pos(2,1,2) = 278.8285; pos(2,2,2) = 278.8285; pos(2,3,2) = 278.8285; pos(2,4,2) = 278.8285; pos(2,5,2) = 278.8285;
pos(3,1,2) = 335.6144; pos(3,2,2) = 335.6144; pos(3,3,2) = 335.6144; pos(3,4,2) = 335.6144; pos(3,5,2) = 335.6144;
pos(4,1,2) = 397.4003; pos(4,2,2) = 397.4003; pos(4,3,2) = 397.4003; pos(4,4,2) = 397.4003; pos(4,5,2) = 397.4003;
pos(5,1,2) = 459.1863; pos(5,2,2) = 459.1863; pos(5,3,2) = 459.1863; pos(5,4,2) = 459.1863; pos(5,5,2) = 459.1863;

pos(1,1,3) = -80; pos(1,2,3) = -80; pos(1,3,3) = -80; pos(1,4,3) = -80; pos(1,5,3) = -80;
pos(2,1,3) = -80; pos(2,2,3) = -80; pos(2,3,3) = -80; pos(2,4,3) = -80; pos(2,5,3) = -80;
pos(3,1,3) = -80; pos(3,2,3) = -80; pos(3,3,3) = -80; pos(3,4,3) = -80; pos(3,5,3) = -80;
pos(4,1,3) = -80; pos(4,2,3) = -80; pos(4,3,3) = -80; pos(4,4,3) = -80; pos(4,5,3) = -80;
pos(5,1,3) = -80; pos(5,2,3) = -80; pos(5,3,3) = -80; pos(5,4,3) = -80; pos(5,5,3) = -80;

% xx = linspace(pos(1,1,1),pos(sss,1,1),sss);
% yy = linspace(pos(1,1,2),pos(1,sss,2),sss);
% 
% [X, Y] = meshgrid(xx,yy);
% pos(:,:,1)=X;
% pos(:,:,2)=flipud(Y);
% pos(:,:,3)=-80;

disp("PULSA PA PALETISAH");

pause;
% i1=1;
% i2=1;
% for i1 = 1:5
%     for i2 = 1:5
%         w1 = [pos(i2,i1,1) pos(i2,i1,2) pos(i2,i1,3)]
%         coge([x y z],w1);
%         pause(7);
%         aparta;
%         pause(2);
%         come([x,y,z],w1);
%         pause(7);
%         aparta;
%         pause(2);
%     end
% end
poner([2,2],[2,3]);
comer([2,2],[3,2]);

% w1 = [pos(2,2,1) pos(2,2,2) pos(2,2,3)]
% w2 = [pos(2,3,1) pos(2,3,2) pos(2,3,3)]
% %%coge([x y z],w1);
% paletizar([x y z],w1,w2);
% %%come([x,y,z],w2);
% pause(10);
% w1 = [pos(2,3,1) pos(2,3,2) pos(2,3,3)]
% w2 = [pos(3,3,1) pos(3,3,2) pos(3,3,3)]
% paletizar([x y z],w1,w2);
% pause(10);
% w1 = [pos(3,3,1) pos(3,3,2) pos(3,3,3)]
% w2 = [pos(3,2,1) pos(3,2,2) pos(3,2,3)]
% paletizar([x y z],w1,w2);

disp(" SE ACABO");
pause;
fclose(ardu);

function aparta
global ardu;
fprintf(ardu,"J1 M2 94\n");
fprintf(ardu,"J1 M3 -70\n");
end

%Funcion para comer una lista de fichas
%Entrada 2 vectores de igual longitud (primera ficha tiene pos letras(1),numeros(1))
function comer(letras, numeros)
global pos;
global x;
global y;
global z;
for i = 1:length(letras)
    w = [pos(numeros(i),letras(i),1) pos(numeros(i),letras(i),2) pos(numeros(i),letras(i),3)];
    come([x,y,z],w);
    pause(6);
    if i == length(letras)
        aparta;
        pause(1);
    end
end
end

%Funcion para colocar fichas
%Entrada 2 vectores de igual longitud (primera ficha tiene pos letras(1),numeros(1))
function poner(letras, numeros)
global pos;
global x;
global y;
global z;
for i = 1:length(letras)
    w = [pos(numeros(i),letras(i),1) pos(numeros(i),letras(i),2) pos(numeros(i),letras(i),3)];
    coge([x,y,z],w);
    pause(6);
    aparta;
    pause(1);
end
end

function serialEvent()
global ardu;
global x;

global y;
global z;
r = fscanf(ardu,"%f %f %f");
[x,y,z]=directa(r(1),deg2rad(r(2)),deg2rad(r(3)));

end
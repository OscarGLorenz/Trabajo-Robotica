global ardu;
global x;
global y;
global z;
    

ardu = serial('COM22','BaudRate',115200);
ardu.Terminator = 'LF';
ardu.OutputBufferSize = 1024;
ardu.BytesAvailableFcnMode =  'terminator';
ardu.BytesAvailableFcn = @(~,~) serialEvent;
        



fopen(ardu);


disp("PULSA HOME");
pause;
fprintf(ardu,"J0");

sss = 5;

pos = zeros(sss,sss,3);

disp("VAMOS A 30");
pause;
fprintf(ardu,"J1 M1 30");

disp("PULSA MODO GÜIDADO");
pause;
fprintf(ardu,"J6");

disp("GUARDA PUNTO 1");
pos(1,1,:) = [x y z];
pause;


disp("GUARDA PUNTO 2");
pos(1,sss,:) = [x y z];
pause;


disp("VAMOS A 220");
pause;
fprintf(ardu,"J6");
pause(1);
fprintf(ardu,"J1 M1 220");

disp("PULSA MODO GÜIDADO");
pause;
fprintf(ardu,"J6");

disp("GUARDA PUNTO 3");
pos(sss,1,:) = [x y z];
pause;

fprintf(ardu,"J6");

xx = linspace(pos(1,1,1),pos(sss,1,1),sss);
yy = linspace(pos(1,1,2),pos(1,sss,2),sss);

[X, Y] = meshgrid(xx,yy);
pos(:,:,1)=X;
pos(:,:,2)=flipud(Y);
pos(:,:,3)=-80;

disp("PULSA PA PALETISAH");

pause;
w1 = [pos(2,2,1) pos(2,2,2) pos(2,2,3)]
w2 = [pos(4,4,1) pos(4,4,2) pos(4,4,3)]
coge([x y z],w1);
paletizar([x y z],w1,w2);
come([x,y,z],w2);
paletizar([x y z],w1,w2);


disp(" SE ACABO");
pause;
fclose(ardu);


function serialEvent()
global ardu;
global x;

global y;
global z;
r = fscanf(ardu,"%f %f %f");
[x,y,z]=directa(r(1),deg2rad(r(2)),deg2rad(r(3)));

end
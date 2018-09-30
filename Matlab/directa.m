function [x2,y2,z2,x1,y1,z1] = directa(q1,q2,q3)
% directa - Cinemática directa del robot
% 
%     Se le pasan las coordenadas articulares en radianes y milimetros
%     y entrega la posición del segundo miembro y del primero
%     
%     [x2,y2,z2] = directa(q1,q2,q3);
%     [x2,y2,z2,x1,y1,z1] = directa(q1,q2,q3);
%

%% Longitud de los brazos
l1 = 300;
l2 = 300;

%% Cinemática directa 
% Extremo del primer miembro
x1 = q1;
y1 = l1 * cos(q2);
z1 = l1 * sin(q2);

% Extremo del segundo miembro
x2 = x1;
y2 = y1 + l2 * cos(q3);
z2 = z1 + l2 * sin(q3);

end
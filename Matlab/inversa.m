function [q1, q2, q3] = inversa(x,y,z)
% inversa - Cinemática inversa del robot
% 
%     Se le pasan las coordenadas cartesianas en mm y calcula las
%     coordenadas articulares en mm y radianes
%     
%     [q1,q2,q3] = inversa(x,y,z);
%

%% Longitud de los brazos
l1 = 300;
l2 = 300;

%% Cinemática inversa
%Coseno del ángulo entre segmentos
cosG = (l1^2 + l2^2 - y.^2 - z.^2) ./ (2 * l1 * l2);
% Eje lineal
q1 = x;
% Angulo del primer motor
q2 = asin(l2 ./ sqrt(y.^2 + z.^2) .* sqrt(1 - cosG .* cosG)) + atan(z./y);
% Angulo del segundo motor
q3 = asin((z - l1 * sin(q2) ) ./ l2);
end

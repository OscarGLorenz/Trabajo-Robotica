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
q2 = atan2(l2 .* sqrt(1 - cosG .* cosG) , sqrt(y.^2 + z.^2 - l2^2 .* (1 - cosG .* cosG))) + atan2(z,y);
% Angulo del segundo motor
weirdZone = ( y.^ 2 +  (z+l2).^ 2 ) >= l1^2;
sign = 2*weirdZone - 1;
q3 = atan2(z - l1 * sin(q2), sign.*sqrt(l2^2 - (z - l1 * sin(q2) ).^2));
% c2 =  (y.^2 + z.^2 - l1^2 - l2^2 ) ./ (2 * l1 * l2);
% s2 = sqrt(1-c2.^2);
% beta = atan2(z, y);
% alfa = atan2((l1*s2), (l1+l2*c2));
% q2=-beta+alfa;
% q3 = q2 - atan2(s2,c2);

end

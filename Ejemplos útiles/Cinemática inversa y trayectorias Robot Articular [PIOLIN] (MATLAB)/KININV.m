close all

%% Ecuación paramétrica de una curva 3D
X = @(t) 0;
Y = @(t) 260*(cosd(t1)+cosd(t2));
Z = @(t) 260*(sind(t1)+sind(t2));

%Posición inicial
x1 = X(0); x2 = Y(0); x3 = Z(0);

%% Configuraciones de gráfica 3D
% Tamaño del gráfico
ax = axes('XLim',[-600 600],'YLim',[0 600],'ZLim',[0 600]);
% Mantener ejes
hold(ax,'on');
% Isómetrica
view(3)
% Rejilla
grid on
% Aspecto 1:1:1
daspect([1 1 1])

%% Uso de la ecuación paramétrica 3D
% i = A:p:B     Para t entre A y B con un paso p
for i=0:5:180
   % Gráfico del siguiente segmento de la curva paramétrica
   plot3([x1 X(i)], [x2 Y(i)], [x3 Z(i)],'g');
   % Actualizar las coordenadas
   x1 = X(i); x2 = Y(i); x3 = Z(i);
   % Cálculos geométricos
   f(x1,x2,x3,i); 
end

%% Cálculos geométricos
function f(iX,iY,iZ,i)
%% Longitud de los brazos
l1 = 260;
l2 = 260;

%% Cinemática inversa

%Radio en el plano XY
iRho = 0;
%Distancia del origen al extremo
l0 = sqrt(iY*iY+iZ*iZ);
%�?ngulo polar del primer segmento
t0 = 0;
%Coseno del ángulo entre segmentos
cosG = (1-((iY*iY+iZ*iZ)/2*l1*l1));
%�?ngulo azimutal del primer segmento
t2 = atand(iZ/iY)+90-acosd(1-(iY*iY+iZ*iZ)/(2*l1));
%�?ngulo barrido por el segundo segmento desde la posición de colinealidad con el primero
t3 = t2 - asin((iZ - l1 * sin(t2)) / l2);

%% Cinemática directa 
%(Posición del extremo del primer segmento)
A = [l1*cos(t0)*cos(t2) l1*sin(t0)*cos(t2) l1*sin(t2)];
%(Posición del extremo del segundo segmento)
B = [(l1*cos(t2)+l2*cos(t2-t3))*cos(t0) (l1*cos(t2)+l2*cos(t2-t3))*sin(t0) l1*sin(t2)+l2*sin(t2-t3)];

%plot (i,[t3 t2], '*'); hold on
%
% Gráfico del primer segmento
plot3([0 A(1)], [0 A(2)], [0 A(3)],'r'); hold on
% Gráfico del segundo segmento
plot3([A(1) B(1)] , [A(2) B(2)], [A(3) B(3)],'b');

end


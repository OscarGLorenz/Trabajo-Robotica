close all

%% Ecuación paramétrica de una curva 3D
X = @(t) 150*cosd(4*t)+200;
Y = @(t) 200;
Z = @(t) 150*sind(4*t)+100;

%Posición inicial
x1 = X(0); x2 = Y(0); x3 = Z(0);

%% Configuraciones de gráfica 3D
% Tamaño del gráfico
ax = axes('XLim',[-5 400],'YLim',[-100 500],'ZLim',[-100 500]);
% Mantener ejes
hold(ax,'on');
% Isómetrica
view(3)
% Rejilla
grid on
% Aspecto 1:1:1
daspect([1 1 1])
    
quiver3(0,0,0,100,0,0,'r','LineWidth',1,'MaxHeadSize',4);
quiver3(0,0,0,0,100,0,'g','LineWidth',1,'MaxHeadSize',4);
quiver3(0,0,0,0,0,100,'b','LineWidth',1,'MaxHeadSize',4);
        
%% Uso de la ecuación paramétrica 3D
% i = A:p:B     Para t entre A y B con un paso p
for i=0:2:90
   % Gráfico del siguiente segmento de la curva paramétrica
   plot3([x1 X(i)], [x2 Y(i)], [x3 Z(i)],'k');
   % Actualizar las coordenadas
   x1 = X(i); x2 = Y(i); x3 = Z(i);
   % Cálculos geométricos
   f(x1,x2,x3,i); 
end

xlabel('X(mm)');
ylabel('Y(mm)');
zlabel('Z(mm)');



%% Cálculos geométricos
function f(iX,iY,iZ,i)
%% Longitud de los brazos
l1 = 220;
l2 = 220;

%% Cinemática inversa

%Distancia del origen al extremo
l0 = sqrt(iY * iY + iZ * iZ);
%Coseno del ángulo entre segmentos
cosG = (l1 * l1 + l2 * l2 - l0 * l0) / (2 * l1 * l2);
%Ángulo azimutal del primer segmento
t2 = asin(l2 / l0 * sqrt(1 - cosG * cosG)) + atan(iZ/iY);
%Ángulo barrido por el segundo segmento desde la posición de colinealidad con el primero
t3 = t2 - asin((iZ - l1 * sin(t2)) / l2);

%% Cinemática directa 
%(Posición del extremo del primer segmento)
A = [iX l1*cos(t2) l1*sin(t2)];
%(Posición del extremo del segundo segmento)
B = [iX (l1*cos(t2)+l2*cos(t2-t3)) l1*sin(t2)+l2*sin(t2-t3)];

%plot (i,[t3 t2], '*'); hold on
%
% Gráfico del primer segmento
plot3([iX A(1)], [0 A(2)], [0 A(3)],'m'); hold on
% Gráfico del segundo segmento
plot3([A(1) B(1)] , [A(2) B(2)], [A(3) B(3)],'c');


end


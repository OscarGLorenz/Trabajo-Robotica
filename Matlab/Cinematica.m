close all

%% Ecuacion parametrica de una curva 3D
X = @(t) t;
Y = @(t) 50;
Z = @(t) 50;

%Posicion inicial
x1 = X(0); x2 = Y(0); x3 = Z(0);

%% Configuraciones de grafica 3D
% Tama�o del grafico
ax = axes('XLim',[-600 600],'YLim',[-600 600],'ZLim',[0 600]);
% Mantener ejes
hold(ax,'on');
% Isometrica
view(3)
% Rejilla
grid on
% Aspecto 1:1:1
daspect([1 1 1])

%% Uso de la ecuacion parametrica 3D
% i = A:p:B     Para t entre A y B con un paso p
for i=0:1:100
   % Gráfico del siguiente segmento de la curva parametrica
   %plot3([x1 X(i)], [x2 Y(i)], [x3 Z(i)],'g');
   % Actualizar las coordenadas
   x1 = X(i); x2 = Y(i); x3 = Z(i);
   % Calculos geometricos
   f(x1,x2,x3); 
end

%% Calculos geometricos
function f(iX,iY,iZ)
%% Longitud de los brazos
l1 = 220;
l2 = 220;

%% Cinemática inversa
%Angulo entre recta que une extremo y plano XY
alpha = tan(iZ/iY);
%Distancia del origen al extremo
l0 = sqrt(iX * iX + iY * iY + iZ * iZ);
%Coseno del angulo entre segmentos
cosG = 1 - ((sqrt(iY * iY + iZ * iZ)) / (2 * l1 * l2));
%Angulo azimutal del primer segmento
t1 = 0.5 * (3.14159 - acos(cosG));
%Angulo del segundo segmento respecto del plano horizontal
t2 = alpha + t1;

%% Cinematica directa 
%(Posicion del extremo del primer segmento)
A = [iX -l1*cos(t1) l1*sin(t1)];
%(Posicion del extremo del segundo segmento)
B = [iX -(l1*cos(t1)+l2*cos(t2)) -(l1*sin(t1)+l2*sin(t2))];

%plot (i,[t3 t2], '*'); hold on
%
% Grafico del primer segmento
plot3([0+iX A(1)], [0 A(2)], [0 A(3)],'r'); hold on
% Grafico del segundo segmento
plot3([A(1) B(1)] , [A(2) B(2)], [A(3) B(3)],'b');
plot3([A(1) iX] , [A(2) iY], [A(3) iZ],'g');

end
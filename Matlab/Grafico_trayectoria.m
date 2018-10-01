close all

%% Ecuación paramétrica de una curva 3D
X = @(t) 100*cosd(4*t)+120;
Y = @(t) 300;
Z = @(t) 100*sind(4*t);

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
   cr = 'k';   
   if constrains([X(i) Y(i) Z(i)]) == 0
       cr = 'r';
   end
   plot3([x1 X(i)], [x2 Y(i)], [x3 Z(i)],cr);
   % Actualizar las coordenadas
   x1 = X(i); x2 = Y(i); x3 = Z(i);
   % Cálculos geométricos
   f(x1,x2,x3); 
end

xlabel('X(mm)');
ylabel('Y(mm)');
zlabel('Z(mm)');



%% Cálculos geométricos
function f(x,y,z)
%% Longitud de los brazos
[q1, q2, q3] = inversa(x,y,z);
[x2,y2,z2,x1,y1,z1] = directa(q1,q2,q3);

% Gráfico del primer segmento
plot3([x1 x1], [0 y1], [0 z1],'m'); hold on
% Gráfico del segundo segmento
plot3([x1 x2] , [y1 y2], [z1 z2],'c');


end


function [pp1,pp2,pp3] = moveCartesianas(app,x0,xf,T,p)
% Esta función manda al robot a una posición en coordenadas cartesianas
%    x0: posición actual. Vector de 3 componentes
%    xf: posición final. Vector de 3 componentes
%    T: tiempo total en segundos
%    p: puntos de interpolación
%
%    pp1,pp2,pp3: polinomios de interpolación

% Muestrear en el espacio de la tarea
x1 = linspace(x0(1),xf(1),p);
x2 = linspace(x0(2),xf(2),p);
x3 = linspace(x0(3),xf(3),p);

% Muestrar el tiempo
t = linspace(0,T,p);

% Cinemática inversa de los puntos
[q1,q2,q3] = inversa(x1,x2,x3);

q2 = rad2deg(q2);
q3 = rad2deg(q3);

% Obtener los polinomios para coordenada articular con pendiente nula al
% principio y al final
pp1 = spline(t,[0 q1 0]);
pp2 = spline(t,[0 q2 0]);
pp3 = spline(t,[0 q3 0]);

% Generar mensaje en formato  d1 d2 d3 c1 c2 c3 b1 b2 b3 a1 a2 a3 t1 t2 t3
str1 = sprintf(' %3.3f',[pp1.coefs(:);t(2:end)']);
str2 = sprintf(' %3.3f',[pp2.coefs(:);t(2:end)']);
str3 = sprintf(' %3.3f',[pp3.coefs(:);t(2:end)']);

command = sprintf('J5 %d%s_%d%s_%d%s_', p-1, str1, p-1, str2, p-1, str3);
% Mandar comandos
mandarComando(app,command);

%Como hacer una gráfica de los resultados
% 
% % Gráficas q-t
% figure
% tt = linspace(0,T);
% subplot(3,1,1);
% plot(t,q1,'o',tt,ppval(pp1,tt),'-'); 
% xlabel('t [s]');
% ylabel('q1 [mm]');
% grid on
% 
% subplot(3,1,2);
% plot(t,q2,'o',tt,ppval(pp2,tt),'-');
% xlabel('t [s]');
% ylabel('q2 [deg]');
% grid on
% 
% subplot(3,1,3);
% plot(t,q3,'o',tt,ppval(pp3,tt),'-');
% xlabel('t [s]');
% ylabel('q3 [deg]');
% grid on
% 
% % Gráficas xyz con puntos de interpolación
% figure
% [x,y,z] = directa(ppval(pp1,tt),deg2rad(ppval(pp2,tt)),deg2rad(ppval(pp3,tt)));
% plot3(x,y,z,'-b'); hold on
% plot3(x1,x2,x3,'or');
% grid on

end


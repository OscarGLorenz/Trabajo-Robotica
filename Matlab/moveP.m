function moveP (app,x0,xf,xc,v,p)
% Mueve el robot en una circunferencia, dados puntos inicial, final e
% intermedio
%
%   x0: punto inicial en mm
%   xf: punto final en mm
%   xc: punto intermedio en mm
%   v:  velocidad media en mm
%   p: puntos de interpolación 

    A = [xc-x0 xc-xf cross(xc-x0,xc-xf)]';
    b = [0.5*dot(xc-x0,xc+x0); 0.5*dot(xc-xf,xc+xf); dot(cross(xc-x0,xc-xf),xc)];
    center = A\b;
    
    theta1 = acos(dot(x0-center,xc-center)/(norm(x0-center)*norm(xc-center)));
    theta2 = acos(dot(xc-center,xf-center)/(norm(xc-center)*norm(xf-center)));
    
    T = (theta1+theta2)*norm(center-x0)/v;
    
    c = (x0-center)/norm(x0-center);
    v = cross(x0-center,xc-center)/norm(cross(x0-center,xc-center));
    s = cross(v,c);
    
    
    thetas = linspace(0,theta1+theta2,p);
        
    X =  center + c * norm(center-x0) * cos(thetas) + s * norm(center-x0) * sin(thetas);
    
    % Muestrar el tiempo
    t = linspace(0,T,p);
    
    % Cinemática inversa de los puntos
    [q1,q2,q3] = inversa(X(1,:),X(2,:),X(3,:));
    
    q2 = rad2deg(q2);
    q3 = rad2deg(q3);
    
    % Obtener los polinomios para coordenada articular con pendiente nula al
    % principio y al final
    pp1 = spline(t,[0 q1 0]);
    pp2 = spline(t,[0 q2 0]);
    pp3 = spline(t,[0 q3 0]);
% 
%     % Generar mensaje en formato  d1 d2 d3 c1 c2 c3 b1 b2 b3 a1 a2 a3 t1 t2 t3
%     str1 = sprintf(' %3.3f',[pp1.coefs(:);t(2:end)']);
%     str2 = sprintf(' %3.3f',[pp2.coefs(:);t(2:end)']);
%     str3 = sprintf(' %3.3f',[pp3.coefs(:);t(2:end)']);
% 
%     command = sprintf('J5 %d%s_%d%s_%d%s_', p-1, str1, p-1, str2, p-1, str3);
%     % Mandar comandos
%     mandarComando(app,command);
%     
%     
%     
    

% Gráficas q-t
figure
tt = linspace(0,T);
subplot(3,1,1);
plot(t,q1,'o',tt,ppval(pp1,tt),'-'); 
xlabel('t [s]');
ylabel('q1 [mm]');
grid on

subplot(3,1,2);
plot(t,q2,'o',tt,ppval(pp2,tt),'-');
xlabel('t [s]');
ylabel('q2 [deg]');
grid on

subplot(3,1,3);
plot(t,q3,'o',tt,ppval(pp3,tt),'-');
xlabel('t [s]');
ylabel('q3 [deg]');
grid on

% Gráficas xyz con puntos de interpolación
figure
[x,y,z] = directa(ppval(pp1,tt),deg2rad(ppval(pp2,tt)),deg2rad(ppval(pp3,tt)));
plot3(x,y,z,'-b'); hold on
plot3(X(1,:),X(2,:),X(3,:),'or');
grid on
 plot3(x0(1),x0(2),x0(3),'og');
 plot3(xc(1),xc(2),xc(3),'om');
 plot3(xf(1),xf(2),xf(3),'oc');
  plot3(center(1),center(2),center(3),'oy');

   axis equal
end
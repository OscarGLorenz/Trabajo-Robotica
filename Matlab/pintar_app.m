function pintar_app(app,q1,q2,q3)

quiver3(app.UIAxes,0,0,0,100,0,0,'r','LineWidth',1,'MaxHeadSize',4);
quiver3(app.UIAxes,0,0,0,0,100,0,'g','LineWidth',1,'MaxHeadSize',4);
quiver3(app.UIAxes,0,0,0,0,0,100,'b','LineWidth',1,'MaxHeadSize',4);

% Borrar grafica anterior
hold off

%% Obtencion de parametros
[x2,y2,z2,x1,y1,z1] = directa(q1,q2,q3);

%% Dibujo
% Linea primer brazo
plot3(app.UIAxes,[x2 x1],[y2 y1],[z2 z1], 'b'); hold on
% Linea segundo brazo
plot3(app.UIAxes,[x1 q1], [y1 0],[z1 0], 'g'); hold on
% Rejilla
grid on
% Titulos
xlabel(app.UIAxes,'x(mm)');
ylabel(app.UIAxes,'y(mm)');
zlabel(app.UIAxes,'z(mm)');

%% Guardar anterior
persistent x;
persistent y;
persistent z;
persistent i;
if isempty(i)
    i=1;
    x=0;
    y=0;
    z=0;
end
x(i) = x2;
y(i) = y2;
z(i) = z2;
i = i + 1;

plot3(app.UIAxes,x, y, z,'r');
axis equal

quiver3(app.UIAxes,0,0,0,100,0,0,'r','LineWidth',1,'MaxHeadSize',4);
quiver3(app.UIAxes,0,0,0,0,100,0,'g','LineWidth',1,'MaxHeadSize',4);
quiver3(app.UIAxes,0,0,0,0,0,100,'b','LineWidth',1,'MaxHeadSize',4);

end
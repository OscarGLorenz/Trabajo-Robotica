function pintar(q1,q2,q3)

% Borrar grafica anterior
hold off

%% Obtencion de parametros
[x2,y2,z2,x1,y1,z1] = directa(q1,q2,q3);

%% Dibujo
% Linea primer brazo
plot3([x2 x1],[y2 y1],[z2 z1], 'b'); hold on
% Linea segundo brazo
plot3([x1 q1], [y1 0],[z1 0], 'g'); hold on
% Rejilla
grid on
% Titulos
xlabel('x(mm)');
ylabel('y(mm)');
zlabel('z(mm)');

end
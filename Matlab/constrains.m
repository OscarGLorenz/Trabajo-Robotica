function inside = constrains(x)
% constrains - Comprueba si un punto en coordenadas cartesianas está dentro
%               del espacio de trabajo
% 
%     Devuelve un 0 o un uno en función si esta en el interior o no del
%     espacio de trabajo
%     
%     inside = constrains(x)
%

q2lim = deg2rad([0, 94]);  % 6 <= q2 <= 115
q3lim = deg2rad([-120,10]); % -116 <= q3 <= 13
rlim = [156, 589];      % 30 <= q2-q3 <=158
q1lim = [0, 250];  % 0 <= q1 <= 250
zlim = -130;
ylim = 0;

[q(1), q(2), q(3)] = inversa(x(1),x(2),x(3));
inside = 1;
if x(3) < zlim
    inside = 0;
elseif q(2) < q2lim(1) || q(2) > q2lim(2)
    inside = 0;
elseif q(3) < q3lim(1) || q(3) > q3lim(2)
    inside = 0;
elseif norm([x(2) x(3)]) < rlim(1) || norm([x(2) x(3)]) > rlim(2)
    inside = 0;
elseif q(1) < q1lim(1) || q(1) > q1lim(2)
    inside = 0;
elseif x(2) < ylim
    inside = 0;
end
    
end

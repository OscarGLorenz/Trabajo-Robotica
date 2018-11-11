
% Calcular coeficientes de los splines
function [a,b,c,d] =  splines(q,t)

% Numero de puntos
n = length(q);

% Matriz del sistema
A = eye(n);
% Término independiente
y = zeros(n,1);

% Coeficientes del polinomio, hay n-1 intervalos
a = zeros(n-1,1);
b = zeros(n-1,1);
c = zeros(n-1,1);
d = zeros(n-1,1);

% las ecuaciones dq(1) = 0 y dq(n) = 0, se expresan añadiendo una fila con
% esa condición. Las filas 2 hasta la n-1 se rellenan como la ecuación [6.7]
for i=2:n-1
    % Se ponen los tres términos no nulos en cada fila
   A(i,i-1:i+1) = [t(i+1) 2*(t(i)+t(i+1)) t(i)];
    % Se ponen los términos independientes
   y(i) = 3/(t(i+1)*t(i)) * ( t(i)^2*(q(i+1)-q(i)) + t(i+1)^2*(q(i)-q(i-1)) );
end

% Se obtienen las velocidades de paso
dq = A \ y;

% Obtener los coeficientes de los n-1 intervalos
for i=1:n-1
   % Ecuación [6.5] del libro
   T = t(i+1) - t(i);
   a(i) = q(i);
   b(i) = dq(i);
   c(i) = 3/T^2* (q(i+1) - q(i)) - 1/T   * (dq(i+1) + 2*dq(i));
   d(i) = -2/T^3*(q(i+1) - q(i)) + 1/T^2 * (dq(i+1) +   dq(i));
end

end
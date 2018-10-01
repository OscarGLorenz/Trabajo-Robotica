function [vol] = Montecarlo(funcion,dimension,puntos,limites)
% Montecarlo - Cálculo de superficie,volumen, etc... por el método de
%               Montecarlo
% 
%     Se debe dar la función que sea la frontera del espacio a medir como
%     una función que dado un vector devuelva si esta dentro o no (1 ó 0)
%     Se debe dar la dimensión, 2 para área, 3 para volumen
%     El número de puntos a probar, la precisión es 1/sqrt(puntos)
%     Los límites del volumen del cuadrado, cubo,... en el que se introduce
%     el cuerpo a medir
%     
%     [vol] = Montecarlo(funcion,dimension,puntos,limites)
%

    puntosDentro = 0;  % Contador
    for i = 1:puntos
        vector = limites(1) + rand(dimension, 1).*(limites(2)-limites(1));
        if funcion(vector) == 1
            puntosDentro = puntosDentro + 1;
            %plot(vector(2),vector(3),'.b'); Dibujar puntos
        end
    end
    vol = puntosDentro/puntos * (limites(2)-limites(1))^dimension;
end
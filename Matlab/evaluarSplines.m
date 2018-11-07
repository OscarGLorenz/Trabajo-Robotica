function [tt,qq] = evaluarSplines(a,b,c,d,t)
tt = linspace(te(1),t(end)); % Muestrear desde el principio hasta el final
qq = zeros(size(tt));       % Vector vacio del mismo tamaño

inte = 1;                    % Intervalo actual
for i=1:length(tt)          % Iterar para todos los valores de muestreo
    if tt(i) > t(inte+1)     % Ver si hemos pasado al siguiente intervalo de tiempo
       inte = inte + 1;       % Sumar 1 al intervalo en ese caso
    end
    
    % Evaluar el polinomio en este intervalo y con el tiempo actual
    % Ecuación [6.5] del libro
    qq(i) =   a(inte) ...
            + b(inte) * (tt(i)-t(inte)) ...
            + c(inte) * (tt(i)-t(inte))^2 ...
            + d(inte) * (tt(i)-t(inte))^3;
end
% Funciones cinemáticas
%---------------------

%Cinemática directa%
function [x,y,z] = Cinematica(xx,yy,zz,D)
    if D == 1 
        %Cinematica Directa, devuelve coordenadas cartesianas y los
        %parametros de entrada son xx (eje lineal) yy (angulo primer
        %segmento con el plano XY) y zz (angulo segundo segmento con el
        %plano XY)
        x=xx;
        y=-(260*cos(yy)+260*cos(zz));
        z=260*sin(yy)+260*sin(zz);
    %Longitud ambos brazos 260 mm%
    else if D == 0
        %Cinematica inversa%
        x =xx;
        y =0;
        z =0;
        else disp('ERROOR');
        end
    end
end
%Cinemática inversa%

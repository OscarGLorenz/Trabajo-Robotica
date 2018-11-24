function moveL (app,x0,xf,v,p)
% Mueve el robot de un punto en linea recta entre el punto
% inicial y el final.
%
%   x0: punto inicial en mm
%   xf: punto final en mm
%   v: velocidad media en mm
%   p: puntos de interpolación

    T = norm(xf-x0)/v;
    moveCartesianas(app,x0,xf,T,p);
    
end
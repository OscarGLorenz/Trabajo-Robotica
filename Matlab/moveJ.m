function moveJ (app,x0,xf,v)
% Mueve el robot de un punto a otro usando una splines entre el punto
% inicial y el final.
%
%   x0: punto inicial en mm
%   xf: punto final en mm
%   v:  velocidad media en mm

    T = norm(xf-x0)/v;
    moveCartesianas(app,x0,xf,T,2);
    
end
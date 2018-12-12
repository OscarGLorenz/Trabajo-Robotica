function poner(letras, numeros)
global pos;
global x;
global y;
global z;
for i = 1:length(letras)
    w = [pos(numeros(i),letras(i),1) pos(numeros(i),letras(i),2) pos(numeros(i),letras(i),3)];
    coge([x,y,z],w);
    pause(6);
    aparta;
    pause(1);
end
end
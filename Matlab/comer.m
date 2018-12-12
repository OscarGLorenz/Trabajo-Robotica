function comer(letras, numeros)
global pos;
global x;
global y;
global z;
for i = 1:length(letras)
    w = [pos(numeros(i),letras(i),1) pos(numeros(i),letras(i),2) pos(numeros(i),letras(i),3)];
    come([x,y,z],w);
    pause(6);
    if i == length(letras)
        aparta;
        pause(1);
    end
end
end
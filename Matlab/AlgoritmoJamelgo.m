global A;

A = zeros(6,6);

A(1,1) = 1;
A(1,6) = -1;
A(6,1) = 1;
A(6,6) = -1;

dibujah();


juega = rand > 0.5;



while(  (sum(A(:) == -1) ~= 0) &&   (sum(A(:) == 1) ~= 0)) 
        dibujah;
    juega = ~juega;
    
    if (juega) 
        hemovido = 0;
        nfichas = sum(sum(A==-1));
                
        [filas,columnas] =  find(A==-1,nfichas);
        
        ficha1mov = opciones(filas(1),columnas(1));
        for idx=1:length(ficha1mov) 
            mov = ficha1mov(idx,:);
            if (A(mov(1),mov(2)) == 1)
                A(mov(1),mov(2)) = -1;
                A(filas(1),columnas(1)) = 0;
                 hemovido = 1;
                break;
            end
        end
        
        if (hemovido)
             continue;
        end
            
        if (nfichas == 2)
            ficha2mov = opciones(filas(2),columnas(2));
            for idx=1:length(ficha2mov)
                mov = ficha2mov(idx,:);
                if (A(mov(1),mov(2)) == 1)
                    A(mov(1),mov(2)) = -1;
                    A(filas(2),columnas(2)) = 0;
                    hemovido = 1;
                    break;
                end
            end
        end
        
        if (hemovido)
             continue;
        end
        

        ficha1mov = opciones(filas(1),columnas(1));
        rnd = randi(length(ficha1mov));
        mov = ficha1mov(rnd,:);
        
        A(mov(1),mov(2)) = -1;
        A(filas(1),columnas(1)) = 0;
        hemovido = 1;
        
        
    else
        while 1
         i = input('Mover la pieza de x:\n');
         j = input('Mover la pieza de y:\n');

         k = input('Mover la pieza a x:\n');
         l = input('Mover la pieza a y:\n');
         
         if (A(i,j) == 1 && A(k,l) ~= 1)
             if (sum(ismember(opciones(i,j),[k l],'rows') == 1)) 
                A(k,l) = 1;
                A(i,j) = 0;
                break;
             end
         end
             disp('CAAACA ESO NO ES TUYO O NO HAY NADA');  
         
        end
        
         

        
    end

end
dibujah;


if (sum(A(:) == 1) ~= 0) 
    disp('HAS GANADO, PERO ERES FEO');
else
    disp('¿TE HAS DEJADO GANAR, VERDAD?');
end
















function op = opciones(i,j)
    movimientos = [i+2 j+1; 
          i+1 j+2; 
          i-1 j+2; 
          i+2 j-1;
          i-2 j+1;
          i+1 j-2;
          i-1 j-2;
          i-2 j-1];
      
   value = (movimientos <= [6,6]) & (movimientos >= [1,1]);
   valido = value(:,1) & value(:,2);
   
   op = movimientos(valido,:);
end




function dibujah()
global A;
hold off
axis equal
for i=1:6
   plot([-0.5+i -0.5+i],[0.5 6.5],'-k'); hold on
   plot([0.5 6.5],[-0.5+i -0.5+i],'-k');
   for j=1:6
       if (A(i,j) == 1)
            plot(i,j,'or');
       elseif(A(i,j) == -1)
            plot(i,j,'xb');
       end
   end
end
   plot([-0.5+7 -0.5+7],[0.5 6.5],'-k');
   plot([0.5 6.5],[-0.5+7 -0.5+7],'-k');
   xlabel('columna');
   ylabel('fila');
   title('columna-fila');
end
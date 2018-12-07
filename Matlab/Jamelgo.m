close all

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                            SETUP
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

if exist('vid','var') == 0
    vid = videoinput('linuxvideo',2,'YUY2_640x480');
    % Set the properties of the video object
    set(vid, 'FramesPerTrigger', Inf);
    set(vid, 'ReturnedColorspace', 'rgb')
    vid.FrameGrabInterval = 5;
end

%% Espera para adecuación de la cámara a la luz ambiente
tic
preview(vid);
while toc < 2
end

%% Deshacer prespectiva
im =getsnapshot(vid);

if exist('a','var') == 0
    imshow(im);
    title('Selecciona las esquinas y pulsa enter');
    [x,y] = getpts;
    a = [x y];
    title('');
    b = (  ((a - mean(a)) > 0) .* [639 479]  ) +1 ; % Maximizar
    my_tform    = fitgeotrans(a,b,'projective'); % Matriz de transformación
    
end
J = imwarp(im,my_tform,'cubic','OutputView', imref2d( size(im) ));

figure
imshow(J);
hold on

if exist('red','var') == 0
        title('Selecciona algo rojo y pulsa enter');
        [x,y] = getpts;
        r = impixel(J,round(x),round(y));
        threshold = 30;
        red = struct('RL',r(1)-threshold,'RH',r(1)+threshold,...
                     'GL',r(2)-threshold,'GH',r(2)+threshold,...
                     'BL',r(3)-threshold,'BH',r(3)+threshold);
        title('Selecciona algo verde y pulsa enter');
        [x,y] = getpts;
        v = impixel(J,round(x),round(y));
        green = struct('RL',v(1)-threshold,'RH',v(1)+threshold,...
                       'GL',v(2)-threshold,'GH',v(2)+threshold,...
                       'BL',v(3)-threshold,'BH',v(3)+threshold);
            
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                        END             SETUP
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                            	 ALGORITMO
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
juega = 0; % Empieza la raza inferior

M = getMatrix(J,red,green);
dibujah(M);

while(  (sum(M(:) == -1) ~= 0) &&   (sum(M(:) == 1) ~= 0)) 
    im =getsnapshot(vid);
    J = imwarp(im,my_tform,'cubic','OutputView', imref2d( size(im) ));
    M = getMatrix(J,red,green);
    dibujah(M);

    juega = ~juega;
    
    if (juega)  % Raza superior
        hemovido = 0;
        nfichas = sum(sum(M==-1));
                
        [filas,columnas] =  find(M==-1,nfichas);
        
        ficha1mov = opciones(filas(1),columnas(1));
        for idx=1:length(ficha1mov) 
            mov = ficha1mov(idx,:);
            if (M(mov(1),mov(2)) == 1)
                %M(mov(1),mov(2)) = -1;
                %M(filas(1),columnas(1)) = 0;
                fprintf("Mueve desde %d,%d a %d,%d\n",   [filas(1) columnas(1) mov(1) mov(2)]);
                pause;
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
                if (M(mov(1),mov(2)) == 1)
                    %M(mov(1),mov(2)) = -1;
                    %M(filas(2),columnas(2)) = 0;
                    fprintf("Mueve desde %d,%d a %d,%d\n",   [filas(2) columnas(2) mov(1) mov(2)]);
                    pause;
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
        
        %M(mov(1),mov(2)) = -1;
        %M(filas(1),columnas(1)) = 0;
        fprintf("Mueve desde %d,%d a %d,%d\n",   [filas(1) columnas(1) mov(1) mov(2)]);
        pause
        
        hemovido = 1;
        
        
    else % Raza inferior
        disp('RAZA INFERIOR MUEVE UNA PIEZA ROJA');
        pause;
    end

end


if (sum(M(:) == 1) ~= 0) 
    disp('HAS GANADO, PERO ERES FEO');
else
    disp('¿TE HAS DEJADO GANAR, VERDAD?');
end




 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                            	 ALGORITMO
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

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

function dibujah(A)
hold off
axis equal
for i=1:6
   plot([-0.5+i -0.5+i],[0.5 6.5],'-k'); hold on
   plot([0.5 6.5],[-0.5+i -0.5+i],'-k');
   for j=1:6
       if (A(i,j) == 1)
            plot(i,j,'or','MarkerSize',20);
       elseif(A(i,j) == -1)
            plot(i,j,'*g','MarkerSize',20);
       end
   end
end
   plot([-0.5+7 -0.5+7],[0.5 6.5],'-k');
   plot([0.5 6.5],[-0.5+7 -0.5+7],'-k');
   xlabel('columna');
   ylabel('fila');
   title('columna-fila');
end

function M = getMatrix(J,red,green)

%% Detectar colores
redBand = J(:,:,1);
greenBand =  J(:,:,2);
blueBand =  J(:,:,3);

%% ROJO
redMask = (redBand >= red.RL) & (redBand <= red.RH);
greenMask = (greenBand >= red.GL) & (greenBand <= red.GH);
blueMask = (blueBand >= red.BL) & (blueBand <= red.BH);
objectsMask =redMask & greenMask & blueMask;

% Detectar objetos
% Eliminar bloques de color pequeños
openBlocks = bwareaopen(objectsMask, 200);
    
% Reconocer bloques y guardar posición y controno   
redStats = regionprops(openBlocks, 'BoundingBox', 'Centroid');
 for object = 1:length(redStats)
       bb = redStats(object).BoundingBox;
       bc = redStats(object).Centroid;
             
       rectangle('Position', bb, 'EdgeColor', 'r', 'LineWidth', 2);
       plot(bc(1),bc(2), '-m+');
 end
 
%% VERDE
redMask = (redBand >= green.RL) & (redBand <= green.RH);
greenMask = (greenBand >= green.GL) & (greenBand <= green.GH);
blueMask = (blueBand >= green.BL) & (blueBand <= green.BH);
objectsMask =redMask & greenMask & blueMask;

% Detectar objetos
% Eliminar bloques de color pequeños
openBlocks = bwareaopen(objectsMask, 200);
    
% Reconocer bloques y guardar posición y controno   
greenStats = regionprops(openBlocks, 'BoundingBox', 'Centroid');
for object = 1:length(greenStats)
      bb = greenStats(object).BoundingBox;
      bc = greenStats(object).Centroid;
            
      rectangle('Position', bb, 'EdgeColor', 'g', 'LineWidth', 2);
      plot(bc(1),bc(2), '-m+');
end
 
  
 %% Generar matriz
M = zeros(6,6);
lx = linspace(639/12,639*11/12,6);
ly = linspace(480/12,480*11/12,6);

for object = 1:length(redStats)
      bc = redStats(object).Centroid;
      [~,i] = min(abs(bc(1)-lx));
      [~,j] = min(abs(bc(2)-ly));
      M(i,j) = 1;
end

for object = 1:length(greenStats)
      bc = greenStats(object).Centroid;
      [~,i] = min(abs(bc(1)-lx));
      [~,j] = min(abs(bc(2)-ly));
      M(i,j) = -1;
end
M = fliplr(M); 



end

%% Definir cámara
if exist('vid','var') == 0
    vid = videoinput('linuxvideo',1,'YUY2_640x480');
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
end

my_tform    = fitgeotrans(a,b,'projective'); % Matriz de transformación
% Deformar la imagen
J = imwarp(im,my_tform,'cubic','OutputView', imref2d( size(im) ));


%% Detectar colores
figure
imshow(J);
hold on

yellow = struct('RL',190,'RH',240,'GL',120,'GH',190,'BL',100,'BH',170);

redBand = J(:,:,1);
greenBand =  J(:,:,2);
blueBand =  J(:,:,3);

% Filtrar colores
redMask = (redBand >= yellow.RL) & (redBand <= yellow.RH);
greenMask = (greenBand >= yellow.GL) & (greenBand <= yellow.GH);
blueMask = (blueBand >= yellow.BL) & (blueBand <= yellow.BH);
objectsMask =redMask & greenMask & blueMask;


%% Detectar objetos
% Eliminar bloques de color pequeños
openBlocks = bwareaopen(objectsMask, 200);
    
% Reconocer bloques y guardar posición y controno   
stats = regionprops(openBlocks, 'BoundingBox', 'Centroid');
 for object = 1:length(stats)
       bb = stats(object).BoundingBox;
       bc = stats(object).Centroid;
             
       rectangle('Position', bb, 'EdgeColor', 'r', 'LineWidth', 2);
       plot(bc(1),bc(2), '-m+');
 end
  
 %% Generar matriz
M = zeros(6,6);
lx = linspace(640/12,640*11/12,6);
ly = linspace(480/12,480*11/12,6);

 for object = 1:length(stats)
       bc = stats(object).Centroid;
       
       [~,i] = min(abs(bc(1)-lx));
       [~,j] = min(abs(bc(2)-ly));
       M(i,j) = 1;
 end
 
M

close all;
global camera; %camara activa
camera = 1;

global IM; % Contenedor de imagenes
% Contenedor para imagenes de 480x640 con los 3 canales RGB y las 2
% cámaras
IM = zeros(480,640,3,2);

global take;
% Nombres de las caras para ir pidiendo en la configuración de la cámara,
% ver notación más abajo, solo pegatinas visibles por las cámaras
take = [...
    "R1", "R2", "R3", "R4", "R6", "R7", "R8",...
    "F1", "F2", "F3", "F4", "F6", "F8", "F9",...
    "U2", "U3", "U4", "U6", "U7", "U8", "U9",...
    "B2", "B3", "B4", "B6", "B7", "B8", "B9",...
    "D1", "D2", "D4", "D6", "D7", "D8", "D9",...
    "L1", "L2", "L4", "L6", "L7", "L8", "L9"];

% Webcams
global camera1;
global camera2;
% Primer uso de la camara
global firstCamera;
firstCamera = 0;

% Crear figura, maximizar, no mostrar titulo, no mostrar menu y callback de
% cerrar
figure('units','normalized','outerposition',[0 0 1 1],...
    'NumberTitle','off','CloseRequestFcn',@my_closereq)

%% Inicialización de cámaras
% Si se usan las camaras por primera vez
if exist('cam','var') == 0
    %Lista de camaras
    list = webcamlist;
    
    % Buscar camaras
    cameras = strcmp('Webcam C170: Webcam C170',list);
    
    % Buscar camara1
    for i=1:length(list)
        if cameras(i)
            camera1 = webcam(i);
            break;
        end
    end
    % Buscar camara 2
    for i=i+1:length(list)
        if cameras(i)
            camera2 = webcam(i);
            break;
        end
    end
    
    updateCamera; %Actualizar camara
    hold on
    
    c = 0; % Boton pulsado para elegir si las camaras estan bien o mal
    
    subplot(1,2,1);
    title("CAMARA SUPERIOR");
    
    subplot(1,2,2);
    title("CAMARA INFERIOR");
    
    % Botones para decidir si las camaras estan bien o mal
    btn1 = uicontrol('Style', 'pushbutton', 'String', 'CORRECTO',...
        'Position', [20 20 300 60],...
        'Callback', 'c=1;','FontSize',20);
    btn2 = uicontrol('Style', 'pushbutton', 'String', 'INVERTIR',...
        'Position', [350 20 300 60],...
        'Callback', 'c=2;','FontSize',20);
    
    % Espera para decidir
    while c==0
        pause(0.1);
    end
    
    if (c==2)
        % Intercambiar las cámaras
        camera3 = camera1;
        camera1 = camera2;
        camera2 = camera3;
    end
    
    % Borrar botones
    delete(btn1);
    delete(btn2);
    
    cam = 1; % No volver a entrar en este bloque de código
    updateCamera;
end

hold on

% Crear botones con sus callbacks
btn1 = uicontrol('Style', 'pushbutton', 'String', 'ACTUALIZAR',...
    'Position', [20 20 300 60],...
    'Callback', @updateCamera,'FontSize',20);
btn2 = uicontrol('Style', 'pushbutton', 'String', 'NUEVAS POSICIONES',...
    'Position', [350 20 300 60],...
    'Callback', @newPositions,'FontSize',20);
btn3 = uicontrol('Style', 'pushbutton', 'String', 'NUEVOS COLORES',...
    'Position', [670 20 300 60],...
    'Callback', @newColors,'FontSize',20);
btn4 = uicontrol('Style', 'pushbutton', 'String', 'RESOLVER',...
    'Position', [990 20 300 60],...
    'Callback', @solve,'FontSize',20);
btn5 = uicontrol('Style', 'pushbutton', 'String', 'CAMBIAR COLOR',...
    'Position', [1310 20 300 60],...
    'Callback', @updateColor,'FontSize',20);
btn6 = uicontrol('Style', 'pushbutton', 'String', 'CALIBRAR',...
    'Position', [990 80 300 60],...
    'Callback', @calibrate,'FontSize',20);
btn7 = uicontrol('Style', 'pushbutton', 'String', 'VER COLORES',...
    'Position', [1310 80 300 60],...
    'Callback', @showColor,'FontSize',20);
% Crear botones de movimientos como matriz
btns = ["R","F","U","B","D","L";"R'","F'","U'","B'","D'","L'";"R2","F2","U2","B2","D2","L2"];
sz = 50;
for row = 1:3
    for col = 1:6
        uicontrol('Style', 'pushbutton', 'String', btns(row,col),...
            'Position', [20+col*sz 250-row*sz sz sz],...
            'Callback', @moveButton,'FontSize',18);
    end
end

% Cuadro para el random
uicontrol('Style','edit','Position',[400 155 100 40],'FontSize',18,'Callback',@randomN);
uicontrol('Style', 'pushbutton', 'String', 'Random',...
    'Position', [500 155 150 40],...
    'Callback', @randomize,'FontSize',18);

% Desplegable serial
serialList=seriallist;
serial = uicontrol('Style','popupmenu',...
    'String',seriallist,...
    'Position',[250 650 400 200],'FontSize',20,'Callback',@popCallback);
set(serial,'BackgroundColor', 'r');
uicontrol('Style','text','String','Puerto serie','Position',[20,750,200,100],'FontSize',20);


%% FUNCIONES CALLBACK %%

%% Actualizar cámara
function updateCamera(~,~)
global camera;
global camera1;
global camera2;
global IM;
global figure1;
global figure2;
global firstCamera;

% Si es la primera vez debemos guardarnos el handle
if (firstCamera==0)
    subplot(1,2,1); % Cambiar figura
    IM(:,:,:,1) = snapshot(camera1);
    figure1 = imshow(snapshot(camera1));
    
    subplot(1,2,2)
    IM(:,:,:,2) = snapshot(camera2);
    figure2 = imshow(snapshot(camera2));
    firstCamera=1;
end

% Actualizar las cámaras
subplot(1,2,1); % Cambiar figura
IM(:,:,:,1) = snapshot(camera1);
figure1 = imshow(snapshot(camera1), 'Parent', figure1.Parent);

subplot(1,2,2)
IM(:,:,:,2) = snapshot(camera2);
figure2 = imshow(snapshot(camera2), 'Parent', figure2.Parent);

subplot(1,2,camera); % Cambiar figura a la inicial
end

%% Nuevas posiciones para el reconocimiento
function newPositions(~,~)
global poi;
global take;
updateCamera;

for camera = 1:2
    subplot(1,2,camera); % Cambiar de figura
    for sticker = 1:21 % Para las 21 pegatinas de cada cámara
        i = sticker+(camera-1)*21; % Número de la pegatina
        title("SELECCIONAR " + take(i)); % Mostrar nombre de la pegatina
        [x, y] = getpts; % Obtener posición
        poi(i,:) = [x y]; % Guardar posición
        % Mostrar texto
        t = text(x,y,take(i));
        set(t, 'FontName', 'Arial', 'FontWeight', 'bold', 'FontSize', 14, 'Color', 'k','HorizontalAlignment', 'center');
    end
    title("");
end
updateCamera;
end

%% Nuevos colores de referencia
function newColors(~,~)
updateCamera;
global colors;
global camera;
global IM;

% Colores en RGB para dibujar los cuadraditos de colores
% Rojo-Blanco-Azul-Amarillo-Verde-Naranja
color = [1 0 0; 1 1 1; 0 0 1; 1 1 0; 0 1 0; 1 0.6 0];

% Nombres de las caras
cara = ["RIGHT", "FRONT", "UP", "BACK","DOWN","LEFT"];

% Inicializar el contenedor de los colores hay 6 colores en 6 caras
% y cada uno tiene 3 canales (RGB), una matriz 36x3
colors = zeros(6*6,3);

% Actualizar camara 1
updateCamera;

for camera=1:2 % Ejecutar el siguiente código para las dos cámaras
    
    subplot(1,2,camera); % Cambiar figura
    title("SELECCIONAR COLOR");
    hold on
    
    for face=1:3 % Iterar por las 3 caras de cada cámara
        for c=1:6 % Iterar por los 6 colores de cada cara
            % Calcular la cara, hay 3 caras por cámara
            k = face + (camera-1) * 3;
            btn5 = uicontrol('Style', 'pushbutton',...
                'Position', [800 800 300 60],'FontSize',20,'String',"SELECT " + cara(k));
            % Dibujar el color que se debe seleccionar
            set(btn5,'BackgroundColor', color(c,:));
            
            % Obtener coordenadas al hacer clic y hacer la media,
            % esta vez pueden ser varias casillas y se hará la media
            [xi,yi] = getpts;
            xi = round(xi);
            yi = round(yi);
            % Desglosar la imagen en sus tres canales
            redBand = IM(:,:,1, camera);
            greenBand = IM(:,:,2, camera);
            blueBand = IM(:,:,3, camera);
            % Esta matriz tendrá un número de columnas igual al de
            % veces que se haya hecho clic justo encima
            auxColor = zeros(length(xi),3);
            for i = 1:length(xi) % Iterar por esta matriz
                % Definir una máscara entorno al punto xi, yi de 10x10
                % Esto es una matriz binaria del mismo tamaño de la
                % imagen, con 1 dentro de la máscara
                mask = poly2mask([xi(i)+5 xi(i)+5 xi(i)-5 xi(i)-5 xi(i)+5],[yi(i)+5 yi(i)-5 yi(i)-5 yi(i)+5 yi(i)+5],480,640);
                % Guardar la media de cada canal RGB de la imagen en
                % la máscara
                auxColor(i,:) = [mean(redBand(mask)) mean(greenBand(mask)) mean(blueBand(mask))];
            end
            % Calcular por que color vamos de los 36, hay 6 colores por
            %cámara y 3*8 colores por cámara
            k = c + (face-1)*6 + (camera-1)*18;
            % Hacer la media de todas las medias de las máscaras del
            % bucle anterior y guardar en su sitio
            colors(k,:) = [mean(auxColor(:,1)), mean(auxColor(:,2)), mean(auxColor(:,3))];
            delete(btn5);
            
        end
    end
    title("");
end
updateCamera;
end

%% Resolver el cubo de rubik
function solve(~,~)
%% Deteccion de colores
% Colores en RGB para dibujar los cuadraditos de colores
% Rojo-Blanco-Azul-Amarillo-Verde-Naranja
color = [1 0 0; 1 1 1; 0 0 1; 1 1 0; 0 1 0; 1 0.6 0];

global colors;
global camera;
global IM;
global poi;
global take;
global arduino;
updateCamera;

faceColor = zeros(42,1);

l = 0; % Variable auxiliar para detectar que ya hemos terminado una cara
estupidomatlab = rgb2lab(colors);

for camera=1:2  % Ejecutar el siguiente código para las dos cámaras
    % Cambiar figura
    subplot(1,2,camera);
    
    % Desglosar la imagen en sus tres canales
    jodetematlab = rgb2lab(IM(:,:,:, camera));
    L = jodetematlab(:,:,1);
    A = jodetematlab(:,:,2);
    B = jodetematlab(:,:,3);
    hold on
    
    for sticker=1:21 % Para cada una de las 21 pegatinas de las 2 cámaras
        
        % Si empezamos una cara nueva sumar a l una unidad
        % En 1, 8 y 15 hemos empezado caras nuevas
        % Podria hacerse con mod(sticker-1,7) == 0
        if sticker == 1 || sticker == 8 || sticker == 15
            l = l + 1;
        end
        
        % Calcular por que pegatina vamos, en cada cámara hay 21 pegatinas
        i = sticker+(camera-1)*21;
        
        % Obtener las coordenadas de la pegatina actual
        mx = poi(i,1);
        my = poi(i,2);
        
        % Definir una máscara entorno al punto mx, my de 10x10
        % Esto es una matriz binaria del mismo tamaño de la
        % imagen, con 1 dentro de la máscara
        mask = poly2mask([mx+5 mx+5 mx-5 mx-5 mx+5],[my+5 my-5 my-5 my+5 my+5],480,640);
        
        % Hacer la media del color dentro de la máscara anterior
        block = [mean(L(mask)) mean(A(mask)) mean(B(mask))];
        
        % Distancia mínima a un color, un valor muy grande y desconocido
        minDist = 10E8;
        % Color más cercano
        minColor = 0;
        
        % Para cada uno de los 6 colores de esta cara ver el más cercano al
        % que había dentro de la máscara
        for k=1:6
            clr = k + (l-1)*6; % �?ndice del color, l denota la cara actual
            % Si la distancia al color clr es menor que la que había
            if norm(estupidomatlab(clr,:)-block) < minDist
                % Guardar distancia y guardar color
                minDist =  norm((estupidomatlab(clr,:)-block).*[0.8 1 1]);
                if (k == 1)
                    minDist = minDist * 1;
                end
                minColor = k;
            end
        end
        
        % Dibujar un rectangulo 40x40 centrado en mx,my del color
        % más parecido al de la máscara
        rectangle('Position', [mx-20 my-20 40 40], 'EdgeColor', color(minColor,:), 'LineWidth', 2);
        
        % Dibujar una cruceta en mx,my
        plot(mx,my, '-m+');
        
        faceColor(i) = minColor;
    end
end
%% Notación de Koecimba para el algoritmo
%
%               |************|
%               |*U1**U2**U3*|
%               |************|
%               |*U4**U5**U6*|
%               |************|
%               |*U7**U8**U9*|
%               |************|
%  |************|************|************|************|
%  |*L1**L2**L3*|*F1**F2**F3*|*R1**R2**F3*|*B1**B2**B3*|
%  |************|************|************|************|
%  |*L4**L5**L6*|*F4**F5**F6*|*R4**R5**R6*|*B4**B5**B6*|
%  |************|************|************|************|
%  |*L7**L8**L9*|*F7**F8**F9*|*R7**R8**R9*|*B7**B8**B9*|
%  |************|************|************|************|
%               |************|
%               |*D1**D2**D3*|
%               |************|
%               |*D4**D5**D6*|
%               |************|
%               |*D7**D8**D9*|
%               |************|
%
%
% A cube definition string "UBL..." means for example: In position U1 we
% have the U-color, in position U2 we have the B-color, in position U3 we
% have the L color etc. according to the order
% U1, U2, U3, U4, U5, U6, U7, U8, U9, R1, R2, R3, R4, R5, R6, R7, R8, R9,
% F1, F2, F3, F4, F5, F6, F7, F8, F9, D1, D2, D3, D4, D5, D6, D7, D8, D9,
% L1, L2, L3, L4, L5, L6, L7, L8, L9, B1, B2, B3, B4, B5, B6, B7, B8, B9
% of the enum constants.
% Order of faces: U R F D L B


% Cadena de caracteres para enviar al algoritmo de resolución, hay un
% caracter por cada casilla y con la nomenclatura y orden del comentario
% anterior
posicion = blanks(54);
face = ['L', 'B', 'U', 'F', 'D', 'R'];
for j=1:54
    posicion(j) = " ";
end

% Pasar de la tabla de colores que hemos reconocido a la cadena de caracteres
for i = 1:42 % Hemos reconocido 42 pegatinas
    % Guardar en posicion el valor de cada pegatina
    % En el lado derecho de la asignación hay que convertir de color a cara
    % usando el array face. En el lado izquierdo hay que convertir el
    % nombre de cada casilla a la posición en la cadena de caracteres.
    % indexFacelet convierte "U1" en 1, y "B9" en 54 por ejemplo
    posicion(indexFacelet(take(i))) = face(faceColor(i));
end

%% Criba
% Rellenar los centros, que aunque sean triviales nos los habiamos tenido
% en cuenta hasta ahora.
posicion(indexFacelet('U5')) = 'U';
posicion(indexFacelet('L5')) = 'L';
posicion(indexFacelet('F5')) = 'F';
posicion(indexFacelet('R5')) = 'R';
posicion(indexFacelet('B5')) = 'B';
posicion(indexFacelet('D5')) = 'D';

% Utilizar la función rotación para cribar la cadena de caracteres y
% encontrar las pegatinas ocultas
cribado = Rotacion(posicion);

%% Solución
% Cadena donde se encuentra el algoritmo en Java, ojo que cambia
% dependiendo del ordenador.
% TODO: programar para que lo encuentre el solito usando una ruta relativa
javaaddpath("/ownCloud/Share/Rubik/Programas/MATLAB/Java");
sol = "";
i = 1;
while strlength(sol) < 15 && i < 20 % Probar 20 veces
    % Ejecutar el algoritmo de resolución, en 21 movimientos y 5 segundos de
    % tiempo como máximo antes de abortar
    solucion = RubikSolver.solver(cribado,21,5);
    % Mostras movimientos
    
    sol = regexprep(string(solucion),' ','');
    
    i = i + 1;
end

%% Serial
if strlength(sol) > 15 % Si hemos encontrado una solucion
    msgbox(string(solucion),'SOLUCION'); % TODO: ESPERA PARA SACAR CAMARA DEL MOVIL
    fwrite(arduino, strcat("100 250 ",regexprep(sol,' ','')));
else % Errores varios
    switch(str2num(regexprep(sol,'\D','')))
        case 1
            msgbox('There is not exactly one facelet of each colour', 'ERROR 1');
        case 2
            msgbox('Not all 12 edges exist exactly once', 'ERROR 2');
        case 3
            msgbox('EFlip error: One edge has to be flipped', 'ERROR 3');
        case 4
            msgbox('Not all corners exist exactly once', 'ERROR 4');
        case 5
            msgbox('Twist error: One corner has to be twisted', 'ERROR 5');
        case 6
            msgbox('Parity error: Two corners or two edges have to be exchanged', 'ERROR 6');
        case 7
            msgbox('No solution exists for the given maxDepthe', 'ERROR 7');
        case 8
            msgbox('Timeout, no solution within given time', 'ERROR 8');
    end
end
end

%% Actualizar solo un color de referencia
function updateColor(~,~)
global colors;
global IM;

color = [1 0 0; 1 1 1; 0 0 1; 1 1 0; 0 1 0; 1 0.6 0];
cara = ["RIGHT", "FRONT", "UP", "BACK","DOWN","LEFT"];
colorList = {"Rojo","Blanco","Azul","Amarillo","Verde","Naranja"};
faceList = {"RIGHT","FRONT","UP","BACK","DOWN","LEFT"};
% Rojo-Blanco-Azul-Amarillo-Verde-Naranja

f = listdlg('PromptString','Elige cara','SelectionMode','single','ListString',faceList);
c = listdlg('PromptString','Elige color','SelectionMode','single','ListString',colorList);
if f < 4
    camera = 1;
else
    camera = 2;
end
btn5 = uicontrol('Style', 'pushbutton',...
    'Position', [900 900 300 60],'FontSize',20,'String',"SELECT " + cara(f));
% Dibujar el color que se debe seleccionar
set(btn5,'BackgroundColor', color(c,:));

% Obtener coordenadas al hacer clic y hacer la media,
% esta vez pueden ser varias casillas y se hará la media
[xi,yi] = getpts;
xi = round(xi);
yi = round(yi);
% Desglosar la imagen en sus tres canales
redBand = IM(:,:,1, camera);
greenBand = IM(:,:,2, camera);
blueBand = IM(:,:,3, camera);
% Esta matriz tendrá un número de columnas igual al de
% veces que se haya hecho clic justo encima
auxColor = zeros(length(xi),3);
for i = 1:length(xi) % Iterar por esta matriz
    % Definir una máscara entorno al punto xi, yi de 10x10
    % Esto es una matriz binaria del mismo tamaño de la
    % imagen, con 1 dentro de la máscara
    mask = poly2mask([xi(i)+5 xi(i)+5 xi(i)-5 xi(i)-5 xi(i)+5],[yi(i)+5 yi(i)-5 yi(i)-5 yi(i)+5 yi(i)+5],480,640);
    % Guardar la media de cada canal RGB de la imagen en
    % la máscara
    auxColor(i,:) = [mean(redBand(mask)) mean(greenBand(mask)) mean(blueBand(mask))];
end

% Hacer la media de todas las medias de las máscaras del
% bucle anterior y guardar en su sitio
colors((f-1)*6+c,:) = [mean(auxColor(:,1)), mean(auxColor(:,2)), mean(auxColor(:,3))];
delete(btn5);
end

%% Efectuar un movimiento
function moveButton(src,~)
global arduino;
fwrite(arduino, strcat("100 250 ",src.String)); %colocar los dato que queremos enviar
pause(0.5);
updateCamera;
end

%% Elegir puerto serie
function popCallback(source,~)
global arduino;
arduino=serial(source.String,'BaudRate',9600);
fopen(arduino);
set(source,'BackgroundColor', 'g');
end

%% Callback al cerrar la GUI
function my_closereq(~,~)
global arduino;
selection = questdlg('Cerrar GUI',...
    '¿Quieres cerrar?',...
    'Si','No','Si');
switch selection
    case 'Si'
        % stop(tim);
        delete(gcf);
        try
            fclose(arduino);
        catch
        end
    case 'No'
        return
end
end

%% Elegir numero de movimientos aleatorios
function randomN(src,~)
global rndN;
rndN = str2num(src.String);
end

%% Ejecutar movimientos aleatorios
function randomize(~,~)
global rndN;
global arduino;
str = "";
face = ["L", "B", "U", "F", "D", "R"];
extra = ["", "'", "2"];
for i=1:rndN
    str = str + face(randi(6)) + extra(randi(3));
end
fwrite(arduino,strcat("200 150 ", str, "\n")); %colocar los dato que queremos enviar
updateCamera;
end

function showColor(~,~) 
global colors;
color = [1 0 0; 1 1 1; 0 0 1; 1 1 0; 0 1 0; 1 0.6 0];
cara = ["R", "F", "U", "B","D","L"];
figure(2);
for c=1:6
   rectangle('Position',[(c)*6 0 6 6],'FaceColor',color(c,:));
   
end
for f=1:6
       text('Position',[3 (f)*6+3],'string',cara(f))
    for c=1:6
       rectangle('Position',[(c)*6 (f)*6 6 6],'FaceColor',colors((f-1)*6+c,:)./255.0);
    end
end
daspect([1 1 1])
axis off
end
%% Calib ración automática
%cara = ["RIGHT", "FRONT", "UP", "BACK","DOWN","LEFT"];

function calibrate(~,~)
global IM;
global camera;
global poi;
global colors;
global arduino;
%(FACE,COLOR)
secuencias = ...
    [""             "R'LU'DF'BR'L"  "L'RB'FD'UL'R"  "U'DLR'B'FU'D"  "FB'L'RU'DFB'"  "F2U2D2B2"      ;
    "L'RB'FD'UL'R"  ""              "L'RFB'UD'L'R"  "L2U2D2R2"      "R'LU'DF'BR'L"  "D'UF'BRL'D'U"  ;
    "U'DLR'B'FU'D"  "L'RB'FD'UL'R"  ""              "R'LF'BD'UR'L"  "B2R2L2F2"      "R'LU'DF'BR'L"  ;
    "D'UF'BRL'D'U"  "L2U2D2R2"      "R'LU'DF'BR'L"  ""              "L'RFB'UD'L'R"  "L'RB'FD'UL'R"  ;
    "R'LU'DF'BR'L"  "D'UF'BRL'D'U"  "B2R2L2F2"      "L'RB'FD'UL'R"  ""              "U'DLR'B'FU'D"  ;
    "F2U2D2B2"      "U'DLR'B'FU'D"  "FB'L'RU'DFB'"  "R'LU'DF'BR'L"  "L'RB'FD'UL'R"  ""             ];

inversas  = ...
    [""             "L'RB'FD'UL'R"  "R'LU'DF'BR'L"  "D'UF'BRL'D'U"  "BF'D'UR'LBF'"  "B2D2U2F2"      ;
    "R'LU'DF'BR'L"  ""              "R'LDU'BF'R'L"  "R2D2U2L2"      "L'RB'FD'UL'R"  "U'DLR'B'FU'D"  ;
    "D'UF'BRL'D'U"  "R'LU'DF'BR'L"  ""              "L'RU'DB'FL'R"  "F2L2R2B2"      "L'RB'FD'UL'R"  ;
    "U'DLR'B'FU'D"  "R2D2U2L2"      "L'RB'FD'UL'R"  ""              "R'LDU'BF'R'L"  "R'LU'DF'BR'L"  ;
    "L'RB'FD'UL'R"  "U'DLR'B'FU'D"  "F2L2R2B2"      "R'LU'DF'BR'L"  ""              "D'UF'BRL'D'U"  ;
    "B2D2U2F2"      "D'UF'BRL'D'U"  "BF'D'UR'LBF'"  "L'RB'FD'UL'R"  "R'LU'DF'BR'L"  ""             ];
          
for f = 1:6
    for c = 1:6
        fwrite(arduino,strcat("500 100 ",secuencias(f,c),'\n'));
        pause(1.0);
        updateCamera;
        
        if f < 4
            camera = 1;
        else
            camera = 2;
        end
        redBand = IM(:,:,1, camera);
        greenBand = IM(:,:,2, camera);
        blueBand = IM(:,:,3, camera);
        
        auxColor = zeros(7,3);
        for p = 1:7 % Iterar por esta matriz
            idx = p+(f-1)*7;
            
            mask = poly2mask([poi(idx,1)+5 poi(idx,1)+5 poi(idx,1)-5 poi(idx,1)-5 poi(idx,1)+5],...
                             [poi(idx,2)+5 poi(idx,2)-5 poi(idx,2)-5 poi(idx,2)+5 poi(idx,2)+5],480,640);
            
            auxColor(p,:) = [mean(redBand(mask)) mean(greenBand(mask)) mean(blueBand(mask))];
        end
        
        colors((f-1)*6+c,:) = [mean(auxColor(:,1)), mean(auxColor(:,2)), mean(auxColor(:,3))];
        
        fwrite(arduino,strcat("200 150 ",inversas(f,c),'\n'));
        pause(0.5);
    end
end
end

%% FUNCIONES CALLBACK %%


%% FUNCIONES AUXILIARES %%

%% Función Rotación (Criba)
% Esta función completa las esquinas ocultas del cubo dado en la notación
% del algoritmo
function criba = Rotacion(posicion)

% Posición geometrica de las esquinas en sentido horario según la notación
% Hay 8 esquinas y cada esquina tiene 3 pegatinas por tanto tenemos una
% matrix 8x3. Por que pegatina se empieza es arbitrario y no afecta en nada
corners = ["U7" "F1" "L3"; "U9" "R1" "F3"; "U3" "B1" "R3"; "U1" "L1" "B3"; "F7" "D1" "L9"; "F9" "R7" "D3"; "R9" "B7" "D9"; "B9" "L7" "D7"];

% Hacemos una lista con los índices del vector posicion que esten vacios
% Si las cámaras no se mueven siempre van a ser las mismas
blank = find(posicion == ' ');

% Iteramos por cada uno de los huecos (deberian ser 6)
for i=1:length(blank)
    
    cor_colors = 0; % Variable auxiliar para guardar la fila en la tabla
    
    % Buscamos la esquina que nos falta en la tabla de esquinas
    for j=1:8  % Iteramos por filas
        for k=1:3 % Iteramos por columnas
            % Ver si es nuestra esquina
            if blank(i) == indexFacelet(corners(j,k))
                cor_colors = j; % Guardar la fila
                pos_color = k; % Guardar la columna
            end
        end
    end
    
    % Vamos a guardarnos los valores en nuestro cubo de la esquina en la
    % que nos falta una pegatina, un valor será ' '
    esquina = blanks(3);
    for k=1:3 % Hay 3 pegatinas en esta esquina
        % Obtener en el vector posicion que esta en la notación del
        % algoritmo el valor de cada una de las esquinas
        esquina(k) = posicion(indexFacelet(corners(cor_colors,k)));
    end
    
    % Elegimos sentido horario o antihorario para encontrar la etiqueta que
    % falta, si falta la etiqueta central de la esquina elegimos antihorario
    % Ejecutaremos la función horario para descubrir la esquina que falta
    if esquina(2) == ' '
        % Se invierte el sentido y se elimina el espacio en blanco
        aux =  horario(corners, reverse(regexprep(esquina,' ','')));
    else
        % Se elimina el espacio en blanco
        aux =  horario(corners, regexprep(esquina,' ',''));
    end
    
    %Encontrada la etiqueta, guardar
    posicion(indexFacelet(corners(cor_colors,pos_color))) = aux;
end
% Guardar la cadena de caracteres una vez cribada
criba = posicion;
end

%% Función Auxiliar Horario para determinar el color que falta
% Esta función haya el color restante de la esquina. Usando la tabla de
% esquinas y los dos valores conocidos de la esquina
function color = horario(corners, unknown)
color = 0;
%Buscar por las esquinas el patrón dado, ya en sentido horario siempre
for i=1:8
    % Elimina los números de esta esquina de la tabla de esquinas
    % O sea de la fila de la matriz de corners une las 3 cadenas en una
    % sola cadena y elimina los números nos quedamos con 3 caracteres
    % Transformamos en cadena de caracteres
    str = convertStringsToChars(regexprep(strjoin(corners(i,:)),'[1-9 ]',''));
    
    %Encuentra el que falta según el orden, en cualquiera de los tres casos
    if (unknown(1) ==  str(1)) && (unknown(2) ==  str(2))
        color = str(3);
    elseif (unknown(1) ==  str(2)) && (unknown(2) ==  str(3))
        color = str(1);
    elseif (unknown(1) ==  str(3)) && (unknown(2) ==  str(1))
        color = str(2);
    end
end
end

%% Funcion IndexFacelet
% Función auxiliar para ordenar según la notación de min2phase (U1 -> 1...)
function n = indexFacelet(c)
chars = convertStringsToChars(c); % Convierte a cadena de caracteres
faces = ['U' 'R' 'F' 'D' 'L' 'B']; % Orden de las caras según la notación
% Buscar el indice del primer caracter en el array faces, hay 9 pegatinas
% por cara y sumar el segundo caracter
n = (find(faces == chars(1)) - 1) * 9 + str2num(chars(2));
end
%% FUNCIONES AUXILIARES %%

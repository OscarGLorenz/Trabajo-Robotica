%Cerrar todo
clear variables

figure('units','normalized','outerposition',[0 0 1 1])

vlast = zeros(3,3);
%Tamaño de los Ejes
ax = axes('XLim',[-10 10],'YLim',[-10 15],'ZLim',[-10 10]);
hold(ax,'on');

%Isométrica
view(3)
grid on

%Mantener aspecto entre los ejes
daspect([1 1 1])

A1=0; A2 = 0; 
B1 =50; B2 = 0;
w1 = 0.5; w2 = 0;
phi = pi/2;
k1 = pi/5;
k2 = 0;
%Ángulos horizontales y verticales
angleV = @(n) A1+B1*sin(w1*cputime+k1*n);
angleH = @(n) A2+B2*sin(w2*cputime+k2*n+pi/2);
    
et = cputime;
while cputime - et < 1000 %Variable tiempo
    %Marices de rotación
    GH = @(theta) [cosd(theta) sind(theta) 0 0; -sind(theta) cosd(theta) 0 0; 0 0 1 0; 0 0 0 1];
    GV = @(theta) [1 0 0 0; 0 cosd(theta) -sind(theta) 0; 0 sind(theta) cosd(theta) 0; 0 0 0 1];
    TR = @(vec) [1 0 0 vec(1); 0 1 0 vec(2); 0 0 1 vec(3); 0 0 0 1];
    %Matriz para la posición de cada servo
    X = zeros(4,13);
    
    X(:,1) = [0; 0; 0; 1];
    %Posición inicial
    X(:,2) = [0; 1; 0; 1];
    Y(15) = plot3([X(1,1) X(1,2)]', [X(2,1) X(2,2)]', [X(3,1) X(3,2)]','c'); hold on
    
    %Posición 1
    X(:,3) =  TR(X(:,2))*GH(angleV(0))*X(:,2);
    %Dibujar y guardar en vector de figuras
    Y(1) = plot3([X(1,2) X(1,3)]', [X(2,2) X(2,3)]', [X(3,2) X(3,3)]','m'); hold on
    
    %Iteración por todos los servos
    for i = 4:1:14
        %Si es par se mueven los verticales
        if (mod(i,2) == 1)
            X(:,i) = TR(X(:,i-1))*GV(angleV(i-3))*TR(-X(:,i-2))*X(:,i-1);
            %Dibujar y guardar en vector de figuras
            Y(i) = plot3([X(1,i-1) X(1,i)]', [X(2,i-1) X(2,i)]', [X(3,i-1) X(3,i)]','m');
            %Si es impar se mueven los verticales
        else
            X(:,i) = TR(X(:,i-1))*GH(angleH(i-3))*TR(-X(:,i-2))*X(:,i-1);
            %Dibujar y guardar en vector de figuras
            Y(i) = plot3([X(1,i-1) X(1,i)]', [X(2,i-1) X(2,i)]', [X(3,i-1) X(3,i)]','c');
        end
    end
    
    m = zeros(3,14);
    
    for j = 1:1:3
        m(j,:) = X(j,:);
    end
    
    p = mean(m');
    
    P = m' - p;
    
    [U, S, V] = svd(P);
    V(:,3) = cross(V(:,1),V(:,2));
    
    c = det(V+vlast);

    if c < 1

        if norm(V(:,1)+vlast(:,1)) < 0.2
            V(:,1) = -V(:,1);
        end
        if norm(V(:,2)+vlast(:,2)) < 0.2
            V(:,2) = -V(:,2);
        end
        V(:,3) = cross(V(:,1),V(:,2));
        c = det(V+vlast);
        
    end

    vlast = V;
    T = [V p'; 0 0 0 1];
 
    X(:,1) = T\X(:,1);
    for i = 2:1:13
        if (mod(i,2) == 1)
            X(:,i) = T\X(:,i);
            %Dibujar y guardar en vector de figuras
            Y(i+15) = plot3([X(1,i-1) X(1,i)]', [X(2,i-1) X(2,i)]', [X(3,i-1) X(3,i)]','y');
            %Si es impar se mueven los verticales
        else
            X(:,i) = T\X(:,i);
            %Dibujar y guardar en vector de figuras
            Y(i+15) = plot3([X(1,i-1) X(1,i)]', [X(2,i-1) X(2,i)]', [X(3,i-1) X(3,i)]','Color',[0.5 0.5 0.5]);
        end
    end
    
    set(findall(gca, 'Type', 'Line'),'LineWidth',3);

    Y(29) = plot3([p(1)+V(1,1) p(1)]', [p(2)+V(2,1) p(2)]', [p(3)+V(3,1) p(3)]','r');
    Y(30) = plot3([p(1)+V(1,2) p(1)]', [p(2)+V(2,2) p(2)]', [p(3)+V(3,2) p(3)]','b');
    Y(31) = plot3([p(1)+V(1,3) p(1)]', [p(2)+V(2,3) p(2)]', [p(3)+V(3,3) p(3)]','g');
    
    Y(32) = plot3([0 1]', [0 0]', [0 0]','r');
    Y(33) = plot3([0 0]', [0 1]', [0 0]','b');
    Y(34) = plot3([0 0]', [0 0]', [0 1]','g');
    
    Z = inv(T);
    Y(35) = plot3([Z(1,4)+Z(1,1) Z(1,4)]', [Z(2,4)+Z(2,1) Z(2,4)]', [Z(3,4)+Z(3,1) Z(3,4)]','r');
    Y(36) = plot3([Z(1,4)+Z(1,2) Z(1,4)]', [Z(2,4)+Z(2,2) Z(2,4)]', [Z(3,4)+Z(3,2) Z(3,4)]','b');
    Y(37) = plot3([Z(1,4)+Z(1,3) Z(1,4)]', [Z(2,4)+Z(2,3) Z(2,4)]', [Z(3,4)+Z(3,3) Z(3,4)]','g');
    %[x,y,z] = ellipsoid(0,0,0,S(1,1)/3,S(2,2),S(3,3)*2);
    %O = surf(x,y,z,'EdgeColor','none','FaceAlpha',0.2);
    pause(1/100);
   
    %delete(O);
    %Borrado del vector de figuras
    delete(Y);
end


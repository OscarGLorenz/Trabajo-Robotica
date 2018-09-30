q2lim = deg2rad([6, 115]);  % 6 <= q2 <= 115
q3lim = deg2rad([-116,13]); % -116 <= q3 <= 13
q23lim = deg2rad([30, 158]);      % 30 <= q2-q3 <=158
q1lim = deg2rad([0, 250]);  % 0 <= q1 <= 250
zlim = -60;
close all
% figure
ax = axes('XLim',[-300 820],'YLim',[-250 400]);
hold(ax,'on');
% view(3);
% grid on
% close all

l = linspace(q2lim(1),q2lim(2),100);
[x, y, z] = directa(0,l,q3lim(1));
plot(y,z,'r'); hold on
text(mean(y),mean(z)-100,"q_2 \geq 6º",'Color','r') 

[x, y, z] = directa(0,l,q3lim(2));
plot(y,z,'r'); hold on
text(mean(y)+90,mean(z),"q_2 \leq 115",'Color','r') 

l = linspace(q3lim(1),q3lim(2),100);
[x, y, z] = directa(0,q2lim(1),l);
plot(y,z,'b'); hold on
text(mean(y)+50,mean(z)-50,"q_3 \geq -116º",'Color','b') 

[x, y, z] = directa(0,q2lim(2),l);
plot(y,z,'b'); hold on
text(mean(y)-50,mean(z)+50,"q_3 \leq 13º",'Color','b') 

l = linspace(q2lim(1),q2lim(2)*0.4,100);
[x, y, z] = directa(0,l,l-q23lim(1));
plot(y,z,'k'); hold on
text(mean(y)+40,mean(z),"q_2 - q_3 \geq 30º",'Color','k')

l = linspace(q2lim(1),q2lim(2),100);
[x, y, z] = directa(0,l,l-q23lim(2));
plot(y,z,'k'); hold on
text(mean(y)-250,mean(z)-10,"q_2 - q_3 \leq 158",'Color','k')

l = linspace(0,600,100);
plot(l,-60*ones(100,1),'Color',[0 0.5 0]); hold on
text(mean(l),-80,"z \geq -60",'Color',[0 0.5 0 ])

l = linspace(-200,200,100);
plot(zeros(100,1),l,'Color',[0 0.5 0]); hold on
text(0,220,"y \geq 0",'Color',[0 0.5 0 ])

axis equal
grid on
xlabel('Y(mm)')
ylabel('Z(mm)')

A = Montecarlo(@(x) constrains(x),3,1e5,[-600 600]) / 10^3;
disp("El volumen de trabajo tiene " + num2str(A) + " cm^3");

figure
ax = axes('XLim',[q2lim(1)-0.1 q2lim(2)+0.1],'YLim',[q3lim(1)-0.1 q3lim(2)+0.1]);
hold(ax,'on');

l = linspace(q3lim(1),q3lim(2),100);
plot(q2lim(1)*ones(100,1),l,'Color','r'); hold on
text(q2lim(1),0,"q_2 \geq 6º",'Color','r')
plot(q2lim(2)*ones(100,1),l,'Color','r'); hold on
text(q2lim(2),0,"q_2 \leq 115º",'Color','r')

l = linspace(q2lim(1),q2lim(2),100);
plot(l,q3lim(1)*ones(100,1),'Color','b'); hold on
text(0,q3lim(1),"q_3 \geq -116",'Color','b')
plot(l,q3lim(2)*ones(100,1),'Color','b'); hold on
text(0,q3lim(2),"q_3 \leq 13",'Color','b')

l = linspace(q2lim(1),q3lim(2)+q23lim(1),100);
plot(l,l-q23lim(1),'Color','k'); hold on
text(mean(l),mean(l-q23lim(1)),"q_2 - q_3 \geq 30º",'Color','k')
l = linspace(q3lim(1),q2lim(2)-q23lim(2),100);
plot(l+q23lim(2),l,'Color','k'); hold on
text(mean(l+q23lim(2)),mean(l),"q_2 - q_3 \leq 158",'Color','k')

l = linspace(-500,500,100);
[q1,q2,q3] = inversa(zeros(1,100),l,ones(1,100).*-60);
plot(q2,q3,'Color',[0 0.5 0 ]); hold on
text(mean(q2),mean(q3),"z \geq - 60",'Color',[0 0.5 0]);

l = linspace(-1,1.001,100);
[q1,q2,q3] = inversa(zeros(1,100),zeros(1,100),l);
plot(q2,q3,'Color',[0 0.5 0]); hold on
text(mean(q2),mean(q3),"y \geq 0",'Color',[0 0.5 0]);

axis equal
grid on
xlabel('q_2(rad)')
ylabel('q_3(rad)')

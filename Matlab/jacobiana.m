function J = jacobiana(q1,q2,q3) 
l1 = 300;
l2 = 300;
J = [1  0        0;
     0  -l1*sin(q2) -l2*sin(q3);
     0  l1*cos(q2)  l2*cos(q3)];


end
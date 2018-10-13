function J = jacobiana(q1,q2,q3) 
J = [1  0        0;
     0  -sin(q2) -sin(q3);
     0  cos(q2)  cos(q3)];


end
function q = consultarPosicion(app,motor)
while app.arduino.TransferStatus ~= "idle"
   pause(0.001); 
end
fprintf(app.arduino,"J20 M%d\n",motor,'async');
fprintf("J20 M%d\n",motor);
pause(0.05);
q = fscanf(app.arduino,"D1 M%*d %f\n");
fprintf("D1 M%d %.3f\n",motor, q);    
end
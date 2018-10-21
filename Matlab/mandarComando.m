function mandarComando(app,str)
while app.arduino.TransferStatus ~= "idle"
   pause(0.001); 
end
fprintf(app.arduino,"%s\n",str,'async');
fprintf("%s\n",str);
end


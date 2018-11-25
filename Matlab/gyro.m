

ardu = serial('/dev/ttyACM1','BaudRate',9600);
fopen(ardu);
pause(2);
%fprintf(ardu,'J0\n');
%pause(5);

connector on testing;

m = mobiledev

m.OrientationSensorEnabled = 1;
m.Logging = 1;
pause(1);


for i=1:1000

pause(0.5);

[o, to] = orientlog(m);

pitch = o(:, 2);
ma(ardu,sprintf('J1 M3 %f\n',-pitch(end)));

end
m.Logging = 0;


fclose(ardu);


%% Clean up
% Turn off the enabled sensors and clear |mobiledev|.

m.AngularVelocitySensorEnabled = 0;
m.OrientationSensorEnabled = 0;

clear m;

function ma(app,str)
while app.TransferStatus ~= "idle"
   pause(0.001); 
end
fprintf(app,"%s\n",str,'async');
fprintf("%s\n",str);
end
BAUDIOS = 57600;

frec = 2000;
scope = dsp.TimeScope(2,'SampleRate', frec, 'TimeSpan', 0.320, ...
    'YLimits', [-1, 6], 'ShowGrid', true,'ReduceUpdates',true, 'ShowLegend',true,'ChannelNames',{'Uout (V)','Uin(V)'},...
    'Position',[0 400 560 420]);
scope2 = dsp.TimeScope('SampleRate', frec, 'TimeSpan', 0.320, ...
    'YLimits', [-3.5, 3.5], 'ShowGrid', true,'ReduceUpdates',true, 'ShowLegend',true,'ChannelNames',{'I(mA)'},...
    'Position',[800 400 560 420]);
figure('Name','Control','NumberTitle','off');
uicontrol('Style', 'pushbutton', 'String', 'STOP',...
    'Position', [20 20 100 50],...
    'Callback', 'stop=false;','FontSize', 20);  
uicontrol('Style', 'pushbutton', 'String', 'PAUSE',...
    'Position', [20 80 100 50],...
    'Callback', 'pausa=~pausa;','FontSize', 20);  
set(gcf, 'Position', [625, 400, 140, 140]);

serialList=seriallist;
[indx,tf] = listdlg('Name','Puerto Serie','ListString',seriallist,'SelectionMode','single');
serialPort = serialList(indx);
arduino = serial(serialPort,'BaudRate',BAUDIOS);
try
    fopen(arduino);
catch ME
    fclose(arduino);
    delete(arduino);
    fprintf(2,'\nPuerto no valido, conecta y desconecta el USB o elige otro puerto\n')
    return;
end

oldT =0; oldS = 0; oldC = 0;
stop = true;
pausa = false;
m = 1;
while stop
    try
    data = fscanf(arduino,'%u %u %u\n',[3 1]);
    m = m + 1;
    catch
        fprintf(2,'\nRenicia MATLAB\n');
        close all
        return;
    end
    while pausa && stop && m == 320
        pause(0.1);
    end
    if length(data) == 3
        tension = data(1)*5.0/1023.0;
        st = data(3)*2+1.5;
        corriente =  data(2)*5.0/1023.0-2.5;
        scope(tension(1),st(1));
        scope2(corriente(1));
        oldT = tension(1);
        oldS = st(1);
        oldC = corriente(1);
    else
        scope(oldT,oldS);
        scope2(oldC);
    end
    if m == 320
        m = 1;
    end
end

release(scope);
release(scope2);
fclose(arduino);
delete(arduino);
close all
clear all

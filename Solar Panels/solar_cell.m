function Ia = solar_cell(Va,Irradiance,TaC,measured_voltage,measured_current)
%Code produced by the Dept of Computer Science and Electrical Engineering,
%University of Queensland, Australia and modified by Embedded Systems Group - UFSC

if nargin == 5  % verify if measured voltage and measured current were passed to the function
    Va = measured_voltage(1:10:length(measured_voltage));   % reduce voltage data points by a factor of 10
    measured_current = measured_current(1:10:length(measured_current)); % reduce current data points by a factor of 10
end

k = 1.38e-23; % Boltzman’s constant
q = 1.60e-19; % charge on an electron

A = 2.1; % diode base quality factor, obtained by curve fitting
Vg = 1.12; % band gap voltage, 1.12eV for xtal Si, ˜1.75 for amorphous Si.
Ns = 10; % number of series connected cells
Np = 4; % number of parallel connected cells
Npv = 3;    % number of panels connected in parallel

base_Irradiance = 1000; % datasheet data irradiance in W/m^2

T1 = 273 + 25;  % convert T1 from ºC to K
Isc_T1 = 0.050; % short circuit current per cell at temperature T1

TaK = 273 + TaC; % convert working temperature from ºC to K

Iph_T1 = Isc_T1 * Irradiance/base_Irradiance; % photocurrent is considered the same as the short circuit current for a given irradiance

a = 0.1848e-3;  % short circuit temperature coefficient, obtained from the datasheet
Iph = Iph_T1 * (1 + a*(TaK - T1));  % calculate new Iph based on short circuit temperature coefficient

b = Vg * q/(A*k);   % calculate diode saturation current constant based on band gap energy, diode quality factor and boltzmann's constant
Ir = 2.37917e-07;   % diode base reverse saturation current, obtained from curve fitting
Ir = Ir * (TaK/T1)^(3/A) * exp(-b*((1/TaK) - (1/T1)));  % calculate diode saturation current at the working temperature

Rs = 5.59404;   % solar cell base series resistance, obtained from curve fitting

Rsh = 954.801;  % solar cell base shunt resistance, obtained from curve fitting

Vt_Ta = A * 1.38e-23 * TaK / 1.60e-19;  % calculate the diode thermal voltage

Vc = Va/Ns; % take the voltage in each cell

Ia = zeros(size(Vc));   % allocate array for solar cell current

for k=1:5;  % calculate solar cell current using newton-raphson method
    Ia = Ia -(Iph - Ia - Ir.*( exp((Vc+Ia.*Rs)./Vt_Ta) -1)-(Vc+Ia.*Rs)./Rsh)./ (-1 - (Ir.*( exp((Vc+Ia.*Rs)./Vt_Ta))).*Rs./Vt_Ta - Rs/Rsh);
end
Ia = Np*Npv*Ia; % calculate current for number of cells and panels in parallel

figure; % create new figure to plot data
title('Solar Panel IxV Curve'); % add title to plot
hold on;    % keep same plot
ax = gca;   % take current axis
xlabel('Voltage [V]');  % add label to x axis
ylabel('Current [A]');  % add label to y axis

if nargin == 5  % verify if the measured voltage and the measured current were passed to the function
    yyaxis left;    % take left axis of plot
    ax.YColor = 'k';    % change axis color to black
    ylabel('Error [%]');    % add label to left y axis
    yyaxis right;   % take right axis of plot
    ylabel('Current [A]');  % add label to right y axis
    ax.YColor = 'k';    % change axis color to black
    error_percent = zeros(size(Vc));  % allocate memory for error array
    for i=1:length(Vc)  % loop through all the data
        error_percent(i) = 100*((Ia(i) - measured_current(i))/measured_current(i));   % calculate error from model (%)
    end
    yyaxis left    % take left axis of the plot
    p_error = plot(Va, error_percent); % plot error (%)
    yyaxis right;   % take right axis of the plot
    plot(Va, measured_current, 'r'); % plot measured IxV curve
end

p = plot(Va, Ia, 'g');   % plot model IxV curve

if nargin == 5  % verify if measured current and measured current were passed to the function
    legend('error', 'measured curve', 'model curve');   %   add legend for three curves if the measurements were passed to the function
else
    legend('model curve');  % add legend to the single curve if the measurements were not passed to the function
end

ylim([0 inf]);  % limit plot to not show the negative portion of the curve, which is not relevant
uicontrol('Style','text','Position',[345 5 40 20],'string','Rs');  % add Rs slider label
uicontrol('Style','text','Position',[520 5 40 20],'string','A');  % add A slider label
uicontrol('Style','text','Position',[680 5 40 20],'string','Ir');  % add Ir slider label
uicontrol('Style','text','Position',[840 5 40 20],'string','Rsh');  % add Rsh slider label
uicontrol('position',[270,30,40,20],'style','edit','string',Rs);    % add Rs value to screen
uicontrol('position',[445,30,40,20],'style','edit','string',A);    % add A value to screen
uicontrol('position',[605,30,40,20],'style','edit','string',Ir);    % add Ir value to screen
uicontrol('position',[765,30,40,20],'style','edit','string',Rsh);    % add Rsh value to screen
uicontrol('Style', 'slider','Min',-2,'Max',10,'Value',Rs,'Position', [225 5 120 20],'Callback', @plot_curve_Rs);   % create uicontrol object for Rs slider
uicontrol('Style', 'slider','Min',1,'Max',3,'Value',A,'Position', [400 5 120 20],'Callback', @plot_curve_A);   % create uicontrol object for A slider
uicontrol('Style', 'slider','Min',1e-7,'Max',1e-5,'Value',Ir,'Position', [560 5 120 20],'Callback', @plot_curve_Ir);   % create uicontrol object for Ir slider
uicontrol('Style', 'slider','Min',20,'Max',1500,'Value',Rsh,'Position', [720 5 120 20],'Callback', @plot_curve_Rsh);   % create uicontrol object for Rsh slider

    function plot_curve_Rs(source, ~)  % callback function for slider
        Rs = source.Value;  % get Rs value
        uicontrol('position',[270,30,40,20],'style','edit','string',Rs);    % add Rs value to screen
        Ia = zeros(size(Vc));   % set current array to zero in case the method didn't converge in the last change of parameters
        for j=1:5;  % calculate solar cell current using newton-raphson method
            Ia = Ia -(Iph - Ia - Ir.*( exp((Vc+Ia.*Rs)./Vt_Ta) -1)-(Vc+Ia.*Rs)./Rsh)./ (-1 - (Ir.*( exp((Vc+Ia.*Rs)./Vt_Ta))).*Rs./Vt_Ta - Rs/Rsh);
        end
        Ia = Np*Npv*Ia; % calculate current for number of cells and panels in parallel
        assignin('base', 'Io', Ia); % assign current values to variable in the base workspace
        yyaxis right;   % take right axis of plot
        set(p, 'YData', Ia);    % plot graph with new Ia value
        if (isgraphics(p_error) == 1)   % verify if the error is being plotted (in case measurements were passed to the function)
            error_percent = zeros(size(Vc));    % set percentual error array to zero
            for l=1:length(Vc)
                error_percent(l) = 100*((Ia(l) - measured_current(l))/measured_current(l));   % calculate error from model (%)
            end
            yyaxis left;    % take left axis of plot
            set(p_error, 'YData', error_percent);   % plot graph with new error value
        end
    end

    function plot_curve_A(source, ~)  % callback function for slider
        A = source.Value;  % get A value
        uicontrol('position',[445,30,40,20],'style','edit','string',A);    % add A value to screen
        Ia = zeros(size(Vc));   % set current array to zero in case method didn't converge in the last change of value 
        Vt_Ta = A * 1.38e-23 * TaK / 1.60e-19; %    calculate thermal voltage
        for j=1:5;  % calculate solar cell current using newton-raphson method
            Ia = Ia -(Iph - Ia - Ir.*( exp((Vc+Ia.*Rs)./Vt_Ta) -1)-(Vc+Ia.*Rs)./Rsh)./ (-1 - (Ir.*( exp((Vc+Ia.*Rs)./Vt_Ta))).*Rs./Vt_Ta - Rs/Rsh);
        end
        Ia = Np*Npv*Ia; % calculate current for number of cells and panels in parallel
        assignin('base', 'Io', Ia); % assing current value to variable in the base workspace
        yyaxis right;   % take right axis of plot
        set(p, 'YData', Ia);    % plot graph with new Ia value
        if (isgraphics(p_error) == 1)   % verify if error is being plotted (in case measurements were passed to the function)
            error_percent = zeros(size(Vc));    % set percentual error array to zero
            for l=1:length(Vc)
                error_percent(l) = 100*((Ia(l) - measured_current(l))/measured_current(l));   % calculate error from model (%)
            end
            yyaxis left;    % take left axis of plot
            set(p_error, 'YData', error_percent);   % plot graph with new error value
        end
    end

    function plot_curve_Ir(source, ~)  % callback function for slider
        Ir = source.Value;  % get Ir value
        uicontrol('position',[605,30,40,20],'style','edit','string',Ir);    % add Ir value to screen
        Ia = zeros(size(Vc));   % set current array to zero in case method didn't converge in the last change of value 
        for j=1:5;  % calculate solar cell current using newton-raphson method
            Ia = Ia -(Iph - Ia - Ir.*( exp((Vc+Ia.*Rs)./Vt_Ta) -1)-(Vc+Ia.*Rs)./Rsh)./ (-1 - (Ir.*( exp((Vc+Ia.*Rs)./Vt_Ta))).*Rs./Vt_Ta - Rs/Rsh);
        end
        Ia = Np*Npv*Ia; % calculate current for number of cells and panels in parallel
        assignin('base', 'Io', Ia); % assing current value to variable in the base workspace
        yyaxis right;   % take right axis of plot
        set(p, 'YData', Ia);    % plot graph with new Ia value
        if (isgraphics(p_error) == 1)   % verify if error is being plotted (in case measurements were passed to the function)
            error_percent = zeros(size(Vc));    % set percentual error array to zero
            for l=1:length(Vc)
                error_percent(l) = 100*((Ia(l) - measured_current(l))/measured_current(l));   % calculate error from model (%)
            end
            yyaxis left;    % take left axis of plot
            set(p_error, 'YData', error_percent);   % plot graph with new error value
        end
    end

    function plot_curve_Rsh(source, ~) % callback function for slider
        Rsh = source.Value;  % get Rsh value
        uicontrol('position',[765,30,40,20],'style','edit','string',Rsh);    % add Rsh value to screen
        Ia = zeros(size(Vc));   % set current array to zero in case method didn't converge in the last change of value 
        for j=1:5;  % calculate solar cell current using newton-raphson method
            Ia = Ia -(Iph - Ia - Ir.*( exp((Vc+Ia.*Rs)./Vt_Ta) -1)-(Vc+Ia.*Rs)./Rsh)./ (-1 - (Ir.*( exp((Vc+Ia.*Rs)./Vt_Ta))).*Rs./Vt_Ta - Rs/Rsh);
        end
        Ia = Np*Npv*Ia; % calculate current for number of cells and panels in parallel
        assignin('base', 'Io', Ia); % assing current value to variable in the base workspace
        yyaxis right;   % take right axis of plot
        set(p, 'YData', Ia);    % plot graph with new Ia value
        if (isgraphics(p_error) == 1)   % verify if error is being plotted (in case measurements were passed to the function)
            error_percent = zeros(size(Vc));    % set percentual error array to zero
            for l=1:length(Vc)
                error_percent(l) = 100*((Ia(l) - measured_current(l))/measured_current(l));   % calculate error from model (%)
            end
            yyaxis left;    % take left axis of plot
            set(p_error, 'YData', error_percent);   % plot graph with new error value
        end
    end
end

function Ia = solar_cell(Va,Suns,TaC,measured_voltage,measured_current)
%Code produced by the Dept of Computer Science and Electrical Engineering,
%University of Queensland, Australia
%and modified by Embedded Systems Group - UFSC

if nargin == 5
    Va = measured_voltage(1:10:length(measured_voltage));
    measured_current = measured_current(1:10:length(measured_current));
end

% current given voltage, illumination and temperature
% Ia = msx60(Va,G,T) = array voltage
% Ia,Va = array current,voltage
% G = num of Suns (1 Sun = 1000 W/mˆ2)
% T = Temp in Deg C

k = 1.38e-23; % Boltzman’s const
q = 1.60e-19; % charge on an electron

% enter the following constants here, and the model will be
% calculated based on these. for 1000W/mˆ2
A = 2; % "diode quality" factor, =2 for crystaline, <2 for amorphous
Vg = 1.12; % band gap voltage, 1.12eV for xtal Si, ˜1.75 for amorphous Si.
Ns = 10; % number of series connected cells (diodes)
Np = 4; % number of parallel connected cells
Npv = 3;    % number of panels

T1 = 273 + 25;
Voc_T1 =0.630; % open cct voltage per cell at temperature T1
Isc_T1 = 0.050; % short cct current per cell at temp T1

T2 = 273 + 0;
Voc_T2 = 0.6825; % open cct voltage per cell at temperature T2
Isc_T2 = 0.04538; % short cct current per cell at temp T2

TaK = 273 + TaC; % array working temp
TrK = 273 + 25; % reference temp

% when Va = 0, light generated current Iph_T1 = array short cct current
% constant "a" can be determined from Isc vs T

Iph_T1 = Isc_T1 * Suns;
a = (Isc_T2 - Isc_T1)/Isc_T1 * 1/(T2 - T1);
Iph = Iph_T1 * (1 + a*(TaK - T1));

Vt_T1 = k * T1 / q; % = A * kT/q
Ir_T1 = Isc_T1 / (exp(Voc_T1/(A*Vt_T1))-1);
Ir_T2 = Isc_T2 / (exp(Voc_T2/(A*Vt_T1))-1);

b = Vg * q/(A*k);
Ir = Ir_T1 * (TaK/T1).^(3/A) .* exp(-b.*(1./TaK - 1/T1));

X2v = Ir_T1/(A*Vt_T1) * exp(Voc_T1/(A*Vt_T1));
dVdI_Voc =  -0.9635;               % dV/dI at Voc per cell --
% from manufacturers graph
Rs = (Ns)*(- dVdI_Voc - 1/X2v);         % series resistance per cell

%Calculation of serie resistance Rs
%Vpmax_Tref = 0.501; %voltage at maximum power for T=Tref
%Ipmax_Tref = 0.0372; %current at maximum power for T=Tref
%Rs = (Voc_T1-Vpmax_Tref)/Ipmax_Tref;

Rsh = 954.801;

% Ia = 0:0.01:Iph;
Vt_Ta = A * 1.38e-23 * TaK / 1.60e-19; %=A* kT/q

Vc = Va/Ns;
Ia = zeros(size(Vc));
% Iav = Ia;
for k=1:5;
    Ia = Ia -(Iph - Ia - Ir.*( exp((Vc+Ia.*Rs)./Vt_Ta) -1)-(Vc+Ia.*Rs)./Rsh)./ (-1 - (Ir.*( exp((Vc+Ia.*Rs)./Vt_Ta))).*Rs./Vt_Ta - Rs/Rsh);
    % Iav = [Iav;Ia]; % to observe convergence for debugging.
end
Ia = Np*Npv*Ia; % calculate current for number of cells and panels in parallel

figure; % create new figure to plot data
title('Solar Panel IxV Curve');
hold on;    % keep same plot
ax = gca;
xlabel('Voltage [V]');
ylabel('Current [A]');

if nargin == 5  % verify if the measured voltage and the measured current were passed to the function
    yyaxis left;
    ax.YColor = 'k';
    ylabel('Error [%]');
    yyaxis right;
    ylabel('Current [A]');
    ax.YColor = 'k';
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

if nargin == 5
    legend('error', 'measured curve', 'model curve');
else 
    legend('model curve');
end

ylim([0 inf]);
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
uicontrol('Style', 'slider','Min',1e-7,'Max',10e-7,'Value',Ir,'Position', [560 5 120 20],'Callback', @plot_curve_Ir);   % create uicontrol object for Ir slider
uicontrol('Style', 'slider','Min',20,'Max',1500,'Value',Rsh,'Position', [720 5 120 20],'Callback', @plot_curve_Rsh);   % create uicontrol object for Rsh slider

    function plot_curve_Rs(source, ~)  % callback function for slider
        Rs = source.Value;  % get Rs value
        uicontrol('position',[270,30,40,20],'style','edit','string',Rs);    % add Rs value to screen
        Ia = zeros(size(Vc));
        for j=1:5;
            Ia = Ia -(Iph - Ia - Ir.*( exp((Vc+Ia.*Rs)./Vt_Ta) -1)-(Vc+Ia.*Rs)./Rsh)./ (-1 - (Ir.*( exp((Vc+Ia.*Rs)./Vt_Ta))).*Rs./Vt_Ta - Rs/Rsh);
        end
        Ia = Np*Npv*Ia;
        assignin('base', 'Io', Ia);
        yyaxis right;
        set(p, 'YData', Ia);    % plot graph with new Ia value
        if (isgraphics(p_error) == 1)
            error_percent = zeros(size(Vc));
            for l=1:length(Vc)
                error_percent(l) = 100*((Ia(l) - measured_current(l))/measured_current(l));   % calculate error from model (%)
            end
            yyaxis left;
            set(p_error, 'YData', error_percent);
        end
    end

    function plot_curve_A(source, ~)  % callback function for slider
        A = source.Value;  % get A value
        uicontrol('position',[445,30,40,20],'style','edit','string',A);    % add A value to screen
        Ia = zeros(size(Vc));
        Vt_Ta = A * 1.38e-23 * TaK / 1.60e-19; %=A* kT/q;
        for j=1:5;
            Ia = Ia -(Iph - Ia - Ir.*( exp((Vc+Ia.*Rs)./Vt_Ta) -1)-(Vc+Ia.*Rs)./Rsh)./ (-1 - (Ir.*( exp((Vc+Ia.*Rs)./Vt_Ta))).*Rs./Vt_Ta - Rs/Rsh);
        end
        Ia = Np*Npv*Ia;
        assignin('base', 'Io', Ia);
        yyaxis right;
        set(p, 'YData', Ia);    % plot graph with new Ia value
        if (isgraphics(p_error) == 1)
            error_percent = zeros(size(Vc));
            for l=1:length(Vc)
                error_percent(l) = 100*((Ia(l) - measured_current(l))/measured_current(l));   % calculate error from model (%)
            end
            yyaxis left;
            set(p_error, 'YData', error_percent);
        end
    end

    function plot_curve_Ir(source, ~)  % callback function for slider
        Ir = source.Value;  % get Ir value
        uicontrol('position',[605,30,40,20],'style','edit','string',Ir);    % add Ir value to screen
        Ia = zeros(size(Vc));
        for j=1:5;
            Ia = Ia -(Iph - Ia - Ir.*( exp((Vc+Ia.*Rs)./Vt_Ta) -1)-(Vc+Ia.*Rs)./Rsh)./ (-1 - (Ir.*( exp((Vc+Ia.*Rs)./Vt_Ta))).*Rs./Vt_Ta - Rs/Rsh);
        end
        Ia = Np*Npv*Ia;
        assignin('base', 'Io', Ia);
        yyaxis right;
        set(p, 'YData', Ia);    % plot graph with new Ia value
        if (isgraphics(p_error) == 1)
            error_percent = zeros(size(Vc));
            for l=1:length(Vc)
                error_percent(l) = 100*((Ia(l) - measured_current(l))/measured_current(l));   % calculate error from model (%)
            end
            yyaxis left;
            set(p_error, 'YData', error_percent);
        end
    end

    function plot_curve_Rsh(source, ~) % callback function for slider
        Rsh = source.Value;  % get Rsh value
        uicontrol('position',[765,30,40,20],'style','edit','string',Rsh);    % add Rsh value to screen
        Ia = zeros(size(Vc));
        for j=1:5;
            Ia = Ia -(Iph - Ia - Ir.*( exp((Vc+Ia.*Rs)./Vt_Ta) -1)-(Vc+Ia.*Rs)./Rsh)./ (-1 - (Ir.*( exp((Vc+Ia.*Rs)./Vt_Ta))).*Rs./Vt_Ta - Rs/Rsh);
        end
        Ia = Np*Npv*Ia;
        assignin('base', 'Io', Ia);
        yyaxis right;
        set(p, 'YData', Ia);    % plot graph with new Ia value
        if (isgraphics(p_error) == 1)
            error_percent = zeros(size(Vc));
            for l=1:length(Vc)
                error_percent(l) = 100*((Ia(l) - measured_current(l))/measured_current(l));   % calculate error from model (%)
            end
            yyaxis left;
            set(p_error, 'YData', error_percent);
        end
    end
end

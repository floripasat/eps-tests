function Ia = solar_cell(Va,Suns,TaC)
%Code produced by the Dept of Computer Science and Electrical Engineering,
%University of Queensland, Australia
%and modified by Embedded Systems Group - UFSC

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

% Ia = 0:0.01:Iph;
Vt_Ta = A * 1.38e-23 * TaK / 1.60e-19; %=A* kT/q

Vc = Va/Ns;
Ia = zeros(size(Vc));
% Iav = Ia;
for j=1:5;
Ia = Ia -(Iph - Ia - Ir.*( exp((Vc+Ia.*Rs)./Vt_Ta) -1))./ (-1 - (Ir.*( exp((Vc+Ia.*Rs)./Vt_Ta) -1)).*Rs./Vt_Ta);
% Iav = [Iav;Ia]; % to observe convergence for debugging.
end
Ia = Np*Npv*Ia;



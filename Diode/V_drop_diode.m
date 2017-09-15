function Vd = V_drop_diode(I,T)
% Code produced by Embedded Systems Group - UFSC
% Voltage drop to diode : NSVR0320MW2T1G 
% T = Temp in Deg C
% I = Current (A)
 
k = 1.38e-23;    % Boltzman�s const
q = 1.60e-19;    % Charge on an electron
n = 0.9830;      % "diode quality" factor

T_K = 273 + T;   % Temp in Kelvin T
Vt = k*T_K/q;    % Thermal voltage

Is_23 = 1.02696e-6;             % The saturation current at 23�C 
Is = Is_23 + 0.127e-6 * (T-23); % The Saturation Current at Temp T

Vd = n * Vt * log(I/Is);         % Forward Voltage Drop Diode
end
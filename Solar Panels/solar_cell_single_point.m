function Ia = solar_cell_single_point(Va,Irradiance,TaC)
%Code produced by the Dept of Computer Science and Electrical Engineering,
%University of Queensland, Australia and modified by Embedded Systems Group - UFSC

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

Ia = 0;

for k=1:5;  % calculate solar cell current using newton-raphson method
    Ia = Ia -(Iph - Ia - Ir.*( exp((Vc+Ia.*Rs)./Vt_Ta) -1)-(Vc+Ia.*Rs)./Rsh)./ (-1 - (Ir.*( exp((Vc+Ia.*Rs)./Vt_Ta))).*Rs./Vt_Ta - Rs/Rsh);
end
Ia = Np*Npv*Ia; % calculate current for number of cells and panels in parallel
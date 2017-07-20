lambda = 400:10:730; % wavelength values [nm]
K = 102.8938907;    % light intensity normalization factor
I = 2;    % led current [A]
A = 0.02*0.006; % solar cell area [m^2]
eta = 0.22; % solar cell efficiency
n = 40; % number of solar cells in a panel
N = 3;  % number of solar panels

V = [0 0.683 2.732 8.196 15.709 25.954 40.98 62.153 94.937 142.064 220.609 343.549 484.93 588.746 651.582 ...       % luminosity function data (dimensionless)
    679.585 679.585 650.216 594.21 517.031 430.973 343.549 260.223 180.995 119.525 73.081 41.663 21.856 11.611 5.464 2.732 1.366 0.683 0];

figure; % add new figure window
plot(lambda, V);    % plot fitted luminosity function
title('Luminosity Function');   % add title to plot
xlabel('\lambda [nm]'); % add x axis label (wavelength)
ylabel('V(\lambda)');   % add y axis label (luminosity function value)

RLI = [0.000 0.020 0.040 0.110 0.320 0.800 0.880 0.620 0.380 0.190 0.150 0.150 0.210 0.290 0.360 0.400 0.440 0.450 ...  % relative light intensity data [dimensionless]
    0.450 0.443 0.413 0.383 0.353 0.323 0.290 0.250 0.210 0.170 0.145 0.135 0.125 0.115 0.105 0.095];

figure; % add new figure window
plot(lambda, RLI);   % plot fitted RLI
title('Relative Light Intensity');  % add title to plot
xlabel('\lambda [nm]'); % add x axis label (wavelength)
ylabel('RLI(\lambda)'); % add y axis label (relative light intensity)

ALI = K*RLI; % absolute light intensity, obtained by multiplying the relative light intesity by the normalization factor

figure; % add new figure window
plot(ALI);  % plot ALI
title('Absolute Light Intensity');  % add title to plot
xlabel('\lambda [nm]'); % add x axis label (wavelenght)
ylabel('ALI(\lambda) [lm]'); % add y axis label (absolute light intensity)

P = ALI./V; % radiant flux from the led [watts]

Irrad = nansum(P(~isinf(P))/(4*pi*0.193^2)); 
%{ 
irradiance from the led [W/m^2], calulated by formula irrad = E/(4*pi*r^2),
where P is the radiant flux [watts] from the led and r is the distance between 
the led and the solar panel. The distance used was the to the center of the
panel directly below the led.
%}

Ppv = (I/3)*Irrad*A*eta*n*N;    % solar panels maximum power




                                           


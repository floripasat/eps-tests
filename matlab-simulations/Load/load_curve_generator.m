time_step = 0.1;        % time vector step size
load_voltage = 5;       % load voltage in volts

time_vector = 0:time_step:5790;             % time vector, 96.5 minutes total
power_vector = zeros(size(time_vector));    % power vector, the same size as the time vector
current_vector = zeros(size(time_vector));  % current vector, the same size as the time vector

downlink_power = 2.5;           % downlink power consumed in watts
periodic_downlink_power = 2.5;  % periodic downlink power consumed in watts
beacon_power = 2.5;             % beacon power consumed in watts
obdh_static_power = 0.104508;   % obdh static power consumed in watts
heater_power = 3.18;            % heater power consumed in watts

downlink_duration = 600;        % downlink duration in seconds
periodic_downlink_duration = 2; % periodic downlink duration in seconds
beacon_duration = 0.6;          % beacon duration in seconds

periodic_downlink_period = 60;  % periodic downlink period in seconds
beacon_period = 10;             % beacon period in seconds

% compute base power vactor values considering the obdh is always on and
% will always have the same static consumption

for i = 1:1:length(power_vector)
    power_vector(i) = obdh_static_power;
end

% compute power vector values for the first 10 minutes of the orbit,
% considering that the downlink occurs at the start of the orbit and the
% other load is the obdh static consumption

for i = 1:1:(downlink_duration/time_step)
    power_vector(i) = power_vector(i) + downlink_power;
end

% compute power vector values for the moments when the beacon is
% transmitting

for j = ((downlink_duration/time_step)+(1/time_step)):(beacon_period/time_step):(length(power_vector)-1)
    for i = j:1:(j+(beacon_duration/time_step))
        power_vector(i) = power_vector(i) + beacon_power;
    end
end

% compute power vector values when there is a periodic downlink
% transmission

for j = ((downlink_duration/time_step)+(3/time_step)):(periodic_downlink_period/time_step):(length(power_vector)-1)
    for i = j:1:(j+(periodic_downlink_duration/time_step))
        power_vector(i) = power_vector(i) + periodic_downlink_power;
    end
end

% compute power vector when the heater is on (during eclipse)

for i = int32(3664.75/time_step):1:length(power_vector)
    power_vector(i) = power_vector(i) + heater_power;
end

% compute current vector

for i = 1:1:length(current_vector)
    current_vector(i) = -power_vector(i)/load_voltage;
end







 




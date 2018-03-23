# Load Curve Generator
This matlab script generates a vector to be used as the load curve in the EPS tests.

## OBDH Static
To model the OBDH consumption only the static power is considered. This consumption is present during the whole orbit.

## Downlink
As a worst case scenario (energywise), the downlink is performed once every orbit. This consumption is present during the first 10 minutes
of each orbit.

## Beacon
The Beacon consumption is modeled considering a period of 10 seconds and a transmission duration of 0.6 seconds. This consumption is
present during the whole orbit.

## Periodic Downlink
The Periodic Downlink consumption is modeled considering a period of 60 seconds and a transmission duration of 2 seconds. This consumption
is present during the whole orbit.

## Heaters
The heaters are only active during eclipses, so the consumption is present during approximately 1/3 of the orbit.

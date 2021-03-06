# WeatherStation
<p align="center">
  <img src="Website/images/1.png" width="350" title="hover text">
</p>
The main goal of the project is to create an autonomous Wi-Fi weather station based on STM32F103. ESP-01 is used for transmitt data to server and BME280 for getting temperature, humidity, pressure readings. In the future, this weather station will become part of the smart home.
<br>
<br>
<p align="center">
  <img src="Website/images/3.JPG" title="hover text">
</p>

#### Legs configuration for weather station
<p align="center">
  <img src="Website/images/MK legs configuration for weather station.png" title="hover text">
</p>
<br>
<p>In the dark, the work of the station is provided by a 18650 lithium-ion battery, and in the light by the solar panel, which charges the battery and provides sleep power.</p>
<br>
<p align="center">
  <img src="Website/images/4.png" title="hover text">
</p>
<br>

#### PCB Example
<p align="center">
  <img src="Website/images/5.PNG" title="hover text">
</p>

## Сomponents
- STM32F103C8
- ESP-01
- BME280
- TP4056
- MT3608
- Balancer 3.3V
- 3xButton
- 3xLed
- 3xСapacitor (16V, 1000uF)
- 2xConnector
- 5xResistor (3x220Ω, 1x91kΩ, 1x47kΩ)
- 3xSchottky diodes
- 1xSolar panel (6V, 1W)
- 1xNCR18650B (Li-ion)
- 1xPanel for battery (18650)
- 1xSolder breadboard

## Database
The database consists of one table:
<p align="center">
  <img src="Website/images/8.PNG" title="hover text">
</p>

## Display result
For debugging, output to the table is used:
<p align="center">
  <img src="Website/images/6.PNG" title="hover text">
</p>
The average user takes information from the site:
<br>
<p align="center">
  <img src="Website/images/7.PNG" title="hover text">
</p>

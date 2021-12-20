
**DISCLAIMER: This project uses voltages that can kill you and or cause a fire.  This is a proof of concept and not a final product.  Do not attempt working with these voltages unless you know exactly what you are doing.**



# Humidity Controller #

This is an example of using the ESP32PID to control the humidity in the air.  The input is a DHT22 that reads the moisture content in the air.  The output is a mechanical relay that switches on a humidifier.  This example uses a 60 second window to turn on the humidifier for a percentage of the 60 seconds based on the output of the controller.


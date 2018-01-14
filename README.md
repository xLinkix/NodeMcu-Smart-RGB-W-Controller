# NodeMcu-Smart-RGB-W-Controller
Smart RGBW Controller for Node Mcu

ESP 12-E PWM Dimming -> you need external 3v Mosfets to contorl your LED Stripe

Your Wifi password should not contain only "0"!

Pins:
D1 Red      Safe Pin
D2 Green    Safe Pin
D3 Blue

D5 White

Usage
1.  Flash the ino with Arduino https://www.arduino.cc/en/Main/Software on the Node
2.  Connect to the "Smart_Connect" Wifi
3.  type 192.168.4.1 into your browser and past your showed Wifi into the first bar and your Wifi password into the second
4.  Control the RGB Led with this setup
    http://deviceip/?rgb=bbbgggrrr
    bbb = blue 0-100%
    ggg = green 0-100%
    rbb = red 0-100%
    
    Control the white Led with 
    http://deviceip/?w=www
    www = white 0-100%
    
    Example:
    Full On
    http://deviceip/?rgb=100100100
    http://deviceip/?w=100
    
    Full green
    http://deviceip/?rgb=100000 or ?rgb=000100000
    
Please report Issues and Errors

Smart Connect Core by https://github.com/nichtNicolas

Made with and for ESP-12-E

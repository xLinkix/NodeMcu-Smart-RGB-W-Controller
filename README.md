# NodeMcu-Smart-RGB-W-Controller
Smart RGBW Controller for Node Mcu

ESP 12-E PWM Dimming -> you need external 3v Mosfets to contorl your LED Stripe

Your Wifi password should not contain just "0"s!

Pins:
D1 Red     
D2 Green    
D3 Blue

D5 White

Usage
1.  Flash the .ino (with e.g. Arduino https://www.arduino.cc/en/Main/Software) on the ESP
2.  Connect to the "Smart_Connect" Wifi
3.  Type 192.168.4.1 into your browser and login to your Wifi-Network showed in the list
4.  Control the RGB Led with this setup

        http://deviceip/?r=rrr&b=bbb&g=ggg&w=www
    
    rrr = red 0-100
    ggg = green 0-100
    bbb = blue 0-100      
    www = white 0-100
    
    
    Example for 100% Green:
  
        http://deviceip/?g=100 
    
Please report Issues and Errors

Smart Connect Core by https://github.com/nichtNicolas

Made with and for ESP-12-E

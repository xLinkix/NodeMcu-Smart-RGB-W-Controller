# NodeMcu-Smart-RGB-W-Controller
Smart RGBW Controller for Node Mcu

Your Wifi password should not contain only "0"!

D1 and D2 are safe pins.

Pins:
D1 Red
D2 Green
D3 Blue

D5 White

Usage
1.  Connect to the "Smart_Connect" Wifi
2.  type 192.168.4.1 into your browser and past your showed Wifi into the first bar and your Wifi password into the second
3.  Control the RGB Led with this setup
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

Made with and for ESP-12-E

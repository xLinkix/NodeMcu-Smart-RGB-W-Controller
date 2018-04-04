#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

ESP8266WebServer server(80);

char* ssid = "Smart_Connect";
char* passphrase = ""; 
String st;
String content;
int statusCode;

int restartmin = 60;

const int d1 = 5;
const int d2 = 4;
const int d3 = 0;
const int d4 = 2;
const int d5 = 14;
const int d6 = 12;
const int d7 = 13;
const int d8 = 15;
const int d9 = 3;
const int d10 = 1;

//Pins
const int ledPinR = d1; 
const int ledPinG = d2; 
const int ledPinB = d3; 
const int ledPinW = d5; 

const int powerRelais = d6; // GPIO13 D7
const int powerRelais2 = d7;

//RGBW
int r = 0;
int g = 0;
int b = 0;
int w = 0;

int cycle;
float cycleNum;

void setup() {
  Serial.begin(115200); //baud
  EEPROM.begin(512); 

  //Define Pins
  pinMode(ledPinR,OUTPUT);
  pinMode(ledPinG,OUTPUT);
  pinMode(ledPinB,OUTPUT);
  pinMode(ledPinW,OUTPUT);

  pinMode(powerRelais,OUTPUT);
  pinMode(powerRelais2,OUTPUT);
  
  //Start PWm Pins max duty = 255 o 1023
  cycle = 1023;
  cycleNum = cycle / 10;
  analogWrite(ledPinR, 0);
  analogWrite(ledPinG, 0);
  analogWrite(ledPinB, 0);
  analogWrite(ledPinW, 0); 

  digitalWrite(powerRelais, HIGH);
  digitalWrite(powerRelais2, HIGH);
  
  Serial.println("System Online");
  Serial.println("Version: 1.2");

  restartmin = restartmin *60 * 1000;
  
  //connection
  // read eeprom for ssid and pass
  Serial.println("------------");
  Serial.println("Reading ssid");
  int nullen = 0;
  
  String esid;
  for (int i = 0; i < 32; ++i)
    {
      esid += char(EEPROM.read(i));
      if (EEPROM.read(i) == 0){
        nullen++;
      }
    }
  Serial.println("Reading  password");
  String epass = "";
  for (int i = 32; i < 96; i++)
    {
      epass += char(EEPROM.read(i));
    } 

    /*Serial.println("SSID: ");
    Serial.print(esid);
    Serial.println("PSW: ");
    Serial.print(epass);*/
  
  if ( esid.length() > 1 && nullen < 32) {
      WiFi.begin(esid.c_str(), epass.c_str());
      if (testWifi()) {
        WiFi.softAPdisconnect(true);
        launchWeb(0);
        return;
      } 
  } else {
    Serial.println("Entering Config Mode");
    setupAP(); 
  }                     
}

bool testWifi(void) {
  int c = 0;
  Serial.println("Waiting for Wifi to connect to ESP8266");  
  while ( c < 30 ) {
    if (WiFi.status() == WL_CONNECTED) { return true; } 
    delay(500);
    Serial.print(WiFi.status());    
    c++;
  }
  Serial.println("");
  Serial.println("Connect timed out, opening AP");
  setupAP();
  return false;
} 

void launchWeb(int webtype) {
  Serial.println("");
  Serial.println("WiFi started");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("SoftAP IP: ");
  Serial.println(WiFi.softAPIP());
  createWebServer(webtype);
  // Start the server
  server.begin();
  Serial.println("Server started"); 
}

void setupAP(void) {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
    Serial.println("no networks found");
  }
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
     {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
     }
     
    Serial.println(""); 
    st = "<ol>";
    for (int i = 0; i < n; ++i)
      {
        // Print SSID and RSSI for each network found
        st += "<li>";
        st += WiFi.SSID(i);
        st += " (";
        st += WiFi.RSSI(i);
        st += ")";
        st += (WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*";
        st += "</li>";
      }
    st += "</ol>";
  }
  
  WiFi.softAP(ssid, passphrase);
  Serial.println("softAp");
  launchWeb(1);
  Serial.println("Config Server started");
}

void createWebServer(int webtype)
{
  Serial.println("Webtype: ");
  Serial.println(webtype);
  if ( webtype == 1 ) {
    server.on("/", []() {
        IPAddress ip = WiFi.softAPIP();
        String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
        content = "<!DOCTYPE HTML>\r\n<html>WiFi configuration page for :  ";
        content += ipStr;
        content += "<p>";
        content += st;
        content += "</p><form method='get' action='setting'><label>SSID: </label><input name='ssid' length=32><input name='pass' length=64><input type='submit'></form>";
        content += "</html>";
        server.send(200, "text/html", content);  
    });
    server.on("/setting", []() {
        String qsid = server.arg("ssid");
        String qpass = server.arg("pass");
        if (qsid.length() > 0 && qpass.length() > 0) {
          Serial.println("clearing eeprom");
          for (int i = 0; i < 96; ++i) { EEPROM.write(i, 0); }
          Serial.println(qsid);
          Serial.println("");
          Serial.println(qpass);
          Serial.println("");
            
          Serial.println("writing eeprom ssid:");
          for (int i = 0; i < qsid.length(); ++i)
            {
              EEPROM.write(i, qsid[i]);
              //Serial.print("Wrote: ");
              //Serial.println(qsid[i]); 
            }
          Serial.println("writing eeprom pass:"); 
          for (int i = 0; i < qpass.length(); ++i)
            {
              EEPROM.write(32+i, qpass[i]);
              //Serial.print("Wrote: ");
              //Serial.println(qpass[i]); 
            }    
          EEPROM.commit();
          content = "{\"Success\":\"saved to eeprom....... restarting to boot into new wifi\"}";
          statusCode = 200;
          server.send(statusCode, "application/json", content);
          delay(100);
          ESP.restart();
        } else {
          content = "{\"Error\":\"404 not found\"}";
          statusCode = 404;
          Serial.println("Sending 404");
        }
        server.send(statusCode, "application/json", content);
    });
  } else if (webtype == 0) {
    server.on("/", []() {
    String message = "";
      
      if (server.arg("r")== ""){     //Parameter not found
      }else{     //Parameter found  
          float ppR = server.arg("r").toInt();
          ppR * cycleNum;
          int pR = (int)ppR;
          if(pR > cycle) pR = cycle;
          if(pR >= 0) r = pR;
      }

      if (server.arg("g")== ""){     //Parameter not found
      }else{     //Parameter found  
          float ppG = server.arg("g").toInt();
          ppG * cycleNum;
          int pG = (int)ppG;
          if(pG > cycle) pG = cycle;
          if(pG >= 0) g = pG;
      }

      if (server.arg("b")== ""){     //Parameter not found
      }else{     //Parameter found  
          float ppB = server.arg("b").toInt();
          ppB * cycleNum;
          int pB = (int)ppB;
          if(pB > cycle) pB = cycle;
          if(pB >= 0) b = pB;
      }

      if (server.arg("w")== ""){     //Parameter not found
      }else{     //Parameter found  
          float ppW = server.arg("w").toInt();
          ppW * cycleNum;
          int pW = (int)ppW;
          if(pW > cycle) pW = cycle;
          if(pW >= 0) w = pW;
      }
      
      Serial.print("R: ");
      Serial.println(r);
      
      Serial.print("G: ");
      Serial.println(g);

      Serial.print("B: ");
      Serial.println(b);

      Serial.print("W: ");
      Serial.println(w);
      Serial.println("----------");
      message += millis() / 1000;           
      server.send(200, "text/html", message);
    });
    server.on("/cleareeprom", []() {
      content = "<!DOCTYPE HTML>\r\n<html>";
      content += "<p>Clearing the EEPROM and Restarting</p></html>";
      server.send(200, "text/html", content);
      Serial.println("clearing eeprom and restarting");
      for (int f= 0; f < 95; f++) { 
        EEPROM.write(f, 0); 
      }
      EEPROM.commit();
      delay(200);
      ESP.restart();
    });
    server.on("/runtime", []() {    //Timer
      int time = millis() / 1000;
      content = "<!DOCTYPE HTML>\r\n<html>";
      content += "<p>Time</p>";
      content += time;
      content += "</html>";
      server.send(200, "text/html", content);
    });
  }
}

void reboot(){
  if(millis() > restartmin){
    if(r == 0 && g == 0 && b == 0 && w == 0){
      ESP.restart();
    }      
  }   
}
  
void loop() {
  server.handleClient();  
    
  //Color Change
  analogWrite(ledPinR, r);
  analogWrite(ledPinG, g);
  analogWrite(ledPinB, b);
  analogWrite(ledPinW, w);


  reboot();
}

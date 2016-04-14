#include <SPI.h>
#include <ESP8266WiFi.h>
#include <YalerESP8266WiFiServer.h>

int pin = 12;
int relay = 3;
int button = 2;
int ledpin = 2;

int readValue = 0;

char ssid[] = "DNK INFRA";
char pass[] = "9818337826";
int status = WL_IDLE_STATUS;

boolean door_status = false;
//WiFiServer server(80);
YalerESP8266WiFiServer server("try.yaler.io", 80, "gsiot-xqdh-nx2j");

void setup() {
  // put your setup code here, to run once:
  //pinMode(pin, OUTPUT);
  //pinMode(relay, OUTPUT);
  //pinMode(button, INPUT);
  pinMode(ledpin, OUTPUT);

  Serial.begin(115200);
  Serial.println("Attempting to connect to WPA network...");
  Serial.print("SSID: ");
  Serial.println(ssid);

  Serial.println(status);
  status = WiFi.begin(ssid, pass);
  Serial.println(status);
  Serial.println(WL_CONNECTED);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Couldn't get a wifi connection");
  }
    
  IPAddress myAddress = WiFi.localIP();
  Serial.println(myAddress);
    
  //attachInterrupt(0, setDoorStatus, RISING);
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  // Match the request
  int val = -1; // We'll use 'val' to keep track of both the
                // request type (read/set) and value if set.
  if (req.indexOf("/led/0") != -1)
    val = 0; // Will write LED low
  else if (req.indexOf("/led/1") != -1)
    val = 1; // Will write LED high
  /*else if (req.indexOf("/read") != -1)
    val = -2; // Will print pin reads*/
  // Otherwise request will be invalid. We'll say as much in HTML

  // Set GPIO5 according to the request
  Serial.println(val);
  if (val >= 0)
    digitalWrite(ledpin, val);

  client.flush();

  // Prepare the response. Start with the common header:
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n<html>\r\n";
  // If we're setting the LED, print out a message saying we did
  if (val >= 0)
  {
    s += "LED is now ";
    s += (val)?"off":"on";
  }
  /*else if (val == -2)
  { // If we're reading pins, print out those values:
    s += "Analog Pin (Temperature C) = ";
    float ADC = analogRead(A0) /1024.0;
    float temperatureC = (ADC - 0.5) * 100 ;
    s += String(temperatureC);
    s += "<br>"; // Go to the next line.
    s += "Digital Pin 12 = ";
    s += String(digitalRead(DIGITAL_PIN));
  }*/
  else
  {
    s += "Invalid Request.<br> Try /led/1, /led/0, or /read.";
  }
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed

  
  /*if (Serial.available()) {
    //readValue = Serial.parseInt();
    readValue = 5;
    //Serial.write(readValue);
  }*/
  
  /*WiFiClient client = server.available();
  
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        //Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connnection: close");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<meta http-equiv=\"refresh\" content=\"5\">");
          
            if (door_status == false){
              client.print("Everything is ok");
              client.print(readValue);
            }
            else {
               client.print("Alert ! The door has been opened");
            }       
          client.println("<br />");
          client.println("</html>");
           break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
      // close the connection:
      client.stop();
      //Serial.println("client disonnected");
  }*/
}

//ISR (Interrupt Service Routine)
/*void setDoorStatus() {
  door_status = true;
}*/

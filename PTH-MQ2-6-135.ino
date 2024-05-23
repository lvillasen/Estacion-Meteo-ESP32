// 6/Jan/24 Adapted from https://randomnerdtutorials.com/esp32-web-server-with-bme280-mini-weather-station/

#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <HTTPClient.h>

#define SEALEVELPRESSURE_HPA (1013.25)
// To connect the MQ sensors use pins 32-36 or 39 corresponding to ADC1 as ADC2 is used for WiFi
// Use voltage divider, see https://www.upesy.com/blogs/tutorials/measure-voltage-on-esp32-with-adc-with-arduino-code#

#define LED 2
#define pin_MQ2 33
#define pin_MQ6 32
#define pin_MQ135 35

#define N_meas 10

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

// Replace with your network credentials
const char* ssid     = "TU SSID";
const char* password = "TU CLAVE";


//Your Domain name with URL path or IP address with path
String serverName = "http://TU-SERVIDOR-WEB:3000/enviar_TPAH";
unsigned long lastTime = 0;
unsigned long timerDelay = 5000; // send new measurements every 5 s


// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

float temp;
float pressure;
float humidity;

int MQ2;
int MQ6;
int MQ135;

float temp_avg;
float pressure_avg;
float humidity_avg;

int MQ2_avg;
int MQ6_avg;
int MQ135_avg;

int num_rep;



void setup() {
  Serial.begin(115200);

  pinMode(LED, OUTPUT);

pinMode(pin_MQ2, INPUT);
pinMode(pin_MQ6, INPUT);
pinMode(pin_MQ135, INPUT);

  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}
void loop(){


  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) { 

  MQ2 = analogRead(pin_MQ2);
  MQ6 = analogRead(pin_MQ6);
  MQ135 = analogRead(pin_MQ135);


    // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the table 
            client.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial;}");
            client.println("table { border-collapse: collapse; width:35%; margin-left:auto; margin-right:auto; }");
            client.println("th { padding: 12px; background-color: #0043af; color: white; }");
            client.println("tr { border: 1px solid #ddd; padding: 12px; }");
            client.println("tr:hover { background-color: #bcbcbc; }");
            client.println("td { border: none; padding: 12px; }");
            client.println(".sensor { color:white; font-weight: bold; background-color: #bcbcbc; padding: 1px; }");
            
            // Web Page Heading
            client.println("</style></head><body><h1>ESP32 with BMP280, MQ2,MQ6 and MQ135</h1>");
            client.println("<table><tr><th>MEASUREMENT</th><th>VALUE</th></tr>");
            client.println("<tr><td>Temp. Celsius</td><td><span class=\"sensor\">");
            client.println(bme.readTemperature());
            client.println(" *C</span></td></tr>");  
            //client.println("<tr><td>Temp. Fahrenheit</td><td><span class=\"sensor\">");
            //client.println(1.8 * bme.readTemperature() + 32);
            //client.println(" *F</span></td></tr>");       
            client.println("<tr><td>Pressure</td><td><span class=\"sensor\">");
            client.println(bme.readPressure() / 100.0F);
            client.println(" hPa</span></td></tr>");
            client.println("<tr><td>Approx. Altitude</td><td><span class=\"sensor\">");
            client.println(bme.readAltitude(SEALEVELPRESSURE_HPA));
            client.println(" m</span></td></tr>"); 
            client.println("<tr><td>Humidity</td><td><span class=\"sensor\">");
            client.println(bme.readHumidity());
            client.println(" %</span></td></tr>"); 
            client.println("<tr><td>MQ2</td><td><span class=\"sensor\">");
            client.println(MQ2);
            client.println(" (0-4095)</span></td></tr>"); 
 
            client.println("<tr><td>MQ6</td><td><span class=\"sensor\">");
            client.println(MQ6);
            client.println(" (0-4095)</span></td></tr>"); 

            client.println("<tr><td>MQ135</td><td><span class=\"sensor\">");
            client.println(MQ135);
            client.println(" (0-4095)</span></td></tr>"); 
 
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }


 // Parte de HTTP

 //Send an HTTP POST request every 5 s
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;


  digitalWrite(LED,HIGH);


  temp_avg = 0.0;
pressure_avg = 0.0;
humidity_avg = 0.0;
MQ2_avg = 0 ;
MQ6_avg = 0 ;
MQ135_avg = 0 ;

  // measure N_meas times and average values
  num_rep = 0;
  while (num_rep < N_meas) {

  temp_avg += bme.readTemperature();
  pressure_avg +=  bme.readPressure()/ 100.0F;
  humidity_avg  += bme.readHumidity();

  MQ2_avg += analogRead(pin_MQ2);
  MQ6_avg += analogRead(pin_MQ6);
  MQ135_avg +=  analogRead(pin_MQ135);
  num_rep++;
  }
  temp_avg /= N_meas;
  pressure_avg /= N_meas;
  humidity_avg /= N_meas;
  MQ2_avg /= N_meas;
  MQ6_avg /= N_meas;
  MQ135_avg /= N_meas;

  digitalWrite(LED,LOW);

  

      String serverPath = serverName + "?valor1="+String(temp_avg)+"&valor2=" + String(pressure_avg) + "&valor3=" + String(humidity_avg) + "&valor4=" + String(MQ2_avg ) + "&valor5=" + String(MQ6_avg ) + "&valor6=" + String(MQ135_avg );
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());
      
      // If you need Node-RED/server authentication, insert user and password below
      //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
  
}

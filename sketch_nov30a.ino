
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <string>
#include <iostream>
#include <dht11.h>

  const char* ssid = "ASUS_X00TD";
  const char* password = "chocolaty";

#define DHTPIN   19 // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11
#define AOUT_PIN 32
DHT dht(DHTPIN, DHTTYPE);
int led = 33;
const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";

typedef String function;
float element;
  
//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
AsyncWebServer server(80);

String readDHTTemperature() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // Read temperature as Celsius (the default)
  float t = random(36.0,37.0);
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //float t = dht.readTemperature(true);
  // Check if any reads failed and exit early (to try again).
  if (isnan(t)) {    
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(t);
    return String(t);
  }
}

String readDHTHumidity() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = random(88.0,89.0);
    if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(h);
    return String(h);
  }
}
String readMoisture() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = 100-(((float)analogRead(AOUT_PIN)/5000)*100);
  if (isnan(h)) {
    Serial.println("Failed to read from Moisture sensor!");
    return "--";
  }
  else {
    Serial.println(h);
    return String(h);
  }
}

#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <link rel="stylesheet" href="style.css">
  <style>
    html {
    font-family: Arial;
    display: inline-block;
    margin: 0px auto;
    text-align: center;
}

.units {
   font-size: 1.2rem;
}

.dht-labels{
     font-size: 1.5rem;
     vertical-align:middle;
     padding-bottom: 15px;
   }

h2 {
    font-size: 3.0rem;
}
p {
    font-size: 3.0rem;
}
body {
    max-width: 600px; 
    margin:0px auto; 
    padding-bottom: 25px;
}
.switch {
    position: relative; 
    display: inline-block; 
    width: 120px; 
    height: 68px
} 
.switch input {
    display: none
}
.slider {
    position: absolute; 
    top: 0; 
    left: 0; 
    right: 0; 
    bottom: 0; 
    background-color: #ccc; 
    border-radius: 6px
}
.slider:before {
    position: absolute; 
    content: ""; 
    height: 52px; 
    width: 52px; 
    left: 8px; 
    bottom: 8px; 
    background-color: #fff; 
    -webkit-transition: .4s; 
    transition: .4s; 
    border-radius: 3px
}
input:checked+.slider {
    background-color: #b30000
}
input:checked+.slider:before {
    -webkit-transform: translateX(52px); 
    -ms-transform: translateX(52px); 
    transform: translateX(52px)
}
  </style>
</head>
<body>
  <h2>ESP32 DHT Server</h2>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="dht-labels">Temperature</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="dht-labels">Humidity</span>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units">&percnt;</sup>
  </p>
    <p>
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="dht-labels">Moisture</span>
    <span id="moisture">%MOISTURE%</span>
    <sup class="units">&percnt;</sup>
  </p>
  %BUTTONPLACEHOLDER%

<script>
  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("temperature").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/temperature", true);
    xhttp.send();
  }, 10000 ) ;
  
  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("humidity").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/humidity", true);
    xhttp.send();
  }, 10000 ) ;

  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("moisture").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/moisture", true);
    xhttp.send();
  }, 10000 ) ;
  
  function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/update?output="+element.id+"&state=1", true); }
  else { xhr.open("GET", "/update?output="+element.id+"&state=0", true); }
  xhr.send();
}
</script>
</body>
</html>
)rawliteral";


//long duration;
//float distanceCm;
//float distanceInch;
// Replaces placeholder with DHT values

String outputState(int output){
  if(digitalRead(output)){
    return "checked";
  }
  else {
    return "";
  }
}

String processor(const String& var){
  //Serial.println(var);
    if(var == "BUTTONPLACEHOLDER"){
    String buttons = "";
    buttons += "<h4>Output - GPIO 33</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"33\" " + outputState(33) + "><span class=\"slider\"></span></label>";
    return buttons;
  }
  if(var == "TEMPERATURE"){
    return readDHTTemperature();
  }
  else if(var == "HUMIDITY"){
    return readDHTHumidity();
  }
  else if (var == "MOISTURE")
  {
    return readMoisture() ;
  }
  return String();
}



void setup() 
{
  Serial.begin(115200); // Starts the serial communication

  pinMode(33, OUTPUT);
  digitalWrite(33, LOW);
  
  dht.begin();
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTTemperature().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTHumidity().c_str());
  });
  server.on("/moisture", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readMoisture().c_str());
  });

  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
  String inputMessage1;
  String inputMessage2;
    // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
  if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      digitalWrite(inputMessage1.toInt(), inputMessage2.toInt());
   }
   else {
      inputMessage1 = "No message sent";
      inputMessage2 = "No message sent";
    }

    Serial.print("GPIO: ");
    Serial.print(inputMessage1);
    Serial.print(" - Set to: ");
    Serial.println(inputMessage2);
    request->send(200, "text/plain", "OK");
  });

  // Start server
  server.begin();
}

void loop(){
  String mois = readMoisture();
  String hum = readDHTHumidity();
  String temp = readDHTTemperature();

  Serial.println(WiFi.localIP());
  
  Serial.print("moisture Value: ");
  Serial.println(mois); 

  Serial.print("Humidity Value: ");
  Serial.println(hum);

  Serial.print("Temperature Value: ");
      Serial.println(temp);
  delay(1000);

  bool isToggleOn = digitalRead(led) == LOW;

  if (mois.toFloat()<= 30 && isToggleOn) {
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(2, LOW);
  }
  
}
 
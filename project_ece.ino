
// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
// Replace with your network credentials
const char* ssid = "Tanmay Khabia";
const char* password = "slayertk1";
int actuatorpin [3] = {5 , 14} ;
// 5 for lights , 14 for exhaust
String pinmode[3] = {"light" , "exhaust"};
int wd  = 0; 
int ldr = 34, moist = 33 ; 
#define DHTPIN 27     // Digital pin connected to the DHT sensor

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11

DHT dht(DHTPIN, DHTTYPE);

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
String readLight () {
 
    int lit = analogRead(ldr);

    String s = "" ; 
    
    if(lit > 1200) 
      s += "well lit";
    else 
      s += "poorly lit";
    return s ; 
  }
String readMoist () {
    
    int wat = analogRead(moist) ; 
    
//    Serial.println (wat); 
    String s = "" ; 
    if(wat < 2200) 
      s += "watered";
    else 
      s += "dry";
 
    return s; 
  }
  
String readDHTTemperature() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //float t = dht.readTemperature(true);
  // Check if any reads failed and exit early (to try again).
  if (isnan(t)) {
//    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
//    Serial.println(t);
    return String(t);
  }
}

String readDHTHumidity() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  if (isnan(h)) {
//    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
//    Serial.println(h);
    if(h > 80.0)
      digitalWrite(14, HIGH);
   
    return String(h);
  }
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 34px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 68px}
    input:checked+.slider {background-color: #2196F3}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  </style>
</head>
<body>
  <h2>Smart green house solution </h2>
  <div>
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
    <sup class="units">percent</sup>
  </p>
  <p>
    <i class="fas fa-sun" style="color:orange;"></i>
    <span class="dht-labels">Light</span>
    <span id="Light">%LDR%</span>
    
  </p>
  <p>
    <i class="fas fa-spa" style="color:#059e8a;"></i> 
    <span class="dht-labels">soil condition : </span>
    <span id="moisture">%MST%</span>
    
  </p>
  </div>
  <div>
  %BUTTONPLACEHOLDER%
  </div>

<script>
function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/update?state=1&act=" + element.id , true); }
  else { xhr.open("GET", "/update?state=0&act=" + element.id, true); }
  xhr.send();
}
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 1000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("Light").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/Light", true);
  xhttp.send();
}, 1000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("moisture").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/moisture", true);
  xhttp.send();
}, 1000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 1000 ) ;

</script>
</body>
</html>)rawliteral";
String getStateValue(int swt) {
  if (digitalRead(actuatorpin[swt]))
    return "checked";
  else
    return "";
}
// Replaces placeholder with DHT values

String processor(const String& var) {
//  Serial.println(var);
  if (var == "TEMPERATURE") {
    return readDHTTemperature();
  }
  else if (var == "HUMIDITY") {
    return readDHTHumidity();
  }
  if (var == "BUTTONPLACEHOLDER") {
    String button = "";
    for (int i = 0; i < 2; i++) {
      String StateValue = getStateValue(i);
      button +=  "<h4>" + pinmode[i] + "</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"" + actuatorpin[i] + "\" " + StateValue + "><span class=\"slider\"></span></label>";
    }
    return button ; 
  }
  
  if (var == "LDR"){
      String lit = ""; 
      lit +=  readLight();  
      return lit ; 
    }
  if (var == "MST"){
    String mst  = "" ; 
    mst +=  readMoist() ; 
    return mst ; 
    }
  return String();
}

void setup() {
  // Serial port for debugging purposes

  Serial.begin(115200);
  
  dht.begin();
  for(int i=0; i<3; i++){
    pinMode(actuatorpin[i], OUTPUT);
    digitalWrite(actuatorpin[i], 0);
  }
  pinMode(ldr, INPUT);
  pinMode(moist, INPUT);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    wd ++; 
    if(wd == 3 ) 
      ESP.restart();  
    Serial.println("Connecting to WiFi..");
  }
  
  
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
  
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", readDHTTemperature().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", readDHTHumidity().c_str());
  });
  
  server.on("/Light" , HTTP_GET , [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", readLight().c_str());
  });
  
  server.on("/moisture" , HTTP_GET , [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", readMoist().c_str());
  });
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String inputMessage;

    String inputMessage2;

    // GET input1 value on <ESP_IP>/update?relay=<inputMessage>
    if (request->hasParam("state") & request->hasParam("act")) {
      inputMessage = request->getParam("state")->value();
      inputMessage2 = request->getParam("act")->value();
    }
    //Serial.println (inputMessage.toInt());
//    Serial.println (inputMessage2.toInt());
    
    if(inputMessage.toInt()) {     
      digitalWrite(inputMessage2.toInt(), HIGH);
    }
    else {
      digitalWrite(inputMessage2.toInt(), LOW);
    }

  });

  // Start server
  server.begin();

}
bool flag_t = false , flag_d= false ;

  uint32_t oltime ;
void loop() {
  
  String s =  readMoist() ; 
  if(s.equals("dry condition now")){
      if(flag_t){
        
        if (millis () - oltime  > 5000){
            digitalWrite (4 , HIGH);
            flag_t = false ; 
          } else  if(oltime  > millis ()){
              oltime = millis () ;
            }
        }
      else{
        
        oltime = millis () ; 
        flag_t = true;         
        } 
    }
    else {
        if(flag_d){
        
        if (millis () - oltime  > 1000){
            digitalWrite (4 , LOW);
            flag_d = false ; 
          } else  if(oltime  > millis ()){
              oltime = millis () ;
            }
        }
      else{
        
        oltime = millis () ; 
        flag_d = true;         
        } 
      } 
      
}

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

int baslangic = 1;
char kullanicilar[] = {};
const char KAYIT_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<body>
<meta charset="UTF-8">

<h3> Kullanıcı Kaydı</h3>

<form action="/action_page">
  İsim:<br>
  <input type="text" name="firstname" >
  <br>
  Soyisim:<br>
  <input type="text" name="lastname" >
  <br>
  Kart Id:<br>
  <input type="text" name="kartId" > <span><a href="/idogrenme"><button type="button">Öğrnemek için</button></a></span>
  <br>
  <br>
  <input type="submit" value="Submit">
</form> 

</body>
</html>
)=====";
//-----------------------------------------------
const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>

<head>
    <meta charset="UTF-8">

    <meta http-equiv="refresh" content="1">

</head>

<body>
    <div class="giriscikis">
        <div style="top:5%;
        left:50%;
        width:100px;
        height:10px;
        position: absolute;
        z-index: 2;">
            <a href="/kayit"><button>KAYIT</button></a>
        </div>
        <div id='count'>
            <h3 style="font-size: x-large;"> Giriş </h3>
        </div>
    </div>
</body>

)=====";
String tmpscript ="<script>var search = '%id%'; var replacement = '%bilgiler%';document.body.innerHTML = document.body.innerHTML.split(search).join(replacement)</script>";

const char* ssid = "nodemcu";
const char* password = "burak1234";
StaticJsonBuffer<1000> jsonBuffer;

ESP8266WebServer server(80);  
String tmpString = "";
SoftwareSerial nodemcu(D6, D5);


String s = MAIN_page;
void handleRoot() {

  JsonObject& data = jsonBuffer.parseObject(nodemcu);
  String temp = "<div>  <p>%count% </p> </div>";
  float idNumarasi = data["idNumarasi"];
  if (String(idNumarasi) != "0.00") {
     
      temp.replace("%count%", String(idNumarasi));
      s = s + temp+tmpscript;
      server.send(200, "text/html", s);
      Serial.println("");
    }
  else {
    server.send(200, "text/html", s);
  }
}
void kayit() {

  String k = KAYIT_page;             //Read HTML contents
  server.send(200, "text/html", k);  //Send web page
}


void handleForm() {
  baslangic = baslangic + 1;
  String firstName = server.arg("firstname");
  String lastName = server.arg("lastname");
  String kartId = server.arg("kartId");
  String bilgiler= firstName + lastName;
  String script ="<script>var search = '%id%'; var replacement = '%bilgiler%';document.body.innerHTML = document.body.innerHTML.split(search).join(replacement)</script>";
  script.replace("%id%", String(kartId));
  script.replace("%bilgiler%", String(bilgiler));
  tmpscript=tmpscript+script;
  String s = "<a href='/'> Kayit Basarili Geri Don </a>";
  server.send(200, "text/html", s);  //Send web page
}


void setup(void) {
  Serial.begin(115200);
  nodemcu.begin(115200);

  WiFi.begin(ssid, password);  
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println("WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/", handleRoot);  
  server.on("/kayit", kayit);
  server.on("/action_page", handleForm);  

  server.begin();  
  Serial.println("HTTP server started");
}


void loop(void) {
  server.handleClient();  //Handle client requests
}
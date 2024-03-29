 
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Servo.h>


#define SERVO D6        
#define PWMA  D1       
#define DIRA  D3             
#define PWMB  D2                 
#define DIRB  D4         

const char* ssid = "B1807665";    //Ten wifi
const char* pass = "xeadruino";   //Pass wifi
String b_speed, b_controller;
int Speed;
WiFiClient client;
WiFiServer server(80);
String data = "";
void setup() { 
  pinMode(PWMA, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(DIRB, OUTPUT);
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
  Serial.begin(115200);
  connectWiFi();
  server.begin();
}


void connectWiFi()
{
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
    digitalWrite(2, !digitalRead(2));
  }
  Serial.println("");
  Serial.print(ssid);
  Serial.println(" Connected!");
  IPAddress ip = WiFi.localIP();
  IPAddress gateway = WiFi.gatewayIP();
  Serial.print("Ip client address: ");
  Serial.println(ip);
  Serial.print("IP gateway: ");
  Serial.println(gateway);
  Serial.print("Ready to run !!!");
}
String checkClient (void)
{
  while (!client.available()) delay(1);
  String request = client.readStringUntil('\r');  //Nhan "GET /n HTTP/1.1" tu client
  request.remove(0, 5);                           //Xoa "GET /" ra khoi chuoi request
  request.remove(request.length() - 9, 9);        //Xoa " HTTP/1.1" ra khoi chuoi request
  return request;                                 //Tra ve "n" de di chuyen xe
}

void loop() {
  client = server.available();
  if (!client) return;
  data = checkClient();
  checkController(data);
  data = "";
}
void goT() {
  Serial.println("Tiến");
  analogWrite(PWMA, Speed);
  analogWrite(PWMB, Speed);
  digitalWrite(DIRA, HIGH);
  digitalWrite(DIRB, HIGH);
}
void goTT() {
  Serial.println("Tiến trái");
  analogWrite(PWMA, 100);
  analogWrite(PWMB, Speed);
  digitalWrite(DIRA, HIGH);
  digitalWrite(DIRB, HIGH);
}
void goTP() {
  Serial.println("Tiến phải");
  analogWrite(PWMA, Speed);
  analogWrite(PWMB, 100);
  digitalWrite(DIRA, HIGH);
  digitalWrite(DIRB, HIGH);
}
void goL() {
  Serial.println("Lùi");
  analogWrite(PWMA, Speed);
  analogWrite(PWMB, Speed);
  digitalWrite(DIRA, LOW);
  digitalWrite(DIRB, LOW);
}
void goLP() {
  Serial.println("Lùi phải");
  analogWrite(PWMA, Speed);
  analogWrite(PWMB, 100);
  digitalWrite(DIRA, LOW);
  digitalWrite(DIRB, LOW);
}
void goLT() {
  Serial.println("Lùi trái");
  analogWrite(PWMA, 100);
  analogWrite(PWMB, Speed);
  digitalWrite(DIRA, LOW);
  digitalWrite(DIRB, LOW);
}
void goTr() {
  Serial.println("Trái");
  analogWrite(PWMA, Speed);
  analogWrite(PWMB, Speed);
  digitalWrite(DIRA, LOW);
  digitalWrite(DIRB, HIGH);
}
void goP() {
  Serial.println("Phải");
  analogWrite(PWMA, Speed);
  analogWrite(PWMB, Speed);
  digitalWrite(DIRA, HIGH);
  digitalWrite(DIRB, LOW);
}
void Stop() {
  Serial.println("Stop");
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
}

void checkController(String _controller) {
  /*
     Tiến 1
     Lùi 2
     Phải 3
     Trái 4
     Tiến Trái 5
     Tiến Phải 6
     Lùi Trái 7
     Lùi Phải 8
     Dừng 9
  */
  Speed = 100 * 10.23;
  int controllers = _controller.toInt();
  switch (controllers)
  {
    case 1:
      goT();  // 1 Tiến
      break;
    case 2:
      goL();  // 2 Lùi
      break;
    case 3:
      goP();  // 3 Phải
      break;
    case 4:
      goTr(); // 4 Trái
      break;
    case 5:
      goTT(); // 5 Tiến trái
      break;
    case 6:
      goTP(); // 6 Tiến phải
      break;
    case 7:
      goLT(); // 7 Lùi Trái
      break;
    case 8:
      goLP(); // 8 Lùi Phải
      break;
    case 9:
      Stop(); // 9 Dừng
      break;
  }
}

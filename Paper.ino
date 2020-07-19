//Variables Globales
String fecha;

//Timmer
hw_timer_t * timer = NULL;
bool tmr = false;

//Placa
int led = 2;

//Hora
#include "ESPDateTime.h"

//Wifi
#include <WiFi.h>
#include <HTTPClient.h>
const char* ssid     = "Red_Wifi";
const char* password = "Clave_Wifi";

//MySQL
#include <HTTPClient.h>
const char* serverName1 = "Direccion remota";
const char* serverName2 = "Ip LocalHost"; //checar ip de la lap en ipconfig
String apiKeyValue = "Ejemplo"; //https://randomkeygen.com/
String sensorName = "Ejemplo"; 

//PlanTower PMS7003
#include "PMserial.h"  // Arduino library for PM sensors with serial interface
#define MSG "PMS7003 on HardwareSerial2"
SerialPM pms(PMS7003, Serial2);  // PMSx003, UART

struct Plantower
{ int pm01_sp;  //1.0 um Standar Particle
  int pm25_sp;  //2.5 um Standar Particle
  int pm10_sp;  //10  um Standar Particle
  int pm01_sa;  //1.0 um Atmospherical Enviroment
  int pm25_sa;  //2.5 um Atmospherical Enviroment
  int pm10_sa;   //10  um Atmospherical Enviroment
  int n0p3;
  int n0p5;
  int n1p0;
  int n2p5;
  int n5p0;
  int n10p0;
  bool error;
};
Plantower PM;

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);

  digitalWrite(led, LOW);

  WiFi.begin(ssid, password);
  Serial.println("Conectando...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  digitalWrite(led, HIGH);
  Serial.println("");
  Serial.print("Conectado a red Wifi con la siguiente IP: ");
  Serial.println(WiFi.localIP());

  pms.init();
  Serial.print("\n\n\n");

  //Timmer
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);

  timerAlarmWrite(timer, 1000000, true); // 1 Segundo
  timerAlarmEnable(timer);
  Serial.println("Timmer configrado");

  setupDateTime();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {

    if (tmr) {
      tmr = false;

      PM = PMS_v();
      lect_pms(PM);

      fecha = fech();

      String Cadena = "api_key=" + apiKeyValue
                      + "&table=" + sensorName
                      + "&fecha=" + fecha

                      + "&pm1p0_sp=" + String(PM.pm01_sp)
                      + "&pm2p5_sp=" + String(PM.pm25_sp)
                      + "&pm10p0_sp=" + String(PM.pm10_sp)

                      + "&n0p3=" + String(PM.n0p3)
                      + "&n0p5=" + String(PM.n0p5)
                      + "&n1p0=" + String(PM.n1p0)
                      + "&n2p5=" + String(PM.n2p5)
                      + "&n5p0=" + String(PM.n5p0)
                      + "&n10p0=" + String(PM.n10p0) + "";

      envio_db(Cadena, 0);
      envio_db(Cadena, 1);
    }
  }
  else {
    digitalWrite(led, LOW);
  }
}


void onTimer() {
  tmr = true;
}

Plantower PMS_v() {
  Plantower PM1;
  pms.read();
  if (pms) { // successfull read
    PM1.pm01_sp  = pms.pm01_sp; PM1.pm25_sp  = pms.pm25_sp; PM1.pm10_sp  = pms.pm10_sp;
    PM1.pm01_sa  = pms.pm01_sa; PM1.pm25_sa  = pms.pm25_sa; PM1.pm10_sa  = pms.pm10_sa;
    PM1.n0p3 = pms.n0p3; PM1.n0p5 = pms.n0p5; PM1.n1p0  = pms.n1p0;
    PM1.n2p5 = pms.n2p5; PM1.n5p0 = pms.n5p0; PM1.n10p0 = pms.n10p0;
    PM1.error = 0;
  }
  else
  { PM1.pm01_sp  = NULL; PM1.pm25_sp  = NULL; PM1.pm10_sp  = NULL;
    PM1.pm01_sa  = NULL; PM1.pm25_sa  = NULL; PM1.pm10_sa  = NULL;
    PM1.n0p3 = NULL; PM1.n0p5 = NULL; PM1.n1p0  = NULL;
    PM1.n2p5 = NULL; PM1.n5p0 = NULL; PM1.n10p0 = NULL;
    PM1.error = 1;
  }
  return PM1;
}

void lect_pms(Plantower PMX) {
  //Imprime en el monitor serial los valores del sensor
  Serial.printf("PM1.0 %2d, PM2.5 %2d, PM10 %2d [ug/m3]",
                PMX.pm01_sp, PMX.pm25_sp, PMX.pm10_sp);
  Serial.printf("\tPM1.0 %2d, PM2.5 %2d, PM10 %2d [ug/m3]\n",
                PMX.pm01_sa, PMX.pm25_sa, PMX.pm10_sa);
  Serial.printf("np0.3 %2d, np0.5 %2d, np1.0 %2d [um/Lt]",
                PMX.n0p3, PMX.n0p5, PMX.n1p0);
  Serial.printf("\tnp2.5 %2d, np5.0 %2d, np10 %2d [um/Lt]\n\n",
                PMX.n2p5, PMX.n5p0, PMX.n10p0);
}

void setupDateTime() {
  DateTime.setTimeZone(-5);
  DateTime.begin();
  if (!DateTime.isTimeValid()) {
    Serial.println("Failed to get time from server.");
  }
}

String fech() {
  String date;
  if (!DateTime.isTimeValid()) {
    Serial.println("Failed to get time from server, retry.");
    DateTime.begin();
  }
  else {
    //Serial.println("--------------------");
    date = DateTime.toString();
    //Serial.println(fecha);

  }
  return date;
}

bool envio_db(String httpRequestData, int s) {
  bool dn;
  HTTPClient http;
  if (s == 0) {
    http.begin(serverName1);
  }
  if (s == 1) {
    http.begin(serverName2);
  }
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  // Send HTTP POST request
  int httpResponseCode = http.POST(httpRequestData);

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    dn = 1;
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
    dn = 0;
  }
  http.end();
  return dn;
}

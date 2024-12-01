#include <WiFi.h>

//Credenciales WiFi
const char* ssid = "Nexxt";
const char* password = "ab123456cd";

// Pines

const int motorPin0 = 5;  //
const int motorPin1 = 18;  //
const int lswitch0 = 23;   //
const int lswitch1 = 32;   //

bool limpieza = 0;  //variable para el inicio de ciclo de limpieza
int ciclo = 0;      //variable para marcar el ciclo de limpieza


WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  pinMode(motorPin0, OUTPUT);
  digitalWrite(motorPin0, LOW);
  pinMode(motorPin1, OUTPUT);
  digitalWrite(motorPin1, LOW);

  pinMode(lswitch0, INPUT_PULLUP);
  pinMode(lswitch1, INPUT_PULLUP);

  Serial.println("Conectando al WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando...");
  }
    Serial.println("Conectado al WiFi");
    Serial.println(WiFi.localIP());

    // Iniciar el servidor
    server.begin();

  //   while (digitalRead(lswitch0)) {
  //     digitalWrite(motorPin0, HIGH);
  //     delay(100);
  //   }
  //   digitalWrite(motorPin0, LOW);
  // }
}

void loop() {

Serial.print("Ciclo: ");
Serial.println(ciclo);
Serial.print("LS1: ");
Serial.println(digitalRead(lswitch1));


  WiFiClient client = server.available();

  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();

    if (request.indexOf("/limpiar") != -1) {
      limpieza = 1;
      Serial.println("Motor encendido");
    } else if (request.indexOf("/off") != -1) {
      limpieza = 0;
      digitalWrite(motorPin0, LOW);
      digitalWrite(motorPin1, LOW);
      Serial.println("Motor apagado");
    }

    // Generar la página web con los botones
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    client.println("<!DOCTYPE html>");
    client.println("<html>");
    client.println("<head><title>Control del Motor</title></head>");
    client.println("<body style='text-align: center; font-family: Arial;'>");
    client.println("<h1>Control del Motor</h1>");
    client.println("<button style='font-size: 20px; padding: 10px 20px;' onclick=\"location.href='/limpiar'\">Limpiar</button>");
    client.println("<button style='font-size: 20px; padding: 10px 20px;' onclick=\"location.href='/off'\">Apagar</button>");
    client.println("</body>");
    client.println("</html>");
    client.stop();  // Finalizar conexión
  }

  if (limpieza == 1) {
    switch (ciclo) {
      case 0:
        if (digitalRead(lswitch0) == 0) {
            digitalWrite(motorPin1, LOW);
            delay(2000);
            digitalWrite(motorPin0, HIGH);
            ciclo = 1;
        } else
          digitalWrite(motorPin1, HIGH);  //vuelve a posicion inicial
        break;

      case 1:
        if (digitalRead(lswitch1) == 0) {
          digitalWrite(motorPin0, LOW);
          delay(2000);
          digitalWrite(motorPin1, HIGH);
          ciclo = 2;
        }
        break;

      case 2:
        if (digitalRead(lswitch0) == 0) {
          digitalWrite(motorPin1, LOW);
          ciclo = 0;
          limpieza = 0;
        }
        break;
    }
  }
}

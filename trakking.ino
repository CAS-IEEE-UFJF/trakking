#include <Arduino.h>
#include <ESP8266WiFi.h>

const char* ssid = "Holy";
const char* password = "12345678";

WiFiServer server(80);
String header;

int M1A = 5, M1B = 4;
int M2A = 14, M2B = 12;


#include <HCSR04.h>

UltraSonicDistanceSensor distanceSensor(13, 15);  // Initialize sensor that uses digital pins 13 and 12.

void setup()
{
  pinMode(M1A, OUTPUT);
  pinMode(M1B, OUTPUT);
  pinMode(M2A, OUTPUT);
  pinMode(M2B, OUTPUT);

  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    Serial.print(".");
  }

  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

String all_html ()
{
  String _html = "";
  //_html += "HTTP/1.1 200 OK";
  //_html += "Content-type:text/html";
  //_html += "Connection: close";
  // Display the HTML web page

  _html += "<!DOCTYPE html><html>";
  _html += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  _html += "<link rel=\"icon\" href=\"data:,\">";

  _html += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
  _html += ".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;";
  _html += "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
  _html += ".button2 {background-color: #77878A;}</style></head>";

  // Web Page Heading
  _html += "<body><h1>ESP8266 Web Server</h1>";

  _html += "<p><a href=\"/button1\"><button class=\"button\">CLICK button 1</button></a></p>";
  _html += "<p><a href=\"/button2\"><button class=\"button\">CLICK button 2</button></a></p>";

  _html += "</body></html>";

  return _html;
}

void loop()
{
  WiFiClient client = server.available();

  if (client)
  {
    Serial.println("New Client.");
    String currentLine = "";

    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        Serial.write(c);
        header += c;

        if (c == '\n')
        {
          if (currentLine.length() == 0)
          {
            //client.println(all_html()); //envia o html para o cliente

            break;
          }
          else
          {
            currentLine = "";
          }
        }
        else if (c != '\r')
        {
          currentLine += c;
        }
      }
    }

    if (header.indexOf("GET /F") >= 0)
      frente();
    else if (header.indexOf("GET /T") >= 0)
      tras();
    else if (header.indexOf("GET /D") >= 0)
      direita();
    else if (header.indexOf("GET /E") >= 0)
      esquerda();

    header = "";
    client.stop();
  }

  Serial.println(distanceSensor.measureDistanceCm());
  delay(100);
}

void parar()
{
  digitalWrite(M1A, LOW);
  digitalWrite(M1B, LOW);

  digitalWrite(M2A, LOW);
  digitalWrite(M2B, LOW);
}

void tras()
{
  digitalWrite(M1A, HIGH);
  digitalWrite(M1B, LOW);

  digitalWrite(M2A, HIGH);
  digitalWrite(M2B, LOW);

  delay(100);

  parar();
}

void frente()
{
  digitalWrite(M1A, LOW);
  digitalWrite(M1B, HIGH);

  digitalWrite(M2A, LOW);
  digitalWrite(M2B, HIGH);

  delay(100);

  parar();
}


void direita()
{
  digitalWrite(M1A, LOW);
  digitalWrite(M1B, HIGH);

  digitalWrite(M2A, HIGH);
  digitalWrite(M2B, LOW);

  delay(100);

  parar();
}

void esquerda()
{
  digitalWrite(M1A, HIGH);
  digitalWrite(M1B, LOW);

  digitalWrite(M2A, LOW);
  digitalWrite(M2B, HIGH);

  delay(100);

  parar();
}

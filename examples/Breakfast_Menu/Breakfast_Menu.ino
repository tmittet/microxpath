#include <SPI.h>
#include <Ethernet.h>
#include <MicroXPath.h>

#define ETHERNET_MAC (byte[]) { 0x36, 0xE6, 0xF2, 0x2E, 0x0E, 0xD3 }

EthernetClient client;
MicroXPath xPath = MicroXPath();

void setup()
{
  Serial.begin(9600);
  while (!Serial);

  if (Ethernet.begin(ETHERNET_MAC))
  {
    Serial.println("Connecting...");
    Serial.flush();
    if (client.connect("www.w3schools.com", 80))
    {
      Serial.println("Connected: www.w3schools.com/xml/simple.xml");
      client.println("GET /xml/simple.xml HTTP/1.1");
      client.println("Host: www.w3schools.com");
      client.println("Connection: close");
      client.println();
    }
    else
    {
      Serial.println("Error: Connect");
    }
  }
  else
  {
    Serial.println("Error: DHCP");
  }
}

void loop()
{
  if (client.available())
  {
    char result[100];
    xPath.setPath((const char *[]){ "breakfast_menu", "food", "name" }, 3);
    while (client.available())
    {
      if (xPath.getValue(client.read(), result, sizeof(result)))
      {
        Serial.println();
        Serial.print(result);
        Serial.println(":");
        break;
      }
    }
    xPath.setPath((const char *[]){ "breakfast_menu", "food", "description" }, 3);
    while (client.available())
    {
      if (xPath.getValue(client.read(), result, sizeof(result)))
      {
        Serial.println(result);
        break;
      }
    }
  }
  if (!client.connected())
  {
    client.stop();
    Serial.println();
    if (xPath.errors)
    {
      Serial.println("Error: ");
      Serial.println(xPath.errors, BIN); 
    }
    else
    {
      Serial.println("DONE");
    }
    while (true);
  }
}

/************************************************************************/
/* MicroXPath lib, an XML navigator with a tiny memory footprint, v1.0. */
/*                                                                      */
/* This library is free software: you can redistribute it and/or modify */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or    */
/* (at your option) any later version.                                  */
/*                                                                      */
/* This library is distributed in the hope that it will be useful, but  */
/* WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU     */
/* General Public License for more details.                             */
/*                                                                      */
/* You should have received a copy of the GNU General Public License    */
/* along with this library. If not, see <http://www.gnu.org/licenses/>. */
/*                                                                      */
/* Written by Thomas Mittet (code@lookout.no) December 2014.            */
/************************************************************************/

#include <SPI.h>
#include <Ethernet.h>
#include <MicroXPath.h>

#define ETHERNET_MAC (byte[]) {0x54, 0x48, 0x4F, 0x4D, 0x41, 0x53}
#define ETHERNET_STATIC_IP  (byte[]) {192, 168, 0, 123}
#define ETHERNET_ERROR_DHCP "Error: DHCP"
#define ETHERNET_ERROR_CONNECT "Error: Connect"

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
  }
  else
  {
    Serial.println(ETHERNET_ERROR_DHCP);
    Ethernet.begin(ETHERNET_MAC, ETHERNET_STATIC_IP);
  }
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
    Serial.println(ETHERNET_ERROR_CONNECT);
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
	// Disable XML_PICO_MODE before uncommenting the following:
    //if (xPath.errors)
    //{
    //  Serial.println("Error: ");
    //  Serial.println(xPath.errors, BIN); 
    //}
    Serial.println("DONE");
    while (true);
  }
}

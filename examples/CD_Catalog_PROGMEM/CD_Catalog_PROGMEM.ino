/************************************************************************/
/* MicroXPath lib, an XML navigator with a tiny memory footprint, v1.2. */
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
/* Written by Thomas Mittet (code@lookout.no) January 2014.             */
/************************************************************************/

#include <SPI.h>
#include <Ethernet.h>
#include <MicroXPath_P.h>
#include <avr/pgmspace.h>

#define ETHERNET_ERROR_DHCP "E: DHCP"
#define ETHERNET_ERROR_CONNECT "E: Connect"

const char g_Catalog[] PROGMEM = "CATALOG";
const char g_Cd[] PROGMEM = "CD";
const char g_Title[] PROGMEM = "TITLE";
const char g_Artist[] PROGMEM = "ARTIST";

EthernetClient client;
MicroXPath_P xPath = MicroXPath_P();

byte g_mac[] = {0x54, 0x48, 0x4F, 0x4D, 0x41, 0x53};
IPAddress g_ethernetStaticIP(192, 168, 0, 123);

void setup()
{
  Serial.begin(9600);
  while (!Serial);

  if (Ethernet.begin(g_mac))
  {
    Serial.println("Connecting...");
    Serial.flush();
  }
  else
  {
    Serial.println(ETHERNET_ERROR_DHCP);
    Ethernet.begin(g_mac, g_ethernetStaticIP);
  }
  if (client.connect("www.xmlfiles.com", 80))
  {
    Serial.println("Connected: www.xmlfiles.com/examples/cd_catalog.xml");
    client.println("GET /examples/cd_catalog.xml HTTP/1.1");
    client.println("Host: www.xmlfiles.com");
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
    const char *namePath[] = { g_Catalog, g_Cd, g_Title };
    xPath.setPath(namePath, 3);
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
    const char *descriptionPath[] = { g_Catalog, g_Cd, g_Artist };
    xPath.setPath(descriptionPath, 3);
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

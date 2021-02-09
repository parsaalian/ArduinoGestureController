#include <SPI.h>
#include <WiFi.h>

char ssid[] = "yourNetwork";
char pass[] = "secretPassword";

int status = WL_IDLE_STATUS;

WiFiServer server(23);

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
        ;
    }

    if (WiFi.status() == WL_NO_SHIELD)
    {
        Serial.println("WiFi shield not present");

        while (true)
            ;
    }

    while (status != WL_CONNECTED)
    {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid);
        status = WiFi.begin(ssid, pass);
        delay(10000);
    }

    server.begin();
    printWifiStatus();
}

void loop()
{
    WiFiClient client = server.available();

    if (client)
    {
        if (client.available() > 0)
        {
            // read the bytes incoming from the client:
            char thisChar = client.read();
            // echo the bytes back to the client:
            server.write(thisChar);
            // echo the bytes to the server as well:
            Serial.write(thisChar);
        }
    }
}

void printWifiStatus()
{
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}

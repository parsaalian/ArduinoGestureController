#include <Wire.h>
#include "paj7620.h"

#define GES_REACTION_TIME 800
#define GES_QUIT_TIME 1000

#include <SPI.h>
#include <WiFi.h>

char ssid[] = "yourNetwork";
char pass[] = "secretPassword";

int status = WL_IDLE_STATUS;

WiFiServer server(23);

void setup()
{
    uint8_t error = 0;

    Serial.begin(9600);
    while (!Serial)
    {
        ;
    }
    Serial.println("\nPAJ7620U2 TEST DEMO: Recognize 15 gestures.");

    error = paj7620Init(); // initialize Paj7620 registers
    if (error)
    {
        Serial.print("INIT ERROR,CODE:");
        Serial.println(error);
    }
    else
    {
        Serial.println("INIT OK");
    }
    Serial.println("Please input your gestures:");

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
    uint8_t data = 0, data1 = 0, error;

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

    error = paj7620ReadReg(0x43, 1, &data); // Read Bank_0_Reg_0x43/0x44 for gesture result.
    if (!error && client && client.available() > 0)
    {
        switch (data) // When different gestures be detected, the variable 'data' will be set to different values by paj7620ReadReg(0x43, 1, &data).
        {
        case GES_RIGHT_FLAG:
            delay(GES_REACTION_TIME);
            paj7620ReadReg(0x43, 1, &data);
            if (data == GES_LEFT_FLAG)
            {
                Serial.println("Right-Left");
                server.write("Right-Left");
            }
            else if (data == GES_FORWARD_FLAG)
            {
                Serial.println("Forward");
                server.write("Forward");
                delay(GES_QUIT_TIME);
            }
            else if (data == GES_BACKWARD_FLAG)
            {
                Serial.println("Backward");
                server.write("Backward");
                delay(GES_QUIT_TIME);
            }
            else
            {
                Serial.println("Right");
                server.write("Right");
            }
            break;
        case GES_LEFT_FLAG:
            delay(GES_REACTION_TIME);
            paj7620ReadReg(0x43, 1, &data);
            if (data == GES_RIGHT_FLAG)
            {
                Serial.println("Left-Right");
                server.write("Left-Right");
            }
            else if (data == GES_FORWARD_FLAG)
            {
                Serial.println("Forward");
                server.write("Forward");
                delay(GES_QUIT_TIME);
            }
            else if (data == GES_BACKWARD_FLAG)
            {
                Serial.println("Backward");
                server.write("Backward");
                delay(GES_QUIT_TIME);
            }
            else
            {
                Serial.println("Left");
                server.write("Left");
            }
            break;
            break;
        case GES_UP_FLAG:
            delay(GES_REACTION_TIME);
            paj7620ReadReg(0x43, 1, &data);
            if (data == GES_DOWN_FLAG)
            {
                Serial.println("Up-Down");
                server.write("Up-Down");
            }
            else if (data == GES_FORWARD_FLAG)
            {
                Serial.println("Forward");
                server.write("Forward");
                delay(GES_QUIT_TIME);
            }
            else if (data == GES_BACKWARD_FLAG)
            {
                Serial.println("Backward");
                server.write("Backward");
                delay(GES_QUIT_TIME);
            }
            else
            {
                Serial.println("Up");
                server.write("Up");
            }
            break;
        case GES_DOWN_FLAG:
            delay(GES_REACTION_TIME);
            paj7620ReadReg(0x43, 1, &data);
            if (data == GES_UP_FLAG)
            {
                Serial.println("Down-Up");
                server.write("Down-Up");
            }
            else if (data == GES_FORWARD_FLAG)
            {
                Serial.println("Forward");
                server.write("Forward");
                delay(GES_QUIT_TIME);
            }
            else if (data == GES_BACKWARD_FLAG)
            {
                Serial.println("Backward");
                server.write("Backward");
                delay(GES_QUIT_TIME);
            }
            else
            {
                Serial.println("Down");
                server.write("Down");
            }
            break;
        case GES_FORWARD_FLAG:
            delay(GES_REACTION_TIME);
            paj7620ReadReg(0x43, 1, &data);
            if (data == GES_BACKWARD_FLAG)
            {
                Serial.println("Forward-Backward");
                server.write("Forward-Backward");
                delay(GES_QUIT_TIME);
            }
            else
            {
                Serial.println("Forward");
                server.write("Forward");
                delay(GES_QUIT_TIME);
            }
            break;
        case GES_BACKWARD_FLAG:
            delay(GES_REACTION_TIME);
            paj7620ReadReg(0x43, 1, &data);
            if (data == GES_FORWARD_FLAG)
            {
                Serial.println("Backward-Forward");
                server.write("Backward-Forward");
                delay(GES_QUIT_TIME);
            }
            else
            {
                Serial.println("Backward");
                server.write("Backward");
                delay(GES_QUIT_TIME);
            }
            break;
        case GES_CLOCKWISE_FLAG:
            Serial.println("Clockwise");
            server.write("Clockwise");
            break;
        case GES_COUNT_CLOCKWISE_FLAG:
            Serial.println("Anti-Clockwise");
            server.write("Anti-Clockwise");
            break;
        default:
            paj7620ReadReg(0x44, 1, &data1);
            if (data1 == GES_WAVE_FLAG)
            {
                Serial.println("Wave");
                server.write("Wave");
            }
            break;
        }
    }
    delay(100);
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
#include "Particle.h"

/*
 * Project alonetogether2020
 * Description:
 * Author:
 * Date:
 */
#include "MQTT.h"


void handleMessage(const char* event, const char* data);
void setup();
void loop();
void connect();


static const char* topic = "home1/noahsbedroom/device/lightshow";

void callback(char* topic, byte* payload, unsigned int length)
{

}

MQTT client("192.168.2.226", 1883, callback);


int led2 = D7;
int saw_message = 0;

void handleMessage(const char* event, const char* data)
{
    saw_message = 1;
}

void connect()
{
    if(!client.isConnected())
    {
        Particle.publish("MQTT", "Connecting", 60 , PRIVATE);
        client.connect("alonetogether");
        Particle.publish("MQTT", "Connected", 60, PRIVATE);
    }  
}


void setup() {

    pinMode(led2, OUTPUT);

    Particle.connect();

    Time.zone(-4);  // in DST
    Particle.syncTime();

    Particle.subscribe("alonetogether2020", handleMessage, ALL_DEVICES);

    connect();

}

void turn_on_light_show()
{
    int hour = Time.hour(Time.now());

    if((hour > 9) && (hour < 20))
    {
        client.publish(topic, "on");
        delay(2000); // give it time to start
        client.publish(topic, "off"); // only do a single "run"
    }
}

void flash_led()
{
    for(int i = 0; i < 10; i++)
    {
        digitalWrite(led2, HIGH);
        delay(750);
        digitalWrite(led2, LOW);
        delay(750);
    }   
}

void loop() 
{
    if (client.isConnected())
    {
        client.loop();
    }
    else
    {
        connect();
    }
  
    if(saw_message == 1)
    {
        saw_message = 0;
        turn_on_light_show();

        flash_led();
    }
}
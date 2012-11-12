#include <SPI.h> // needed in Arduino 0019 or later
#include <Ethernet.h>
#include <Twitter.h>

// Ethernet Shield Settings
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Your Token to Tweet (get it from http://arduino-tweet.appspot.com/)
Twitter twitter("938006472-wQp8DRWV5bbUfz5yxd5N7gK2vr63UKdbgC8pK7WB");

char msg_on[] = "Someone is working in the space!";
char msg_off[] = "No one is currently in the space.";

// PostMsg takes a message and an update number
// Update number required to avoid Twitter duplicate message error.
void postMsg(char* msg, int num) {
  Serial.println("connecting ...");
  String toPost = String("Update #") + num + " " + String(msg);
  char buf[140];
  toPost.toCharArray(buf,140);
  if (twitter.post(buf)) {
    int status = twitter.wait(&Serial);
    if (status == 200) {
      if (D) Serial.println("OK.");
    } else {
      if (D) Serial.print("failed : code ");
      if (D) Serial.println(status);
    }
  } else {
    if (D) Serial.println("connection failed.");
  }
}

int current_state = 0;
int last_state = 0;
int sw = 7;
int D = 0;

void setup()
{
  delay(1000);
  pinMode(sw, INPUT);
  digitalWrite(sw, HIGH); // Use internal pull-up resistor
  Ethernet.begin(mac);
  if (D) Serial.begin(9600);

}
int update_number = 1;

void loop()
{
  delay(1000);
  current_state = digitalRead(sw);
  if (current_state != last_state)
  {
    if (D) Serial.println("posting new message!");
    if (D) Serial.println(current_state);
    if (current_state == 1)
    {
      postMsg(msg_on, update_number);
    }
    else
    {
      postMsg(msg_off, update_number++);
    }
  }
  last_state = current_state;
}

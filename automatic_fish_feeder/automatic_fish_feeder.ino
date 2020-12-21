#include <Servo.h>               // include servo library

Servo myservo;                   // declare variable for servo

const int SW = 4;                // selector switch is connected to pin 4
const int IR = A2;               // IR sensor is connected to pin A2
const int buzzer = 12;           // buzzer connected to pin 12

int pos = 10;                    // idle position for servo at 10 degree
int detect=0;                    // variable to hold IR value
int i;                           // variable to provide loop
int heartbeat = 0;               // variable to provide delay
int hold = 0;                    // variable to provide delay
int mode;                        // variable to hold selector switch value

void setup() {
  Serial.begin(9600);            // start the serial communication
  myservo.attach(9);             // servo is connected to pin 9
  
  pinMode(buzzer,OUTPUT);        // declare buzzer as output
  pinMode(SW,INPUT);             // declare selector switch as input
  }

void loop() {  

  myservo.write(pos);            // move the servo to its initial position
  
  detect = analogRead(IR);       // IR sensor detect whether fish food is available or not

  Serial.print(detect);
  Serial.print("\t");

  mode = digitalRead(SW);        // selector switch detect the mode chosen by user

  if (detect >= 1005) {          // if IR sensor does not detect any fish food

    int j;
                                 // buzzer will alert the user to refill the fish food
    for(j=0;j<80;j++) { 
      digitalWrite(buzzer,HIGH);      
      delay(1);//wait for 1ms
      digitalWrite(buzzer,LOW);
      delay(1);//wait for 1ms
    }
                                 // servo will not move until the fish food is refilled
    for(j=0;j<100;j++) {
      digitalWrite(buzzer,HIGH);
      delay(2);//wait for 2ms
      digitalWrite(buzzer,LOW);
      delay(2);//wait for 2ms
    }
  }
  
  else if (detect < 1005) {      // IR sensor detect fish food is present

    hold++;                      // increase hold value by 1
    Serial.println(heartbeat);
   
    if (mode == 1) {             // if user choose to feed the fish twice a day
      
      if (hold >= 5268) {        // this will make sure heartbeat value reach 1000
        heartbeat++;             // in 12 hours
        hold = 0;                // reset hold value
      }
    }

    else if (mode == 0) {        // if user choose to feed the fish once a day
      
      if (hold >= 10536) {        // this will make sure heartbeat value reach 1000
        heartbeat++;             // in 24 hours
        hold = 0;                // reset hold value
      }
    }
   
    if (heartbeat == 1000) {             // fish will be fed when heartbeat value reaches 1000
      for (pos = 15; pos <= 40; pos += 1) {
        myservo.write(pos);              // servo will open the cap of the container
        delay(15);                       // wait 15ms for the servo to reach the position
      }

      for (pos = 40; pos >= 15; pos -= 1) {
        myservo.write(pos);              // servo will close the cap of the container
        delay(15);                       // waits 15ms for the servo to reach the position
      }

      heartbeat = 0;                     // reset heartbeat value
    }
  }
}

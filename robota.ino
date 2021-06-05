//including the libraries
#include <SoftwareSerial.h> // TX RX software library for bluetooth
#include <Servo.h>
const int servo1pin=5;
const int servo2pin=4;
Servo mys1;
Servo mys2;


//Initializing pins for bluetooth Module
int bluetoothTx = 2; // bluetooth tx to 2 pin
int bluetoothRx = 3; // bluetooth rx to 3 pinint Motor1_Pin2 = 8;  --

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

//Front Motor Pins  
int Enable1 = 6;
int M1_Pin1 = 7; // M defines motor  
int M1_Pin2=8;   // M defines motor

//Back Motor Pins      
int M2_Pin1 = 9;  // M defines motor
int M2_Pin2 = 10; // M defines motor
int Enable2 = 11; 

//LEDs
int r_pin=13;  // r defines Red
int b_pin=12;  // b defines Blue

//turn speed and forward speed and delay for IR mode
int f_l_speed=100;
int f_r_speed=100;
int tf_speed=130;
int t_speed=60;
int t_delay=10;
int b_l_speed=100;
int b_r_speed=100;
int f_li_speed=90;
int f_ri_speed=60;

//IR sensors
int l_s=A2; //S defines sensor
int r_s=A1; //S defines sensor
int f_s=A3; //S defines sensor
int m_s=A0; //S defines sensor

//IR states
int l_s_state;
int r_s_state;
int f_s_state;
int m_s_state;

char command ; //variable to store the data
int velocity = 0; //Variable to control the speed of motor

void setup() 
{       
  //Set the baud rate of serial communication and bluetooth module at same rate.
  Serial.begin(9600);  
  bluetooth.begin(9600);

  //Setting the L298N, LED and RGB LED pins as output pins.
  pinMode(M1_Pin1, OUTPUT);  
  pinMode(M1_Pin2, OUTPUT);
  pinMode(Enable1, OUTPUT);
  pinMode(M2_Pin1, OUTPUT);  
  pinMode(M2_Pin2, OUTPUT);
  pinMode(Enable2, OUTPUT); 
  pinMode(l_s, INPUT);
  pinMode(r_s, INPUT);
  pinMode(f_s, INPUT);
  pinMode(r_pin, OUTPUT);
  pinMode(b_pin, OUTPUT);
  pinMode(m_s, INPUT);
  mys1.attach(servo1pin,600,2300);
  mys2.attach(servo2pin,600,2300);
  
}

void loop(){
  if(bluetooth.available() > 0){  //Checking if there is some data available or not
    command = bluetooth.read();   //Storing the data in the 'command' variable
    Serial.println(command);     //Printing it on the serial monitor
    digitalWrite(b_pin, LOW);
    digitalWrite(r_pin, HIGH);}
    //Change pin mode only if new command is different from previous.   
    switch(command){
    case 'B':  //Moving the Car Backward
      digitalWrite(M2_Pin2, HIGH);
      digitalWrite(M2_Pin1, LOW);
      digitalWrite(M1_Pin1, HIGH);
      digitalWrite(M1_Pin2, LOW);
      analogWrite(Enable1,b_l_speed);
      analogWrite(Enable2,b_r_speed);
      //delay(100);
      
      break;
    case 'F':  //Moving the Car Forward
      digitalWrite(M2_Pin1, HIGH);
      digitalWrite(M2_Pin2, LOW);
      digitalWrite(M1_Pin1, LOW);
      digitalWrite(M1_Pin2, HIGH);
      analogWrite(Enable1,f_l_speed);
      analogWrite(Enable2,f_r_speed);
      //delay(100);
      break;
    case 'L':  //Moving the Car Left
      digitalWrite(M1_Pin1, LOW);
      digitalWrite(M1_Pin2, HIGH);
      digitalWrite(M2_Pin1, LOW);
      digitalWrite(M2_Pin2, HIGH);
      analogWrite(Enable1,tf_speed);
      analogWrite(Enable2,t_speed);
      
      break;
    case 'R':   //Moving the Car Right
      digitalWrite(M1_Pin2, LOW);
      digitalWrite(M1_Pin1, HIGH);  
      digitalWrite(M2_Pin1, HIGH);
      digitalWrite(M2_Pin2, LOW);
      analogWrite(Enable1,t_speed);
      analogWrite(Enable2,tf_speed);
      break;
    case 'S':   //Stop
      digitalWrite(M2_Pin2, LOW);
      digitalWrite(M2_Pin1, LOW);
      digitalWrite(M1_Pin2, LOW);
      digitalWrite(M1_Pin1, LOW);
      break;
       
    case 'I':
    while(1)
    {
    digitalWrite(r_pin, LOW);
    digitalWrite(b_pin, HIGH);
      l_s_state=analogRead(l_s);
      r_s_state=analogRead(r_s);
      f_s_state=analogRead(f_s);
      m_s_state=analogRead(m_s);
      digitalWrite(M2_Pin1, HIGH);
      digitalWrite(M2_Pin2, LOW);
      digitalWrite(M1_Pin1, LOW);
      digitalWrite(M1_Pin2, HIGH);
      analogWrite(Enable1,f_li_speed);
      analogWrite(Enable2,f_ri_speed);

      if(f_s_state<200)
      {
        Serial.println("stop");
      digitalWrite(M2_Pin2, LOW);
      digitalWrite(M2_Pin1, LOW);
      digitalWrite(M1_Pin2, LOW);
      digitalWrite(M1_Pin1, LOW);
      break;
      }

      else 
      {
    
           if(r_s_state < 500 && l_s_state > 500)
          {
  Serial.println("turning right");
  
      digitalWrite(M1_Pin2, LOW);
      digitalWrite(M1_Pin1, HIGH);  
      digitalWrite(M2_Pin1, HIGH);
      digitalWrite(M2_Pin2, LOW);
      analogWrite(Enable1,t_speed);
      analogWrite(Enable2,tf_speed);
      while(r_s_state < 500 && l_s_state > 500)
      {
      l_s_state=analogRead(l_s);
      r_s_state=analogRead(r_s);
      }
        }
        else if(r_s_state > 500 && l_s_state < 500)
       {
     Serial.println("turning left");
      digitalWrite(M1_Pin1, LOW);
      digitalWrite(M1_Pin2, HIGH);
      digitalWrite(M2_Pin1, LOW);
      digitalWrite(M2_Pin2, HIGH);
      analogWrite(Enable1,tf_speed);
      analogWrite(Enable2,t_speed);
      while(r_s_state > 500 && l_s_state < 500)
      {
      l_s_state=analogRead(l_s);
      r_s_state=analogRead(r_s);
      }

       }
       else if(m_s_state>500&&(l_s_state<500||r_s_state<500))
      {
        digitalWrite(M2_Pin2, HIGH);
        digitalWrite(M2_Pin1, LOW);
        digitalWrite(M1_Pin1, HIGH);
        digitalWrite(M1_Pin2, LOW);
        analogWrite(Enable1,b_l_speed);
        analogWrite(Enable2,b_r_speed);
      while(m_s_state>500&&(l_s_state<500||r_s_state<500))
      {
        m_s_state=analogRead(m_s);
        l_s_state=analogRead(l_s);
        r_s_state=analogRead(r_s);
      }
        
      }
  
 
  }
        }
      break;
    case 'D':
    mys1.write(180);
    mys2.write(180);
    break;

    case 'U':
    mys1.write(50);
    mys2.write(50);
    break;
    

    default:
    digitalWrite(M2_Pin2, LOW);
    digitalWrite(M2_Pin1, LOW);
    digitalWrite(M1_Pin2, LOW);
    digitalWrite(M1_Pin1, LOW);
     
 
      
      }
      delay(50);
      digitalWrite(M2_Pin2, LOW);
      digitalWrite(M2_Pin1, LOW);
      digitalWrite(M1_Pin2, LOW);
      digitalWrite(M1_Pin1, LOW);
      
      
    }

#include "PS2X_lib.h"  //for v1.6
#include <Servo.h>
Servo servoTAYGAP, servoTRAIPHAI, servoLENXUONG;
int pos = 0;

/******************************************************************
 * set pins connected to PS2 controller:
 *   - 1e column: original 
 *   - 2e colmun: Stef?
 * replace pin numbers by the ones you use
 ******************************************************************/
#define PS2_DAT               47  //13    
#define PS2_CMD               8   //11
#define PS2_SEL               9   //10
#define PS2_CLK               51  //12
#define PIN_SERVO_TAYGAP      4
#define PIN_SERVO_LENXUONG    5
#define PIN_SERVO_TRAIPHAI    6

/******************************************************************
 * select modes of PS2 controller:
 *   - pressures = analog reading of push-butttons 
 *   - rumble    = motor rumbling
 * uncomment 1 of the lines for each mode selection
 ******************************************************************/
//#define pressures   true
#define pressures   false
//#define rumble      true
#define rumble      false

PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning 
//you must always either restart your Arduino after you connect the controller, 
//or call config_gamepad(pins) again after connecting the controller.

int error = 0;
byte type = 0;
byte vibrate = 0;

//Motor A
int enA = 2; 
int in1 = 24; 
int in2 = 26;  

//Motor B
int enB = 3; 
int in3 = 28; 
int in4 = 30;  


void ControlServoTAYGAPL2()
{
    pos += 3;
    pos = (pos > 180) ? 180: pos;
    servoTAYGAP.write(pos);
    delay(10);
}

void ControlServoTAYGAPR2()
{
    pos -= 3;
    pos = (pos < 0) ? 0: pos;
    servoTAYGAP.write(pos);
    delay(10);
}

void ControlServoLEN_PADUP()
{
    pos -= 5;
    pos = (pos > 180) ? 180: pos;
    servoLENXUONG.write(pos);
    delay(10);
}

void ControlServoXUONG_PADDOWN()
{
    pos += 5;
    pos = (pos < 0) ? 0: pos;
    servoLENXUONG.write(pos);
    delay(10);
}

void ControlServoPHAI_PADRIGHT()
{
    pos += 3;
    pos = (pos > 180) ? 180: pos;
    servoTRAIPHAI.write(pos);
    delay(10);
}

void ControlServoTRAI_PADLEFT()
{
    pos -= 3;
    pos = (pos < 0) ? 0: pos;
    servoTRAIPHAI.write(pos);
    delay(10);
}

void ControlMotor()
{
    int HeSoRe = 132 - ps2x.Analog(PSS_RX);
    int DongCo = 123 - ps2x.Analog(PSS_LY);
    if (HeSoRe != 0)
    {
      if (HeSoRe > 5) 
      {
          digitalWrite(in1, LOW);
          digitalWrite(in2, HIGH);
          digitalWrite(in3, HIGH);
          digitalWrite(in4, LOW);
          analogWrite(enA, HeSoRe);
          analogWrite(enB, HeSoRe);
      }
      else
        if (HeSoRe < -5)
        {
            HeSoRe = -HeSoRe;
            digitalWrite(in1, HIGH);
            digitalWrite(in2, LOW);
            digitalWrite(in3, LOW);
            digitalWrite(in4, HIGH);
            analogWrite(enA, HeSoRe);
            analogWrite(enB, HeSoRe);
        }
    }
    else
      if (DongCo >= 0)
      {
          digitalWrite(in1, LOW);
          digitalWrite(in2, HIGH);
          digitalWrite(in3, LOW);
          digitalWrite(in4, HIGH);
          DongCo *= 2.07;
          analogWrite(enA, DongCo);
          analogWrite(enB, DongCo);
      }
      else
      {
          digitalWrite(in1, HIGH);
          digitalWrite(in2, LOW);
          digitalWrite(in3, HIGH);
          digitalWrite(in4, LOW);
          DongCo *= -1.92;
          analogWrite(enA, DongCo);
          analogWrite(enB, DongCo);
      }
    
    Serial.println(DongCo);
    Serial.println(HeSoRe);
    /*Serial.print(ps2x.Analog(PSS_LY)); //Left stick, Y axis. Other options: LX, RY, RX  
    Serial.print(",");
    Serial.print(ps2x.Analog(PSS_LX), DEC); 
    Serial.print(",");
    Serial.print(ps2x.Analog(PSS_RY), DEC); 
    Serial.print(",");
    Serial.println(ps2x.Analog(PSS_RX), DEC);*/
}

void PickUp()
{
    if (ps2x.Button(PSB_L2))  { ControlServoTAYGAPL2();  Serial.println("L2 pressed");}
    if (ps2x.Button(PSB_R2))  { ControlServoTAYGAPR2();  Serial.println("R2 pressed");}
}

void setup(){
    Serial.begin(57600);
    servoTAYGAP.attach(PIN_SERVO_TAYGAP);
    servoTRAIPHAI.attach(PIN_SERVO_TRAIPHAI);
    servoLENXUONG.attach(PIN_SERVO_LENXUONG);
    
    pinMode(enB, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    
    delay(500);  //added delay to give wireless ps2 module some time to startup, before configuring it
     
    //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************
    
    //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
    error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
    
    if(error == 0){
      Serial.print("Found Controller, configured successful ");
      Serial.print("pressures = ");
    if (pressures)
      Serial.println("true ");
    else
      Serial.println("false");
    Serial.print("rumble = ");
    if (rumble)
      Serial.println("true)");
    else
      Serial.println("false");
      Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
      Serial.println("holding L1 or R1 will print out the analog stick values.");
      Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
    }  
    else if(error == 1)
      Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
     
    else if(error == 2)
      Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");
  
    else if(error == 3)
      Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
    
    type = ps2x.readType(); 
    switch(type) {
      case 0:
        Serial.print("Unknown Controller type found ");
        break;
      case 1:
        Serial.print("DualShock Controller found ");
        break;
      case 2:
        Serial.print("GuitarHero Controller found ");
        break;
      case 3:
        Serial.print("Wireless Sony DualShock Controller found ");
        break;
     }
}

void loop() {
    /* You must Read Gamepad to get new values and set vibration values
       ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
       if you don't enable the rumble, use ps2x.read_gamepad(); with no values
       You should call this at least once a second
     */  
      if(error == 1) //skip loop if no controller found
         return;
      
      PickUp(); 
      ControlMotor();
      
      ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed
      
      if(ps2x.Button(PSB_START))         //will be TRUE as long as button is pressed
        Serial.println("Start is being held");
        
      if(ps2x.Button(PSB_SELECT))
        Serial.println("Select is being held");      
  
      if(ps2x.Button(PSB_PAD_UP)) 
      {
        ControlServoLEN_PADUP();
        Serial.print("Up held this hard: ");
        Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
      }

      if(ps2x.Button(PSB_PAD_DOWN))
      {
        ControlServoXUONG_PADDOWN();
        Serial.print("DOWN held this hard: ");
        Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
      } 
      
      if(ps2x.Button(PSB_PAD_RIGHT))
      {
        ControlServoPHAI_PADRIGHT();
        Serial.print("Right held this hard: ");
        Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
      }
      if(ps2x.Button(PSB_PAD_LEFT)){
        ControlServoTRAI_PADLEFT();
        Serial.print("LEFT held this hard: ");
        Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
      }
      
        
      vibrate = ps2x.Analog(PSAB_CROSS);  //this will set the large motor vibrate speed based on how hard you press the blue (X) button

      if (ps2x.NewButtonState()) 
      {        //will be TRUE if any button changes state (on to off, or off to on)
        if(ps2x.Button(PSB_L3))
          Serial.println("L3 pressed");
        if(ps2x.Button(PSB_R3))
          Serial.println("R3 pressed");
        if(ps2x.Button(PSB_TRIANGLE))
          Serial.println("Triangle pressed");        
      }
  
      if(ps2x.ButtonPressed(PSB_CIRCLE))               //will be TRUE if button was JUST pressed
        Serial.println("Circle just pressed");
      if(ps2x.NewButtonState(PSB_CROSS))               //will be TRUE if button was JUST pressed OR released
        Serial.println("X just changed");
      if(ps2x.ButtonReleased(PSB_SQUARE))              //will be TRUE if button was JUST released
        Serial.println("Square just released");     
  
      if(ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) { //print stick values if either is TRUE
        Serial.print("Stick Values:");
        Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX  
        Serial.print(",");
        Serial.print(ps2x.Analog(PSS_LX), DEC); 
        Serial.print(",");
        Serial.print(ps2x.Analog(PSS_RY), DEC); 
        Serial.print(",");
        Serial.println(ps2x.Analog(PSS_RX), DEC);     
    }
    delay(50);  
}

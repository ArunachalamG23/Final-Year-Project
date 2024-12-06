/*
 *  seedsowingnpk@gmailos.com
 *  1234567890
 */
 




#define BLYNK_TEMPLATE_ID           "TMPL39bKxH6Zc"
#define BLYNK_TEMPLATE_NAME         "Quickstart Template"
#define BLYNK_AUTH_TOKEN            "FhyLBUAKVvyx_uoyVP3CNW5mU0jBaAur"




#define BLYNK_PRINT Serial       "Abc"
#include <Blynk.h>                "asd"
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

#define RXD2 16 //(RX2)16
#define TXD2 17 //(TX2)17
#define mod Serial2 

#define R1 15
#define R2 2
#define R3 4
#define R4 5

#define R5 21
#define R6 22
#define R7 18
#define R8 19

#define R9  32
#define R10 25
#define R11 26
#define R12 27

#define RE 33
#define DE 13

// Modbus RTU requests for reading NPK values
const byte nitro[] = {0x01,0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
const byte phos[] = {0x01,0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};
const byte pota[] = {0x01,0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};

// A variable used to store NPK values
byte values[11];
char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "admin";             //WiFi SSID
char pass[] = "1234567890";        //WiFi Password


int x1,x2,x3,x4,x5,x6,x7,x8,x9,x10;
byte val1,val2,val3;
int nv,pv,kv;
BLYNK_WRITE(V0) 
{
x1 = param.asInt();
if(x1==1){digitalWrite(R1,HIGH);digitalWrite(R2,LOW);Serial.println("Forward");} 
if(x1==0){digitalWrite(R1,LOW);digitalWrite(R2,LOW);digitalWrite(R3,LOW);digitalWrite(R4,LOW);} // FORWARD
}
BLYNK_WRITE(V1) 
{
x2 = param.asInt();
if(x2==1){digitalWrite(R1,LOW);digitalWrite(R2,HIGH);Serial.println("Reverse");} 
if(x2==0){digitalWrite(R1,LOW);digitalWrite(R2,LOW);digitalWrite(R3,LOW);digitalWrite(R4,LOW);} // REVERSE
}
BLYNK_WRITE(V2) 
{
x3 = param.asInt();
if(x3==1){digitalWrite(R3,LOW);digitalWrite(R4,HIGH);Serial.println("Right");} 
if(x3==0){digitalWrite(R1,LOW);digitalWrite(R2,LOW);digitalWrite(R3,LOW);digitalWrite(R4,LOW);} // RIGHT 
}
BLYNK_WRITE(V3) 
{
x4 = param.asInt();
if(x4==1){digitalWrite(R3,HIGH);digitalWrite(R4,LOW);Serial.println("Left");} 
if(x4==0){digitalWrite(R1,LOW);digitalWrite(R2,LOW);digitalWrite(R3,LOW);digitalWrite(R4,LOW);} // LEFT
}

BLYNK_WRITE(V4) 
{
x5 = param.asInt();
if(x5==1){digitalWrite(R5,HIGH);digitalWrite(R6,LOW);Serial.println("BORE FORWARD");} // BORE FORWARD
if(x5==0){digitalWrite(R5,HIGH);digitalWrite(R6,HIGH);} 
}
BLYNK_WRITE(V5) 
{
x6 = param.asInt();
if(x6==1){digitalWrite(R5,LOW);digitalWrite(R6,HIGH);Serial.println("BORE REVERSE");} // BORE REVERSE
if(x6==0){digitalWrite(R5,HIGH);digitalWrite(R6,HIGH);} 
}
BLYNK_WRITE(V6) 
{
x7 = param.asInt();
if(x7==1){digitalWrite(R7,HIGH);digitalWrite(R8,LOW);Serial.println("SENSOR INSERT forward");} // SENSOR INSERT FORWARD
if(x7==0){digitalWrite(R7,HIGH);digitalWrite(R8,HIGH);} // RIGHT 
}

BLYNK_WRITE(V7) 
{
x8 = param.asInt();
if(x8==1){digitalWrite(R7,LOW);digitalWrite(R8,HIGH);Serial.println("SENSOR INSERT reverse");} // SENSOR INSERT REVERSE
if(x8==0){digitalWrite(R7,HIGH);digitalWrite(R8,HIGH);} // RIGHT 
}

BLYNK_WRITE(V8) 
{
x9 = param.asInt();
if(x9==1){digitalWrite(R9,LOW);Serial.println("CLOSE ON");} // CLOSE ON
if(x9==0){digitalWrite(R9,HIGH);Serial.println("CLOSE OFF");} // CLOSE OFF
}


void setup()
{
    Serial.begin(9600);
mod.begin(4800, SERIAL_8N1, RXD2, TXD2); 
      
pinMode(R1, OUTPUT);
pinMode(R2, OUTPUT);
pinMode(R3, OUTPUT);
pinMode(R4, OUTPUT);
pinMode(R5, OUTPUT);
pinMode(R6, OUTPUT);
pinMode(R7, OUTPUT);
pinMode(R8, OUTPUT);
pinMode(R9, OUTPUT);
pinMode(R10, OUTPUT);
pinMode(R11, OUTPUT);
pinMode(R12, OUTPUT);

digitalWrite(R1,LOW);digitalWrite(R2,LOW);digitalWrite(R3,LOW);digitalWrite(R4,LOW);
digitalWrite(R5,HIGH);digitalWrite(R6,HIGH);digitalWrite(R7,HIGH);digitalWrite(R8,HIGH);
digitalWrite(R9,HIGH);digitalWrite(R10,HIGH);digitalWrite(R11,HIGH);digitalWrite(R12,HIGH);
    
    Blynk.begin(auth, ssid, pass);
 
  // Set the baud rate for the SerialSoftware object
  mod.begin(4800, SERIAL_8N1, RXD2, TXD2); 

  // Define pin modes for RE and DE
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);
 
 
}
 
void loop()
{

    Blynk.run();

Blynk.virtualWrite(V9,  "N:",val1," mg/kg-"  , "   P:",val2," mg/kg-",  "   K:",val3," mg/kg");

   // Read values
  
  val1 = nitrogen();
  delay(250);
  val2 = phosphorous();
  delay(250);
  val3 = potassium();
  delay(250);

  // Print values to the serial monitor
  Serial.print("Nitrogen: ");
  Serial.print(val1);
  Serial.println(" mg/kg");
  Serial.print("Phosphorous: ");
  Serial.print(val2);
  Serial.println(" mg/kg");
  Serial.print("Potassium: ");
  Serial.print(val3);
  Serial.println(" mg/kg");
  
//  delay(2000);

if(val1>120 && val1<150)
{
  digitalWrite(R10,LOW);
  Blynk.virtualWrite(V9,"N value high");delay(500);
}  
if(val1>150){nv=0;digitalWrite(R10,HIGH);}

if(val2>60 && val2<90)
{
  digitalWrite(R11,LOW);pv=1;
  Blynk.virtualWrite(V9,"P value high");delay(500);
}  
if(val2>90){pv=0;digitalWrite(R11,HIGH);}

if(val3>30 && val3<60)
{
  digitalWrite(R12,LOW);kv=1;
  Blynk.virtualWrite(V9,"K value high");delay(500);
}  
if(val3>60){nv=0;digitalWrite(R12,HIGH);}

if(val1<120){digitalWrite(R10,HIGH);nv=0;}
if(val2<60){digitalWrite(R11,HIGH);pv=0;}
if(val3<30){digitalWrite(R12,HIGH);kv=0;}
/*
if(val2>60 && pv==0)
{
  digitalWrite(R11,LOW);pv=1;
  for(int i=0;i<30;i++)
  {
    delay(1000);
    Blynk.virtualWrite(V9,"P value high:",i);
  }
}  
if(val2<60 && pv==1){pv=0;digitalWrite(R11,HIGH);}

if(val3>30 && kv==0)
{
    digitalWrite(R12,LOW);kv=1;
    Blynk.virtualWrite(V9,"K value high:",i);
  }
}  
if(val3<30 && kv==1){kv=0;digitalWrite(R12,HIGH);}
*/
}

byte nitrogen() {
  // clear the receive buffer
 // mod.flushInput();
 
  // switch RS-485 to transmit mode
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(2);
 
  // write out the message
  for (uint8_t i = 0; i < sizeof(nitro); i++ ) mod.write( nitro[i] );
 
  // wait for the transmission to complete
  mod.flush();
  
  // switching RS485 to receive mode
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
 
  // delay to allow response bytes to be received!
  delay(50);
 
  // read in the received bytes
  for (byte i = 0; i < 7; i++) {
    values[i] = mod.read();
  //  Serial.print(values[i], HEX);
  //  Serial.print(' ');
  }
  return values[4];
}
 
byte phosphorous() {
 // mod.flushInput();
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(2);
  for (uint8_t i = 0; i < sizeof(phos); i++ ) mod.write( phos[i] );
  mod.flush();
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
// delay to allow response bytes to be received!
  delay(50);
  for (byte i = 0; i < 7; i++) {
    values[i] = mod.read();
  //  Serial.print(values[i], HEX);
  //  Serial.print(' ');
  }
  return values[4];
}
 
byte potassium() {
//  mod.flushInput();
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(2);
  for (uint8_t i = 0; i < sizeof(pota); i++ ) mod.write( pota[i] );
  mod.flush();
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
// delay to allow response bytes to be received!
  delay(50);
  for (byte i = 0; i < 7; i++) {
    values[i] = mod.read();
  //  Serial.print(values[i], HEX);
  //  Serial.print(' ');
  }
  return values[4];
}

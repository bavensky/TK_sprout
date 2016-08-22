#include <Ultrasonic.h>
#include <Wire.h>
int trigpin = 3;//appoint trigger pin
int echopin = 2;//appoint echo pin
Ultrasonic ultrasonic(trigpin,echopin);
int in1B = 8 ;
int in1A = 7;
int pwm1 = 6;
int en = 9;
int cs = 10;

int LED_out=4;
int pume_out=11;
int temp_s=0;
float kpt = 0, kit = 0, kdt = 0;
float Input, Output;
float errSum, dErr, error, lastErr;
float ref_t=0.0;

float limit(float input, int min_limit, int max_limit)
{
  if (input > max_limit)input = max_limit;
  if (input < min_limit)input = min_limit;
  return input;
}
void myPID()
{
  lastErr = error;
  error = ref_t - temp_s;
  dErr = (error - lastErr) * 2.0f;
  errSum = limit(errSum + (error * 0.02f), 0, 300);

  Input = limit(((kpt*0.02f) * error) + (kit * errSum) + (kdt * dErr),0,100);
 
}
void setup() {
  Serial.begin(115200);
  Wire.begin(127);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  pinMode(in1B, OUTPUT);
  pinMode(in1A, OUTPUT);
  pinMode(en, OUTPUT);
  pinMode(cs, OUTPUT);
  pinMode(pwm1, OUTPUT);
  pinMode(LED_out, OUTPUT);
  pinMode(pume_out, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(en, HIGH); 
  digitalWrite(cs, HIGH); 
  
  analogWrite(pwm1, 150);
}
float cmdistance,indistance;
void loop() {
  long microsec = ultrasonic.timing();
  cmdistance = ultrasonic.CalcDistance(microsec,Ultrasonic::CM);//this result unit is centimeter
  Serial.println(cmdistance);
  //////
  digitalWrite(13, HIGH);   
  digitalWrite(in1B, HIGH); 
  //digitalWrite(LED_out, HIGH);
  digitalWrite(pume_out, HIGH);
  delay(1000);              
  digitalWrite(13, LOW);   
  //digitalWrite(LED_out, LOW);
  digitalWrite(pume_out, LOW);
   digitalWrite(in1A, LOW);  
  delay(1000);    
}
void receiveEvent(int howMany) {


    byte h = Wire.read(); // receive byte as a character
    byte t = Wire.read();
    byte led_l = Wire.read();
    byte pum = Wire.read();
    byte ref_t = Wire.read();
    t =temp_s;
    Serial.print(ref_t);
    Serial.print(" ");
    Serial.print(pum);
    Serial.print(" ");
    Serial.print(led_l);
    Serial.print(" ");
    Serial.print(t);
    Serial.print(" ");
    Serial.println(h);         // print the integer

}
 void requestEvent() {
  byte data=cmdistance;
  Wire.write(data);
}
void cool(byte p){
   digitalWrite(en, HIGH); 
  digitalWrite(cs, HIGH);
   digitalWrite(in1B, HIGH); 
   digitalWrite(in1A, LOW); 
      if(p>150)
      p = 150;
   analogWrite(pwm1, p);
}
void hot(byte p){
   digitalWrite(en, HIGH); 
  digitalWrite(cs, HIGH);
   digitalWrite(in1B, LOW); 
   digitalWrite(in1A, HIGH); 
   if(p>150)
   p = 150;
   analogWrite(pwm1, p);
}

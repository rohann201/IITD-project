#include <Servo.h>
#include <TimeLib.h>
#include <ESP8266WiFi.h>

#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define TIME_REQUEST  7 
Servo servo_test;        //initialize a servo object for the connected servo  
int obstaclePin =D3;  // This is our input pin
int hasObstacle = HIGH;  // HIGH MEANS NO OBSTACLE
int LED = D4; 
int a,b;                
int angle = 0;   
#define TRIGGER D1
#define ECHO D2
//LED
// NodeMCU Pin D1 > TRIGGER | Pin D2 > ECHO
 const char* ssid = "OnePlus 6T";
const char* password = "Gagan1234";
WiFiServer server(80);
void setup()
{Serial.begin (9600);
   delay(10);
 // pinMode(0, OUTPUT);
 // digitalWrite(0, LOW);
   pinMode(LED, OUTPUT);
  pinMode(obstaclePin, INPUT);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  server.begin();
  Serial.println("Server started");
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
  
  
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(BUILTIN_LED, OUTPUT);
  //pinMode(D1,INPUT);
  //pinMode(D2,OUTPUT);
  servo_test.attach(D0);
   setSyncProvider( requestSync);  //set function to call when sync required
  Serial.println("Waiting for sync message");
}
 
void loop() {

   hasObstacle = digitalRead(obstaclePin); //Reads the output of the obstacle sensor from the 7th PIN of the Digital section of the arduino
  if (hasObstacle == HIGH) //LOW means something is ahead, so illuminates the 13th Port connected LED
  {
    Serial.println("Stop something is ahead!!");
    digitalWrite(LED, HIGH);//Illuminates the 13th Port LED
  }
  else
  {
    Serial.println("Path is clear");
    digitalWrite(LED, LOW);
  }
  delay(200);
  
  
  
  WiFiClient client = server.available();
  int var = digitalRead(D3);
  if (var == HIGH)
  {
    digitalWrite(D4,HIGH);
  }
  else
  {
    digitalWrite(D4,LOW);
  }
if (Serial.available()) {
    processSyncMessage();
  }
  if (timeStatus()!= timeNotSet) {
    digitalClockDisplay();  
  }
  
  long duration, distance;
  digitalWrite(TRIGGER, LOW);  
  delayMicroseconds(2); 
  
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10); 
  
  digitalWrite(TRIGGER, LOW);
  duration = pulseIn(ECHO, HIGH);
  distance = (duration/2) / 29.1;
  Serial.print("Centimeter:");
  Serial.println(distance);
  
  if(distance<8)
   {   
  
  for(angle = 180; angle>=1; angle-=5)     // command to move from 180 degrees to 0 degrees 
  {                                
    servo_test.write(angle);              //command to rotate the servo to the specified angle
    delay(5);                       
  } 

    delay(1000);
   {
      for(angle = 0; angle < 180; angle += 5)    // command to move from 0 degrees to 180 degrees 
  {                                  
    servo_test.write(angle);                 //command to rotate the servo to the specified angle
    delay(15);                       
  }  
     }}

     client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html lang=en-IN>");

  client.println("<a><h2>Location : IIT  </h2></a>");

   if(b<30)
  {
  if(a==1 || a==2 || a==3)
    {  client.println("<a><h2> The van reqired for collection is :VAN 1 </h2></a>");
    }
  if(a==4)
    {  client.println("<a><h2> The van reqired for collection is :VAN 2 </h2></a>");  
    }
  if(a==5)
    { client.println("<a><h2> The van reqired for collection is :VAN 3 </h2></a>");
    }
  if(a==6)
    { client.println("<a><h2> The van reqired for collection is :VAN 4 </h2></a>");
    }
  if(a==7)
    { client.println("<a><h2> The van reqired for collection is :VAN 5 </h2></a>");
    }
}
  if(b>30)
    {
      if(a==1|| a==2 || a==3)
    { client.println("<a><h2> The van reqired for collection is :VAN 1 </h2></a>");
    }
    if(a==4)
    { client.println("<a><h2> The van reqired for collection is :VAN 2 </h2></a>");
    }
    if(a==5)
    { client.println("<a><h2> The van reqired for collection is :VAN 4 </h2></a>");
    }
    if(a==6 || a==7)
    { client.println("<a><h2> The van reqired for collection is :VAN 5 </h2></a>");
    }
    }}


void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
  a=hour();
  b=minute();
}
void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

  if(Serial.find(TIME_HEADER)) {
     pctime = Serial.parseInt();
     if( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
       setTime(pctime); // Sync Arduino clock to the time received on the serial port
     }
  }
}

time_t requestSync()
{
  Serial.write(TIME_REQUEST);  
  return 0; // the time will be sent later in response to serial mesg
}

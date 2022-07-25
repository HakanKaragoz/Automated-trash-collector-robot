//pins of the cover and lever system, which are connected from the motor driver to the arduino
const int openFront = 9;
const int closeFront = 8;
const int armUp = 7;
const int armDown = 6;

//pins of the right and left motor, which are connected from the motor driver to the arduino
const int leftForward = 52;
const int rightForward = 50;
const int leftBackward  = 53;
const int rightBackward = 51;


//pins of color sensor 1
int S01 = 26;
int S11 = 27;
int S21 = 25;
int S31 = 24;
int OUT1 = 28;

//pins of color sensor 2
int S02 = 34;
int S12 = 35;
int S22 = 32;
int S32 = 33;
int OUT2 = 36;

//pins of vehicle front distance sensor
const int trig_front = 38;     
const int echo_front = 39;
int time_front;
int distance_front;
int netDistance_front;

//pins of vehicle right distance sensor
const int trig_right = 42;     
const int echo_right = 43;
int time_right;
int distance_right;
int netDestance_right;

//pins of vehicle left distance sensor
const int trig_left = 46;      
const int echo_left = 47;
int time_left;
int distance_left;
int netDistance_left;


//approximate values of colors read by the sensor
int colorPercent_trash[5][3] = {   {45,26,28}, // Yellow 
                            {37,34,26}, //  Blue 
                            {100,100,100}, // White 
                            {53,30,22}, // Red 
                            {38,33,27} }; // Green 


int colorPercent_floor[5][3] = {   {182,390,177}, // Yellow 
                            {52,210,54}, //  Blue 
                            {100,100,100}, // White 
                            {145,310,70}, // Red 
                            {72,261,81} }; // Green 

                            
String colors_trash[5] = {"Yellow", " Blue", "White", "Red", "Green"};
String colors_floor[5] = {"Yellow", " Blue", "White", "Red", "Green"};

int scale_trash = 3;
int scale_floor = 10;
int RedPercent_trash, GreenPercent_trash,  BluePercent_trash;
int RedPercent_floor, GreenPercent_floor,  GreenPercent_floor;

//measuring distance function of the sensor on the cover
void measureDistance_front(){
digitalWrite(trig_front,LOW);
delay(2);
digitalWrite(trig_front,HIGH);
delay(10);
digitalWrite(trig_front,LOW);
time_front=pulseIn(echo_front,HIGH);
distance_front=time_front/58.2;
if (distance_front > 0 )
{
  netDistance_front=distance_front;
}
delay(50);
}


//measuring distance function of the sensor on the right front side
void measureDistance_right(){
digitalWrite(trig_right,LOW);
delay(2);
digitalWrite(trig_right,HIGH);
delay(10);
digitalWrite(trig_right,LOW);
time_right=pulseIn(echo_right,HIGH);
distance_right=time_right/58.2;
if (distance_right >0 )
{
  netDestance_right=distance_right;
}
delay(50);
}


//measuring distance function of the sensor on the left front side
void measureDistance_left(){
digitalWrite(trig_left,LOW);
delay(2);
digitalWrite(trig_left,HIGH);
delay(10);
digitalWrite(trig_left,LOW);
time_left=pulseIn(echo_left,HIGH);
distance_left=time_left/58.2;
if (distance_left >0 )
{
  netDistance_left=distance_left;
}
delay(50);
}


//function of taking trash from the floor
void takeTrash()
{ wait();
 digitalWrite(closeFront,HIGH); 
 digitalWrite(openFront,LOW);
 digitalWrite(armUp,LOW);
 digitalWrite(armDown,LOW);         //object received
 Serial.println("object received");
 delay(5000);
 digitalWrite(armUp,HIGH);
 digitalWrite(armDown,LOW);         //arm up
 Serial.println("arm lifted");
}


//function of dropping trash in the box
void dropTrash()
{
  wait();
  digitalWrite(openFront,HIGH); 
  digitalWrite(closeFront,LOW);     //object dropped
  Serial.println("object dropped");
  delay(1000);
  backward();
  delay(4000);
  wait();
  digitalWrite(armDown,HIGH);
  digitalWrite(armUp,LOW);        //arm lowered
  Serial.println("arm lowered");
  delay(1500);                    //arm lowering time
  digitalWrite(armDown,LOW);
  digitalWrite(armUp,LOW);  
}

//function of stop the vehicle
void wait()
{
  digitalWrite(leftForward,LOW);
  digitalWrite(leftBackward ,LOW);
  digitalWrite(rightForward,LOW);
  digitalWrite(rightBackward,LOW);
}

//function of run forward the vehicle
void forward()
{  
  digitalWrite(leftForward,HIGH);
  digitalWrite(leftBackward ,LOW);
  digitalWrite(rightForward,HIGH);
  digitalWrite(rightBackward,LOW);
}

//function of box search 
void findTrash()  /
{ 
   forward();
   Serial.println(netDistance_front);
   digitalWrite(openFront,HIGH);
   digitalWrite(closeFront,LOW);
   findColor_trash();                               //It constantly checks the colors in front of it.
   measureDistance_front();                         //Measures the distance between walls and vehicle
  
 
  if(netDistance_front <= 25)          //if distance between vehicle and walls are lower than 25 centimeters
  {  
    digitalWrite(leftForward,HIGH);
    digitalWrite(leftBackward ,LOW);
    digitalWrite(rightForward,LOW);
    digitalWrite(rightBackward,HIGH);
    delay(500);
    findTrash();
    }
     else
    {findTrash();  
      }
 }


//functions of color sensor
void TCS3200_turnOn() {
  digitalWrite(S01,HIGH); // output frequency scaling (100%)
  digitalWrite(S11,HIGH);
  delay(5);
}
void TCS3200_turnOf() {
  digitalWrite(S01,LOW); // power off sensor
  digitalWrite(S11,LOW);
}
void Filtersiz() { 
  digitalWrite(S21,HIGH); // select no filter
  digitalWrite(S31,LOW);
  delay(5);
}
void RedFilter() { 
  digitalWrite(S21,LOW); // select red filter
  digitalWrite(S31,LOW);
  delay(5);
}
void GreenFilter() { 
  digitalWrite(S21,HIGH); // select green filter
  digitalWrite(S31,HIGH);
  delay(5);
}
void  BlueFilter() { 
  digitalWrite(S21,LOW); // select blue filter
  digitalWrite(S31,HIGH);
  delay(5);
}



//function of searching box with the same color as the received trash
void findBox()                   
{Serial.println("Searching for box with the same color as the received object");
  forward();
  measureDistance_right();
  measureDistance_left();
  findColor_floor();           //measuring the color of the floor
  
  if(netDestance_right<=15 || netDistance_left<=15)  
  {
    digitalWrite(leftForward,HIGH);
    digitalWrite(leftBackward ,LOW);
    digitalWrite(rightForward,LOW);
    digitalWrite(rightBackward,HIGH);
    delay(500);
    findBox();
  }

  else
  {
    findBox();
  }
}

void backward()
{
digitalWrite(leftForward,LOW);
digitalWrite(leftBackward ,HIGH);
digitalWrite(rightForward,LOW);
digitalWrite(rightBackward,HIGH);
}


//function of searching floor with the same color as the received trash
void findColor_floor()    //  command to find the color of the floor
{
  float whiteFrequency_floor, RedFrequency_floor, GreenFrequency_floor,  BlueFrequency_floor;
  
  digitalWrite(S02,HIGH); // output frequency scaling (100%)
  digitalWrite(S12,HIGH);
  delay(5);
  digitalWrite(S22,HIGH); // select no filter
  digitalWrite(S32,LOW);
  delay(5);
  whiteFrequency_floor = float(pulseIn(OUT2,LOW,40000)); 
  digitalWrite(S22,LOW); // select red filter
  digitalWrite(S32,LOW);
  delay(5);
  RedFrequency_floor = float(pulseIn(OUT2,LOW,40000)); 
  digitalWrite(S22,HIGH); // select green filter
  digitalWrite(S32,HIGH);
  delay(5);
  GreenFrequency_floor = float(pulseIn(OUT2,LOW,40000)); 
  digitalWrite(S22,LOW); // select blue filter
  digitalWrite(S32,HIGH);
  delay(5);
   BlueFrequency_floor = float(pulseIn(OUT2,LOW,40000)); 
  digitalWrite(S02,LOW); // power off sensor
  digitalWrite(S12,LOW);
  RedPercent_floor   = int((whiteFrequency_floor / RedFrequency_floor)   * 100.0);
  GreenPercent_floor = int((whiteFrequency_floor / GreenFrequency_floor) * 100.0);
  GreenPercent_floor = int((whiteFrequency_floor / BlueFrequency_floor)  * 100.0); 

  Serial.println("Color percentages on the floor  :");
    
    Serial.print("Red=");
    Serial.print(RedPercent_floor);
    
    Serial.print("   Blue=");
    Serial.print( BluePercent_floor);
    
    Serial.print("  Green=");
    Serial.println(GreenPercent_floor);
    
    Serial.println();
    Serial.println("color of floor  =");
    
    int beenRead=0;
    for(int color =0; color < 5; color ++){
      if(RedPercent_floor > colorPercent_floor[color][0] - scale_floor && RedPercent_floor < colorPercent_floor[color][0] + scale_floor && 
         BluePercent_floor > colorPercent_floor[color][1] - scale_floor && BluePercent_floor < colorPercent_floor[color][1] + scale_floor && 
         GreenPercent_floor > colorPercent_floor[color][2] - scale_floor && GreenPercent_floor < colorPercent_floor[color][2] + scale_floor )
       {
         Serial.println(colors_floor[color]);
         Serial.println(colors_floor[color]);

         if (colors_trash[color]= colors_floor[color])  //if the floor and trash are the same color
         {
           wait();
           delay(2000);
           measureDistance_right();
           measureDistance_left();           
           
           if (netDestance_right > netDistance_left)
           {
            digitalWrite(leftForward,LOW);
            digitalWrite(leftBackward ,LOW);
            digitalWrite(rightForward,HIGH);
            digitalWrite(rightBackward,LOW);
              if (netDestance_right+5 == netDistance_left || netDestance_right == netDistance_left+5)
              {
                wait();
                measureDistance_right();
                measureDistance_left();
                delay(1000);
                forward();
                if (netDestance_right >= 5 && netDestance_right <= 10)
                { 
                  wait();
                  dropTrash();
                  delay(1000);
                  findTrash();
                }
              }
           }
       
           else if (netDistance_left>netDestance_right)
           {
            digitalWrite(leftForward,HIGH);
            digitalWrite(leftBackward ,LOW);
            digitalWrite(rightForward,LOW);
            digitalWrite(rightBackward,LOW);
              if (netDestance_right+5 == netDistance_left || netDestance_right == netDistance_left+5)
              { 
                Serial.println("konum alındı");
                wait();
                measureDistance_left();
                measureDistance_right();
                delay(1000);
                forward();
                if (netDestance_right >= 5 && netDestance_right <= 10)
                { 
                  wait();
                  dropTrash();
                  delay(1000);
                  findTrash();
           }
         }
       }  
           
           else if (netDestance_right+5 == netDistance_left || netDistance_left+5 == netDestance_right)
           {
            wait();
            delay(1000);
            forward();
            if(netDestance_right >= 5 && netDestance_right <= 10)
           {  
               wait();
               delay(1000);
               dropTrash();
               delay(1000);
               findTrash();
         }
        }
         
         beenRead=1;
         break;
  
    }
    if(beenRead == 0)
      Serial.println("color not detected");
      
    Serial.println();
    Serial.println();
  } 
}
}


void findColor_trash() 
{
  float whiteFrequency_trash, redFrequency_trash, greenFrequency_trash,  blueFrequency_trash;
  TCS3200_turnOn();
  Filtersiz();
  whiteFrequency_trash = float(pulseIn(OUT1,LOW,40000)); 
  RedFilter();
  redFrequency_trash = float(pulseIn(OUT1,LOW,40000)); 
  GreenFilter();
  greenFrequency_trash = float(pulseIn(OUT1,LOW,40000)); 
   BlueFilter();
   blueFrequency_trash = float(pulseIn(OUT1,LOW,40000)); 
  TCS3200_turnOf();
  RedPercent_trash = int((whiteFrequency_trash / redFrequency_trash) * 100.0);
  GreenPercent_trash = int((whiteFrequency_trash / greenFrequency_trash) * 100.0);
   BluePercent_trash = int((whiteFrequency_trash /  blueFrequency_trash) * 100.0); 
  
   Serial.println("Color percentages of the trash :");
    
    Serial.print("Red=");
    Serial.print(RedPercent_trash);
    
    Serial.print("   Blue=");
    Serial.print(BluePercent_trash);
    
    Serial.print("  Green=");
    Serial.println(GreenPercent_trash);
    
    Serial.println();
    Serial.print("Color of thrash   =");
    
    int beenRead=0;
    for(int color =0; color < 5; color ++)      //It is checked that the object is one of the searched colors.
    {
      if(RedPercent_trash > colorPercent_trash[color][0] - scale_trash && RedPercent_trash < colorPercent_trash[color][0] + scale_trash && 
         BluePercent_trash > colorPercent_trash[color][1] - scale_trash &&  BluePercent_trash < colorPercent_trash[color][1] + scale_trash && 
         GreenPercent_trash > colorPercent_trash[color][2] - scale_trash && GreenPercent_trash < colorPercent_trash[color][2] + scale_trash )
         
         {
         
         Serial.println(colors_trash[color]);
         Serial.println(colors_trash[color]); 
          
          if (colors_trash[color]=="Red" || colors_trash[color]=="Yellow" || colors_trash[color]=="Green")
           { Serial.println("trash found");
           wait();
           delay(2000);
           takeTrash();
           delay(3000);
           findBox();//cop arama
           break;
           }
         
        beenRead=1;
         break;
      }      
    }
    if(beenRead == 0)
      {
        Serial.println("color not detected");
        Serial.println();
        }
}


void setup() {
  Serial.begin(9600);
  pinMode (openFront,OUTPUT);
  pinMode (closeFront,OUTPUT);
  pinMode (armUp,OUTPUT);
  pinMode (armDown,OUTPUT);

  pinMode(leftForward,OUTPUT);
  pinMode(rightForward,OUTPUT);
  pinMode(leftBackward ,OUTPUT);
  pinMode(rightBackward,OUTPUT);
  
  pinMode(S01,OUTPUT); 
  pinMode(S11,OUTPUT); 
  pinMode(S21,OUTPUT); 
  pinMode(S31,OUTPUT); 
  pinMode(OUT1,INPUT); 

  pinMode(S02,OUTPUT); 
  pinMode(S12,OUTPUT); 
  pinMode(S22,OUTPUT); 
  pinMode(S32,OUTPUT); 
  pinMode(OUT2,INPUT);

  pinMode(trig_front,OUTPUT);
  pinMode(echo_front,INPUT);

  pinMode(trig_right,OUTPUT);
  pinMode(echo_right,INPUT);
 
  pinMode(trig_left,OUTPUT);
  pinMode(echo_left,INPUT);
  
 
}


void loop() { 
  findTrash();
  delay(400);
}

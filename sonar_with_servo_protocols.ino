#include <Servo.h> 

Servo myServo; 

const int pwpin = 7; 

//We create a structure to return two values from a function

struct rms_mean {

  float mean ;
  float rms ; 
  
  };

typedef struct rms_mean RMS_mean;

//implement a function to measure distance with the sonar
float measure(int pwpin){
  float distance; 
  float pulse;
  float inches;
  float cm ;
  pinMode(pwpin, INPUT);
  pulse = pulseIn(pwpin , HIGH);
  inches = pulse/147.0;
  cm = inches*2.54;

  return cm;

  
}

//function that help us take the mean of several measures and the RMS

RMS_mean measure_mean(int pwpin){
  float medidas=0;
  float squared = 0;
  int count ; 
  for (count = 0; count<=4 ; count++){
    medidas+=measure(pwpin);
    squared+=medidas*medidas;
   }
  RMS_mean estrutura;

  estrutura.mean = medidas/count; 
  estrutura.rms = sqrt(squared/count);

  return estrutura;
  
}

void setup() {
  myServo.attach(9);
  pinMode(pwpin, HIGH);
  Serial.begin(9600);
  while (!Serial) {;}
  delay(200);

}

char line[32];


void loop() {
  //We will need commands from the serial line to activate our protocols. 
  if (Serial.available()){
    Serial.readStringUntil('n').toCharArray(line, 30); 
    //depending on which command we type on the serial line, we have 4 different protocols
    if (strstr(line , "go to") == line){
      //forces the servo to rotate
      
    }
    else if (strstr(line , "scan") == line){
      //servo sweeps while scanning
      for (int pos =0; pos<=60 ; pos++){
        myServo.write(pos);
        delay(100);
        struct rms_mean medidas = measure_mean(pwpin);
        Serial.print(medidas.mean);
        Serial.print(" , ");
        Serial.print(medidas.rms);
        Serial.print(" , ");
        Serial.println(pos);
      }
      for (int pos =60; pos>=0 ; pos--){
        myServo.write(pos);
        delay(100);
        struct rms_mean medidas = measure_mean(pwpin);
        Serial.print(medidas.mean);
        Serial.print(" , ");
        Serial.print(medidas.rms);
        Serial.print(" , ");
        Serial.println(pos);
      } 
    }
    else if (strstr(line , "measure") == line){
      //soanr measures distances without rotating.
      struct rms_mean medidas = measure_mean(pwpin);
      Serial.print(medidas.mean);
      Serial.print(" , ");
      Serial.println(medidas.rms);
      
    }
    else if (strstr(line , "go to and measure") == line){
      //servo rotates to the position typed and then sonar measures.
      myServo.write(10);
      RMS_mean medidas = measure_mean(pwpin);
      Serial.print(medidas.mean);
      Serial.print(" , ");
      Serial.print(medidas.rms);
      Serial.print(" , ");
      Serial.println(10);
    }
    else{
      Serial.println("Unknown command, please type one of the following: ");
      Serial.println("go to #");
      Serial.println("scan");
      Serial.println("measure");
      Serial.println("go to # and measure");
      
    }
  }

}

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
float measure(int pwpin) {
  float distance;
  float pulse;
  float inches;
  float cm ;
  pinMode(pwpin, INPUT);
  pulse = pulseIn(pwpin , HIGH);
  inches = pulse / 147.0;
  cm = inches * 2.54;

  return cm;


}

float sum_array(float a[] , int len , float mean) {
  int i = 0;
  float sum = 0;
  for (i = 0; i <= len; i++) {
    sum += (a[i] - mean) * (a[i] - mean);

  }
  return sum;

}


//function that help us take the mean of several measures and the RMS

RMS_mean measure_mean(int pwpin) {
  float medidas = 0;
  float squared = 0;
  int count = 0;
  float arr[5];
  for (count = 0; count <= 4 ; count++) {
    medidas += measure(pwpin);
    arr[count] = medidas;

  }
  RMS_mean estrutura;

  estrutura.mean = medidas / count;
  estrutura.rms = sqrt(sum_array(arr , 5 , medidas / count) / count);

  return estrutura;

}


void setup() {
  //initialize our setup.
  myServo.attach(9);
  pinMode(pwpin, HIGH);
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  delay(200);

}

//create an array of char to read from the serial line.

char line[26];


void loop() {
  //We will need commands from the serial line to activate our protocols.
  float matrix[5][3];
  if (Serial.available()) {
    Serial.readStringUntil('\n').toCharArray(line, 26);

    //depending on which command we type on the serial line, we have 4 different protocols
    if (strstr(line , "GOTO") == line) {
      uint16_t pos = (int)atof(&line[5]);
      //forces the servo to rotate
      if (pos <= 180) {
        myServo.write(pos);
        Serial.println(pos);
      }
      else {
        Serial.println("Servo can only rotate 180 degrees ");
      }

    }
    else if (strstr(line , "SCAN?") == line) {
      int servo_pos = myServo.read();
      if (servo_pos<=90){
        int  count = 0;
        //servo sweeps while scanning, returns a matrix so that it is compatible with epics control system
         for (int pos = 0; pos <= 60 ; pos+=12) {
          myServo.write(pos);
          delay(100);
          struct rms_mean medidas = measure_mean(pwpin);
          matrix[count][0] = medidas.mean;
          matrix[count][1] = medidas.rms;
          matrix[count][2] = pos;
          count++;
        }
      }
      else{
        int count = 0;
        for (int pos = 60; pos >= 0 ; pos-= 12) {
          myServo.write(pos);
          delay(100);
          struct rms_mean medidas = measure_mean(pwpin);
          matrix[count][0] = medidas.mean;
          matrix[count][1] = medidas.rms;
          matrix[count][2] = pos;
          count;
        }
      }
      
    }
    else if (strstr(line , "WF DIS?") == line){
        for ( int i = 0 ; i<=5 ; i++){
          if (i<5){
            Serial.print(matrix[0][i]);
            Serial.print(",");
          }
        }
        Serial.println(matrix[0][5]);
      }
    else if (strstr(line , "WF RMS?") == line){
        for ( int i = 0 ; i<=5 ; i++){
          if (i<5){
            Serial.print(matrix[1][i]);
            Serial.print(",");
          }
        }
        Serial.println(matrix[1][5]);
      }
     else if (strstr(line , "WF POS?") == line){
        for ( int i = 0 ; i<=5 ; i++){
          if (i<5){
            Serial.print(matrix[2][i]);
            Serial.print(",");
          }
        }
       Serial.println(matrix[2][5]);
      }
    else if (strstr(line , "MEASURE?") == line) {
      //soanr measures distances without rotating.
      int can = 0;
      struct rms_mean medidas = measure_mean(pwpin);
      delay(100);
      Serial.print(medidas.mean);
      Serial.print(" , ");
      Serial.print(medidas.rms);
      Serial.print(" , ");
      Serial.println(can);

    }


    else {
      Serial.println("Unknown command, please type one of the following: ");
      Serial.println("go to #");
      Serial.println("scan");
      Serial.println("measure");
      Serial.println("go to # and measure");

    }
  }

}

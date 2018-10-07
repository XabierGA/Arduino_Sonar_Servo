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

//Function to convert the number written to the Serial line to int. Since we already restricted the commands given, we can use the position of the number in the string.

int parsear(char* input){
  char* auxi = NULL; 
  auxi = input + 6 ;
  int posi = 0;
  int reto = 0;
  while (*auxi >= 48 && *auxi <=57){
    if ((*auxi < 48 || *auxi >57 ) ){
      Serial.println(*auxi);
      Serial.println("Unknown command");
      return -1;
    }
    else {
      if (posi){
        reto*=10;
        reto+= *auxi -48;
        
      }
      else{
        reto = *auxi - 48; 
        posi++;
      }
    }
    auxi++; 
  }
  return reto;
}

//this function will be used to compare commands extracting the non-numeric values.

int comparar(char* str) {
  int ret = 0 ;
  char* aux = NULL;
  int len = (int)strlen(str);
  aux = (char*)malloc(len*sizeof(char));
  int count = 0;
  int i = 0;
  while (*(str+i) != '\0') {
    if (len<10){
      if ((*(str+i)>=48 && *(str+i) <=57) || ((*(str+i+1)>=48 && *(str+i+1) <=57) && *(str+i) == ' ')){
        i++;
      }
      else {
        *(aux + count) = *(str+i);
        i++;
        count++;
      }
    }
    else{
      if ((*(str+i)>=48 && *(str+i) <=57) || ((*(str+i-1)>=48 && *(str+i-1) <=57) && *(str + i) == ' ')){
        i++;
      }
      else{
        *(aux + count) = *(str+i);
        i++;
        count++;
       
      }
       
    }
    
  }
  
  *(aux + count ) = '\0';
  
  String car = String(aux);
  String opcion1 = "go to";
  String opcion2 = "go to and measure";
  if (car==opcion1){
    ret = 1;
  }
  else if (car ==opcion2){
    ret = 2;
  }
  else {
    Serial.println("Error");
  }
  
  return ret;
}


void setup() {
  //initialize our setup.
  myServo.attach(9);
  pinMode(pwpin, HIGH);
  Serial.begin(9600);
  while (!Serial) {;}
  delay(200);

}

//create an array of char to read from the serial line.

char line[26];


void loop() {
  //We will need commands from the serial line to activate our protocols. 
  if (Serial.available()){
    Serial.readStringUntil('\n').toCharArray(line, 26); 

    //depending on which command we type on the serial line, we have 4 different protocols
    if (comparar(line) == 1){
      //forces the servo to rotate
      int pos = parsear(line);
      if (pos<= 180){
        myServo.write(pos);
        Serial.println(pos);
      }
      else{
        Serial.println("Servo can only rotate 180 degrees ");
      }
    
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
    else if (comparar(line)==2){
      //servo rotates to the position typed and then sonar measures.
      int pos = parsear(line);
      if (pos<=180){
        myServo.write(pos);
        RMS_mean medidas = measure_mean(pwpin);
        Serial.print(medidas.mean);
        Serial.print(" , ");
        Serial.print(medidas.rms);
        Serial.print(" , ");
        Serial.println(pos);
      }
      else {
        Serial.println("Servo can only move 180 degrees");
      }
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

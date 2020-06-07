#include <DHT.h> // sensor de DHT - sensor de temperatura
#include <DHT_U.h>

int sensorDHT = 2; // pin en el que se conecta el sensor 
int LED1 = 7;
int LED2 = 1;
int LED3 = 0;
int tiempoE=5;          // Tiempo de espera  (Velocidad del Motor)
int movimiento;         // giro, si es + gira a la izq, si es negativo gira hacia la der
int motor = 0;   // Valor en que se encuentra el motor

// motor step 
int IN1STEP = 8;
int IN2STEP = 9;
int IN3STEP = 10;
int IN4STEP = 11;

// motor DC
int IN1 = 4;      
int IN2 = 3;      
int ENA = 5;  

int vel;    // velocidad del motor DC  0 - 235
int x = 0; // iteraciones para el riego del jardin. 


DHT dht (sensorDHT, DHT11); // inicializa el objeto

void setup() {
  
  Serial.begin(9600);
  dht.begin(); 

pinMode(IN1STEP, OUTPUT);
pinMode(IN2STEP,OUTPUT);
pinMode(IN3STEP, OUTPUT);
pinMode(IN4STEP, OUTPUT);

  pinMode(IN1, OUTPUT);   
  pinMode(IN2, OUTPUT);  
  pinMode(ENA, OUTPUT);   
  
}

void loop() {
  
int luz = analogRead(A0); // lee la lectura del sensor de luz
int lecturaSensor = analogRead(A1); // lee la lectura del sensor de humedad 
int temperatura = dht.readTemperature(); // lee la lectura del sensor de temperatura 

//TEMPERATURA 

Serial.print("Temperatura: ");
Serial.println(temperatura);

if ( temperatura >=26){ // corre el techo 

Serial.println("Demasiado sol");

 movimiento = -90; // lado contrario de las agujas del reloj
 delay(tiempoE);
 movimiento = (movimiento * 1.4222222222); // Ajuste de 512 vueltas a los 360 grados
 while (movimiento<motor){   // Giro hacia la derecha - demasiado sol
        paso_der();
        motor = motor -1;
   }
 apagado();   
 
} 
else if (temperatura < 26) { // quita el techo 
  
Serial.println("Clima Fresco");

movimiento = 90; // en el sentido de las agujas del reloj
delay(tiempoE);
movimiento = (movimiento * 1.4222222222); // Ajuste de 512 vueltas a los 360 grados

while (movimiento>motor){   // Giro hacia la izquierda - clima fresco
       paso_izq();
       motor = motor + 1;
 }
  
apagado();   

}

//LUZ

if ( luz < 150 ) { // investigar parametros de la luminosidad

Serial.println("Es de noche");

digitalWrite(LED1, HIGH); // enciende el led
digitalWrite(LED2, HIGH);
digitalWrite(LED3, HIGH);
delay (1000); // valor en milisegundos = 1 segundo 
 
} else {

 Serial.println("Es de dia");
 digitalWrite (LED1, LOW);
 digitalWrite (LED2, LOW);
 digitalWrite (LED3, LOW);

}




// HUMEDAD 
Serial.print("La humedad es ");
Serial.println(lecturaSensor);

if ( lecturaSensor >=1000){
  Serial.println(" el sensor esta desconectado o fuera del suelo");
}
else if ( lecturaSensor < 1000 && lecturaSensor >= 600 ){
Serial.println(" suelo seco");

while ( x <= 2 ){
  
   for (vel = 0; vel < 150; vel++){  //investigar motor DC
    
  analogWrite(ENA, vel);        // pin PWM -  ENA
  digitalWrite(IN1, LOW);       // IN1 en 0
  digitalWrite(IN2, HIGH);        // IN2 en 1
  delay(50);            // demora de 50 milisegundos para aumentar la velocidad
  }
  x = x +1 ;
}
 

  digitalWrite(ENA, LOW); // ENA en 0 para deshabilitar motor


} else if ( lecturaSensor < 600 && lecturaSensor >= 370 ){
 Serial.println("suelo humedo");
}
else if ( lecturaSensor < 370){
  Serial.println("sensor esta en agua");
}

delay(2000);
}

void paso_der(){         // Pasos a la derecha - demasiado sol - pone el techo 
 digitalWrite(IN4STEP, LOW); 
 digitalWrite(IN3STEP, LOW);  
 digitalWrite(IN2STEP, HIGH);  
 digitalWrite(IN1STEP, HIGH);  
   delay(tiempoE); 
 digitalWrite(IN4STEP, LOW); 
 digitalWrite(IN3STEP, HIGH);  
 digitalWrite(IN2STEP, HIGH);  
 digitalWrite(IN1STEP, LOW);  
   delay(tiempoE); 
 digitalWrite(IN4STEP, HIGH); 
 digitalWrite(IN3STEP, HIGH);  
 digitalWrite(IN2STEP, LOW);  
 digitalWrite(IN1STEP, LOW);  
  delay(tiempoE); 
 digitalWrite(IN4STEP, HIGH); 
 digitalWrite(IN3STEP, LOW);  
 digitalWrite(IN2STEP, LOW);  
 digitalWrite(IN1STEP, HIGH);  
  delay(tiempoE);  
}

void paso_izq() {        // Pasos a la izquierda - clima fresco - quita el techo
 digitalWrite(IN4STEP, HIGH); 
 digitalWrite(IN3STEP, HIGH);  
 digitalWrite(IN2STEP, LOW);  
 digitalWrite(IN1STEP, LOW);  
  delay(tiempoE); 
 digitalWrite(IN4STEP, LOW); 
 digitalWrite(IN3STEP, HIGH);  
 digitalWrite(IN2STEP, HIGH);  
 digitalWrite(IN1STEP, LOW);  
  delay(tiempoE); 
 digitalWrite(IN4STEP, LOW); 
 digitalWrite(IN3STEP, LOW);  
 digitalWrite(IN2STEP, HIGH);  
 digitalWrite(IN1STEP, HIGH);  
  delay(tiempoE); 
 digitalWrite(IN4STEP, HIGH); 
 digitalWrite(IN3STEP, LOW);  
 digitalWrite(IN2STEP, LOW);  
 digitalWrite(IN1STEP, HIGH);  
  delay(tiempoE); 
}
        
void apagado() {         // Apagado del Motor
 digitalWrite(IN4STEP, LOW); 
 digitalWrite(IN3STEP, LOW);  
 digitalWrite(IN2STEP, LOW);  
 digitalWrite(IN1STEP, LOW);  
 }

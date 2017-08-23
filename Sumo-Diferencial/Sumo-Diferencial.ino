/*
||
|| @sumo_dif.ino
|| @version 1.6
|| @author Club de Robotica del CECYT 9
|| @contact leonardon473@gmail.com
||
|| @description
|| | Basic code to robot sumo
|| #
||
|| @license
|| | This code is under MIT licence, you can find the complete file here: www.mylicencefile.com
|| | Copyright (c) 2017 Club de Robotica del CECYT 9.
|| #
||
*/

//Sensores Sharp
int SensorDer = A0;   int SensorIzq = A1;     //Sensor Sharp Delantero Derecha(0) e Izquierda (1)
int dis = 50;
int dif = 30;
//Motores
int mDerA = 10;       int mIzqA = 5;         //Motor Derecho Adelante y Izquierdo Adelante  Salida con PWM
int mDerB = 9;       int mIzqB = 6;        //Motor Derecho Adenlate(Back) e Izquierdo Adelante (Back) Salida digital

//Velocidades
int velVE = 64;

int velmin = 128;           //Velocidad mínima del motor 50%   128
int vel = 191;//Velocidad del motor al 75%       191


int vel90 = 230;            //velocidad del motor 90%          23
int velmax = 255;           //Velocidad máxima del motor 100%  255

// Lecturas y conversiones
float lecD, lecI;   //Lecturas de los sensores Sharp 0,1,2,3
int cmD, cmI;         //Lecturas a cm de los sensores Sharp 0,1,2,3
int cmDT = 0, cmIT = 0;
int boton = 13;
int n = 8;
// banderas
byte estado = 0; // 0 buscando    1 vi algo a la izquierda    2 vi algo a la derecha    3 los dos ven algo
byte estadoA = 0;


// Modo
int modo = A0;
byte MOD;

//     Funcion principal

void setup() {
  Serial.begin(115200);
  //pinMode(13, INPUT);
  //Pines de salida
  pinMode(mDerA, OUTPUT);
  pinMode(mDerB, OUTPUT);
  pinMode(mIzqA, OUTPUT);
  pinMode(mIzqB, OUTPUT);
  pinMode(modo, INPUT);
  //delay (4500);   // Tiempo de seguridad
  der();
}

void loop() {
  if (0) {
    sensores();
    if ((cmD < dis) && (cmI < dis) && (cmI - cmD > dif))estado = 1;
    if ((cmD < dis) && (cmI < dis) && (cmI - cmD < -dif))estado = 2;
    if ((cmD < dis) && (cmI < dis) && (cmI - cmD > -dif && cmI - cmD < dif))estadoA = 1;
    else {
      estadoA = 0;
    }

    if (estadoA == 1) {
      atacar();
      Serial.print("atacar   ");
    }
    else {
      switch (estado) {
        case 0:    // your hand is on the sensor
          der();
          Serial.print("buscando ");
          break;
        case 1:    // your hand is close to the sensor
          izq();
          Serial.print("izquierda");
          break;
        case 2:    // your hand is a few inches from the sensor
          der();
          Serial.print("derecha  ");
          break;
      }
    }
    Serial.print("\t");
    Serial.print(estado);
    Serial.print("\t");
    Serial.print(estadoA);
    Serial.print("\t");
    Serial.print(cmD);
    Serial.print("\t");
    Serial.print(cmI);
    Serial.print("\t");
    Serial.println(cmI - cmD);
  }
  else {
    analogWrite (mDerA, 0);
    analogWrite (mIzqA, 0);
    analogWrite (mDerB, 0);
    analogWrite (mIzqB, 0);
    delay(2000);
    Serial.println("der");
    der();
    delay(2000);
    Serial.println("izq");
    izq();
    delay(2000);
    Serial.println("ata");
    atacar();
    delay(2000);
    sensores();
  }
}

void sensores() {

  cmDT = 0;
  cmIT = 0;
  for (int i = 0; i < n; i++) {
    lecD = analogRead (SensorDer);           //Lectura del Sharp 0   = 4800/(sensor-20)
    cmD = pow(3027.4 / lecD, 1.2134);        //Conversion a cm Sharp 0  pow(3027.4/lec0, 1.2134);
    lecI = analogRead (SensorIzq);           //Lectura del Sharp 1
    cmI = pow(3027.4 / lecI, 1.2134);        //Conversion a cm Sharp 1  pow(3027.4/lec0, 1.2134);

    cmDT += cmD;
    cmIT += cmI;
    delay(1);
  }
  cmD = cmDT / n;
  cmI = cmIT / n;
  Serial.print(cmD);
  Serial.print(",");
  Serial.print(cmI);
  Serial.println();
}



void izq() {
  analogWrite (mDerB, velmax);
  analogWrite (mIzqA, velmax);
  analogWrite(mDerA, 0);
  analogWrite(mIzqB, 0);
}

void der() {
  analogWrite (mDerB, 0);
  analogWrite (mIzqA, 0);
  analogWrite (mDerA, velmax);
  analogWrite (mIzqB, velmax);
}

void atras() {
  analogWrite (mDerA, velmax);
  analogWrite (mIzqA, velmax);
  analogWrite (mDerB, 0);
  analogWrite (mIzqB, 0);
}

void atacar() {
  analogWrite (mDerB, velmax);
  analogWrite (mIzqB, velmax);
  analogWrite (mDerA, 0);
  analogWrite (mIzqA, 0);
}
void avanzar() {
  analogWrite (mDerA, velmin);
  analogWrite (mIzqA, velmin);
  analogWrite (mDerB, 0);
  analogWrite (mIzqB, 0);
}

int spe;

void manual() {

  if (Serial.available()) {
    char ser = Serial.read();

    if (spe != velVE) {
      if (spe != velmin) {
        if (spe != vel) {
          if (spe != velmax) {
            spe = velmin;
          }
        }
      }
    }


    switch (ser) {

      case '1':
        spe = velVE;
        break;

      case '2':
        spe = velmin;
        break;

      case '3':
        spe = vel;
        break;

      case '4':
        spe = velmax;
        break;

      case 'a': //Adelante
        analogWrite (mDerA, 0);
        analogWrite (mIzqA, 0);
        analogWrite (mDerB, spe);
        analogWrite (mIzqB, spe);
        break;

      case 'b': //Izquierda
        analogWrite (mDerA, 0);
        analogWrite (mIzqA, spe - 7);
        analogWrite (mDerB, spe);
        analogWrite (mIzqB, 0);
        break;

      case 'c': //Alto
        analogWrite (mDerA, 0);
        analogWrite (mIzqA, 0);
        analogWrite (mDerB, 0);
        analogWrite (mIzqB, 0);
        break;

      case 'd': //Derecha
        analogWrite (mDerA, spe);
        analogWrite (mIzqA, 0);
        analogWrite (mDerB, 0);
        analogWrite (mIzqB, spe);
        break;

      case 'e': //Atrás
        analogWrite (mDerA, spe - 20);
        analogWrite (mIzqA, spe);
        analogWrite (mDerB, 0);
        analogWrite (mIzqB, 0);
        break;

    }
  }

}

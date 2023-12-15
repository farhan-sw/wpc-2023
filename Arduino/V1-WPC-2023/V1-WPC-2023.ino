// Basic demo for accelerometer readings from Adafruit MPU6050

// --------------------- SENSOR MPU 6050 ------
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
//---------------------------------------------

// ---------------------- BUTTON -------------
#define BUTTON_PIN    4
// -------------------------------------------

// --------------- AKTUATOR SERVO -----------
#include <ESP32Servo.h>
#define PIN_SERVO     23
Servo myservo;

// ------------ PARAMETER PIN ---------------
#define pin_merah     5
#define pin_hijau     18
#define pin_biru      19

// ------------------------------------------

// --------- THRESHOLD -----------------
#define THRESHOLD     7.5
// ------------------------------------------


// ---------------- LOGIKA BUKA ------------
bool isHide         = false;
int state           = 0;
int seq             = 0;
int buttonState     = 0;

Adafruit_MPU6050 mpu;



void setup(void) {
  Serial.begin(115200);
  while (!Serial) {
    delay(10); // will pause Zero, Leonardo, etc until serial console opens
  }

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  

  // ----------- SETUP SERVO ------------------
//  // Allow allocation of all timers
//  ESP32PWM::allocateTimer(0);
//  ESP32PWM::allocateTimer(1);
//  ESP32PWM::allocateTimer(2);
//  ESP32PWM::allocateTimer(3);
//  myservo.setPeriodHertz(50);
  myservo.attach(PIN_SERVO);
  // ------------------------------------------


  // ---------------- SETUP BUTTON -----------
  pinMode(BUTTON_PIN, INPUT);
  // -----------------------------------------


  // ----------- SETUP RGB -------------------
  pinMode(pin_merah, OUTPUT);
  pinMode(pin_hijau, OUTPUT);
  pinMode(pin_biru, OUTPUT);
  // -----------------------------------------
  
  Serial.println("");
  delay(100);
}

void loop() {

  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  Serial.print("AccelX:");
  Serial.print(a.acceleration.x);
  Serial.print(",");
  Serial.print("AccelY:");
  Serial.print(a.acceleration.y);
  Serial.print(",");
  Serial.print("AccelZ:");
  Serial.print(a.acceleration.z);
  Serial.print(", ");
  Serial.print("State:");
  Serial.print(state);
  Serial.print(", ");
  Serial.print("Seq:");
  Serial.print(seq);
  Serial.print(", ");
  Serial.print("isHide:");
  Serial.print(isHide);
  Serial.println("");

  // --------------- PROSEDUR PENGECEKAN POSISI ------------------
  // PEMBUATAN PENGECEKAN POSISI ALAT
  if(a.acceleration.y < (-THRESHOLD)) {
    state = 1;
    rgb(255, 0, 0);                     // MERAH
  }
  else if (a.acceleration.y > (THRESHOLD)){
    state = 2;
    rgb(120, 0, 120);                  // MERAH UNYU
  }

  else if (a.acceleration.z > (THRESHOLD)){
    state = 3;
    rgb(0, 255, 0);                    // HIJAU
  }

  else if (a.acceleration.z < (-THRESHOLD)){
    state = 4;
    rgb(0, 255, 120);                 // HIJAU UNYU
  }

  else if (a.acceleration.x > (THRESHOLD)){
    state = 5;
    rgb(0, 0, 255);                  // BIRU
  }

  else if (a.acceleration.x < (-THRESHOLD)){
    state = 6;
    rgb(60, 0, 255);                  // BIRU UNYU
  }
  else {
    state = 0;
    rgb(0,0,0);
  }
  // ---------------------- PROSEDUR PENGECEKAN POSISI || -----------


  // ------------------------ PROSEDUR PERUBAHAAN SEQ --------------
  // seq = HijauMuda, Merah, Hijau Muda 
  // seqBack = 
  buttonState = digitalRead(BUTTON_PIN);
  if (seq == 0){  // KONDISI AWAL SEQ
    isHide = false;
    
    if( buttonState == HIGH && state == 4 ){
      seq = 1;
    } 
  } else if (seq == 1) {
    if( buttonState == HIGH && state == 1 ){
      seq = 2;
    }
  } else if (seq == 2) {
    if( buttonState == HIGH && state == 4 ){
      seq = 3;                                    // DI Sequence 3 Sembunyi
      isHide = true;
    }
  } else if (seq == 3) {
    if( buttonState == HIGH && state == 5 ){
      seq = 4;                                    // BIRU
    }
  } else if (seq == 4) {
    if( buttonState == HIGH && state == 4 ){
      seq = 5;                                    // Hijau Muda
    }
  } else {
    isHide = false;
  }
  // ------------------------ PROSEDUR PERUBAHAN SEQ || -------------
  


  // ----------------- PROSEDUR PEMBUKAAN ---------------------------
  if (isHide){
    myservo.write(0);
  } else {
    myservo.write(18j0);
  }
  // ----------------- PROSEDUR PEMBUKAAN || ------------------------


  
  // ---------------- PROSEDUR RESET ---------------------------------
  if( buttonState == HIGH && state == 6 ){
      seq = 0;
  } 
  // ------------------- PROSEDUR RESET }} ---------------------------
  

  
  delay(10);
}

void rgb(int merah_255, int biru_255, int hijau_255){
  analogWrite(pin_merah, 255-merah_255);
  analogWrite(pin_hijau, 255-biru_255);
  analogWrite(pin_biru, 255-hijau_255);
}

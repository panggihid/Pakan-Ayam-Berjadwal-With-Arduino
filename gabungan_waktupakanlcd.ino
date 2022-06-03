#define pinServoMakanan               8
   
#define waktuBukaServo                2000//milidetik
#define servoBuka                     180//derajat
#define servoTutup                    60//derajat
   
#define waktuMakan1                   DateTime(0, 1, 1,  17, 12, 0, 0)//jam 8 pagi
#define waktuMakan2                   DateTime(0, 1, 1, 17, 13, 0, 0)//jam 5 sore
#define waktuMakan3                   DateTime(0, 1, 1, 17, 14, 0, 0)//jam 9 sore
   
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Sodaq_DS3231.h>
#include <Servo.h>
   
LiquidCrystal_I2C lcd(0x27, 16, 2);//coba juga 0x27
Servo servoMakanIkan;
   
byte detikSebelumnya;
char buf[17];
byte ldr = A0;
byte led = 9;
int nilai;
const int pinBuzzer = 10;
 
   
void setup() {
  pinMode(led,OUTPUT);
  pinMode(pinBuzzer, OUTPUT);
  Serial.begin(9600);
     
  servoMakanIkan.attach(pinServoMakanan);
  servoMakanIkan.write(servoTutup);
   
  Wire.begin();
  rtc.begin();
  
  DateTime dt(2022, 5, 31, 17, 11, 0, 3); // set tanggal dan waktu (format): tahun, bulan tanggal, jam, menit, detik, hari (1=minggu, 7=sabtu)
  rtc.setDateTime(dt);
  Wire.beginTransmission(0x3F);
  if (Wire.endTransmission())
  {
    lcd = LiquidCrystal_I2C(0x27, 16, 2);
  }
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(4, 0);
  lcd.print("Pemberi ");
  lcd.setCursor(4, 1);
  lcd.print("Pakan Ayam");
  delay(3000);
  lcd.clear();
   
  Serial.println("Sistem mulai");
  sprintf(buf, "Set waktu 1 = %02d:%02d (%lu)", waktuMakan1.hour(), waktuMakan1.minute(), waktuMakan1.get());
  Serial.println(buf);
  sprintf(buf, "Set waktu 2 = %02d:%02d (%lu)", waktuMakan2.hour(), waktuMakan2.minute(), waktuMakan2.get());
  Serial.println(buf);
  sprintf(buf, "Set waktu 3 = %02d:%02d (%lu)", waktuMakan3.hour(), waktuMakan3.minute(), waktuMakan3.get());
  Serial.println(buf);
}
   
void loop() {

  nilai = analogRead(ldr);
      if (nilai < 500) {
        digitalWrite(led, HIGH);
        digitalWrite(pinBuzzer, HIGH);
      }
      else {
        digitalWrite(led, LOW);
        digitalWrite(pinBuzzer, LOW);
      }
   
  DateTime now = rtc.now();
  if (detikSebelumnya != now.second())
  {
    sprintf(buf, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
    lcd.setCursor(4, 0);
    lcd.print(buf);
    Serial.print(buf);
   
    detikSebelumnya = now.second();
   
    uint32_t epoch = now.get() % 86400;//hanya jam menit detik
   
    if ((epoch == waktuMakan1.get()) ||
        (epoch == waktuMakan2.get())||
        (epoch == waktuMakan3.get())
        )
    {
      char buf[17];
      sprintf(buf, "Pakan = %02d:%02d", now.hour(), now.minute());
      lcd.setCursor(0, 1);
      lcd.print(buf);
      delay(3000);
      lcd.clear();
      
      Serial.println(buf);
   
      servoMakanIkan.write(servoBuka);
      delay(waktuBukaServo);
      servoMakanIkan.write(servoTutup);
      
    }
  }
}

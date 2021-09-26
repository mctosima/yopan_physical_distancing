//PENGGUNA : Rahman Indra

#include  <SPI.h>                   //library SPI
#include <nRF24L01.h>               //library nRF24l01
#include <RF24.h>                   //library RF24
#include <EEPROM.h>                 //library EEPROMbu
#include <LiquidCrystal_I2C.h>      //library LCD
//esp
#include <SoftwareSerial.h>
#include <Wire.h> 

String SSID_WIFI="qwerty";
String SSID_KEY="qwerty123";
String WEB_SERVER="192.168.43.70"; //Host //10.120.121.187
bool koneksiWifi;   //Simpan status koneksi
//String QUERY="/SistemKeamanan-PPL/arduino/insert?logMsg=";   //Kueri
String QUERY="/websensor/arduino.php?pelanggar=Rahman%20Indra&&dilanggar=";   //Kueri
SoftwareSerial myESP(2,3); // RX, TX
bool konekWifi();
bool kirimLog(String pesanLog);   
bool initWifi();

LiquidCrystal_I2C lcd(0x27,20,4);   // alamat lcd 0x27 (16 char 2 baris)

RF24 radio(9,10);                   //deklarasi sensor nRF menggunakan pin 9 dan 10
int pelanggaranku;                  //pelanggaran pengguna

const int led = 6, ledesp=8;   
const int buzzer = 7;
const byte address[6] = "00001";    //alamat NRF
int rom = 1;                        //tempat penyimpanan data pada eeprom 
bool cekstatus = false;                   //deklarasi variabel penyimpan data pelanggaran
int hitungPelanggaran = EEPROM.read(rom);        //membaca data pada variabel rom
int button2 = 5;           

//FUNGSI DAN DEKLARASI MILLIS
//unsigned long waktuSebelum=0;       // deklarasi untuk menghitung millis
//bool statusLed = HIGH;              //deklarasi statusled menjadi HIGH
//int jumlah = 0;                     //deklarasi jumlah sebagai penghitung led berkedip
//int lamaLed = 500;                  //sebagai lama waktu led berkedip (500 mili / 0,5 detik) saat dimamsukkan rumus pada void loop
//void waktu(){
//  unsigned long waktuSekarang=millis();
//  int lamaHijau=500;q
//  if(waktuSekarang-waktuSebelum>=lamaHijau && jumlah<5){
//    statusLed=!statusLed;
//    digitalWrite(led1,statusLed);
//    waktuSebelum=millis();
//    if(statusLed==LOW){
//      jumlah++;
//      Serial.println(jumlah);
//    }
//  }
//}

void(*mulai_reset) (void) = 0;      //perintah reset


void LCD_TAMPIL(){                  //FUNGSI tampilkan data pada LCD
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Pelanggaran: " );
  lcd.print(hitungPelanggaran);
  lcd.display();
}

void setup() {
  myESP.begin(9600);
  koneksiWifi = konekWifi(); //MATIIN SEMENTARA SUPAYA CEPAT
  
  lcd.init();                       //LCD mulai
  lcd.init();

  pinMode(button2, INPUT);
  pinMode(led,OUTPUT);
  pinMode(ledesp,OUTPUT);
  pinMode(buzzer,OUTPUT);
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0,address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_2MBPS);
  radio.startListening();
  Serial.println("mulai dari awal");
  Serial.println();
  
      digitalWrite(led,HIGH);
      delay(2000);
      digitalWrite(led,LOW);
      delay(1000);
  noTone(buzzer);
}

void loop() {
    unsigned long cekwaktu=millis();
    
  char text[32] = "";
  if (radio.available()){
//    delay(3000);  //tambahkan nanti
    radio.read(&text, sizeof(text));
    String nama = String(text);

    //CEK Muhammad Idris
    if (nama == "idris") { // && cekwaktu> 
      //kirim ke database namapenguna dan nama pelanggar
//      Serial.println(cekwaktu);
      
      LCD_TAMPIL();
      if(cekstatus==false){
        hitungPelanggaran++; //data yang terdapat dalam EEPROM (blok 1) akan ditambah 1
                     //menghitung pelanggaran yopan
        EEPROM.update(rom, hitungPelanggaran); //mengupdate data EEPROM dengan nilai Count
        Serial.print("Pelanggaran Anda adalah sebanyak: ");
        Serial.println(hitungPelanggaran); 
        cekstatus=true; 
      }
      Serial.print("Terdeteksi : ");
      Serial.println(nama);

      if(hitungPelanggaran>0 && hitungPelanggaran<=2){
        for(int i=0; i<4;  i++){
          digitalWrite(led,HIGH);
          tone(buzzer, 300);
          delay(800);
          digitalWrite(led,LOW);
          noTone(buzzer);
          delay(800);
        }
      }
      
      else if(hitungPelanggaran>=3 && hitungPelanggaran<=5){
        for(int i=0; i<5;  i++){
          digitalWrite(led,HIGH);
          tone(buzzer, 900);
          delay(500);
          digitalWrite(led,LOW);
          noTone(buzzer);
          delay(500);
        }
      }
      
     else if(hitungPelanggaran>=6 && hitungPelanggaran<=8){
        for(int i=0; i<7;  i++){
          digitalWrite(led,HIGH);
          tone(buzzer, 1700);
          delay(250);
          digitalWrite(led,LOW);
          noTone(buzzer);
          delay(250);
        }
      }
      else{
//        noTone(buzzer);
        for(int i=0; i<20;  i++){
          digitalWrite(led,HIGH);
          tone(buzzer, 2000);
          delay(100);
          digitalWrite(led,LOW);
          noTone(buzzer);
          delay(100);
        }
      }
     
     if(koneksiWifi){
        kirimLog(String("Muhammad Idris"));
      }else{
        konekWifi();
      }
      
      cekwaktu=0; //ga berpengaruh.
      Serial.println("RESET");
      delay(200);
      mulai_reset();
    }
    
//IMAM WICAKSONO========================================================================================
//NOTED : gunakan hitungPelanggar, sebagai akumulasi jumlah pelanggaran pengguna
    else if (nama == "imam" ) { //&& cekwaktu>7000
      //kirim ke database nama pengguna dan imam sbg pelanggar atau melanggar
//      Serial.println(cekwaktu);
      
      LCD_TAMPIL();
      if(cekstatus==false){
        hitungPelanggaran++; //data yang terdapat dalam EEPROM (blok 1) akan ditambah 1
                     //menghitung pelanggaran yopan
        EEPROM.update(rom, hitungPelanggaran); //mengupdate data EEPROM dengan nilai Count
        Serial.print("Pelanggaran Anda adalah sebanyak: ");
        Serial.println(hitungPelanggaran); 
        cekstatus=true; 
      }
      Serial.print("Terdeteksi : ");
      Serial.println(nama);
//      for(int i=0; i<5;  i++){      //HANYA SEBAGAI PERANTARA LCD
//          digitalWrite(led,HIGH);
//          tone(buzzer, 300);
//          delay(1000);
//          digitalWrite(led,LOW);
//          noTone(buzzer);
//          delay(1000);
//        }
      if(hitungPelanggaran>0 && hitungPelanggaran<=2){
        for(int i=0; i<4;  i++){
          digitalWrite(led,HIGH);
          tone(buzzer, 300);
          delay(800);
          digitalWrite(led,LOW);
          noTone(buzzer);
          delay(800);
        }
      }
      
      else if(hitungPelanggaran>=3 && hitungPelanggaran<=5){
        for(int i=0; i<5;  i++){
          digitalWrite(led,HIGH);
          tone(buzzer, 900);
          delay(500);
          digitalWrite(led,LOW);
          noTone(buzzer);
          delay(500);
        }
      }
      
     else if(hitungPelanggaran>=6 && hitungPelanggaran<=8){
        for(int i=0; i<7;  i++){
          digitalWrite(led,HIGH);
          tone(buzzer, 1700);
          delay(250);
          digitalWrite(led,LOW);
          noTone(buzzer);
          delay(250);
        }
      }
      else{
//        noTone(buzzer);
        for(int i=0; i<20;  i++){
          digitalWrite(led,HIGH);
          tone(buzzer, 2000);
          delay(100);
          digitalWrite(led,LOW);
          noTone(buzzer);
          delay(100);
        }
      }
      
      if(koneksiWifi){
        kirimLog(String("Imam Wicaksono"));
      }else{
//        konekWifi();  //MATIIN SEMENTARA SUPAYA CEPAT
      }
      
      cekwaktu=0; //ga berpengaruh.
      Serial.println("RESET");
      delay(200);
      mulai_reset();
    }

    else{
//      Serial.println(cekwaktu);
      digitalWrite(led,LOW);
//      digitalWrite(buzzer,HIGH);
       Serial.print("NO");
      delay(500);
    }
  }
  else{
    Serial.println("tidak ada radio");
    Serial.print("pelanggaran : ");
    Serial.println(hitungPelanggaran);
//      digitalWrite(buzzer,HIGH);
//      cekwaktu=0;
    delay(500);
  }
 // if(digitalRead(button2)==HIGH){
 //   LCD_TAMPIL();
 //   delay(2000);
 //   lcd.noDisplay();
 //   lcd.noBacklight();
 // }
}

//=====================================================================================
bool konekWifi(){
    Serial.begin(9600);
    //setmode
    myESP.println("AT+CWMODE=1");
    if(myESP.find("OK")){
        Serial.println("CWMODE OK");
        String w = "AT+CWJAP=\"";
        w = w + SSID_WIFI;
        w = w + "\",\"";
        w = w + SSID_KEY;
        w = w + "\"";
        myESP.println(w);
        Serial.println(w);
        myESP.setTimeout(15000);
        if(myESP.find("WIFI GOT IP")){
            Serial.println("CWJAP OK");
            digitalWrite(ledesp,HIGH);
            delay(200);
            digitalWrite(ledesp,LOW);
            myESP.setTimeout(2000);
            myESP.println("AT+CIPMUX=0");
            if(myESP.find("OK")){
                Serial.println("CIPMUX OK\n");
                digitalWrite(ledesp,HIGH);
                delay(200);
                digitalWrite(ledesp,LOW);
                return 1;
            }else{
                Serial.println("CIPMUX NOT OK");
                return 0;
            }
        }else{
            Serial.println("CWJAP NOT OK");
            return 0;
        }
    }else{
        Serial.println("CWMODE NOT OK");
        return 0;
        
    }
}

bool kirimLog(String pesanLog){
    pesanLog.replace(" ", "%20");
    Serial.begin(9600);
    myESP.setTimeout(200);
    String con;
    con = "AT+CIPSTART=\"TCP\",\"";
    con = con + WEB_SERVER;
    con = con +"\",80";
    Serial.println(con);
    myESP.println(con);
    myESP.setTimeout(1000);
    if(myESP.find("CONNECT")){
        Serial.println("READY TO GET");
    }else{
        Serial.println("CANNOT OPEN COMMUNICATION");
        return 0;
    }
    String request;
    request = "";
    request.concat("GET ");
    request.concat(QUERY);
    request.concat(pesanLog);
    request.concat(" HTTP/1.1\r\n");
    request.concat("Host: ");
    request.concat(WEB_SERVER);
    request.concat("\r\n\r\n");
    request.concat("Host: ");
    request.concat(WEB_SERVER);
    request.concat("\r\n\r\n");
    
    Serial.println("TCP REQUEST: ");
    Serial.println(request);
    myESP.print("AT+CIPSEND=");
    myESP.println(request.length());

    if(myESP.find(">")){
        myESP.println(request);
        if(myESP.find("SEND OK")){
            Serial.print("HTTP REQ SENT\n\n");
            myESP.println("AT+CIPCLOSE");
            if(myESP.find("CLOSED")){
                return 1;
            }
        }
    }else{
        Serial.println("CANNOT SEND REQUEST");
        return 0;
    }
}

//Pertama kali dijalankan untuk mereset wifi
bool initWifi(){
    myESP.setTimeout(5000);
    myESP.println("AT+RST");
    delay(1000);
}

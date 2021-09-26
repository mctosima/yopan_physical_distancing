//pengguna : indra
#include  <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9,10);

const byte address[6] = "00001";

void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_2MBPS);
  radio.stopListening();
}

void loop() {
  const char nama[] = "imam";
  radio.write(&nama, sizeof(nama));
  delay(1000);
}

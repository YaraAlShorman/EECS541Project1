const int tx_pin = PD2;
const String data = "ABC\n";
  // "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer ac augue a nibh scelerisque interdum ac id magna."
  // " Nulla blandit semper tortor, a sollicitudin ligula efficitur quis. Vivamus quis metus eu quam tincidunt feugiat."
  // " Fusce posuere elementum lectus in eleifend. Praesent porta neque metus, ut condimentum augue pulvinar non."
  // " Ut id venenatis nibh. Quisque condimentum pellentesque mauris lacinia scelerisque."
  // "\n\n";

#define BIT_DELAY_us 200
#define PACKET_BYTES 8

void setup() {
  pinMode(tx_pin, OUTPUT);
  Serial.begin(9600);
  Serial.println("board initialized\nsending sync wave for 5 seconds...");

  unsigned long endTime = millis() + 5000;
  bool sync = true;
  while ( millis() < endTime ) {
    digitalWrite( tx_pin, sync ? HIGH : LOW );
    sync = !sync;
    delayMicroseconds( BIT_DELAY_us );
  }

  Serial.println("sync done");
}

// Hamming [8,4] extended-parity encoding
const byte nybble_to_hamming[16] = { 0x00, 0xD2, 0x55, 0x87, 0x99, 0x4B, 0xCC, 0x1E, 0xE1, 0x33, 0xB4, 0x66, 0x78, 0xAA, 0x2D, 0xFF };

void transmitByte( byte data ) {
  byte hcode = nybble_to_hamming[data >> 4];

  for ( int i = 7; i >= 0; --i ) {
    // TODO: avoid bitRead to make this more efficient?
    digitalWrite(tx_pin, bitRead(hcode, i));
    delayMicroseconds(BIT_DELAY_us);
  }
  Serial.print(hcode, HEX);

  hcode = nybble_to_hamming[data & 0x0F];
  for ( int i = 7; i >= 0; --i ) {
    digitalWrite(tx_pin, bitRead(hcode, i));
    delayMicroseconds(BIT_DELAY_us);
  }
  Serial.print(" : ");
  Serial.println(hcode, HEX);
}

void loop() {
  Serial.println("TX starting");

  int count = 0;
  for ( int i = 0; i < data.length(); ++i ) {
    if ( count == 0 ) { // Packet Header
      digitalWrite(tx_pin, HIGH);
      delayMicroseconds(8*BIT_DELAY_us);
    }
    transmitByte( data.charAt(i) );
    count = (count + 1)%PACKET_BYTES;
  }
  // finish transmission of final packet if partial
  while ( count > 0 ) {
    transmitByte( 0x00 );
    count = (count + 1)%PACKET_BYTES;
  }

  Serial.println("TX done, idle 5 seconds...");
  digitalWrite(tx_pin, LOW);
  delay(5000);
}

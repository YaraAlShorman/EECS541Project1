#include <arduino-timer.h>

const int tx_pin = PD2;
const String data = "ABC alfalfa\n";
  // "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer ac augue a nibh scelerisque interdum ac id magna."
  // "\nNulla blandit semper tortor, a sollicitudin ligula efficitur quis. Vivamus quis metus eu quam tincidunt feugiat."
  // "\nFusce posuere elementum lectus in eleifend. Praesent porta neque metus, ut condimentum augue pulvinar non."
  // "\nUt id venenatis nibh. Quisque condimentum pellentesque mauris lacinia scelerisque."
  // "\n\n";
const unsigned long data_len = data.length();

Timer<1, micros> timer;

#define BIT_DELAY_us 400
#define PACKET_BYTES 8

// Hamming [8,4] extended-parity encoding
const byte nybble_to_hamming[16] = { 0x00, 0xD2, 0x55, 0x87, 0x99, 0x4B, 0xCC, 0x1E, 0xE1, 0x33, 0xB4, 0x66, 0x78, 0xAA, 0x2D, 0xFF };

byte data_buffer;
byte mask = 0;

unsigned long idle_until;
bool header = true;
int data_index = 0;
int packet_index = 0;
bool firstNybble = true;

bool transmit_bit(void) {
  if ( data_buffer & mask ) {
    PORTD |= B00000100;
  } else {
    PORTD &= B11111011;
  }
  //digitalWrite( tx_pin, (data_buffer & mask) ? HIGH : LOW );
  mask >>= 1;
  return true; // repeat forever
}

void setup() {
  pinMode(tx_pin, OUTPUT);
  Serial.begin(115200);

  idle_until = micros() + 5000000; // 5 seconds
  timer.every( BIT_DELAY_us, transmit_bit );
  Serial.println("transmitter initialized");
}

//unsigned long total_dur = 0;
//unsigned long count = 0;
void loop() {
  timer.tick(); // potentially send a bit...

  // if a full byte has been sent, update state of everything
  // NOTE: can only exit idle/sync state on this 8-bit boundary!
  if ( mask == 0 ) {
    //unsigned long int start = micros();
    mask = 1 << 7;

    // if data has looped but the current packet isn't full, send padding bytes
    if ( data_index == 0 && packet_index != 0 ) {
      data_buffer = 0x00;
      if ( !firstNybble ) {
        //Serial.println("PAD");
        packet_index = (packet_index + 1) % PACKET_BYTES; // every byte in the packet consists of *two* 8-bit Hamming [8,4] codewords!
      }
      firstNybble = !firstNybble;
      header = true;
    
    // all padding has been sent, check if still in idle period
    } else if ( micros() < idle_until ) {
      data_buffer = 0xAA;

    // send the header byte if starting a new packet
    } else if ( header ) {
      // Serial.println("packet:");
      header = false;
      data_buffer = 0xFF;

    // send first half of current character
    } else if ( firstNybble ) {
      firstNybble = false;
      data_buffer = nybble_to_hamming[ data[data_index] >> 4 ];
      Serial.print( data[data_index] );
      // Serial.print( data[data_index], HEX );
      // Serial.print("|");
      // Serial.print( data_buffer, HEX );
      // Serial.print(":");

    // send second half of current character
    } else {
      firstNybble = true;
      data_buffer = nybble_to_hamming[ data[data_index] & 0x0F ];
      //Serial.println( data_buffer, HEX );

      // advance to next character, idle for 5 seconds if we loop to start of message
      data_index = (data_index + 1) % data_len;
      if ( data_index == 0 ) {
        Serial.println("idling");
        //Serial.println((float)total_dur/count);
        idle_until = micros() + 5000000; // 5 seconds
        // technically, this should happen only after sending the padding bytes, if any, but the logic for that is too complicated
      }

      // advance packet count too, enabling the header if a new packet is next
      packet_index = (packet_index + 1) % PACKET_BYTES;
      header = packet_index == 0;
    }

    // unsigned long dur = micros() - start;
    // if ( dur > 100 ) {
    //   total_dur += dur;
    //   count++;
    // }
  }
}

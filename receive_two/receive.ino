const int rx_pin = PC2;

#define BIT_DELAY_us 200
#define PACKET_BYTES 8

int threshold = 512;
int signal;
byte packet[PACKET_BYTES] = {0};
byte data_buffer = 0;

int bit_error_count;
int error_count_start_ms;
unsigned long time;

// Hamming [8,4] extended-parity decoding
// High nybble : # of bit errors detected
// Low  nybble : data bits for this codeword
const byte hamming_to_nybble[256] = {
	0x00, 0x10, 0x10, 0x29, 0x10, 0x2e, 0x2b, 0x13, 0x10, 0x2e, 0x2d, 0x15, 0x2e, 0x1e, 0x17, 0x2e, 
	0x10, 0x29, 0x29, 0x19, 0x2a, 0x12, 0x17, 0x29, 0x2c, 0x14, 0x17, 0x29, 0x17, 0x2e, 0x07, 0x17, 
	0x10, 0x2e, 0x2d, 0x19, 0x2e, 0x1e, 0x1b, 0x2e, 0x2e, 0x1e, 0x1d, 0x2e, 0x1e, 0x0e, 0x2e, 0x1e, 
	0x2c, 0x19, 0x19, 0x09, 0x1a, 0x2e, 0x2b, 0x19, 0x1c, 0x2e, 0x2d, 0x19, 0x2e, 0x1e, 0x17, 0x2f, 
	0x10, 0x28, 0x2b, 0x15, 0x2b, 0x12, 0x1b, 0x2b, 0x2c, 0x15, 0x15, 0x05, 0x16, 0x2e, 0x2b, 0x15, 
	0x2c, 0x12, 0x11, 0x29, 0x12, 0x02, 0x2b, 0x12, 0x1c, 0x2c, 0x2c, 0x15, 0x2c, 0x12, 0x17, 0x2f, 
	0x2c, 0x18, 0x1b, 0x2b, 0x1b, 0x2e, 0x0b, 0x1b, 0x1c, 0x2e, 0x2d, 0x15, 0x2e, 0x1e, 0x1b, 0x2f, 
	0x1c, 0x2c, 0x2c, 0x19, 0x2c, 0x12, 0x1b, 0x2f, 0x0c, 0x1c, 0x1c, 0x2f, 0x1c, 0x2f, 0x2f, 0x1f, 
	0x10, 0x28, 0x2d, 0x13, 0x2a, 0x13, 0x13, 0x03, 0x2d, 0x14, 0x1d, 0x2d, 0x16, 0x2e, 0x2d, 0x13, 
	0x2a, 0x14, 0x11, 0x29, 0x1a, 0x2a, 0x2a, 0x13, 0x14, 0x04, 0x2d, 0x14, 0x2a, 0x14, 0x17, 0x2f, 
	0x2d, 0x18, 0x1d, 0x2d, 0x1a, 0x2e, 0x2d, 0x13, 0x1d, 0x2e, 0x0d, 0x1d, 0x2e, 0x1e, 0x1d, 0x2f, 
	0x1a, 0x2a, 0x2d, 0x19, 0x0a, 0x1a, 0x1a, 0x2f, 0x2d, 0x14, 0x1d, 0x2f, 0x1a, 0x2f, 0x2f, 0x1f, 
	0x28, 0x18, 0x11, 0x28, 0x16, 0x28, 0x2b, 0x13, 0x16, 0x28, 0x2d, 0x15, 0x06, 0x16, 0x16, 0x2f, 
	0x11, 0x28, 0x01, 0x11, 0x2a, 0x12, 0x11, 0x2f, 0x2c, 0x14, 0x11, 0x2f, 0x16, 0x2f, 0x2f, 0x1f, 
	0x18, 0x08, 0x2d, 0x18, 0x2b, 0x18, 0x1b, 0x2f, 0x2d, 0x18, 0x1d, 0x2f, 0x16, 0x2f, 0x2f, 0x1f, 
	0x2c, 0x18, 0x11, 0x2f, 0x1a, 0x2f, 0x2f, 0x1f, 0x1c, 0x2f, 0x2f, 0x1f, 0x2f, 0x1f, 0x1f, 0x0f, 
};

void setup() {
    pinMode(rx_pin, INPUT_PULLUP);
    Serial.begin(9600);
    Serial.println("receiver initialized\nsynchronizing...");

    time = micros();
    // TODO: establish threshold
    // TODO: align clock to waveform extrema

    Serial.println("sync done");
}



// function to make the receiver manufacture a 200us delay between reading bits
void timeWizard() {
    Serial.println(time);
    unsigned long new_time = micros();
    unsigned long diff = new_time - time;
    time = new_time;
    // i suspect that this is where the problem is happening 
    //delayMicroseconds(BIT_DELAY_us - diff);
}

byte receiveByte() {
    byte code, result;

    // read next *two* bytes of Hamming [8,4]-encoded data to get *one* byte of data
    for ( int i = 0; i < 8; ++i ) {
        signal = analogRead(rx_pin);
        data_buffer <<= 1;
        data_buffer |= (signal > threshold ? 1 : 0);
        timeWizard();
        // delayMicroseconds(BIT_DELAY_us);
    }
    code = hamming_to_nybble[data_buffer];

    bit_error_count += (code >> 4);
    result = (code & 0x0F);
    result << 4;

     //Serial.print(code, HEX);
     //Serial.print(" : ");

    for ( int i = 0; i < 8; ++i ) {
        signal = analogRead(rx_pin);
        data_buffer <<= 1;
        data_buffer |= (signal > threshold ? 1 : 0);
        timeWizard();
        // delayMicroseconds(BIT_DELAY_us);
    }
    code = hamming_to_nybble[data_buffer];

    bit_error_count += (code >> 4);
    result |= (code & 0x0F);

    Serial.println(code, HEX);

    return result;
}

// !!! TODO !!! :
// Convert this to use an interrupt-based timer callback, or otherwise compute the appropriate delay?
//
// Currently, it takes a *significant* amount of time to process each incoming bit,
//   enough that a substatial proportion of the BIT_DELAY_us period has already been
//   consumed, creating a very large desync of the analogRead frequency compared with
//   the actual frequency of the bitstream, leading in turn to an absurd bit error rate.

// Also TODO: display the bit error rate somehow

void loop() {
    // read 1 bit into the buffer
    signal = analogRead(rx_pin);
    data_buffer <<= 1;
    data_buffer |= (signal > threshold ? 1 : 0);
    timeWizard();

    // is this the right data_buffer to look for?
    // this code segment did not run when i tested it
    if (data_buffer == 0xAA)  {  // when the receiver detects a signal of repeated high low for more than 8 bits, 
        // sync and decide on a threshold
        int counter = 0;
        int threshSum = 0;
        //time = micros();
        while ( data_buffer == 0xAA ) { // if received data is 10101010
          threshSum = threshSum + analogRead(rx_pin);
          counter = counter + 8; // with every buffer we receive 8 bits
        }
        threshold = threshSum / counter;
        int bittime = (micros() - time)/1000000;
        Serial.println("Bitrate: " + counter/bittime);
    }

    // check for packet header
    if ( data_buffer == 0xFF ) {
        //time = micros();
        // read a packet's worth of data
        for ( int i = 0; i < PACKET_BYTES; ++i ) {
            packet[i] = receiveByte();
        }
        // and push it to Serial out
        // Serial.write(packet, PACKET_BYTES);
    }
}

int last_rx = LOW;
const byte rx_pin = PC2; // PC2 = A2 on board

unsigned long lastedge_time_us;

void setup() {
  pinMode(rx_pin, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("Now reading");
}

// int count = 0;
// float freq_hz;

int cycle_count = 0;
int min = 1023;
int max = 0;

void loop() {
  // int rx = digitalRead(rx_pin);
  // if ( rx != last_rx && rx == HIGH ) {
  //   count++;
  //   unsigned long time_us = micros();
  //   freq_hz = 1000000.0 / (time_us - lastedge_time_us);
  //   lastedge_time_us = time_us;
  // }
  // if ( count == 1000 ) {
  //   Serial.print('\r');
  //   Serial.print(freq_hz, 3);
  //   Serial.println(" [Hz]");
  //   count = 0;
  // }
  // last_rx = rx;


  int rx = analogRead(rx_pin);
  
  if ( rx > max ) max = rx;
  if ( rx < min ) min = rx;

  if ( ++cycle_count%100 == 0 ) {
    Serial.print(min);
    Serial.print(' ');
    Serial.print(max);
    Serial.print(' ');
    Serial.print(rx);
    Serial.print(' ');
    Serial.println(cycle_count);

    if ( cycle_count == 1000 ) {
      cycle_count = 0;
      min = 1023;
      max = 0;
    }
  }
}

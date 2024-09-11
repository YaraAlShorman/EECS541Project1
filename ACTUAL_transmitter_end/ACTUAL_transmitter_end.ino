const byte tx_pin = PD2;

void setup() {
  pinMode(tx_pin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Now transmitting");
}

// HIGH = 1
// LOW = 0
int signal = HIGH;

// String data = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer ac augue a nibh scelerisque interdum ac id magna. Nulla blandit semper tortor, a sollicitudin ligula efficitur quis. Vivamus quis metus eu quam tincidunt feugiat. Fusce posuere elementum lectus in eleifend. Praesent porta neque metus, ut condimentum augue pulvinar non. Ut id venenatis nibh. Quisque condimentum pellentesque mauris lacinia scelerisque."

void loop() {
  signal = !signal;
  digitalWrite(tx_pin, signal);
  delayMicroseconds(200); // a period of 800us for a bit tranmission rate of approx 1200 bits/s
}
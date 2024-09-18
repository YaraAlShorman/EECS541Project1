const byte tx_pin = PD2;
String data = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer ac augue a nibh scelerisque interdum ac id magna. Nulla blandit semper tortor, a sollicitudin ligula efficitur quis. Vivamus quis metus eu quam tincidunt feugiat. Fusce posuere elementum lectus in eleifend. Praesent porta neque metus, ut condimentum augue pulvinar non. Ut id venenatis nibh. Quisque condimentum pellentesque mauris lacinia scelerisque.";

void setup() {
  pinMode(tx_pin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Now transmitting");


 // String data = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer ac augue a nibh scelerisque interdum ac id magna. Nulla blandit semper tortor, a sollicitudin ligula efficitur quis. Vivamus quis metus eu quam tincidunt feugiat. Fusce posuere elementum lectus in eleifend. Praesent porta neque metus, ut condimentum augue pulvinar non. Ut id venenatis nibh. Quisque condimentum pellentesque mauris lacinia scelerisque.";


//   for(int i=0; i<data.length(); i++){

//     char myChar = data.charAt(i);
    
//       for(int i=7; i>=0; i--){
//         bytes = bitRead(myChar,i);
//         Serial.print(bytes, BIN);
//         Serial.println("");
//       }

//       //Serial.println("");
//   }
}

int high = HIGH;

void loop() {
  int count = 0;
  digitalWrite(tx_pin, HIGH);
  delayMicroseconds(2400);
    for(int i=0; i<data.length(); i++){

    char myChar = data.charAt(i);
      for(int i=7; i>=0; i--){
        byte bytes = bitRead(myChar,i);
        digitalWrite(tx_pin, bytes);
        delayMicroseconds(400);
        //Serial.print(bytes, BIN);
        //Serial.println("");
      }

      //Serial.println("");
  }
}

  // while (count < byte.length()){
  //   digitalWrite(tx_pin, bytes);
  //   Serial.print(bytes, BIN);
  //   bytes >> 1;
  //   count++;
  // signal = !signal;
  // digitalWrite(tx_pin, signal);
  // delayMicroseconds(400); // a period of 800us for a bit tranmission rate of approx 1200 bits/s
//}
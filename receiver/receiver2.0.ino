int inPin = PD2; // Pin connected to on the Arduino Nano
int rec = 0; // Received value (1 or 0)
String checker = ""; // Reads in 1s, resets on every 0
String received = ""; // Received 0s and 1s
String result = ""; // Received information in letters
int mistakes = 0; // Number of errors
int read = 0; // Start/stop reading in info
int num_bits = 0; // Number of bits received
String finalString = ""; // Final string after conversion

void setup() {
  pinMode(inPin, INPUT); // Assigning the input pin
  Serial.begin(9600); // Start serial communication for output
}

// Expected received data
String exp_data = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer ac augue a nibh scelerisque interdum ac id magna. Nulla blandit semper tortor, a sollicitudin ligula efficitur quis. Vivamus quis metus eu quam tincidunt feugiat. Fusce posuere elementum lectus in eleifend. Praesent porta neque metus, ut condimentum augue pulvinar non. Ut id venenatis nibh. Quisque condimentum pellentesque mauris lacinia scelerisque.";

void loop() {
  rec = digitalRead(inPin);
  checker.concat((char)(rec + '0')); // Convert recfrom int 0/1 to char '0'/'1'
  
  // Resets our checker if the 1 streak is broken
  if (rec == 0) {
    checker = "";
  }

  // Starts reading in the message
  if (read == 1) {
    received.concat((char)(rec + '0')); // Add the binary digit to received
    num_bits++; //counts number of bits read in
  }

  // Checks if we read in 6 1s, if so stops reading
  if (checker == "111111") {
    read = 0;
    int length = received.length();

    // Converts the binary string we received into the phrase, 
    // and counts how many mistakes
    for (int i = 0, j = 0; i < length; i += 8) {
      String byteStr = received.substring(i, i + 8); // Get 8 bits as a substring
      char character = (char) strtol(byteStr.c_str(), NULL, 2); // Convert binary to char
      finalString += character; // Append the character to finalstring
      
      // Check character by character if the letters match to count the mistakes
      if (exp_data[j++] != character) {
        mistakes++;
      }
    }

    Serial.print("Received string: ");
    Serial.println(finalString);
    Serial.print("Number of Mistakes: ");
    Serial.println(mistakes);

    // Reset variables for the next read
    finalString = "";
    received = "";
    mistakes = 0;
    num_bits = 0;
  }

  // Checks if we have read in 7 1s, if so starts reading
  else if (checker == "1111111") {
    read = 1; // Sets it to start reading
    checker = "";
  }
}

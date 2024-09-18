int inPin = PD2;//pin connected to on the aurduino nano
int rec = 0;//received value (1 or 0)
string temp = "";//reads in 1s resets on every 0
string received = "";//received 0s and 1s
string result = "";//received information in letters
int mistakes = 0;//# of errors
int read = 0;//Start/stop reading in info

void setup() {
  pinMode(inPin, INPUT);//assigning the input pin
}

//expected received data
String data = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer ac augue a nibh scelerisque interdum ac id magna. Nulla blandit semper tortor, a sollicitudin ligula efficitur quis. Vivamus quis metus eu quam tincidunt feugiat. Fusce posuere elementum lectus in eleifend. Praesent porta neque metus, ut condimentum augue pulvinar non. Ut id venenatis nibh. Quisque condimentum pellentesque mauris lacinia scelerisque."

void loop() {

  rec = digitalRead(inPin);
  temp.concat(rec)
  
  //resets our checker if the 1 streak is broken
  if(rec == 0){
    temp = "";
  }
  
  //starts reading in the message
  if (read == 1){
    received.concat(rec);
  }
  
  //checks if we read in 6 1s, if so stops reading
  if(temp == '111111'){
    read = 0;
  }

  //checks if we have read in 7 1s if so starts reading
  else if (temp == '1111111'){
    read = 1;//sets it to start reading
    temp = "";
  }
  


}

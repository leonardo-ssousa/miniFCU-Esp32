int centerText(String text, int line){
  int textSize = (text.length()*5)+(text.length()-1);

  display.setCursor((display.width() - textSize)/2, line);
  display.println(text);

  return (display.width() - textSize)/2;
}

void awaitReleaseButton(){
  while(!digitalRead(BTN)){
    ESP.wdtFeed(); // Reinicia o watchdog timer
  }
}

String httpRequest(String link){
  WiFiClient client;
  HTTPClient http;
  String payload = "";

    if (http.begin(client, link)) {  // HTTP
      
      int httpCode = http.GET(); // start connection and send HTTP header
      // httpCode will be negative on error
      if (httpCode > 0) {        
        Serial.printf("[HTTP] GET... code: %d\n", httpCode); // HTTP header has been send and Server response header has been handled
        
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) { // file found at server
          payload = http.getString();
          Serial.println(payload);
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      Serial.println("[HTTP] Unable to connect");
    }
  return payload;
}

//#######################
//### Segment Display ###
//#######################

void segmentsDisplayTest(){
  const int digits[] = {0b10000000, 0b01000000, 0b00100000, 0b00010000};
  const int segments[] = {0b01111111, 0b10111111, 0b11011111, 0b11101111, 0b11110111, 0b11111011, 0b11111101, 0b11111110, 0b11111111};

  //Testa cada segmento digito por digito
  for(int j = 0; j < 4; j++){    
    for(int i = 0; i <= 8; i++){
    shiftOut(shiftRegisterDataPin, shiftRegisterClockPin, LSBFIRST, segments[i]);
    shiftOut(shiftRegisterDataPin, shiftRegisterClockPin, LSBFIRST, digits[j]);
    digitalWrite(shiftRegisterSendPin, HIGH);
    digitalWrite(shiftRegisterSendPin, LOW);
    delay(50);
    }
  }

  shiftOut(shiftRegisterDataPin, shiftRegisterClockPin, LSBFIRST, 0b00000000);
  shiftOut(shiftRegisterDataPin, shiftRegisterClockPin, LSBFIRST, 0b11111111);
  digitalWrite(shiftRegisterSendPin, HIGH);
  digitalWrite(shiftRegisterSendPin, LOW);
  delay(1000);

  shiftOut(shiftRegisterDataPin, shiftRegisterClockPin, LSBFIRST, 0b00000000);
  shiftOut(shiftRegisterDataPin, shiftRegisterClockPin, LSBFIRST, 0b00000000);
  digitalWrite(shiftRegisterSendPin, HIGH);
  digitalWrite(shiftRegisterSendPin, LOW);
  delay(1000);

  shiftOut(shiftRegisterDataPin, shiftRegisterClockPin, LSBFIRST, 0b00000000);
  shiftOut(shiftRegisterDataPin, shiftRegisterClockPin, LSBFIRST, 0b11111111);
  digitalWrite(shiftRegisterSendPin, HIGH);
  digitalWrite(shiftRegisterSendPin, LOW);
  delay(1000);
}

void segmentDisplayClear(){
  digitalWrite(shiftRegisterSendPin, HIGH);
  shiftOut(shiftRegisterDataPin, shiftRegisterClockPin, LSBFIRST, 0b11111111);
  shiftOut(shiftRegisterDataPin, shiftRegisterClockPin, LSBFIRST, 0b11111111);
  shiftOut(shiftRegisterDataPin, shiftRegisterClockPin, LSBFIRST, 0b11111111);
  shiftOut(shiftRegisterDataPin, shiftRegisterClockPin, LSBFIRST, 0b11111111);
  //shiftOut(shiftRegisterDataPin, shiftRegisterClockPin, LSBFIRST, 0b00000000);
  digitalWrite(shiftRegisterSendPin, LOW);
  digitalWrite(shiftRegisterSendPin, HIGH);
}

void segmentDisplayShow(int numberToDisplay, int delayValue){

  const int digits[] = {0b10000000, 0b01000000, 0b00100000, 0b00010000, 0b00001000, 0b00000100, 0b00000010, 0b00000001};
  const int numList[10] = {0b00000011, 0b10011111, 0b00100101, 0b00001101,  0b10011001, 0b01001001, 0b01000001, 0b00011111, 0b000000001, 0b00001001};
  const int contCarctNumber = String(numberToDisplay).length();
  segmentDisplayClear();

  digitalWrite(shiftRegisterSendPin, HIGH);
  for(int i = contCarctNumber; i >= 0; i--){
    int digitToDisplay = String(numberToDisplay).substring(i, i + 1).toInt();

    shiftOut(shiftRegisterDataPin, shiftRegisterClockPin, LSBFIRST, numList[digitToDisplay]);
    //shiftOut(shiftRegisterDataPin, shiftRegisterClockPin, LSBFIRST, digits[i]);
  }
  digitalWrite(shiftRegisterSendPin, LOW);
  digitalWrite(shiftRegisterSendPin, HIGH);
  //delay(delayValue);
}
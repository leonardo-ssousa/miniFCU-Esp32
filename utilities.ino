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
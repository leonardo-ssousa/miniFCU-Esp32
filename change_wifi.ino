void screenChangeWifi(){
  setPassword = "";
  cursor = 0;
  encoderIsValue = false;
  

  int frame = 0;
  int ssidList = searchWifi();

  Serial.print("Tamanho ssid: ");
  Serial.print(setSSID);
  Serial.print(" ");
  Serial.println(setSSID.length());


  //Seleciona Rede
  while(setSSID.length() <= 0){
    if(cursor > ssidList - 1){
      cursor = ssidList - 1;
    }

    if(cursor < 0){
      cursor = 0;
    }

    display.clearDisplay();
    display.setCursor(4, 0);
    display.println("Selecione");
    display.setCursor(4, 8);
    display.println("Sua rede:");
    display.drawLine(4, 17, 124, 17, WHITE);
    display.drawBitmap(8, 53, icon_left_arrow_3_5, 3, 5, WHITE);
    display.drawBitmap(120, 53, icon_right_arrow_3_5, 3, 5, WHITE);
    centerText(String(cursor+1) + "/" + String(ssidList), 52);
    centerText(String(WiFi.SSID(cursor)), 29);
    display.display();

    int progress = 0;
    if(!digitalRead(BTN)){
      while(!digitalRead(BTN)){
        display.drawRect(109, 0, progress, 16, WHITE);
        display.drawBitmap(111, -1, icon_reset_11_16, 11, 16, BLACK);
        display.display();
        progress = progress < 16 ? progress + 1 : 16;
      }

      setSSID = WiFi.SSID(cursor);

      Serial.print("Selected SSID: ");
      Serial.println(setSSID);
    }

    if(progress >= 16){
      ssidList = searchWifi();
      progress = 0;
    }
  }


  //Seleciona Senha
  int charSelector = 0;
  int charCounter = 0;
  char tempPassword[] = {};
  while(setPassword.length() <= 0 && setSSID.length() > 0){
    //Mode encoder;
    encoderIsValue = true;
    //Configura intervalo tabela ASCII
    if(encoderValue < 33){
      encoderValue = 126;
    } 
    if(encoderValue > 126){
      encoderValue = 33;
    } 

    display.clearDisplay();
    
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("Rede:");
    display.println(setSSID);    
    display.setCursor(0,23);
    display.println("Senha:");

    for(int i = 0; i < charCounter; i++){
      display.setCursor((i*7+1), 51);
      display.setTextColor(WHITE);
      display.println(tempPassword[i]);
    }

    display.drawBitmap((charCounter*7+1), 45, icon_up_arrow_5_3, 5, 3, WHITE);
    display.drawBitmap((charCounter*7+1), 61, icon_down_arrow_5_3, 5, 3, WHITE);

    //Seletor
    char iToChar = encoderValue;
    display.fillRect((charCounter*7), 50, 7, 9, WHITE);
    display.setCursor((charCounter*7+1), 51);
    display.setTextColor(BLACK);
    display.println(iToChar);


    display.display();

    int isHolding = 0;
    if(!digitalRead(BTN)){
      while(!digitalRead(BTN)){
        display.drawRect(112, 0, isHolding, 15, WHITE);
        display.setCursor(114, 4);
        display.setTextColor(BLACK);
        display.println("OK");
        display.display();
        isHolding = isHolding < 15 ? isHolding + 1 : 15;
      }

      if(isHolding >= 15){
        setPassword = String(tempPassword);
      }

      tempPassword[charSelector] = encoderValue;
      charSelector = charSelector + 1;
      charCounter = charCounter + 1;
      // Serial.println(tempPassword);
      // Serial.println(charCounter) ;
    }

  }
  tryConectionWifi(); 

}

int searchWifi() {  
  long timeSearchWifi;
  int ssidList;
  setSSID = "";

  //Searching wifi UI
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 24);
  display.println("Buscando");
  display.println("Redes Wifi...");
  display.display();

  ssidList = WiFi.scanNetworks();
  Serial.print("SSIDs: ");
  Serial.println(ssidList);
  
  return ssidList;
}

void tryConectionWifi(){
  //Dev
  Serial.println("-----------------");
  Serial.println("Wifi configurado:");
  Serial.print("SSID: ");
  Serial.print(setSSID);
  Serial.print(" Senha: ");
  Serial.println(setPassword);
  Serial.println("-----------------");

  //Tentando conectar UI
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(16, 24);
  display.println("Conectando");
  display.setCursor(16, 32);
  display.println("a rede ...");
  display.drawBitmap(93, 23, icon_wifi_full_19_16, 19, 16, WHITE);
  display.display();

  
  WiFi.mode(WIFI_STA);
  WiFi.begin(setSSID, setPassword);
  WiFi.setAutoConnect(true);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  if(WiFi.status() == WL_CONNECTED){
    Serial.println("");
    Serial.println("Connectado!!");
    isConnected = true;
  } else {
    Serial.println("");
    Serial.println("Falha na conexao...");

    //Falha ao conectar UI
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setCursor(16, 24);
    display.println("Falha");
    display.setCursor(16, 32);
    display.println("na conexao");
    display.drawBitmap(93, 23, icon_wifi_not_connected_19_16, 19, 16, WHITE);
    display.display();
    isConnected = false;
    delay(5000);
  }


}
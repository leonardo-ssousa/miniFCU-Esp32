bool screenChangeWifi(){
  ssid = "";
  password = "";
  cursor = 0;
  

  int frame = 0;
  int ssidList = searchWifi(); 
  int holdBTNDelay = 2000;

  //Seleciona Rede
  while(ssid.length() <= 0){
    if(cursor > ssidList - 1){
      cursor = ssidList - 1;
    }

    if(cursor < 0){
      cursor = 0;
    }

    display.clearDisplay();
    //centerText("Selecione sua rede:", 0);
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
        //display.drawLine(0,63, progress, 63, WHITE);
        display.drawRect(109, 0, progress, 16, WHITE);
        display.drawBitmap(111, -1, icon_reset_11_16, 11, 16, BLACK);
        display.display();
        progress = progress < 16 ? progress + 1 : 16;
        Serial.println(progress);
      }

      ssid = WiFi.SSID(cursor);
      Serial.println(ssid);
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
  while(password.length() <= 0 && ssid.length() > 0){
    //Mode encoder;
    encoderIsValue = true;
    //Configura inicio tabela ASCII
    if(encoderValue < 33){
      encoderValue = 33;
    } 
    if(encoderValue > 126){
      encoderValue = 126;
    } 

    display.clearDisplay();
    
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("Rede:");
    display.println(ssid);    
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

    int progress = 0;
    if(!digitalRead(BTN)){
      while(!digitalRead(BTN)){
        display.drawRect(112, 0, progress, 15, WHITE);
        //display.drawBitmap(111, -1, icon_reset_11_16, 11, 16, BLACK);
        display.setCursor(114, 4);
        display.setTextColor(BLACK);
        display.println("OK");
        display.display();
        progress = progress < 15 ? progress + 1 : 15;
      }

      tempPassword[charSelector] = encoderValue;
      charSelector = charSelector + 1;
      charCounter = charCounter + 1;
      Serial.println(tempPassword);
      Serial.println(charCounter) ;
    }

    if(progress >= 15){
      // ssidList = searchWifi();
      // progress = 0;
    }
  }



  return false;
}

int searchWifi() {  
  long timeSearchWifi;
  int ssidList;
  ssid = "";

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
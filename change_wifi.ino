bool screenChangeWifi(){
  ssid = "";
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
    centerText("Selecione sua rede:", 0);
    display.drawLine(4, 9, 124, 9, WHITE);
    display.drawBitmap(8, 53, icon_left_arrow_3_5, 3, 5, WHITE);
    display.drawBitmap(120, 53, icon_right_arrow_3_5, 3, 5, WHITE);
    centerText(String(cursor+1) + "/" + String(ssidList), 52);
    centerText(String(WiFi.SSID(cursor)), 23);
    display.display();

    int progress = 0;
    if(!digitalRead(BTN)){
      while(!digitalRead(BTN)){
        display.drawLine(0,63, progress, 63, WHITE);
        display.display();
        progress+=4;
      }

      ssid = WiFi.SSID(cursor);
      Serial.println(ssid);
    }

    if(progress >= 124){
      ssidList = searchWifi();
      progress = 0;
    }
  }


  //Seleciona Senha
  while(password.length() <= 0 && ssid.length() > 0){
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Rede:");
    display.println(ssid);
    display.display();



    int progress = 0;
    if(!digitalRead(BTN)){
      while(!digitalRead(BTN)){
        display.drawLine(0,63, progress, 63, WHITE);
        display.display();
        progress+=4;
      }

      // ssid = WiFi.SSID(cursor);
      // Serial.println(ssid);
    }

    if(progress >= 124){
      ssid = "";
      progress = 0;
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
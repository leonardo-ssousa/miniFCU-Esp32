void screenMainMenu(int toCursor){
  display.clearDisplay();
  encoderIsValue = false;
  cursor = toCursor;

  //Força primeira requisição do relogio
  lastMillisTime = 0;

  const unsigned char* icons[] = {icon_volume_20_20, icon_speaker_device_20_20, icon_plane_20_20, icon_sleep_mode_20_20, icon_settings_20_20};
  String titles[] = {"Volume Mixer", "Dispositivo de Saida", "Flight Simulator", "Sleep", "Configuracoes"};
  int sizeTitle = (sizeof(titles) / sizeof(titles[0])) - 1;

  while(true){
    display.clearDisplay();

    //Backgrounds Icons
    display.fillRoundRect(45, 0, 38, 38, 2, WHITE);
    display.drawRoundRect(0, 4, 30, 30, 1, WHITE);
    display.drawRoundRect(98, 4, 30, 30, 1, WHITE);

    cursor = cursor > sizeTitle ? cursor = 0 : cursor;
    cursor = cursor < 0 ? cursor = sizeTitle : cursor;

    //Icone anterior
    int previousCursor = (cursor - 1) < 0 ? sizeTitle : cursor - 1;
    display.drawBitmap(5, 9, icons[previousCursor], 20, 20, WHITE);

    //Icone atual
    display.drawBitmap(54, 9, icons[cursor], 20, 20, BLACK);

    //Proximo icone
    int nextCursor = (cursor + 1) > sizeTitle ? 0 : cursor + 1;
    display.drawBitmap(103, 9, icons[nextCursor], 20, 20, WHITE);

    // ### Mostra Cursor
    int dotsSize = (sizeTitle+1)*2 + sizeTitle*4;
    int dotsPosition = (display.width()-dotsSize)/2;

    display.setCursor(dotsPosition, 44);    
    for(int i = 0; i <= sizeTitle; i++){ 
      int currentDot = display.getCursorX();
      display.fillRect(currentDot, display.getCursorY(), 2, 2, WHITE);
      display.setCursor(currentDot + 6, display.getCursorY());
    }
    
    int selectorPosition = (dotsPosition-1)+(cursor*6);
    display.fillRect(selectorPosition, display.getCursorY()-1, 4, 4, WHITE);

    //Mostra titulo
    display.setTextColor(WHITE);
    display.setTextSize(1);
    centerText(titles[cursor], 53);

    display.display();

    if(millis() - lastMillisTime > 1000*30 || lastMillisTime == 0){
      lastMillisTime = millis();
      segmentDisplayHour();      
    }

    //Selecionado tela
    Screens selectedScreen = Screens(cursor);
    if(!digitalRead(BTN)){
      awaitReleaseButton();

      switch(selectedScreen){
        case mixer:
        Serial.println("Go to: Volume mixer");
        screenVolumeMixer();
        break;
        case outputDevice:
        Serial.println("Go to: Disp. Saida");
        screenOutputDevice();
        break;
        case flightSim:
        Serial.println("Go to: Flight Sim");
        break;
        case config:
        segmentsDisplayTest();
        Serial.println("Go to: Configs");
        break;
        case sleep:
        Serial.println("Go to: Sleep Mode");
        screenSleepMode();
        break;
      }
    }
  }
}
















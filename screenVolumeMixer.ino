void screenVolumeMixer(){
  display.clearDisplay();
  encoderIsValue = false;
  debounceEncoder = 50;

  String tempData[] = {"Spotify", "Chrome", "Flight Sim", "Discord"};
  int tempDataSize = sizeof(tempData) / sizeof(tempData[0]) - 1;

  while(true){
    display.clearDisplay();

    cursor = cursor > tempDataSize ? cursor = 0 : cursor;
    cursor = cursor < 0 ? cursor = tempDataSize : cursor;
    

    //Cabeçalho
    centerText("Volume mixer", 1);
    display.drawLine(0, 9, 127, 9, WHITE);

    //Item anterior
    display.setCursor(0, 15);
    int previousCursor = (cursor + 1) > tempDataSize ? 0 : cursor + 1;
    display.println(tempData[previousCursor]);

    //Atual item
    display.setFont(&muHeavy8ptBold);
    display.setCursor(6, 39);
    display.println(tempData[cursor]);
    display.drawBitmap(1, 27, draw_volumeBorderSeletor_122_18, 122, 18, WHITE);
    display.setFont();

    //Proximo item
    display.setCursor(0, 51);
    int nextCursor = (cursor - 1) < 0 ? tempDataSize : cursor - 1;
    display.println(tempData[nextCursor]);

    int backProgress = 0;
    if(!digitalRead(BTN)){

      //Pressionado longo
      while(!digitalRead(BTN)){
        display.fillRect(0, 0, backProgress, 9, WHITE);
        display.drawBitmap(3, 2, icon_left_arrow_3_5, 3, 5, BLACK);
        display.display();
        backProgress = backProgress < 10 ? backProgress + 1 : 10;
      }
      
      if(backProgress >= 10){
        Serial.println("Back to menu");
        screenMainMenu(0); 
      }      

      //Presionado curto
      screenVolumeValue(tempData[cursor]);
    }

    display.display();
  }
} 
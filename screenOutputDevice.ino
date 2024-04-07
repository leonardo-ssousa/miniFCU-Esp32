void screenOutputDevice(){
  display.clearDisplay();
  encoderIsValue = false;
  debounceEncoder = 50;

  String tempData[] = {"Alto-falantes (Realtek High Definition Audio)", "1 - LG FULL HD (3- AMD High Definition Audio Device)", "Sharkoon Headset"}; //16 Caracteres
  bool tempStatus[] = {true, false, false};
  int tempDataSize = sizeof(tempData) / sizeof(tempData[0]) - 1;

  while(true){
    display.clearDisplay();
    display.setTextWrap(false);

    cursor = cursor > tempDataSize ? cursor = 0 : cursor;
    cursor = cursor < 0 ? cursor = tempDataSize : cursor;
    

    //Cabeçalho
    centerText("Disp. de Saida", 1);
    display.drawLine(0, 9, 127, 9, WHITE);

    //Item anterior
    display.drawCircle(4, 18, 4, WHITE);
    display.setCursor(12, 15);
    int previousCursor = (cursor + 1) > tempDataSize ? 0 : cursor + 1;
    display.println(tempData[previousCursor].substring(0,16));
    if(tempStatus[previousCursor]){
      display.fillCircle(4, 18, 2, WHITE);
    }

    //Atual item
    display.setCursor(18, 32);
    display.println(tempData[cursor].substring(0,16));
    display.drawCircle(9, 35, 4, WHITE);
    display.drawBitmap(1, 27, draw_volumeBorderSeletor_122_18, 122, 18, WHITE);
    if(tempStatus[cursor]){
      display.fillCircle(9, 35, 2, WHITE);
    }

    //Proximo item
    display.drawCircle(4, 54, 4, WHITE);
    display.setCursor(12, 51);
    int nextCursor = (cursor - 1) < 0 ? tempDataSize : cursor - 1;
    display.println(tempData[nextCursor].substring(0,16));
    if(tempStatus[nextCursor]){
      display.fillCircle(4, 54, 2, WHITE);
    }

    //Scroll Bar
    display.setCursor(126, 12);
    for(int i = 0; i <= 17; i++){ 
      display.fillRect(display.getCursorX(), display.getCursorY(), 1, 1, WHITE);
      display.setCursor(display.getCursorX(), display.getCursorY()+3);
    }    
    int scrollSize = 56/(tempDataSize+1);           // Area util / Quantidade de itens     
    int scrollSPosition = 10+((cursor)*scrollSize); // Desconsidera o valor do cabeçalho
    display.fillRoundRect(125, scrollSPosition, 3, scrollSize, 1, WHITE);  
    

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
        screenMainMenu(1); 
      }      

      //Presionado curto
      //screenVolumeValue(tempData[cursor]);
    }

    display.display();
  }
} 
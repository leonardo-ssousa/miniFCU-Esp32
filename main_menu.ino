void screenMainMenu(){
  display.clearDisplay();
  encoderIsValue = false;
  cursor = 0;

  const unsigned char* icons[] = {icon_volume_20_20, icon_speaker_device_20_20, icon_plane_20_20, icon_settings_20_20};
  String titles[] = {"Volume Mixer", "Dispositivo de Saida", "Flight Simulator", "Configuracoes"};



  int sizeTitle = (sizeof(titles) / sizeof(titles[0])) - 1;
  while(true){
    display.clearDisplay();

    //Backgrounds Icons
    display.fillRoundRect(45, 0, 38, 38, 2, WHITE);
    display.drawRoundRect(0, 4, 30, 30, 1, WHITE);
    display.drawRoundRect(98, 4, 30, 30, 1, WHITE);

    cursor = cursor > sizeTitle ? cursor = 0 : cursor;
    cursor = cursor < 0 ? cursor = sizeTitle : cursor;
    Serial.println(cursor);

    //Previous icon
    int previousCursor = (cursor - 1) < 0 ? sizeTitle : cursor - 1;
    display.drawBitmap(5, 9, icons[previousCursor], 20, 20, WHITE);

    //Current icon
    display.drawBitmap(54, 9, icons[cursor], 20, 20, BLACK);

    //Next icon
    int nextCursor = (cursor + 1) > sizeTitle ? 0 : cursor + 1;
    display.drawBitmap(103, 9, icons[nextCursor], 20, 20, WHITE);


    //Print title
    display.setTextColor(WHITE);
    display.setTextSize(1);
    centerText(titles[cursor], 55);

    display.display();
  }


  //Selector
  // display.fillCircle(69, 45, 1, WHITE);
  // display.fillCircle(63, 45, 1, WHITE);
  // display.fillCircle(57, 45, 1, WHITE);
  // display.drawRect(62, 44, 3, 3, WHITE);

}
















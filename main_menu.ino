void screenMainMenu(){
  display.clearDisplay();
  
  String title = "Dispositivo de Saida";

  //Backgrounds Icons
  display.drawBitmap(46, 0, bgBigIcon_36_36, 36, 36, WHITE);
  display.drawBitmap(9, 4, bgSmallIcon_28_28, 28, 28, WHITE);
  display.drawBitmap(91, 4, bgSmallIcon_28_28, 28, 28, WHITE);

  //Draw Icons
  display.drawBitmap(13, 10, icon_volume_20_16, 20, 16, BLACK);
  display.drawBitmap(54, 10, icon_speaker_device_19_16, 19, 16, BLACK);
  display.drawBitmap(97, 10, icon_plane_16_16, 16, 16, BLACK);


  //Print title
  display.setTextColor(WHITE);
  display.setTextSize(1);
  centerText(title, 57);


  //Selector
  display.fillCircle(69, 45, 1, WHITE);
  display.fillCircle(63, 45, 1, WHITE);
  display.fillCircle(57, 45, 1, WHITE);
  display.drawRect(62, 44, 3, 3, WHITE);

  display.display();
}
















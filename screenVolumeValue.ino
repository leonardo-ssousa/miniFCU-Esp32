void screenVolumeValue(String title){
  encoderIsValue = true;
  encoderValue = 75;

  while(true){
    display.clearDisplay();

    display.drawBitmap(0, 0, bg_screenVolumeValue, 128, 64, WHITE);
    display.fillRect(14, 22, encoderValue, 9, WHITE);

    display.setFont(&muHeavy8ptBold);
    display.setCursor(13, 45);
    display.print(String(title));
    display.setFont();

    display.setCursor(13, 50);
    display.print(encoderValue);

    display.setCursor(28, 50);
    display.print(" %");

    display.display();

    if(!digitalRead(BTN)){
    awaitReleaseButton();
    screenVolumeMixer();
    }

  }
}
void screenVolumeValue(String title){
  encoderIsValue = true;  
  debounceEncoder = 100;
  encreaseValue = 1;
  encoderValue = 50;
  

  while(true){
    display.clearDisplay();

    encoderValue = encoderValue > 100 ? 100 : encoderValue;
    encoderValue = encoderValue < 0 ? 0 : encoderValue;

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
    segmentDisplayShow(encoderValue, 0);
    // shiftOut(shiftRegisterDataPin, shiftRegisterClockPin, LSBFIRST, 0b00000000);
    // shiftOut(shiftRegisterDataPin, shiftRegisterClockPin, LSBFIRST, 0b00000000);
    // shiftOut(shiftRegisterDataPin, shiftRegisterClockPin, LSBFIRST, 0b00000000);
    // shiftOut(shiftRegisterDataPin, shiftRegisterClockPin, LSBFIRST, 0b00000000);
    // digitalWrite(shiftRegisterSendPin, LOW);
    // digitalWrite(shiftRegisterSendPin, HIGH);

    if(!digitalRead(BTN)){
    awaitReleaseButton();
    screenVolumeMixer();
    }

  }
}
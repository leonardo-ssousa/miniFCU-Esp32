void screenSleepMode(){

  for(int i = 3; i >= 0; i--){
    display.clearDisplay();
    display.setTextSize(1);
    centerText("Dormindo em:", 14);
    display.setTextSize(2);
    display.setCursor(59, 29);
    display.println(i);
    display.display();
    delay(1000);
  }

  Serial.println("Sleep Mode: ON");
  isSleeping = true;
  
  while(isSleeping){
    display.clearDisplay();
    segmentDisplayClear();
    display.display();
  }

  segmentDisplayHour();

}
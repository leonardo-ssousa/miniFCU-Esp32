int centerText(String text, int line){
  int textSize = (text.length()*5)+(text.length()-1);

  display.setCursor((display.width() - textSize)/2, line);
  display.println(text);

  return (display.width() - textSize)/2;
}

void awaitReleaseButton(){
  while(!digitalRead(BTN)){
    ESP.wdtFeed(); // Reinicia o watchdog timer
  }
}

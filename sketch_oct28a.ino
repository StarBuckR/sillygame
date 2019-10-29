#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define btnRIGHT 0
#define btnUP 1
#define btnDOWN 2
#define btnLEFT 3
#define btnSELECT 4
#define btnNONE 5

int adcKey = 0;
int lcdKey = 0;

char screenUp[30];
char screenDown[30];
int charPos = 2;
int tempPos = 2;

char gameCharacter = '|';
char enemy = '-';

int timeCounter = 0;
int enemyTimeCounter = 0;

int readButtons(){
  adcKey = analogRead(0);
  if(adcKey > 1000) return btnNONE;
  if(adcKey < 50) return btnRIGHT;
  if(adcKey < 195) return btnUP;
  if(adcKey < 380) return btnDOWN;
  if(adcKey < 555) return btnLEFT;
  if(adcKey < 790) return btnSELECT;
  return btnNONE;
}

void(* resetFunc) (void) = 0;

void gameOver(){
  lcd.setCursor(0, 0);
  lcd.print("    Game        ");
  lcd.setCursor(0, 1);
  lcd.print("        Over    ");
  while(true){
    lcdKey = readButtons();
    if(lcdKey != btnNONE)
      resetFunc();
    delay(100);
  }
}

void getButton(){
  lcdKey = readButtons();
  tempPos = charPos;
  
  switch(lcdKey){
    case btnRIGHT:
      if(charPos < 30 && charPos != 15){
        if(screenUp[charPos + 1] == enemy || screenDown[charPos - 15 + 1] == enemy)
          gameOver();
        charPos++;
      }
      break;
    case btnLEFT:
      if(charPos > 1 && charPos != 16){
        if(screenUp[charPos - 1] == enemy || screenDown[charPos - 15 - 1] == enemy)
          gameOver();
        charPos--;
      }
      break;
    case btnDOWN:
      if(charPos <= 15){
        if(screenDown[charPos] == enemy)
          gameOver();
        charPos += 15;
      }
      break;
    case btnUP:
      if(charPos >= 15){
        if(screenUp[charPos - 15] == enemy)
          gameOver();
        charPos -= 15;
      }
      break;
  }
}

void characterMovement(){
  if(tempPos != charPos){
    if(charPos < 16){
      if(tempPos > 15){
        if(charPos == 16)
          screenDown[tempPos - 16] = ' ';
        else
          screenDown[tempPos - 15] = ' ';
        lcd.setCursor(0, 1);
        lcd.print(screenDown);
      }
      else
        screenUp[tempPos] = ' ';
      screenUp[charPos] = gameCharacter;
      lcd.setCursor(0, 0);
      lcd.print(screenUp);
    }
    else{
      if(tempPos < 16){
        screenUp[tempPos] = ' ';
        lcd.setCursor(0, 0);
        lcd.print(screenUp);
      }
      else
        screenDown[tempPos - 15] = ' ';
        screenDown[charPos - 15] = gameCharacter;
      lcd.setCursor(0, 1);
      lcd.print(screenDown);
    }
  }
}

void scrollMap(){
  if(timeCounter == 3){
      // TODO game scroll code here
      for(int i = 0; i < 16; i++){
        if((screenDown[i + 1] == enemy && screenDown[i] == gameCharacter) || (screenUp[i + 1] == enemy && screenUp[i] == gameCharacter)){
          gameOver();  
        }
        if(screenUp[i] != gameCharacter && screenDown[i] != gameCharacter){
          if(screenUp[i + 1] == gameCharacter){
            screenUp[i] = ' ';
            screenDown[i] = screenDown[i + 1];
          }
          else if(screenDown[i + 1] == gameCharacter){
            screenDown[i] = ' ';
            screenUp[i] = screenUp[i + 1];
          }
          else{
            screenUp[i] = screenUp[i + 1];
            screenDown[i] = screenDown[i + 1];
          }
        }else{
          if(screenUp[i] == gameCharacter){
            screenDown[i] = screenDown[i + 1]; 
          }
          else{
            screenUp[i] = screenUp[i + 1];
          }
        }
      }
      
      lcd.setCursor(0, 0);
      lcd.print(screenUp);
      lcd.setCursor(0, 1);
      lcd.print(screenDown);
      enemyTimeCounter++;
      timeCounter = 0;   
  }
  else
    timeCounter++;
}

void spawnEnemy(){
  if(enemyTimeCounter == 5){
      // TODO enemy spawn time here
      if(random(0, 2) == 1){
        screenDown[15] = enemy;
      }
      else{
        screenUp[15] = enemy;
      }
      enemyTimeCounter = 0;
  }
}

void setup() {
  // put your setup code here, to run once:
  digitalWrite(30, HIGH);
  lcd.begin(16,2);
  lcd.setCursor(0, 0);
  for(int i = 0; i < 17; i++){
    screenUp[i] = ' ';
    screenDown[i] = ' ';
  }
  screenUp[15] = enemy;
  screenUp[2] = gameCharacter;
  lcd.print(screenUp);
}

void loop() {
  // put your main code here, to run repeatedly:
  getButton();
  characterMovement();
  scrollMap();
  spawnEnemy();
  delay(150);
}

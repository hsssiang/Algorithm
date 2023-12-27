const int BUTTON_PIN = 8; // 按鍵的接腳
const int LED_PIN = 7; // LED燈的接腳
int buttonState = 0; // 按鈕的狀態
int flag = 0;

void setup() {
pinMode(LED_PIN, OUTPUT); //設定LED的PIN腳為輸出
pinMode(BUTTON_PIN, INPUT); //設定按鈕的接腳為輸入}
flag = 0;
Serial.begin(9600);
}

void loop(){
  Serial.println(digitalRead(BUTTON_PIN));
  /*
  Serial.println("buttom detected(closed), please wait for LED");
  delay(10000);
  Serial.println("you can push the buttom");
    do{
      digitalWrite(LED_PIN, LOW);
      delay(200);
      Serial.println("finish close");
    }while(digitalRead(BUTTON_PIN) == LOW);

    Serial.println("buttom detected(open), please wait for LED");
    delay(10000);
    Serial.println("you can push the buttom");
    do{
      digitalWrite(LED_PIN, HIGH); //LED就亮了
      delay(200);
      digitalWrite(LED_PIN, LOW); //LED就不亮
      delay(200);
      Serial.println("finish on");
    }while(digitalRead(BUTTON_PIN) == LOW);
  */
}

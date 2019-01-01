#include "Keyboard.h"
#include "Mouse.h"



#define Serial_OUT Serial1 //"Serial1" for external 0 1 port, "Serial" for internal USB serial port.
#define BUFFSIZE 100

int incomingByte = 0;   // for incoming Serial_OUT data
char buff[BUFFSIZE+1];
char para[BUFFSIZE-1];
char paras[3][BUFFSIZE-1];
int buff_c=0;

void setup() {
   Serial_OUT.begin(9600);     // opens Serial_OUT port, sets data rate to 9600 bps
   while (!Serial_OUT) ;
   Keyboard.begin();
   Mouse.begin();
}

int parse_exec(char* buff){
  memcpy(para,&buff[3], sizeof(para));
  if(buff[0] == 'k'){
    Serial_OUT.print(F("Key"));
    if(buff[1] == 'd'){
      int keycode=atoi(para);
      Serial_OUT.print(F("Down       Code:"));
      Serial_OUT.print(keycode);
      Keyboard.press(keycode);
    }
    else if(buff[1] == 'u'){
      int keycode=atoi(para);
      Serial_OUT.print(F("Up         Code:"));
      Serial_OUT.print(keycode);
      Keyboard.release(keycode);
    }
    else if(buff[1] == 'c'){
      int keycode=atoi(para);
      Serial_OUT.print(F("Click      Code:"));
      Serial_OUT.print(keycode);
      Keyboard.write(keycode);
    }
    else if(buff[1] == 'U'){
      Serial_OUT.print(F("Up         Rlsd:"));
      Keyboard.releaseAll();
      Serial_OUT.print("All");
    }
    else if(buff[1] == 'p'){
      Serial_OUT.print(F("Print      Stri:"));
      Serial_OUT.print(para);
      Keyboard.print(para);
    }
    else if(buff[1] == 'P'){
      Serial_OUT.print(F("Println    Stri:"));
      Serial_OUT.print(para);
      Keyboard.println(para);
    }
    else{
      Serial_OUT.print(F("               :Invalid Operation"));
    }
  }
  else if(buff[0] == 'm'){
//MOUSE_LEFT 1
//MOUSE_RIGHT 2
//MOUSE_MIDDLE 4
    Serial_OUT.print(F("Mouse"));
    if(buff[1] == 'd'){
      int keycode=atoi(para);
      Serial_OUT.print(F("Down     Code:"));
      Serial_OUT.print(keycode);
      Mouse.press(keycode);
    }
    else if(buff[1] == 'u'){
      int keycode=atoi(para);
      Serial_OUT.print(F("Up       Code:"));
      Serial_OUT.print(keycode);
      Mouse.release(keycode);
    }
    else if(buff[1] == 'c'){
      int keycode=atoi(para);
      Serial_OUT.print(F("Click    Code:"));
      Serial_OUT.print(keycode);
      Mouse.click(keycode);
    }
    else if(buff[1] == 'D'){
      int keycode=atoi(para);
      Serial_OUT.print(keycode);
      Serial_OUT.print(F("Stat    Resu:"));
      int mouseState=Mouse.isPressed();
      Serial_OUT.print(mouseState);
      
    }
    else if(buff[1] == 'm'){
      int p_c = 0;
      int p_i = 0;
      paras[0][0] = '\x00';
      paras[1][0] = '\x00';
      paras[2][0] = '\x00';
      for(int m_c = 0; m_c < BUFFSIZE-1 && p_i <= 2 ; m_c++){
        if(para[m_c] != ' ' && para[m_c] != '\x00' ){
          paras[p_i][p_c] = para[m_c];
          p_c += 1;
        }
        else{
          paras[p_i][p_c] = '\x00';
          p_c = 0;
          p_i += 1;
        }
        //Serial_OUT.print(p_c);
        //Serial_OUT.println(p_i);
      }
      int mx = atoi(paras[0]);
      int my = atoi(paras[1]);
      int mw = atoi(paras[2]);

      Serial_OUT.print(F("Move     Cord:"));
      Serial_OUT.print(F(" X:"));
      Serial_OUT.print(mx);
      Serial_OUT.print(F(" Y:"));
      Serial_OUT.print(my);
      Serial_OUT.print(F(" Wheel:"));
      Serial_OUT.print(mw);

      Mouse.move(mx, my, mw);
    }
    else{
      Serial_OUT.print(F("               :Invalid Operation"));
    }
  }
  else{
    Serial_OUT.print(F("Invalid device"));
    Serial_OUT.flush();
  }
  Serial_OUT.println("");

  
  return 0;
}

void loop() {
    memset(buff,0,sizeof(buff));
    //Serial_OUT.println("waiting");
    if (Serial_OUT.available() > 0) {
      buff_c=0;
      while(1){
        if (Serial_OUT.available() > 0){
          incomingByte = Serial_OUT.read();
          Serial_OUT.write(incomingByte);
          if(incomingByte==10 || incomingByte==13){
            break;
          }
          buff[buff_c] = ((incomingByte==10 || incomingByte==13)?'\x00':incomingByte);
          buff_c += 1;
        }
        if(buff_c >= BUFFSIZE){
          break;
        }
      }
      parse_exec(buff);

    }

}

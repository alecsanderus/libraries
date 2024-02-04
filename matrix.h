#include "HardwareSerial.h"
#include "Arduino.h"
#pragma once
#include <string.h>
class matrix {
    
public:

  matrix (int _latchPin ,int _clockPin ,int _dataPin)
  {
    latchPin=_latchPin;
    clockPin=_clockPin;
    dataPin=_dataPin;
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    tim = millis();
  }


  

  void In_Loop ()//вызывается в loop
  {
    if (mode == 1 && millis() > tim + sped)
      {
  
        tim = millis();
        for (int i = 0;i < 8;i++){
          if (naprav == 0)
          {
          sost[0][i]  >>= 1;
          sost[1][i] >>= 1;
          }
           if (naprav == 1)
          {
          sost[0][i]  <<= 1;
          sost[1][i] <<= 1;
          }
        }
        
      for (int i = 0;i < 8;i++) {  
         if (naprav == 0)   
         {
        if (clr == 0 || clr == 2) bitWrite (sost[0][i], 7, bitRead(stroc[((int)TecEl/7)*8+i],TecEl%7));
        if (clr == 1 || clr == 2) bitWrite (sost[1][i], 7, bitRead(stroc[((int)TecEl/7)*8+i],TecEl%7));
         }
          if (naprav == 1)   
         {
        if (clr == 0 || clr == 2) bitWrite (sost[0][i], 0, bitRead(stroc[((int)TecEl/7)*8+i],8-(TecEl%7)));
        if (clr == 1 || clr == 2) bitWrite (sost[1][i], 0, bitRead(stroc[((int)TecEl/7)*8+i],8-(TecEl%7)));
         }
      }
      
        TecEl ++;
      if (TecEl >= longg - 3)
        TecEl = 0;
      }


    byte sss = B10000000;
    for (int i = 0;i < 8; i++)
    {
       // устанавливаем синхронизацию "защелки" на LOW
    digitalWrite(latchPin, LOW);
   
    shiftOut(dataPin, clockPin, MSBFIRST, ~sost[0][i]); 
    
    shiftOut(dataPin, clockPin, MSBFIRST, sss);

    shiftOut(dataPin, clockPin, MSBFIRST, ~sost[1][i]);
    //"защелкиваем" регистр, тем самым устанавливая значения на выходах
    digitalWrite(latchPin, HIGH);
    sss = sss >> 1;

    }
  }
    SetPicture(byte Picture [2][8]) // показывает изображение из масива
    {
      mode = 0;
      if (longg != 0)
        clear();
      for (int i = 0;i < 8;i++)
      {
        sost[0][i] = Picture[0][i];
        sost[1][i] = Picture[1][i];        
      }        
    }

    SetImage(int number , int color)  //показывает изображение из встроеной библиотеки
    {
      mode = 0;
      if (longg != 0)
        clear();
       for (int i = 0;i < 8;i++)
       {
        if (color == 0 || color == 2)   sost[0][i] = images[number][i];
        if (color == 1 || color == 2)   sost[1][i] = images[number][i];
       }   
    }

    
    void SetStroka (byte *Stroka , int Stroka_Long) //устанавливает строку
  {
    mode = 1;    
    clear();
    longg = Stroka_Long;
    stroc = Stroka;
    
  }
    void AddStrokImage (int number)//добавляет к строке изображение из встроеной библиотеки
    {
      mode=1;
      byte *fff = new byte [8];
      for (int i = 0;i < 8;i++)
        fff[i] = images[number][i];
      AddStrokPicture(fff,8);
    }
    

    void AddStrokPicture (byte *Stroka , int Stroka_Long)//добавляет к строке изображение из масива
    {
      mode = 1;
      byte *fff = new byte [longg + Stroka_Long];
      for (int i = 0;i < longg + Stroka_Long;i++)
      {        
        if (i < longg) fff[i] = stroc[i];
        else  fff[i] = Stroka[i - longg];
       
      }
     // clear();
      rvny(stroc,fff,longg + Stroka_Long);
      longg = longg + Stroka_Long;
      delete [] Stroka;

   }


   void SetStrocSettings(int _color, int _speed = 500, int napravlenie = 1)
   {
    clr = _color;
    sped = _speed;
    naprav = napravlenie;
   }
  
private:
int TecEl = 0;
float sped = 500;
float tim;
void clear ()
{
  delete [] stroc;

}


void crreat()
{
//  clear();
  byte *fff = new byte [longg];
  
  rvny (stroc,fff,longg);
}

void rvny (byte *a,byte *b,int llnngg)
{
  for (int i = 0;i < llnngg;i++)
  {
    a[i] = b[i];
  }
  delete [] b;

}
int naprav = 1;
int longg = 0;
int mode = 0;
int clr = 0;
 //Пин подключен к ST_CP входу 74HC595
int latchPin = 8;
//Пин подключен к SH_CP входу 74HC595
int clockPin = 12;
//Пин подключен к DS входу 74HC595
int dataPin = 11;
byte sost [2][8];
byte *stroc;
byte images[10][8]
{
 {B01111100,
	B11000110,
	B11001110,
	B11011110,
	B11110110,
	B11100110,
	B01111100,
	B00000000},


  {B00110000,
	B01110000,
	B00110000,
	B00110000,
	B00110000,
	B00110000,
	B00110000,
	B00000000},


 {B01111000,
	B11001100,
	B00001100,
	B00111000,
	B01100000,
	B11000000,
	B11111100,
	B00000000},

 {B01111000,
	B11001100,
	B00001100,
	B00111000,
	B00001100,
	B11001100,
	B01111000,
	B00000000},

 {B00011100,
	B00111100,
	B01101100,
	B11001100,
	B11111110,
	B00001100,
	B00001100,
	B00000000},

 {B11111100,
	B11000000,
	B11111000,
	B00001100,
	B00001100,
	B11001100,
	B01111000,
	B00000000},

 {B00111000,
	B01100000,
	B11000000,
	B11111000,
	B11001100,
	B11001100,
	B01111000,
	B00000000},

 {B11111100,
	B00001100,
	B00001100,
	B00011000,
	B00110000,
	B00110000,
	B00110000,
	B00000000},

 {B01111000,
	B11001100,
	B11001100,
	B01111000,
	B11001100,
	B11001100,
	B01111000,
	B00000000},

 {B01111000,
	B11001100,
	B11001100,
	B01111100,
	B00001100,
	B00011000,
	B01110000,
	B00000000}
};


};

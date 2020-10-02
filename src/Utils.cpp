#include "Utils.h"
#include <Arduino.h>
//#if not defined ESP8266 && not defined ESP32
#ifdef PMIC_PRESENT
byte readRegister(byte address) {
    Wire.beginTransmission(PMIC_ADDRESS);
    Wire.write(address);

    if (Wire.endTransmission(true) != 0) {
      return -1;
    }

    if (Wire.requestFrom(PMIC_ADDRESS, 1, true) != 1) {
      return -1;
    }

    return Wire.read();
}


void batteryBegin()
{
  Wire.begin();
   //check PMIC version
  if (readRegister(PMIC_VERSION_REGISTER) != 0x23) {
      Serial.println("[ERROR] PMIC NOT COMPATIBLE STOPPING");
      while(true);
  }
}

#endif

String ToHex(String x)
{
  String hex;
  for(int i=0;i<x.length();++i)
    hex+=ToHexT(x[i]);
  return hex;
}

uint8_t hexBinary(char msb, char lsb)
{
  auto hexToDec = [](char x)
  {
    if(x>='a' && x<='f') return 10 + (x-'a');
    else return x-'0';
  };

  return (hexToDec(msb)<<4)|hexToDec(lsb); 
}

String to7bit(String inputString)
{
  String all;
  for (int i = 0; i < inputString.length(); ++i)
  {
      char byte = inputString[i];
      for (int j = 6; j >= 0; --j)
          all += ((byte >> j) & 1) == 1 ? '1' : '0';
  }
  int size = (int)ceil(all.length() / 7.);
  String* septets = new String[size];
  for (int i = 0; i < size; ++i)
  {
      for (int j = 0; j < 7; ++j)
          if (i * 7 + j < all.length()) septets[i] += all[i * 7 + j];
  }
  String converted;
  String toSteal = "";
  int stealSize = 1;
  int index = 0;
  for (int i = 0; i < size;++i)
  {
      if (i + 1 == size)
      {
          while (septets[i].length() < 8)
              septets[i] = '0'+septets[i];
      }
      else
      {
          toSteal = septets[i + 1].substring(7 - stealSize);
          septets[i + 1] = septets[i + 1].substring(0, 7 - stealSize);
          septets[i] = toSteal + septets[i];
      }
      uint8_t num = 0;
      for (int j = 0; j < 8; ++j)
      {
          num <<= 1;
          num |= septets[i][j] == '1' ? 1 : 0;
      }
      converted += ToHexT(num);
      if (stealSize == 7)
      {
          stealSize = 1;
          toSteal = "";
          i++;
      }
      else
      {
          stealSize++;
      }
  }
  return converted;
}

String from7bit(String inputString)
{
  String all;
  for (int i = 1; i<inputString.length(); i+=2)
    {
        String num; num += inputString[i- 1]; num += inputString[i];
        uint8_t byte = strtol(num.c_str(), nullptr, 16);
        for (int i = 7; i >= 0; --i)
            all += ((byte >> i) & 1) == 1 ? '1' : '0';
    }

    String converted;
    String toSteal = "";
    int stealSize = 1;
    int index = 0;
    while(true)
    {
      if (index >= all.length())break;
      String number = all.substring(index + stealSize < all.length()? index+stealSize : all.length()-1, (index + stealSize < all.length()? index+stealSize : all.length()-1) +(8 - stealSize));
      number += toSteal;
      number = '0' + number;

      toSteal = all.substring(index, index + stealSize);

      uint8_t num = 0u;
      for (int i = 0; i <8; ++i)
      {
          num <<= 1;
          num |= number[i] == '1' ? 1 : 0;
      }
      converted += char(num);

      if (stealSize == 8)
      {
          stealSize = 1;
          toSteal = "";
      }
      else
      {
          stealSize++;
          index += 8;
      }
    }
    Serial.println(converted);
  return converted;
}

//30180C7633168533D90C3693CD6032E20C36C3C98833D82C56A3CD6233D92C3883CD60B3E00C3683D582B0180C0683C16030180C0683C16030180C068301


/*

0110000 0110000 0110000 0110000 0111000 0110101 0111001


00110000  00110000  00110000  00110000 00111000 00110101 00111001


00110000 00011000 00001100 10000110 10101011 1100101 0100000


*/
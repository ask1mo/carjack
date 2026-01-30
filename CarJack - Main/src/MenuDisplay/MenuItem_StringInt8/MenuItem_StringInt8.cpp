#include "MenuItem_StringInt8.h"

//Constructor
MenuItem_StringInt8::MenuItem_StringInt8(const char* title, const unsigned char *sprite, void(*entryFunction)(), String entryDialog, uint8_t *data, const char** dataStrings, const unsigned char** dataBitmaps)
{
  //Serial.print(F("Constructing derived (MenuItem_StringInt8): "));
  //Serial.println(title);
  signature = SIGNATURE_SRINGINT;
  this->title = title;
  this->sprite = sprite;
  this->entryFunction = entryFunction;
  this->entryDialog = entryDialog;
  this->data = data;
  this->dataStrings = dataStrings;
  this->dataBitmaps = dataBitmaps;
  
  selected = false;
}

//Public
String MenuItem_StringInt8::toText()
{
  String text = title;
  text += ": ";
  text+= dataStrings[*data];
  return text;
}

const unsigned char* MenuItem_StringInt8::getDataSprite()
{
  //Temporary code
  switch (*data)
  {
    case 0:
    return sprite_slayermark;
    break;
    case 1:
    return sprite_discoball;
    break;
    case 2:
    return sprite_heart;
    break;
  }
  return sprite_error;
  
 
  //The actual code
  //return dataBitmaps[data];
}

void MenuItem_StringInt8::printName()
{
  Serial.print(F("1BOOL: Printing my title: "));
  Serial.print(title);
}
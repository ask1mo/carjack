#include "MenuItem_U8.h"

//Constructor
MenuItem_U8::MenuItem_U8(const char* title, const unsigned char *sprite, void(*entryFunction)(), String entryDialog, uint8_t *data)
{
  //Serial.print(F("Constructing derived (MenuItem_U8): "));
  //Serial.println(title);
  signature = SIGNATURE_U8;
  this->title = title;
  this->sprite = sprite;
  this->entryFunction = entryFunction;
  this->entryDialog = entryDialog;
  this->data = data;
  selected = false;
  //thingsSize = 0;
}

// MenuItem_U8::~MenuItem_U8()
// {
//   //delete things;
// }


//Public
void MenuItem_U8::toText(char* text) //Cast to strin and then cast to char?
{
  /*
  text[0] = '[';
  text[1] = (char)((String)data)[0];
  text[2] = ']';
  strcpy(&text[4], title);
  */
 text[0] = (char)((String)*data)[0];
}
const unsigned char* MenuItem_U8::getDataSprite()
{
  switch (*data)
  {
    case 0:
    return sprite_ledlevel0;
    break;
    case 1:
    return sprite_ledlevel1;
    break;
    case 2:
    return sprite_ledlevel2;
    break;
    case 3:
    return sprite_ledlevel3;
    break;
  }
  return sprite_error;
}

void MenuItem_U8::printName()
{
  Serial.print(F("UINT8: Printing my title: "));
  Serial.print(title);
}

uint8_t MenuItem_U8::getData()
{
  return *data;
}
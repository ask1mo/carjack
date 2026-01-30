#include "MenuItem_Float.h"

//Constructor
MenuItem_Float::MenuItem_Float(String title, const unsigned char *sprite, void(*entryFunction)(), String entryDialog, float *data)
{
  //Serial.print(F("Constructing derived (MenuItem_Float): "));
  //Serial.println(title);
  signature = SIGNATURE_FLOAT;
  this->title = title;
  this->sprite = sprite;
  this->entryFunction = entryFunction;
  this->entryDialog = entryDialog;
  this->data = data;
  selected = false;
  //thingsSize = 0;
}

// MenuItem_Float::~MenuItem_Float()
// {
//   //delete things;
// }


//Public
void MenuItem_Float::toText(char* text) //Cast to strin and then cast to char?
{
  /*
  text[0] = '[';
  text[1] = (char)((String)data)[0];
  text[2] = ']';
  strcpy(&text[4], title);
  */
 text[0] = (char)((String)*data)[0];
}
const unsigned char* MenuItem_Float::getDataSprite()
{
  return sprite_error;
}

void MenuItem_Float::printName()
{
  Serial.print(F("FLOAT: Printing my title: "));
  Serial.print(title);
}

float MenuItem_Float::getData()
{
  return *data;
}

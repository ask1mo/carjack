#include "MenuItem_U64.h"

//Constructor
MenuItem_U64::MenuItem_U64(const char* title, const unsigned char *sprite, void(*entryFunction)(), String entryDialog, uint64_t *data)
{
  //Serial.print(F("Constructing derived (MenuItem_U64): "));
  //Serial.println(title);
  signature = SIGNATURE_U64;
  this->title = title;
  this->sprite = sprite;
  this->entryFunction = entryFunction;
  this->entryDialog = entryDialog;
  this->data = data;
  selected = false;
  //thingsSize = 0;
}

// MenuItem_U64::~MenuItem_U64()
// {
//   //delete things;
// }


//Public
void MenuItem_U64::toText(char* text) //Cast to strin and then cast to char?
{
  /*
  text[0] = '[';
  text[1] = (char)((String)data)[0];
  text[2] = ']';
  strcpy(&text[4], title);
  */
 text[0] = (char)((String)*data)[0];
}
const unsigned char* MenuItem_U64::getDataSprite()
{
  return sprite_error;
}

void MenuItem_U64::printName()
{
  Serial.print(F("FLOAT: Printing my title: "));
  Serial.print(title);
}

uint64_t MenuItem_U64::getData()
{
  return *data;
}

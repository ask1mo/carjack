#include "MenuItem_Static.h"

//Constructor
MenuItem_Static::MenuItem_Static(const char* title, const unsigned char *sprite, void(*entryFunction)(), String entryDialog)
{
  //Serial.print(F("Constructing derived (MenuItem_Static): "));
  //Serial.println(title);
  signature = SIGNATURE_STATIC;
  this->title = title;
  this->sprite = sprite;
  this->entryFunction = entryFunction;
  this->entryDialog = entryDialog;
  selected = false;
  //thingsSize = 0;
}

// MenuItem_Static::~MenuItem_Static()
// {
//   //delete things;
// }


//Public
String MenuItem_Static::toText()
{
  String text = "-*- ";
  text += title;
}

const unsigned char* MenuItem_Static::getDataSprite()
{
  return sprite_MenuItemStatic;
}

void MenuItem_Static::printName()
{
  Serial.print(F("FOLDER: Printing my title: "));
  Serial.print(title);
}
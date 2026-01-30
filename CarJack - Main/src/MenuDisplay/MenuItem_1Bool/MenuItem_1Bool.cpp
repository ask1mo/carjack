#include "MenuItem_1Bool.h"

//Constructor
MenuItem_1Bool::MenuItem_1Bool(const char* title, const unsigned char *sprite, void(*entryFunction)(), String entryDialog, bool *data)
{
  //Serial.print(F("Constructing derived (MenuItem_1Bool): "));
  //Serial.println(title);
  signature = SIGNATURE_1BOOL;
  this->title = title;
  this->sprite = sprite;
  this->entryFunction = entryFunction;
  this->entryDialog = entryDialog;
  this->data = data;
  selected = false;
}

//Public
String MenuItem_1Bool::toText()
{
  String text = "[";

  if      (*data == 0)  text += "O";
  else if (*data == 1)  text += "X";
  else                  text += "?";
  
  text += "] ";
  text += title;
  
  return text;
}
const unsigned char* MenuItem_1Bool::getDataSprite()
{
  if (*data)
  {
    return sprite_MenuItem1Bool_true;
  }
  else 
  {
    return sprite_MenuItem1Bool_false;
  }
}

void MenuItem_1Bool::printName()
{
  Serial.print(F("1BOOL: Printing my title: "));
  Serial.print(title);
}
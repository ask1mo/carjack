#include "MenuItem_Bool.h"

//Constructor
MenuItem_Bool::MenuItem_Bool(const char* title, const unsigned char *sprite, void(*entryFunction)(), String entryDialog, bool *data)
{
  //Serial.print(F("Constructing derived (MenuItem_Bool): "));
  //Serial.println(title);
  signature = SIGNATURE_BOOL;
  this->title = title;
  this->sprite = sprite;
  this->entryFunction = entryFunction;
  this->entryDialog = entryDialog;
  this->data = data;
  selected = false;
}

//Public
String MenuItem_Bool::toText()
{
  String text = "[";

  if      (*data == 0)  text += " ";
  else if (*data == 1)  text += "X";
  else                  text += "?";
  
  text += "] ";
  text += title;

  return text;
}
const unsigned char* MenuItem_Bool::getDataSprite()
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

void MenuItem_Bool::printName()
{
  Serial.print(F("BOOL: Printing my title: "));
  Serial.print(title);
}

#include "MenuItem.h"

MenuItem::~MenuItem()
{
  Serial.print(F("DEstructing base (MenuItem): "));
  Serial.println(title);
}

//Private

//Public
uint8_t MenuItem::click()
{
  Serial.print(F("BASE: Click, signature: "));
  Serial.println(signature);
  if(entryFunction != NULL) entryFunction();
  return signature;
}
String MenuItem::getTitle()
{
  return title;
}
const unsigned char* MenuItem::getSprite()
{
  return sprite;
}

//added in esp32 race
const unsigned char* MenuItem::getDataSprite()
{
  return sprite_noSprite;
}

String MenuItem::toText()
{
  String text = "BSE ";
  text += title;

  return text;
}
void MenuItem::printName()
{
  Serial.print(F("BASE: Printing my title: "));
  Serial.print(title);
}
void MenuItem::printTitleAdress()
{
  //Serial.print(F("'Title' variable adress: "));
  //Serial.println((int)(&title));

  Serial.print(F("'Title' variable adress: "));
  Serial.print((int)(&title));

  Serial.print(F(" - "));
  Serial.println(title);
}
bool MenuItem::getSelected()
{
  return selected;
}
void MenuItem::setSelected(bool selected)
{
  this->selected = selected;
}

String MenuItem::getEntryDialogString()
{
  Serial.print(F("BASE: Returning entryDialog: "));
  return entryDialog;
}
void (*MenuItem::getEntryFunction())()
{
  return entryFunction;
}
uint8_t MenuItem::getSignature()
{
  return signature;
}
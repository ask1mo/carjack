#include "MenuItem_Folder.h"

//Constructor
MenuItem_Folder::MenuItem_Folder(const char* title, const unsigned char *sprite, void(*entryFunction)(), void(*exitFunction)(), String entryDialog, String exitDialogString)
{
  //Serial.print(F("Constructing derived (MenuItem_Folder): "));
  //Serial.println(title);
  signature = SIGNATURE_FOLDER;
  menuItems = nullptr;
  menuItemsLength = 0;
  this->title = title;
  this->sprite = sprite;
  this->entryFunction = entryFunction;
  this->exitFunction = exitFunction;
  this->entryDialog = entryDialog;
  this->exitDialogString = exitDialogString;
  parentMenuItem_Folder = NULL;
  lastIndex = 0;
  lastIndexOffset = 0;
  selected = false;
}
//Public
uint8_t MenuItem_Folder::clickContainedItem(uint8_t index)
{
  return menuItems[index]->click();
}
void MenuItem_Folder::addMenuItem(MenuItem *menuItem)
{
  /*
  Serial.print(F("Adding item to folder: "));
  Serial.print(title);
  Serial.print(F("/"));
  Serial.print(menuItem->getTitle());
  Serial.print(F("Adress: "));
  Serial.println((int)menuItem);
  */

  // Reallocate memory for menuItems
  MenuItem** newMenuItems = new MenuItem*[menuItemsLength + 1];

  // Copy old menuItems to newMenuItems
  for (int i = 0; i < menuItemsLength; i++) {
    newMenuItems[i] = menuItems[i];
  }

  // Add new menuItem to newMenuItems
  newMenuItems[menuItemsLength] = menuItem;

  // Delete old menuItems
  delete[] menuItems;

  // Point menuItems to newMenuItems
  menuItems = newMenuItems;

  menuItemsLength++;
}
uint8_t MenuItem_Folder::getMenuItemsLength()
{
  return menuItemsLength;
}
MenuItem_Folder* MenuItem_Folder::getSubFolder(uint8_t index)
{
  return (MenuItem_Folder*)menuItems[index];
}
MenuItem_Folder* MenuItem_Folder::getParentFolder()
{
  return parentMenuItem_Folder;
}
void MenuItem_Folder::setParentFolder(MenuItem_Folder *parentFolder)
{
  this->parentMenuItem_Folder = parentFolder;
}
String MenuItem_Folder::toText()
{
  String text = "-> ";
  text += title;
  return text;
}
const unsigned char* MenuItem_Folder::getDataSprite()
{
  return sprite_MenuItemFolder;
}
String MenuItem_Folder::getSubItemText(uint8_t index)
{
  if (index < menuItemsLength)
  {
    return menuItems[index]->toText();
  }
}
String MenuItem_Folder::getSubItemTitle(uint8_t index)
{
  if (index < menuItemsLength)
  {
    return menuItems[index]->getTitle();
  }
  return "";
}
const unsigned char* MenuItem_Folder::getSubItemSprite(uint8_t index)
{
  if (index < menuItemsLength)
  {
    if (menuItems[index]->getSprite() != NULL)
    {
      return menuItems[index]->getSprite();
    }
  }
  return sprite_smallEmpty;
}
const unsigned char* MenuItem_Folder::getSubItemDataSprite(uint8_t index)
{
  if (index < menuItemsLength)
  {
    return menuItems[index]->getDataSprite();
  }
  return sprite_smallEmpty;
}
bool MenuItem_Folder::getSubItemSelected(uint8_t index)
{
  if (index < menuItemsLength)
  {
    return menuItems[index]->getSelected();
  }
  return false;
}
void MenuItem_Folder::printFolders()
{
  Serial.print(F("Printing items contained in "));
  Serial.println(title);

  for (uint8_t i = 0; i < menuItemsLength; i++)
  {
    
    Serial.print(F("/"));
    Serial.print(title);
    Serial.print(F("/"));
    menuItems[i]->printName();
    Serial.println();
    
    Serial.println(F("PRINTIN PRINTIN PRINTIN"));
    String text = menuItems[i]->toText();
    Serial.println(text);
    //menuItems[i]->toText();
  }
  
}
void MenuItem_Folder::printItemAdresses()
{
  Serial.print(F("Printing adresses of items contained in "));
  Serial.println(title);

  for (uint8_t i = 0; i < menuItemsLength; i++)
  {
    
    Serial.println();
    Serial.print(F("Num"));
    Serial.print(i);
    Serial.print(F(": "));
    Serial.println((int)(&(*menuItems[i])));
    menuItems[i]->printTitleAdress();
  }
  
}
void MenuItem_Folder::printTitleAdress()
{
  Serial.print(F("'Title' variable adress: "));
  Serial.print((int)(&title));

  Serial.print(F(" - "));
  Serial.println(title);

  Serial.println(F("(Folder) Printing adresses of items contained:"));

  for (uint8_t i = 0; i < menuItemsLength; i++)
  {
    //Serial.println();
    Serial.print(F("Num"));
    Serial.print(i);
    Serial.print(F(": "));
    //Serial.println((int)(&(*menuItems[i])));
    menuItems[i]->printTitleAdress();
  }
}
void MenuItem_Folder::printName()
{
  Serial.print(F("FOLDER: Printing my title: "));
  Serial.print(title);
}
uint8_t MenuItem_Folder::getLastIndex()
{
  return lastIndex;
}
void MenuItem_Folder::setLastIndex(uint8_t lastIndex)
{
  this->lastIndex = lastIndex;
}
uint8_t MenuItem_Folder::getLastIndexOffset()
{
  return lastIndexOffset;
}
void MenuItem_Folder::setLastIndexOffset(uint8_t lastIndexOffset)
{
  this->lastIndexOffset = lastIndexOffset;
}
void MenuItem_Folder::setSubItemSelected(uint8_t index, bool selected)
{
  if (index < menuItemsLength)
  {
    menuItems[index]->setSelected(selected);
  }
}

void MenuItem_Folder::notifyDeselcted()
{
  if (exitFunction != NULL) exitFunction();
}

MenuItem *MenuItem_Folder::getSubItem(uint8_t index)
{
  if (index < menuItemsLength)
  {
    return menuItems[index];
  }
  return NULL;
}

String MenuItem_Folder::getExitDialogString()
{
  return exitDialogString;
}
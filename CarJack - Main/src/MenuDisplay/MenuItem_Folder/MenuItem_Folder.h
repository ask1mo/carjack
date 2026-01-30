#ifndef MENUITEM_FOLDER_H
#define MENUITEM_FOLDER_H

#include "../MenuItem/MenuItem.h"
#include "../Sprites.h"
#include <Arduino.h>
#include <stdlib.h>

class MenuItem_Folder : public MenuItem
{
    private:
    MenuItem **menuItems;
    MenuItem_Folder *parentMenuItem_Folder;
    uint8_t menuItemsLength;
    uint8_t lastIndex;
    uint8_t lastIndexOffset; //Shouldn't really be here but I'm running out of time and patience
    void(*exitFunction)();
    String exitDialogString;
    

    public:
    MenuItem_Folder(const char* title, const unsigned char *sprite, void(*entryFunction)(), void(*exitFunction)(), String entryDialog, String exitDialogString);
    ~MenuItem_Folder();
    uint8_t clickContainedItem(uint8_t index);
    void addMenuItem(MenuItem *menuItem);
    uint8_t getMenuItemsLength();
    MenuItem_Folder* getSubFolder(uint8_t index);
    MenuItem_Folder* getParentFolder();
    void setParentFolder(MenuItem_Folder *parentFolder);
    String toText();
    String getSubItemText(uint8_t index);
    String getSubItemTitle(uint8_t index);
    const unsigned char* getSubItemSprite(uint8_t index);
    const unsigned char* getSubItemDataSprite(uint8_t index);
    bool getSubItemSelected(uint8_t index);
    const unsigned char* getDataSprite();
    void printFolders();
    void printItemAdresses();
    void printTitleAdress();
    void printMenu();
    void printName();
    uint8_t getLastIndex();
    void setLastIndex(uint8_t lastIndex);
    uint8_t getLastIndexOffset();
    void setLastIndexOffset(uint8_t lastIndexOffset);
    void setSubItemSelected(uint8_t index, bool selected);
    void notifyDeselcted();
    MenuItem *getSubItem(uint8_t index);
    String getExitDialogString();
};

#endif
#ifndef MENUITEM_1BOOL_H
#define MENUITEM_1BOOL_H

//#include "./Menu/MenuItem/MenuItem.h"
#include "../MenuItem/MenuItem.h"

class MenuItem_1Bool : public MenuItem
{
    private:
    bool *data;
    

    public:
    MenuItem_1Bool(const char* title, const unsigned char *sprite, void(*entryFunction)(), String entryDialog, bool *data);
    ~MenuItem_1Bool();
    String toText();
    const unsigned char* getDataSprite();
    void printName();
};

#endif
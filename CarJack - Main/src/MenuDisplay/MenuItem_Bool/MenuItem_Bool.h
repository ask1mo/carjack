#ifndef MENUITEM_BOOL_H
#define MENUITEM_BOOL_H

//#include "./Menu/MenuItem/MenuItem.h"
#include "../MenuItem/MenuItem.h"

class MenuItem_Bool : public MenuItem
{
    private:
    bool *data;
    

    public:
    MenuItem_Bool(const char* title, const unsigned char *sprite, void(*entryFunction)(), String entryDialog, bool *data);
    ~MenuItem_Bool();
    String toText();
    const unsigned char* getDataSprite();
    void printName();
};

#endif
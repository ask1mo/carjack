#ifndef MENUITEM_STATIC_H
#define MENUITEM_STATIC_H

//#include "./Menu/MenuItem/MenuItem.h"
#include "../MenuItem/MenuItem.h"

class MenuItem_Static : public MenuItem
{
    public:
    MenuItem_Static(const char* title, const unsigned char *sprite, void(*function)(), String entryDialog);
    ~MenuItem_Static();
    String toText();
    const unsigned char* getDataSprite();
    void printName();
};

#endif
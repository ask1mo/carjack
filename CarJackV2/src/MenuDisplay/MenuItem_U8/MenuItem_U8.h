#ifndef MENUITEM_U8_H
#define MENUITEM_U8_H

//#include "./Menu/MenuItem/MenuItem.h"
#include "../MenuItem/MenuItem.h"

class MenuItem_U8 : public MenuItem
{
    private:
    uint8_t *data;
    

    public:
    MenuItem_U8(const char* title, const unsigned char *sprite, void(*function)(), String entryDialog, uint8_t *data);
    ~MenuItem_U8();
    void toText(char* text);
    const unsigned char* getDataSprite();
    void printName();
    uint8_t getData();
};

#endif
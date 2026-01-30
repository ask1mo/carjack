#ifndef MENUITEM_STRINGINT_H
#define MENUITEM_STRINGINT_H

#include "../MenuItem/MenuItem.h"

class MenuItem_StringInt8 : public MenuItem
{
    private:
    uint8_t *data;
    const char** dataStrings;
    const unsigned char** dataBitmaps;
    

    public:
    MenuItem_StringInt8(const char* title, const unsigned char *sprite, void(*function)(), String entryDialog, uint8_t *data, const char** dataStrings, const unsigned char** dataBitmaps);
    ~MenuItem_StringInt8();
    String toText();
    const unsigned char* getDataSprite();
    void printName();
};

#endif
#ifndef MENUITEM_FLOAT_H
#define MENUITEM_FLOAT_H

//#include "./Menu/MenuItem/MenuItem.h"
#include "../MenuItem/MenuItem.h"

class MenuItem_Float : public MenuItem
{
    private:
    float *data;
    

    public:
    MenuItem_Float(String title, const unsigned char *sprite, void(*function)(), String entryDialog, float *data);
    ~MenuItem_Float();
    void toText(char* text);
    const unsigned char* getDataSprite();
    void printName();
    float getData();
};

#endif
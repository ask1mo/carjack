#ifndef MENUITEM_U64_H
#define MENUITEM_U64_H

//#include "./Menu/MenuItem/MenuItem.h"
#include "../MenuItem/MenuItem.h"

class MenuItem_U64 : public MenuItem
{
    private:
    uint64_t *data;
    

    public:
    MenuItem_U64(const char* title, const unsigned char *sprite, void(*function)(), String entryDialog, uint64_t *data);
    ~MenuItem_U64();
    void toText(char* text);
    const unsigned char* getDataSprite();
    void printName();
    uint64_t getData();
};

#endif
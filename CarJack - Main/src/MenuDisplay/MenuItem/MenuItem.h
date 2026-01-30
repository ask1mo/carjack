#ifndef MENUITEM_H
#define MENUITEM_H

#include <Arduino.h>
#include "../Sprites.h"

#define SIGNATURE_BASE     0
#define SIGNATURE_FOLDER   1
#define SIGNATURE_1BOOL    2
#define SIGNATURE_BOOL     3
#define SIGNATURE_STATIC   4
#define SIGNATURE_SRINGINT 5
#define SIGNATURE_U8       6
#define SIGNATURE_U64      7
#define SIGNATURE_FLOAT    8

class MenuItem
{
    protected:
    uint8_t signature;
    String title;
    const unsigned char *sprite;
    void(*entryFunction)();
    String entryDialog;
    bool selected;

    
    public:
    ~MenuItem();
    virtual uint8_t click();
    String getTitle();
    const unsigned char* getSprite();
    virtual const unsigned char* getDataSprite();
    virtual String toText();
    void printName();
    virtual void printTitleAdress();
    bool getSelected();
    void setSelected(bool selected);
    String getEntryDialogString();
    void (*getEntryFunction())();
    uint8_t getSignature();
};

#endif
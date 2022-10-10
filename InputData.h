#ifndef _Input_Data_h
#define _Input_Data_h
#include "TypeDef.h"

#include "ANSI-UNICODE-UTF8.h"

FILE* OpenFile(const char seg1[], const char seg2[], const char seg3[], const char name[]);

void bodyTypeCheck(const char bodyType[]);

void getTraits(RBSRoot* traitsroot, FILE* config);

void readConfig(Settings* settings);

bool HasHat();

Settings* InitializeSettings();

#endif
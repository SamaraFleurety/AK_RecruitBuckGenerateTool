#ifndef _CheckFile_h
#define _CheckFile_h
#include "TypeDef.h"

void FileEndCheck(FILE* targetFile);
void FileEndCheckTranslate(FILE* targetFile);

void FileEndRemove(FILE* targetFile);
void FileEndRemoveTranslate(FILE* targetFile);

void TextureRename(char* target, const char prefix[], const char prefix2[], const char agentName[], const char bodyTypeString[], const char postfix[]);

//void FindOrigTexture(char* target, const char bodyTypeString[], const char postfix[]);

bool CheckStandPortrait();

void CheckTextureFolder(AgentType agentType);

void CheckTexture(bool hasHat);

void printOperator(Settings settings);

void CheckAll(Settings settings);

#endif
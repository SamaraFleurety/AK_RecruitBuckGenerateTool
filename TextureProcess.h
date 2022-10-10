#ifndef _CheckFile_h
#define _CheckFile_h
#include "TypeDef.h"

void TextureRename(char* target, const char prefix[], const char prefix2[], const char agentName[], const char bodyTypeString[], const char postfix[]);

//void FindOrigTexture(char* target, const char bodyTypeString[], const char postfix[]);

bool CheckStandPortrait();

void CreateTextureFolder(AgentType agentType);

void CheckTexture(bool hasHat);

void MoveTexture(Settings settings);

#endif
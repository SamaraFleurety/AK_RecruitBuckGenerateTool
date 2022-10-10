#ifndef _TypeDef_h
#define _TypeDef_h

#define thoughtsmax 3

#include "RBT_String.h"

enum VoiceType_enum {
	Ability = 0,
	Die,
	Draft,
	Recruit,
	Select,
	Undraft
};

enum BodyType_enum {
	Thin = 1, Hulk, Fat, Male, Female
};

enum FileType_enum {
	f_hat = 0,
	f_apparel,
	f_weapon,
	f_hair,
	f_operator,
	f_backstory,
	f_trait,
	f_thoughts,
	f_voice,
	fileCnt
};

enum XMLType_enum {
	def = 0,
	languageData
};

enum TextureType_enum {
	hat = 0,
	hatDropped,
	apparelDropped,
	textureTypeCnt
};

typedef struct OutputFile_struct {
	//FILE* inputFile = NULL;
	FILE* configFile = NULL;
	FILE* UTF8ThingsTypeFile = NULL;
	FILE* outputFile[fileCnt];
	FILE* outputFileTranslate[fileCnt];
}OutputFile;

typedef struct UTF8ThingsType_struct {
	char apparel[20];
	char weapon[20];
	char hair[20];
	char hat[20];
}UTF8ThingsType;

typedef struct AgentType_struct {
	char Lower[15];
	char Upper[15];
}AgentType;

typedef struct AgentName_struct {
	char English[20];
	char Chinese[20];
}AgentName;

typedef struct BodyType_struct {
	enum BodyType_enum mark;
	char string[10];
}BodyType;

typedef struct Skills_struct {
	int animals;
	int animalsFire;
	int art;
	int artFire;
	int construction;
	int constructionFire;
	int cook;
	int cookFire;
	int craft;
	int craftFire;
	int intellect;
	int intellectFire;
	int medi;
	int mediFire;
	int melee;
	int meleeFire;
	int mining;
	int miningFire;
	int plants;
	int plantsFire;
	int shoot;
	int shootFire;
	int social;
	int socialFire;
}Skills;

typedef struct Story_struct {
	int backstory[2] = { 0, 0 }; //0ÎÞ£¬1ÓÐ
	char childDesc[500];
	char childName[50];
	char adultDesc[500];
	char adultName[50];
};

typedef struct Settings_struct {
	bool debugOverride = false;
	int generateMode;
	AgentName agentName;
	AgentType agentType;
	int age;
	Story_struct story;
	char bodyType[10];
	char descEng[500];
	char descChi[500];
	Skills skills;
	bool has[textureTypeCnt];
	OutputFile outputFile;
	RBSRoot* traitsRoot;
	int* thought;
}Settings;

void AnyKeyQuit();

#endif

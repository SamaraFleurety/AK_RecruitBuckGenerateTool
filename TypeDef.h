#ifndef _TypeDef_h
#define _TypeDef_h

#define fileCnt 8
#define f_hat 0
#define f_apparel 1
#define f_weapon 2
#define f_hair 3
#define f_operator 4
#define f_backstory 5
#define f_trait 6
#define f_thoughts 7
#define thoughtsmax 3

#include "RBT_String.h"

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

enum BodyType_enum {
	Thin = 1, Hulk, Fat, Male, Female
};

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
	int backstory[2] = { 0, 0 }; //0�ޣ�1��
	char childDesc[500];
	char childName[50];
	char adultDesc[500];
	char adultName[50];
};


typedef struct Settings_struct {
	int generateMode;
	AgentName agentName;
	AgentType agentType;
	int age;
	Story_struct story;
	char bodyType[10];
	char descEng[500];
	char descChi[500];
	Skills skills;
	bool hasHat;
	OutputFile outputFile;
	RBSRoot* traitsRoot;
	int* thought;
}Settings;

void AnyKeyQuit();

#endif

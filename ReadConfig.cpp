#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <io.h>
#include "ReadConfig.h"

/// <summary>
/// 打开xml文件，从config中读取干员属性
/// </summary>

FILE* OpenFile(const char seg1[], const char seg2[], const char seg3[], const char name[]) {
	FILE* target;
	char* path = (char*)malloc(300);
	strcpy(path, seg1);
	if (strlen(seg2) != 0) {
		strcat(path, seg2);
	}
	if (strlen(seg3) != 0) {
		strcat(path, seg3);
	}
	if (fopen_s(&target, path, "a+") != 0) {
		printf("打开 %s 失败\n", name);
		AnyKeyQuit();
	}
	else {
		printf("打开 %s 成功\n", name);
	}
	rewind(target);
	return target;
 }

void bodyTypeCheck(const char bodyType[]) {
	if (strcmp(bodyType, "Thin") == 0 || strcmp(bodyType, "Hulk") == 0 || strcmp(bodyType, "Male") == 0 || strcmp(bodyType, "Female") == 0 || strcmp(bodyType, "Fat") == 0) return;
	printf("错误的身体类型:%s\n", bodyType);
	AnyKeyQuit();
	return;
}

static void RemoveTrailingSpace (char *string) {
	if (strlen(string) <= 1) return;
	int sp = 0;
	while (string[sp] == '\n' || string[sp] == ' ') {
		string = &string[1];
		++sp;
	}

	sp = strlen(string) - 1;
	while (string[sp] == '\n' || string[sp] == ' ') {
		string[sp] = '\0';
		--sp;
	}
	return;
}

void getTraits(RBSRoot* traitsroot, FILE* config) {
	fscanf(config, ">Traits:\n");
	int errFlag;
	char trait[30];
	int degree;
	RBSTree node;
	errFlag = fscanf(config, "%s %d\n", trait, &degree);
	while (strcmp(trait, "<") != 0) {
		node = rbts_insertVal(traitsroot, trait);
		if (errFlag == 2) node->degree = degree;
		else node->degree = 0;
		errFlag = fscanf(config, "%s %d\n", trait, &degree);
	} 
}

void readConfig(Settings* settings) {
	int errFlag = 0;
	char temp[500] = "\0";
	errFlag += fscanf(settings->outputFile.configFile, ">Mode:%d<%[^>]", &settings->generateMode, temp);
	errFlag += fscanf(settings->outputFile.configFile, ">EnglishName:%[^<]<\n", settings->agentName.English);
	errFlag += fscanf(settings->outputFile.configFile, ">ChineseName:%[^<]<\n", settings->agentName.Chinese);

	errFlag += fscanf(settings->outputFile.configFile, ">AgentType:%[^<]<%[^>]", settings->agentType.Upper, temp);
	strcpy(settings->agentType.Lower, settings->agentType.Upper);
	settings->agentType.Lower[0] = tolower(settings->agentType.Lower[0]);

	errFlag += fscanf(settings->outputFile.configFile, ">Age:%d<\n", &settings->age);

	errFlag += fscanf(settings->outputFile.configFile, ">BackStory:%d %d<\n", &settings->story.backstory[0], &settings->story.backstory[1]);
	errFlag += fscanf(settings->outputFile.configFile, ">ChildName:%[^<]<\n", settings->story.childName);
	errFlag += fscanf(settings->outputFile.configFile, ">ChildDesc:%[^<]<\n", settings->story.childDesc);
	errFlag += fscanf(settings->outputFile.configFile, ">AdultName:%[^<]<\n", settings->story.adultName);
	errFlag += fscanf(settings->outputFile.configFile, ">AdultDesc:%[^<]<\n", settings->story.adultDesc);

	errFlag += fscanf(settings->outputFile.configFile, ">BodyType:%[^<]<%[^>]", settings->bodyType, temp);
	bodyTypeCheck(settings->bodyType);

	errFlag += fscanf(settings->outputFile.configFile, ">EngDesc:%[^<]<\n", settings->descEng);
	errFlag += fscanf(settings->outputFile.configFile, ">ChinDesc:%[^<]<\n", settings->descChi);
	RemoveTrailingSpace(settings->descChi);
	RemoveTrailingSpace(settings->descEng);
	
	errFlag += fscanf(settings->outputFile.configFile, ">Skills:\nAnimals:%d %d\n", &settings->skills.animals, &settings->skills.animalsFire);
	errFlag += fscanf(settings->outputFile.configFile, "Artistic:%d %d\n", &settings->skills.art, &settings->skills.artFire);
	errFlag += fscanf(settings->outputFile.configFile, "Construction:%d %d\n", &settings->skills.construction, &settings->skills.constructionFire);
	errFlag += fscanf(settings->outputFile.configFile, "Cooking:%d %d\n", &settings->skills.cook, &settings->skills.cookFire);
	errFlag += fscanf(settings->outputFile.configFile, "Crafting:%d %d\n", &settings->skills.craft, &settings->skills.craftFire);
	errFlag += fscanf(settings->outputFile.configFile, "Intellectual:%d %d\n", &settings->skills.intellect, &settings->skills.intellectFire);
	errFlag += fscanf(settings->outputFile.configFile, "Medicine:%d %d\n", &settings->skills.medi, &settings->skills.mediFire);
	errFlag += fscanf(settings->outputFile.configFile, "Melee:%d %d\n", &settings->skills.melee, &settings->skills.meleeFire);
	errFlag += fscanf(settings->outputFile.configFile, "Mining:%d %d\n", &settings->skills.mining, &settings->skills.miningFire);
	errFlag += fscanf(settings->outputFile.configFile, "Plants:%d %d\n", &settings->skills.plants , &settings->skills.plantsFire);
	errFlag += fscanf(settings->outputFile.configFile, "Shooting:%d %d\n", &settings->skills.shoot, &settings->skills.shootFire);
	errFlag += fscanf(settings->outputFile.configFile, "Social:%d %d\n<\n", &settings->skills.social, &settings->skills.socialFire);

	settings->traitsRoot = rbts_create();
	getTraits(settings->traitsRoot, settings->outputFile.configFile);

	settings->thought = (int*)(malloc(sizeof(int) * 4));
	errFlag += fscanf(settings->outputFile.configFile, ">Thought:");
	for (int i = 0; i < thoughtsmax; ++i) {
		errFlag = fscanf(settings->outputFile.configFile, "%d ", &settings->thought[i]);
		if (errFlag == -1) settings->thought[i] = 0;
	}
	errFlag += fscanf(settings->outputFile.configFile, "%[^>]", temp);

	errFlag += fscanf(settings->outputFile.configFile, ">WeaponName:%[^<]<\n", settings->weaponName);
	errFlag += fscanf(settings->outputFile.configFile, ">WeaponDesc:%[^<]<\n", settings->weaponDesc);
	RemoveTrailingSpace(settings->weaponName);
	RemoveTrailingSpace(settings->weaponDesc);
	errFlag += fscanf(settings->outputFile.configFile, ">VoiceFolder:%[^<]<\n", settings->voicePath);
	printf("%d\n%d\n%d\n", settings->thought[0], settings->thought[1], settings->thought[2]);
	printf("%s\n%s\n%s\n", settings->weaponDesc, settings->weaponName, settings->voicePath);
	//FIXME:
	//printf("%d\n%s %s\n%s\n%d\n%d\n%d\n%s\n", settings->generateMode, settings->agentName.English, settings->agentName.Chinese, settings->agentType.Upper, settings->age, settings->backstory[0], settings->backstory[1], settings->bodyType.string);
	return;
}

bool HasHat() { //已经从落后的手动输入变成自动检测
	int temp = _access(".\\input\\aaH_south.png", 6);

	if (temp == 0) {
		printf("有帽子.\n");
		return true;
	}


	printf("没有帽子.\n");
	return false;
}


Settings* InitializeSettings() {
	Settings* settings = (Settings*)malloc(sizeof(Settings));

	settings->outputFile.configFile = OpenFile(".\\input\\config.txt", "", "", "config");

	readConfig(settings);
	char temp1[15];
	int i = fopen_s(&settings->outputFile.UTF8ThingsTypeFile, "UTF8TypeFile.txt", "r");
	if (fscanf(settings->outputFile.UTF8ThingsTypeFile, "%s %s %s %s", temp1, temp1, temp1, temp1) == 4) {
		rewind(settings->outputFile.UTF8ThingsTypeFile);
		printf("找到正确的UTFType文件\n");
	}
	else {
		printf("未找到正确的UTFType文件\n");
		AnyKeyQuit();
	}
	//干员文件
	settings->outputFile.outputFile[f_operator] = OpenFile(".\\output\\Defs\\OperatorDef\\Recruit_", settings->agentType.Upper, ".xml", "OperatorDef");
	settings->outputFile.outputFileTranslate[f_operator] = OpenFile(".\\output\\Languages\\ChineseSimplified (简体中文)\\DefInjected\\AK_DLL.OperatorDef\\RecruitTranslate_", settings->agentType.Upper, ".xml", "Operator翻译");
	//服装
	settings->outputFile.outputFile[f_apparel] = OpenFile(".\\output\\Defs\\ThingsDefs\\ThingsDefs_Apparel\\Apparel_", settings->agentType.Upper, ".xml", "ApparelDef");
	//settings->outputFile.outputFileTranslate[f_apparel] = OpenFile(".\\output\\Languages\\English\\DefInjected\\Things\\ApparelTranslate_", settings->agentType.Upper, ".xml", "ApparelDef翻译");
	//武器
	settings->outputFile.outputFile[f_weapon] = OpenFile(".\\output\\Defs\\ThingsDefs\\ThingsDefs_Weapon\\Weapon_", settings->agentType.Upper, ".xml", "WeaponDef");
	//settings->outputFile.outputFileTranslate[f_weapon] = OpenFile(".\\output\\Languages\\English\\DefInjected\\Things\\WeaponTranslate_", settings->agentType.Upper, ".xml", "WeaponDef翻译");
	//头发
	settings->outputFile.outputFile[f_hair] = OpenFile(".\\output\\Defs\\HairDefs\\Hair_", settings->agentType.Upper, ".xml", "HairDef");
	//settings->outputFile.outputFileTranslate[f_hair] = OpenFile(".\\output\\Languages\\English\\DefInjected\\HairDef\\HairTranslate_", settings->agentType.Upper, ".xml", "HairDef翻译");
	//特性
	settings->outputFile.outputFile[f_trait] = OpenFile(".\\output\\Defs\\TraitsDefs\\Trait_", settings->agentType.Upper, ".xml", "TraitDef");
	//settings->outputFile.outputFileTranslate[f_trait] = OpenFile(".\\output\\Languages\\English\\DefInjected\\Trait\\_", settings->agentType.Upper, ".xml", "TraitDef翻译");
	//特性->想法
	settings->outputFile.outputFile[f_thoughts] = OpenFile(".\\output\\Defs\\TraitsDefs\\Thought\\Thought_", settings->agentType.Upper, ".xml", "ThoughtDef");
	//settings->outputFile.outputFileTranslate[f_thoughts] = OpenFile(".\\output\\Languages\\English\\DefInjected\\Trait\\Thought\\_", settings->agentType.Upper, ".xml", "ThoughtDef翻译");
	//背景故事
	settings->outputFile.outputFile[f_backstory] = OpenFile(".\\output\\Defs\\BackStory\\Story_", settings->agentType.Upper, ".xml", "BSDef");
	//settings->outputFile.outputFileTranslate[f_backstory] = OpenFile(".\\output\\Languages\\English\\DefInjected\\StoriesRetold\\_", settings->agentType.Upper, ".xml", "BSDef翻译");
	//语音
	settings->outputFile.outputFile[f_voice] = OpenFile(".\\output\\Defs\\VoiceDef\\Voice_", settings->agentType.Upper, ".xml", "VoiceDef");

	settings->has[hat] = HasHat();
	settings->has[apparelDropped] = _access(".\\input\\aa.png", 6) == 0 ? true : false;
	settings->has[hatDropped] = _access(".\\input\\aaH.png", 6) == 0 ? true : false;
	if (settings->has[hat]) {
		settings->outputFile.outputFile[f_hat] = OpenFile(".\\output\\Defs\\ThingsDefs\\ThingsDefs_Apparel\\Apparel_Hat.xml", "", "", "HatDef");
		//settings->outputFile.outputFileTranslate[f_hat] = OpenFile(".\\output\\Languages\\English\\DefInjected\\Things\\ApparelTranslate_Hat.xml", "", "", "HatDef翻译");
	}

	return settings;
}

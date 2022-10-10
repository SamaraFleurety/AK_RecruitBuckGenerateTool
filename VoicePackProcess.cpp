#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <io.h>
#include <direct.h>
#include "TypeDef.h"
#include "xmlEndProcess.h"
#include "VoicePackProcess.h"

static char* GetVoiceFileNameFromNumber(int number) {
	if (number >= 100 || number <= 0) {
		printf("error:非法语音编号\n");
		return NULL;
	}
	char* path = (char*)malloc(50);
	strcpy(path, ".\\input\\CN_0");
	if (number < 10) {
		strcat(path, "0");
	}
	sprintf(path, "%s%d.wav\0", path, number);
	return path;
}

static int GetVoiceFileNumbered(int number) {
	char* path = GetVoiceFileNameFromNumber(number);
	if (!path) return 0;
	if (_access(path, 6) == 0) return 1;
	return 0;
}

void PrintVoiceCount(){
	printf("Recruit Voice: %d\n", GetVoiceFileNumbered(11));
	printf("Died Voice: %d\n", GetVoiceFileNumbered(32));
	int temp = 0;
	temp += GetVoiceFileNumbered(21);
	temp += GetVoiceFileNumbered(22);
	temp += GetVoiceFileNumbered(34);
	temp += GetVoiceFileNumbered(36);
	printf("Select Voice: %d\n", temp);
	temp = GetVoiceFileNumbered(23);
	temp += GetVoiceFileNumbered(24);
	printf("Draft Voice: %d\n", temp);
	temp = 0;
	for (int i = 25; i <= 28; ++i) {
		temp += GetVoiceFileNumbered(i);
	}
	printf("Ability Voice: %d\n", temp);
	return;
}

static char* GetNewPath(AgentName* agentName, AgentType* agentType, char* voiceType, char* voiceNumber) {
	char* newPath = (char*)malloc(100);
	sprintf(newPath, ".\\output\\Sounds\\Operator\\%s\\%s\\voice\\%s_%s%s.wav", agentType->Upper, agentName->English, voiceType, agentName->English, voiceNumber);
	return newPath;
}

static char** VoiceType_Create() {
	char** voiceType = (char**)malloc(sizeof(char*) * 5);
	for (int i = 0; i < 5; ++i) {
		voiceType[i] = (char*)malloc(15);
	}
	strcpy(voiceType[Ability], "Ability");
	strcpy(voiceType[Die], "Die");
	strcpy(voiceType[Draft], "Draft");
	strcpy(voiceType[Select], "Select");
	strcpy(voiceType[Recruit], "Recruit");
	return voiceType;
}

static char** VoiceNumber_Create() {
	char** voiceNumber = (char**)malloc(sizeof(char*) * 6);
	for (int i = 0; i < 6; ++i) {
		voiceNumber[i] = (char*)malloc(5);
	}
	strcpy(voiceNumber[0], "\0");
	strcpy(voiceNumber[1], "I");
	strcpy(voiceNumber[2], "II");
	strcpy(voiceNumber[3], "III");
	strcpy(voiceNumber[4], "IV");
	strcpy(voiceNumber[5], "V");
	return voiceNumber;
}

static int ProcessVoice(int voiceNumber, Settings* settings, char* voiceType, char* voiceNumberString) {
	if (GetVoiceFileNumbered(voiceNumber) == 0) return 0;
	char* oldPath = GetVoiceFileNameFromNumber(voiceNumber);
	char* newPath = GetNewPath(&settings->agentName, &settings->agentType, voiceType, voiceNumberString);
	if (settings->debugOverride) printf("%s -> %s\n", oldPath, newPath);

	rename(oldPath, newPath);

	fprintf(settings->outputFile.outputFile[f_voice],
		"\t<SoundDef ParentName = \"AK_VoiceBase\">\n"
		"\t\t<defName>AK_Voice_%s_%s%s</defName>\n"
		"\t\t<subSounds>\n\t\t\t<li>\n"
		"\t\t\t\t<onCamera>True</onCamera>\n"
		"\t\t\t\t\t<grains>\n\t\t\t\t\t\t<li Class=\"AudioGrain_Clip\">\n"
		"\t\t\t\t\t\t\t<clipPath>Operator/Supporter/Istina/char_195_glassb/Ability_IstinaIV</clipPath>\n"
		"\t\t\t\t\t\t</li>\n\t\t\t\t\t</grains>\n"
		"\t\t\t\t<volumeRange>40~40</volumeRange>\n"
		"\t\t\t\t<distRange>10~100</distRange>\n"
		"\t\t\t\t<sustainLoop>False</sustainLoop>\n"
		"\t\t\t</li>\n\t\t</subSounds>\n"
		"\t</SoundDef>\n\n"
		, voiceType, settings->agentName.English, voiceNumberString, &newPath[16]
		);

	return 1;
}

static void CreatVoicePackPath(AgentName* agentName, AgentType* agentType) {
	_mkdir(".\\output\\Sounds");
	_mkdir(".\\output\\Sounds\\Operator");

	char path[100] = ".\\output\\Sounds\\Operator";
	strcat(path, "\\");
	strcat(path, agentType->Upper);
	_mkdir(path);

	strcat(path, "\\");
	strcat(path, agentName->English);
	_mkdir(path);

	strcat(path, "\\voice");
	_mkdir(path);
	
	return;
}

void AutoProcessVoicePack(Settings* settings) {
	char** voiceType = VoiceType_Create();
	char** voiceNumber = VoiceNumber_Create();

	CreatVoicePackPath(&settings->agentName, &settings->agentType);

	printf("正在处理 语音文件及其def\n");

	ProcessVoice(11, settings, voiceType[Recruit], voiceNumber[0]);

	ProcessVoice(32, settings, voiceType[Die], voiceNumber[0]);

	int temp = 1;
	temp += ProcessVoice(21, settings, voiceType[Select], voiceNumber[temp]);
	temp += ProcessVoice(22, settings, voiceType[Select], voiceNumber[temp]);
	temp += ProcessVoice(34, settings, voiceType[Select], voiceNumber[temp]);
	temp += ProcessVoice(36, settings, voiceType[Select], voiceNumber[temp]);

	temp = 1;
	temp += ProcessVoice(23, settings, voiceType[Draft], voiceNumber[temp]);
	temp += ProcessVoice(24, settings, voiceType[Draft], voiceNumber[temp]);

	temp = 1;
	for (int i = 25; i <= 28; ++i) {
		temp += ProcessVoice(i, settings, voiceType[Ability], voiceNumber[temp]);
	}

	FileEndRestore(settings->outputFile.outputFile[f_voice]);
	
	return;
}

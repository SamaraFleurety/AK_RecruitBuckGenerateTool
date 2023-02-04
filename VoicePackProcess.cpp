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
		printf("error:�Ƿ��������\n");
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
	char** voiceType = (char**)malloc(sizeof(char*) * 6);
	for (int i = 0; i < 6; ++i) {
		voiceType[i] = (char*)malloc(15);
	}
	strcpy(voiceType[Ability], "Ability");
	strcpy(voiceType[Die], "Die");
	strcpy(voiceType[Draft], "Draft");
	strcpy(voiceType[Select], "Select");
	strcpy(voiceType[Recruit], "Recruit");
	strcpy(voiceType[Undraft], "Undraft");
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

static int GenVoiceDef(Settings* settings, char* voiceType, char* voiceNumberString) {
	char temp[500]; 
	strcpy(temp, voiceType);
	strcat(temp, "_");
	strcat(temp, settings->agentName.English);
	strcat(temp, voiceNumberString);


	fprintf(settings->outputFile.outputFile[f_voice],
		"\t<SoundDef ParentName = \"AK_VoiceBase\">\n"
		"\t\t<defName>AK_Voice_%s</defName>\n"
		"\t\t<subSounds>\n\t\t\t<li>\n"
		"\t\t\t\t<onCamera>True</onCamera>\n"
		"\t\t\t\t\t<grains>\n\t\t\t\t\t\t<li Class=\"AudioGrain_Clip\">\n"
		"\t\t\t\t\t\t\t<clipPath>Operator/%s/%s/%s/%s</clipPath>\n"
		"\t\t\t\t\t\t</li>\n\t\t\t\t\t</grains>\n"
		"\t\t\t\t<volumeRange>40~40</volumeRange>\n"
		"\t\t\t\t<distRange>10~100</distRange>\n"
		"\t\t\t\t<sustainLoop>False</sustainLoop>\n"
		"\t\t\t</li>\n\t\t</subSounds>\n"
		"\t</SoundDef>\n\n"
		, temp, settings->agentType.Upper, settings->agentName.English, settings->voicePath, temp
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

	printf("���ڴ��� �����ļ�����def\n");

	GenVoiceDef(settings, voiceType[Recruit], voiceNumber[0]);

	GenVoiceDef(settings, voiceType[Die], voiceNumber[0]);

	GenVoiceDef(settings, voiceType[Undraft], voiceNumber[0]);

	for (int i = 1; i <= 4; ++i) {
		GenVoiceDef(settings, voiceType[Select], voiceNumber[i]);
	}

	for (int i = 1; i <= 2; ++i) {
		GenVoiceDef(settings, voiceType[Draft], voiceNumber[i]);
	}

	for (int i = 1; i <= 4; ++i) {
		GenVoiceDef(settings, voiceType[Ability], voiceNumber[i]);
	}
	
	return;
}

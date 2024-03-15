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

void AutoProcessVoicePack(Settings* settings) {
	char** voiceType = VoiceType_Create();
	char** voiceNumber = VoiceNumber_Create();

	printf("正在处理 语音def\n");

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

//--------------------------------------------------------------------------------------------------------以下是新写的处理wav文件

static void CreatVoicePath(Settings* settings, char* path) {
	strcat(path, "\\Sounds");
	_mkdir(path);

	strcat(path, "\\Operator");
	_mkdir(path);

	strcat(path, "\\");
	strcat(path, settings->agentType.Upper);
	_mkdir(path);

	strcat(path, "\\");
	strcat(path, settings->agentName.English);
	_mkdir(path);

	strcat(path, "\\");
	strcat(path, settings->voicePath);
	_mkdir(path);
	return;
}

static void CreatAllVoicePath(Settings *settings) {
	char* path = (char*)malloc(200);
	strcpy(path, ".\\output");
	CreatVoicePath(settings, path);

	strcpy(path, ".\\output\\Sound_EN");
	_mkdir(path);
	CreatVoicePath(settings, path);

	strcpy(path, ".\\output\\Sound_CN");
	_mkdir(path);
	CreatVoicePath(settings, path);

	return;
}

static char* getOriName(const char* source, char* voiceType, char* voiceNumber) {
	char* oriName = (char*)malloc(100);
	sprintf(oriName, ".\\input\\%s\\%s_aa%s.wav", source, voiceType, voiceNumber);
	return oriName;
}

static char* getNewName(const char* destination, Settings* settings, char* voiceType, char* voiceNumber) {
	char* newName = (char*)malloc(500);
	sprintf(newName, ".\\output\\%sOperator\\%s\\%s\\%s\\%s_%s%s.wav", destination, settings->agentType.Upper, settings->agentName.English, settings->voicePath, voiceType, settings->agentName.English, voiceNumber);
	return newName;
}

void ProcessWav(const char* source, const char* destination, Settings* settings, char* voiceType, char* voiceNumber) {
	char* oldName = getOriName(source, voiceType, voiceNumber);
	if (_access(oldName, 6) != 0) return;
	//printf("%s\n->\n%s\n", oldName, getNewName(destination, settings, voiceType, voiceNumber));
	rename(oldName, getNewName(destination, settings, voiceType, voiceNumber));
	return;
}

void AutoProcessWav(const char* source, const char* destination, Settings* settings) {
	char** voiceType = VoiceType_Create();
	char** voiceNumber = VoiceNumber_Create();

	ProcessWav(source, destination, settings, voiceType[Recruit], voiceNumber[0]);

	ProcessWav(source, destination, settings, voiceType[Die], voiceNumber[0]);

	ProcessWav(source, destination, settings, voiceType[Undraft], voiceNumber[0]);

	for (int i = 0; i <= 4; ++i) {
		ProcessWav(source, destination, settings, voiceType[Ability], voiceNumber[i]);
		ProcessWav(source, destination, settings, voiceType[Select], voiceNumber[i]);
		ProcessWav(source, destination, settings, voiceType[Draft], voiceNumber[i]);
	}

	return;
}


void AutoProcessVoiceFile(Settings* settings) {

	if (settings->hasVoice != 0) printf("无语音文件");
	printf("正在处理 语音文件\n");

	CreatAllVoicePath(settings);

	AutoProcessWav("Sound", "Sounds\\", settings);

	AutoProcessWav("Sound_CN", "Sound_CN\\Sounds\\", settings);

	AutoProcessWav("Sound_EN", "Sound_EN\\Sounds\\", settings);
}

#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <io.h>
#include <direct.h>
#include "TextureProcess.h"
#include "TypeDef.h"
#include "xmlEndProcess.h"

//--------------------

void duplicateTextureDelete(char* target) {
	if (_access(target, 0) == 0) {
		int i = remove(target);
		if (i != 0) {
			printf("有重复的：%s\n并且无法删除.\n", target);
			AnyKeyQuit();
		}
		else {
			printf("删除了重复的:\n%s\n", target);
		}
	}
}

void TextureRename(char* target, const char prefix[], const char prefix2[], const char agentName[], const char bodyTypeString[], const char postfix[]) {
	strcpy(target, ".\\output\\");
	strcat(target, prefix);
	if ((strcmp(prefix2, "-1") != 0)) {
		strcat(target, prefix2);
		strcat(target, "\\");
	}
	strcat(target, agentName);
	if ((strcmp(bodyTypeString, "-1")) != 0) {
		strcat(target, "_");
		strcat(target, bodyTypeString);
		strcat(target, "_");
	}
	strcat(target, postfix);
	strcat(target, ".png");
	printf("%s\n", target);
	duplicateTextureDelete(target);
	return;
}

/*void FindOrigTexture(char* target, const char bodyTypeString[], const char postfix[]) {
	strcpy(target, ".\\output\\aa_");
	strcat(target, bodyTypeString);
	strcat(target, "_");
	strcat(target, postfix);
	strcat(target, ".png");
}*/

//-------------------------
bool CheckStandPortrait() {
	int temp = _access(".\\input\\aaStand.png", 6);
	temp += _access(".\\input\\aaPortrait.png", 6);
	if (temp == 0) return true;
	return false;
}
void CreateTextureFolder(AgentType agentType) {
	int i = _access(".\\output\\Textures", 0);
	if (true) {
		char generalPath[100] = ".\\output\\Textures\\Things\\";
		char temp[100];

		i = _mkdir(".\\output\\Textures");
		i += _mkdir(".\\output\\Textures\\Things");

		i += _mkdir(".\\output\\Textures\\Things\\AK_Agents");
		strcpy(temp, generalPath);
		strcat(temp, "AK_Agents\\");
		strcat(temp, agentType.Upper);
		i += _mkdir(temp);

		strcpy(temp, generalPath);
		strcat(temp, "\\AK_Agents\\HatAccessory");
		i += _mkdir(temp);

		i += _mkdir(".\\output\\Textures\\Things\\AK_Agents\\Weapon");
		strcpy(temp, generalPath);
		strcat(temp, "\\AK_Agents\\Weapon\\");
		strcat(temp, agentType.Upper);
		i += _mkdir(temp);

		i += _mkdir(".\\output\\Textures\\Things\\Hair");
		strcpy(temp, generalPath);
		strcat(temp, "Hair\\");
		strcat(temp, agentType.Upper);
		i += _mkdir(temp);

		strcpy(temp, ".\\output\\NoFace");
		i += _mkdir(temp);
		strcat(temp, "\\Textures");
		i += _mkdir(temp);
		strcat(temp, "\\Things");
		i += _mkdir(temp);
		strcat(temp, "\\Hair");
		i += _mkdir(temp);
		strcat(temp, "\\");
		strcat(temp, agentType.Upper);
		i += _mkdir(temp);

		if (true) {
			strcpy(temp, ".\\output\\Textures\\UI\\Image\\");
			strcat(temp, agentType.Upper);
			i += _mkdir(".\\output\\Textures\\UI");
			i += _mkdir(".\\output\\Textures\\UI\\Image");
			i += _mkdir(temp);
		}
	}

	/*if (i != 0) {
		printf("贴图文件夹不存在且无法建立新的.\n");
		AnyKeyQuit();
	}*/

	return;
}

void CheckTexture(bool hasHat) {
	int checkResult = 0;
	checkResult += _access(".\\input\\aa_bbb_south.png", 6);
	checkResult += _access(".\\input\\aa_bbb_north.png", 6);
	checkResult += _access(".\\input\\aa_bbb_east.png", 6);
	//access函数模式：0只存在，2写权限，4读权限，6写和读权限
	//checkResult += _access(".\\input\\aa.png", 6);
	checkResult += _access(".\\input\\aaW.png", 6);
	checkResult += _access(".\\input\\aa_southHair.png", 6);
	checkResult += _access(".\\input\\aa_northHair.png", 6);
	checkResult += _access(".\\input\\aa_eastHair.png", 6);

	checkResult += _access(".\\input\\aaNF_south.png", 6);
	checkResult += _access(".\\input\\aaNF_north.png", 6);
	checkResult += _access(".\\input\\aaNF_east.png", 6);

	if (hasHat == true) {
		//checkResult += _access(".\\input\\aaH.png", 6);
		checkResult += _access(".\\input\\aaH_south.png", 6);
		checkResult += _access(".\\input\\aaH_north.png", 6);
		checkResult += _access(".\\input\\aaH_east.png", 6);
	}

	if (checkResult != 0) {
		printf("缺乏贴图文件或文件名不正确\n");
		AnyKeyQuit();
	}

	return;
}

void MoveTexture(Settings* settings) {

	//char* oldName = (char*)malloc(sizeof(char) * 50);
	char* newName = (char*)malloc(sizeof(char) * 100);

	TextureRename(newName, "Textures\\Things\\AK_Agents\\", settings->agentType.Upper, settings->agentName.English, "-1", "");
	rename(".\\input\\aa.png", newName);

	TextureRename(newName, "Textures\\Things\\AK_Agents\\", settings->agentType.Upper, settings->agentName.English, settings->bodyType, "south");
	//FindOrigTexture(oldName, settings->bodyType, "south");
	int i = rename(".\\input\\aa_bbb_south.png", newName);

	TextureRename(newName, "Textures\\Things\\AK_Agents\\", settings->agentType.Upper, settings->agentName.English, settings->bodyType, "north");
	//FindOrigTexture(oldName, settings->bodyType, "north");
	i += rename(".\\input\\aa_bbb_north.png", newName);

	TextureRename(newName, "Textures\\Things\\AK_Agents\\", settings->agentType.Upper, settings->agentName.English, settings->bodyType, "east");
	//FindOrigTexture(oldName, settings->bodyType, "east");
	i += rename(".\\input\\aa_bbb_east.png", newName);

	TextureRename(newName, "Textures\\Things\\AK_Agents\\Weapon\\", settings->agentType.Upper, settings->agentName.English, "-1", "W");
	i += rename(".\\input\\aaW.png", newName);

	TextureRename(newName, "Textures\\Things\\Hair\\", settings->agentType.Upper, settings->agentName.English, "-1", "_south");
	i += rename(".\\input\\aa_southHair.png", newName);

	TextureRename(newName, "Textures\\Things\\Hair\\", settings->agentType.Upper, settings->agentName.English, "-1", "_north");
	i += rename(".\\input\\aa_northHair.png", newName);

	TextureRename(newName, "Textures\\Things\\Hair\\", settings->agentType.Upper, settings->agentName.English, "-1", "_east");
	i += rename(".\\input\\aa_eastHair.png", newName);

	//
	TextureRename(newName, "NoFace\\Textures\\Things\\Hair\\", settings->agentType.Upper, settings->agentName.English, "-1", "_south");
	i += rename(".\\input\\aaNF_south.png", newName);

	TextureRename(newName, "NoFace\\Textures\\Things\\Hair\\", settings->agentType.Upper, settings->agentName.English, "-1", "_north");
	i += rename(".\\input\\aaNF_north.png", newName);

	TextureRename(newName, "NoFace\\Textures\\Things\\Hair\\", settings->agentType.Upper, settings->agentName.English, "-1", "_east");
	i += rename(".\\input\\aaNF_east.png", newName);

	if (settings->has[hat] == true) {
		TextureRename(newName, "Textures\\Things\\AK_Agents\\HatAccessory\\", "-1", settings->agentName.English, "-1", "H");
		rename(".\\input\\aaH.png", newName);

		TextureRename(newName, "Textures\\Things\\AK_Agents\\HatAccessory\\", "-1", settings->agentName.English, "-1", "H_south");
		i += rename(".\\input\\aaH_south.png", newName);

		TextureRename(newName, "Textures\\Things\\AK_Agents\\HatAccessory\\", "-1", settings->agentName.English, "-1", "H_north");
		i += rename(".\\input\\aaH_north.png", newName);

		TextureRename(newName, "Textures\\Things\\AK_Agents\\HatAccessory\\", "-1", settings->agentName.English, "-1", "H_east");
		i += rename(".\\input\\aaH_east.png", newName);
	}

	if (CheckStandPortrait() == true) {
		printf("找到立绘与头像文件.\n");
		TextureRename(newName, "Textures\\UI\\Image\\", settings->agentType.Upper, settings->agentName.English, "-1", "Stand");
		i += rename(".\\input\\aaStand.png", newName);

		TextureRename(newName, "Textures\\UI\\Image\\", settings->agentType.Upper, settings->agentName.English, "-1", "Portrait");
		i += rename(".\\input\\aaPortrait.png", newName);
	}
	else printf("没有头像和立绘\n");

	if (i != 0) {
		printf("无法正确地移动全部贴图.\n");
		AnyKeyQuit();
	}

	if (settings->generateMode == 1) {
		printf("已完成贴图重置.");
		AnyKeyQuit();
	}
	return;
}
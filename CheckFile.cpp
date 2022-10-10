#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
//#include <fcntl.h>
#include <io.h>
#include <direct.h>
#include "TypeDef.h"
#include "CheckFile.h"
#include "AutoProcessVoicePack.h"
//----------------------------
void FileEndCheck(FILE* targetFile) {
	char fileEnd[25] = "\0";

	fseek(targetFile, -7, SEEK_END);
	fgets(fileEnd, 20, targetFile);
	printf("\t�õ�:%s  ", fileEnd);
	if (strcmp(fileEnd, "</Defs>") == 0) {
		printf("�ļ�β��ȷ\n");
	}
	else
	{
		printf("�ļ�β����.��������˳�.\n");
		AnyKeyQuit();
	}
}

void FileEndCheckTranslate(FILE* targetFile) {
	char fileEnd[25] = "\0";

	fseek(targetFile, -15, SEEK_END);
	fgets(fileEnd, 20, targetFile);
	//printf("\t�õ�:%s\n", fileEnd);
	if (strcmp(fileEnd, "</LanguageData>") == 0) {
		printf("�ļ�β��ȷ\n");
	}
	else
	{
		printf("�ļ�β����.��������˳�.\n");
		AnyKeyQuit();
	}
}

void FileEndRemove(FILE* targetFile) {
	int temp = 0;

	fseek(targetFile, 0, SEEK_END);
	temp = ftell(targetFile);
	_chsize(_fileno(targetFile), temp - 7);
	rewind(targetFile);

}

void FileEndRemoveTranslate(FILE* targetFile) {
	int temp = 0;

	fseek(targetFile, 0, SEEK_END);
	temp = ftell(targetFile);
	_chsize(_fileno(targetFile), temp - 15);
}
//--------------------

void duplicateTextureDelete(char* target) {
	if (_access(target, 0) == 0) {
		int i = remove(target);
		if (i != 0) {
			printf("���ظ��ģ�%s\n�����޷�ɾ��.\n", target);
			AnyKeyQuit();
		}
		else {
			printf("ɾ�����ظ���:\n%s\n", target);
		}
	}
}

void TextureRename(char* target, const char prefix[], const char prefix2[], const char agentName[], const char bodyTypeString[], const char postfix[]) {
	strcpy(target, ".\\output\\Textures\\");
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
void CheckTextureFolder(AgentType agentType) {
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

		if (true) {
			strcpy(temp, ".\\output\\Textures\\UI\\Image\\");
			strcat(temp, agentType.Upper);
			i += _mkdir(".\\output\\Textures\\UI");
			i += _mkdir(".\\output\\Textures\\UI\\Image");
			i += _mkdir(temp);
		}
	}

	/*if (i != 0) {
		printf("��ͼ�ļ��в��������޷������µ�.\n");
		AnyKeyQuit();
	}*/

	return;
}

void CheckTexture(bool hasHat) {
	int checkResult = 0;
	checkResult += _access(".\\input\\aa_bbb_south.png", 6);
	checkResult += _access(".\\input\\aa_bbb_north.png", 6);
	checkResult += _access(".\\input\\aa_bbb_east.png", 6);
	//access����ģʽ��0ֻ���ڣ�2дȨ�ޣ�4��Ȩ�ޣ�6д�Ͷ�Ȩ��
	//checkResult += _access(".\\input\\aa.png", 6);
	checkResult += _access(".\\input\\aaW.png", 6);
	checkResult += _access(".\\input\\aa_southHair.png", 6);
	checkResult += _access(".\\input\\aa_northHair.png", 6);
	checkResult += _access(".\\input\\aa_eastHair.png", 6);

	if (hasHat == true) {
		//checkResult += _access(".\\input\\aaH.png", 6);
		checkResult += _access(".\\input\\aaH_south.png", 6);
		checkResult += _access(".\\input\\aaH_north.png", 6);
		checkResult += _access(".\\input\\aaH_east.png", 6);
	}

	if (checkResult != 0) {
		printf("ȱ����ͼ�ļ����ļ�������ȷ, 2\n");
		AnyKeyQuit();
	}

	return;
}

void MoveTexture(Settings settings) {

	//char* oldName = (char*)malloc(sizeof(char) * 50);
	char* newName = (char*)malloc(sizeof(char) * 100);

	TextureRename(newName, "Things\\AK_Agents\\", settings.agentType.Upper, settings.agentName.English, "-1", "");
	rename(".\\input\\aa.png", newName);

	TextureRename(newName, "Things\\AK_Agents\\", settings.agentType.Upper, settings.agentName.English, settings.bodyType, "south");
	//FindOrigTexture(oldName, settings.bodyType, "south");
	int i = rename(".\\input\\aa_bbb_south.png", newName);

	TextureRename(newName, "Things\\AK_Agents\\", settings.agentType.Upper, settings.agentName.English, settings.bodyType, "north");
	//FindOrigTexture(oldName, settings.bodyType, "north");
	i += rename(".\\input\\aa_bbb_north.png", newName);

	TextureRename(newName, "Things\\AK_Agents\\", settings.agentType.Upper, settings.agentName.English, settings.bodyType, "east");
	//FindOrigTexture(oldName, settings.bodyType, "east");
	i += rename(".\\input\\aa_bbb_east.png", newName);

	TextureRename(newName, "Things\\AK_Agents\\Weapon\\", settings.agentType.Upper, settings.agentName.English, "-1", "W");
	i += rename(".\\input\\aaW.png", newName);

	TextureRename(newName, "Things\\Hair\\", settings.agentType.Upper, settings.agentName.English, "-1", "_south");
	i += rename(".\\input\\aa_southHair.png", newName);

	TextureRename(newName, "Things\\Hair\\", settings.agentType.Upper, settings.agentName.English, "-1", "_north");
	i += rename(".\\input\\aa_northHair.png", newName);

	TextureRename(newName, "Things\\Hair\\", settings.agentType.Upper, settings.agentName.English, "-1", "_east");
	i += rename(".\\input\\aa_eastHair.png", newName);

	if (settings.has[hat] == true) {
		TextureRename(newName, "Things\\AK_Agents\\HatAccessory\\", "-1", settings.agentName.English, "-1", "H");
		rename(".\\input\\aaH.png", newName);

		TextureRename(newName, "Things\\AK_Agents\\HatAccessory\\", "-1", settings.agentName.English, "-1", "H_south");
		i += rename(".\\input\\aaH_south.png", newName);

		TextureRename(newName, "Things\\AK_Agents\\HatAccessory\\", "-1", settings.agentName.English, "-1", "H_north");
		i += rename(".\\input\\aaH_north.png", newName);

		TextureRename(newName, "Things\\AK_Agents\\HatAccessory\\", "-1", settings.agentName.English, "-1", "H_east");
		i += rename(".\\input\\aaH_east.png", newName);
	}

	if (CheckStandPortrait() == true) {
		printf("�ҵ�������ͷ���ļ�.\n");
		TextureRename(newName, "UI\\Image\\", settings.agentType.Upper, settings.agentName.English, "-1", "Stand");
		i += rename(".\\input\\aaStand.png", newName);

		TextureRename(newName, "UI\\Image\\", settings.agentType.Upper, settings.agentName.English, "-1", "Portrait");
		i += rename(".\\input\\aaPortrait.png", newName);
	}
	else printf("û��ͷ�������\n");

	if (i != 0) {
		printf("�޷���ȷ���ƶ�ȫ����ͼ.\n");
		AnyKeyQuit();
	}

	return;
}

void printTraits(RBSTree rt) {
	if (!rt) return;
	printf("%s %d\n", rt->string, rt->degree);
	printTraits(rt->left);
	printTraits(rt->right);
	return;
}

void printOperator(Settings settings) {
	printf("\nName: %s\n", settings.agentName.English);
	printf("ñ��: %d\n", settings.has[hat]);
	printf("Type: %s\n", settings.agentType.Upper);
	char input = 'x';
	if (settings.generateMode == 1) {
		printf("�Ƿ�ȷ�Ͽ�ʼ�������棬Y/N\n");
		scanf("%c", &input);
		if (input != 'y' && input != 'Y') {
			printf("ȡ��.\n", input);
			AnyKeyQuit();
		}
		return;
	}
	else {
		#define _SS settings.skills
		printf(
			"Age: %d\n"
			"BS: %d %d\n"
			"BodyType: %s\n"
			,settings.age, settings.story.backstory[0], settings.story.backstory[1], settings.bodyType
		);

		PrintVoiceCount();

	#pragma region PrintSkill&Fire
		if (false) {
			printf(
				"Skills:\n"
				"\t Animals: %d, FireLevel: %d\n"
				"\t Artistic: %d, FireLevel: %d\n"
				"\t Construction: %d, FireLevel: %d\n"
				"\t Cooking: %d, FireLevel: %d\n"
				"\t Crafting: %d, FireLevel: %d\n"
				"\t Intellectual: %d, FireLevel: %d\n"
				"\t Medicine: %d, FireLevel: %d\n"
				"\t Melee: %d, FireLevel: %d\n"
				"\t Mining: %d, FireLevel: %d\n"
				"\t Plants: %d, FireLevel: %d\n"
				"\t Shooting: %d, FireLevel: %d\n"
				"\t Social: %d, FireLevel: %d\n",
				_SS.animals, _SS.animalsFire,
				_SS.art, _SS.artFire,
				_SS.construction, _SS.constructionFire,
				_SS.cook, _SS.cookFire,
				_SS.craft, _SS.craftFire,
				_SS.intellect, _SS.intellectFire,
				_SS.medi, _SS.mediFire,
				_SS.melee, _SS.meleeFire,
				_SS.mining, _SS.miningFire,
				_SS.plants, _SS.plantsFire,
				_SS.shoot, _SS.shootFire,
				_SS.social, _SS.socialFire
			);
		}
	#pragma endregion
		
		printf("Traits:\n");
		printTraits(settings.traitsRoot->node);
		printf("�Ƿ�ȷ�Ͽ�ʼ�����˸�Ա��Y/N\n");
		scanf("%c", &input);
		if (input != 'Y' && input != 'y') {
			printf("ȡ��.\n");
			AnyKeyQuit();
		}
	}
	return;
}

void CheckAll(Settings settings) {
		int i = 0;
	//���xml�ļ�β
		if (!settings.has[hat]) ++i;
		for (; i < fileCnt; ++i) {
			printf("���%d���ļ�β ", i);
			FileEndCheck(settings.outputFile.outputFile[i]);
			if (i == f_voice) continue; //voiceDef����Ҫ����
			printf("���%d�ŷ����ļ�β ", i);
			FileEndCheckTranslate(settings.outputFile.outputFileTranslate[i]);
		}

		CheckTextureFolder(settings.agentType);
		CheckTexture(settings.has[hat]);

		printOperator(settings);

		if (!settings.debugOverride) MoveTexture(settings);
		if (settings.generateMode == 1) {
			printf("�������ͼ����.");
			AnyKeyQuit();
		}


	//�ڼ��֮���Ƴ��ļ�β
		i = 0;
		if (!settings.has[hat]) ++i;
		for (; i < fileCnt; ++i) {
			FileEndRemove(settings.outputFile.outputFile[i]);
			if (i == f_voice) continue; //voiceDef����Ҫ����
			FileEndRemoveTranslate(settings.outputFile.outputFileTranslate[i]);
		}

	return;
}
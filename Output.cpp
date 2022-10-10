#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <io.h>
#include "TypeDef.h"

void numToEnglish(FILE* fp, int i) {
	switch (i) {
	case 2:
		fprintf(fp, "Second");
		break;
	case 3:
		fprintf(fp, "Third");
		break;
	case 4:
		fprintf(fp, "Fourth");
		break;
	default:
		break;
	}
	return;
}

void GiveOperatorTrait(FILE* fp, RBSTree node, char name[]) {
	if (!node) return;
	if (strcmp(node->string, "Personal") == 0) {
		for (int i = node->degree; i > 0; --i) {
			fprintf(fp, "\t\t\t<li><def>AK_Trait_%s", name);
			numToEnglish(fp, i);
			fprintf(fp, "</def></li>\n");
		}
	}
	else {
		fprintf(fp, "\t\t\t<li>\n\t\t\t\t<def>%s</def>\n", node->string);
		if (node->degree != 0) {
			fprintf(fp, "\t\t\t\t<degree>%d</degree>\n", node->degree);
		}
		fprintf(fp, "\t\t\t</li>\n");
	}
	GiveOperatorTrait(fp, node->left, name);
	GiveOperatorTrait(fp, node->right, name);
	return;
}

void NewTraitGenerate(FILE *traitFP, FILE *thoughtFP, int degree, int *thought, char name[]) {
	if (degree == 0) return;
	fprintf(traitFP, "\t<TraitDef>\n"
		"\t\t<ignoreConfigErrors>true</ignoreConfigErrors>\n"
		"\t\t<commonality>0.0</commonality>\n"
		"\t\t<defName>AK_Trait_%s", name);
	numToEnglish(traitFP, degree);
	fprintf(traitFP, "</defName>\n"
		"\t\t<degreeDatas>\n"
		"\t\t\t<li>\n"
		"\t\t\t\t<label></label>\n"
		"\t\t\t\t<description>\n\t\t\t\t</description>\n"
		"\t\t\t</li>\n"
		"\t\t</degreeDatas>\n"
		"\t</TraitDef>\n\n");
	if (thought[degree - 1] != 0) {
		fprintf(thoughtFP, "\t<ThoughtDef ParentName = \"AK_Thought_Permanent\">\n"
			"\t\t<defName>AK_Thought_Trait_%s</defName>\n"
			"\t\t<requiredTraits><li>AK_Trait_%s</li></requiredTraits>\n"
			"\t\t<stages>\n"
			"\t\t\t<li>\n"
			"\t\t\t\t<label></label>\n"
			"\t\t\t\t<description></description>\n"
			"\t\t\t\t<baseMoodEffect>%d</baseMoodEffect>\n"
			"\t\t\t</li>\n"
			"\t\t</stages>\n"
			"\t</ThoughtDef>\n\n", name, name, thought[degree - 1]);
	}
	NewTraitGenerate(traitFP, thoughtFP, degree - 1, thought, name);
}

void FileProcess(Settings settings) {
	UTF8ThingsType UTF8Type;
	rewind(settings.outputFile.UTF8ThingsTypeFile);
	fscanf(settings.outputFile.UTF8ThingsTypeFile, "%s %s %s %s", UTF8Type.apparel, UTF8Type.weapon, UTF8Type.hair, UTF8Type.hat);

	//----------生成招募干员。本体是全英语，而翻译是中文。
	printf("正在处理 OperatorDef.xml\n");
	printf("\t正在处理 干员:%s\n", settings.agentName.English);
	fprintf(settings.outputFile.outputFile[f_operator],
		"\t<AK_DLL.OperatorDef ParentName=\"AK_RecruitBase_%s\">\n"
		"\t\t<defName>AK_Recruit_%s</defName>\n"
		"\t\t<name>%s</name>\n"
		"\t\t<nickname>%s</nickname>\n"
		"\t\t<description>%s</description>\n"
		"\t\t<weapon>AK_Weapon_%s</weapon>\n"
		"\t\t<hair>AK_Hair_%s</hair>\n"
		"\t\t<apparels>\n\t\t\t<li>AK_Apparel_%s</li>\n", settings.agentType.Upper, settings.agentName.English, settings.agentName.English, settings.agentName.English, settings.descEng , settings.agentName.English, settings.agentName.English, settings.agentName.English);
	if (settings.hasHat == true) {
		fprintf(settings.outputFile.outputFile[f_operator],
			"\t\t\t<li>AK_Hat_%s</li>\n", settings.agentName.English);
	}
	fprintf(settings.outputFile.outputFile[f_operator],
		"\t\t</apparels>\n"
		"\t\t<stand>UI/Image/%s/%sStand</stand>\n"
		"\t\t<headPortrait>UI/Image/%s/%sPortrait</headPortrait>\n", settings.agentType.Upper, settings.agentName.English, settings.agentType.Upper, settings.agentName.English);
	if (strcmp(settings.bodyType, "Thin") != 0) {
		fprintf(settings.outputFile.outputFile[f_operator],
			"\t\t<bodyTypeDef>%s</bodyTypeDef>\n", settings.bodyType);
	}

	fprintf(settings.outputFile.outputFile[f_operator], "\t\t<traits>\n");
	GiveOperatorTrait(settings.outputFile.outputFile[f_operator], settings.traitsRoot->node, settings.agentName.English);
	fprintf(settings.outputFile.outputFile[f_operator], "\t\t</traits>\n");


	if (settings.age != 16) fprintf(settings.outputFile.outputFile[f_operator], "\t\t<age>%d</age>\n", settings.age);
	if (settings.story.backstory[0] == 1)fprintf(settings.outputFile.outputFile[f_operator], "\t\t<childHood>AK_BackStory_%s_Child</childHood>\n", settings.agentName.English);
	if (settings.age > 16) fprintf(settings.outputFile.outputFile[f_operator], "\t\t<adultHood>AK_BackStory_%s_Adult</adultHood>\n", (settings.story.backstory[1] == 1 ? settings.agentName.English : "Unknown"));
	
	#define _SS settings.skills
	fprintf(settings.outputFile.outputFile[f_operator], "\t\t<skills>\n"
		"\t\t\t<li>\n\t\t\t\t<skill>Animals</skill>\n\t\t\t\t<level>%d</level>\n\t\t\t\t<fireLevel>%d</fireLevel>\n\t\t\t</li>\n"
		"\t\t\t<li>\n\t\t\t\t<skill>Artistic</skill>\n\t\t\t\t<level>%d</level>\n\t\t\t\t<fireLevel>%d</fireLevel>\n\t\t\t</li>\n"
		"\t\t\t<li>\n\t\t\t\t<skill>Construction</skill>\n\t\t\t\t<level>%d</level>\n\t\t\t\t<fireLevel>%d</fireLevel>\n\t\t\t</li>\n"
		"\t\t\t<li>\n\t\t\t\t<skill>Cooking</skill>\n\t\t\t\t<level>%d</level>\n\t\t\t\t<fireLevel>%d</fireLevel>\n\t\t\t</li>\n"
		"\t\t\t<li>\n\t\t\t\t<skill>Crafting</skill>\n\t\t\t\t<level>%d</level>\n\t\t\t\t<fireLevel>%d</fireLevel>\n\t\t\t</li>\n"
		"\t\t\t<li>\n\t\t\t\t<skill>Intellectual</skill>\n\t\t\t\t<level>%d</level>\n\t\t\t\t<fireLevel>%d</fireLevel>\n\t\t\t</li>\n"
		"\t\t\t<li>\n\t\t\t\t<skill>Medicine</skill>\n\t\t\t\t<level>%d</level>\n\t\t\t\t<fireLevel>%d</fireLevel>\n\t\t\t</li>\n"
		"\t\t\t<li>\n\t\t\t\t<skill>Melee</skill>\n\t\t\t\t<level>%d</level>\n\t\t\t\t<fireLevel>%d</fireLevel>\n\t\t\t</li>\n"
		"\t\t\t<li>\n\t\t\t\t<skill>Mining</skill>\n\t\t\t\t<level>%d</level>\n\t\t\t\t<fireLevel>%d</fireLevel>\n\t\t\t</li>\n"
		"\t\t\t<li>\n\t\t\t\t<skill>Plants</skill>\n\t\t\t\t<level>%d</level>\n\t\t\t\t<fireLevel>%d</fireLevel>\n\t\t\t</li>\n"
		"\t\t\t<li>\n\t\t\t\t<skill>Shooting</skill>\n\t\t\t\t<level>%d</level>\n\t\t\t\t<fireLevel>%d</fireLevel>\n\t\t\t</li>\n"
		"\t\t\t<li>\n\t\t\t\t<skill>Social</skill>\n\t\t\t\t<level>%d</level>\n\t\t\t\t<fireLevel>%d</fireLevel>\n\t\t\t</li>\n",
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
		_SS.social, _SS.socialFire);
	fprintf(settings.outputFile.outputFile[f_operator], "\t\t</skills>\n");

	fprintf(settings.outputFile.outputFile[f_operator],
		"\t</AK_DLL.OperatorDef>\n\n");
		
	printf("处理完成.\n");
	fprintf(settings.outputFile.outputFile[f_operator], "</Defs>");
	fclose(settings.outputFile.outputFile[f_operator]);

	//----------翻译
	printf("正在处理 OperatorDef.xml 的翻译文件.\n");
	printf("\t正在处理 干员%s\n", settings.agentName.English);
	fprintf(settings.outputFile.outputFileTranslate[f_operator],
		"\t<AK_Recruit_%s.name>%s</AK_Recruit_%s.name>\n"
		"\t<AK_Recruit_%s.nickname>%s</AK_Recruit_%s.nickname>\n"
		"\t<AK_Recruit_%s.description>%s</AK_Recruit_%s.description>\n\n", settings.agentName.English, settings.agentName.Chinese, settings.agentName.English, settings.agentName.English, settings.agentName.Chinese, settings.agentName.English, settings.agentName.English, settings.descChi, settings.agentName.English);
		
	printf("处理完成.\n");

	fprintf(settings.outputFile.outputFileTranslate[f_operator], "</LanguageData>");
	fclose(settings.outputFile.outputFileTranslate[f_operator]);

	//---------------生成服装。本体是中文，而翻译是英语，下同。-------------------------------------------------------------------------------------
	printf("正在处理 ApparelDef.xml\n");
		printf("\t正在处理 干员:%s\n", settings.agentName.English);
		fprintf(settings.outputFile.outputFile[f_apparel],
			"\t<ThingDef ParentName=\"AK_ArmorBase_%s\">\n"
			"\t\t<defName>AK_Apparel_%s</defName>\n"
			"\t\t<label>%s%s</label>\n"
			"\t\t<graphicData>\n"
			"\t\t\t<texPath>Things/AK_Agents/GApparel</texPath>\n"
			"\t\t\t<graphicClass>Graphic_Single</graphicClass>\n"
			"\t\t</graphicData>\n"
			"\t\t<apparel>\n"
			"\t\t\t<wornGraphicPath>Things/AK_Agents/%s/%s</wornGraphicPath>\n"
			"\t\t</apparel>\n"
			"\t</ThingDef>\n\n", settings.agentType.Upper, settings.agentName.English, settings.agentName.Chinese, UTF8Type.apparel, settings.agentType.Upper, settings.agentName.English);
	printf("处理完成.\n");
	fprintf(settings.outputFile.outputFile[f_apparel], "</Defs>");
	fclose(settings.outputFile.outputFile[f_apparel]);

	//
	printf("正在处理 ApparelDef.xml 的翻译文件\n");
		printf("\t正在处理 干员:%s\n", settings.agentName.English);
		fprintf(settings.outputFile.outputFileTranslate[f_apparel],
			"\t<AK_Apparel_%s.label>%s's Apparel</AK_Apparel_%s.label>\n\n", settings.agentName.English, settings.agentName.English, settings.agentName.English);
		
	printf("处理完成.\n");
	fprintf(settings.outputFile.outputFileTranslate[f_apparel], "</LanguageData>");
	fclose(settings.outputFile.outputFileTranslate[f_apparel]);

	//帽子
	if (settings.hasHat == true) {
		printf("正在处理帽子\n");
		fprintf(settings.outputFile.outputFile[f_hat],
			"\t<ThingDef ParentName=\"AK_HatBase\">\n"
			"\t\t<defName>AK_Hat_%s</defName>\n"
			"\t\t<label>%s%s</label>\n"
			"\t\t<graphicData>\n"
			"\t\t\t<texPath>Things/AK_Agents/HatAccessory/GHat</texPath>\n"
			"\t\t\t<graphicClass>Graphic_Single</graphicClass>\n"
			"\t\t</graphicData>\n"
			"\t\t<apparel>\n"
			"\t\t\t<wornGraphicPath>Things/AK_Agents/HatAccessory/%sH</wornGraphicPath>\n"
			"\t\t</apparel>\n"
			"\t</ThingDef>\n\n"
			"</Defs>", settings.agentName.English, settings.agentName.Chinese, UTF8Type.hat, settings.agentName.English);
		fclose(settings.outputFile.outputFile[f_hat]);

		fprintf(settings.outputFile.outputFileTranslate[f_hat],
			"\t<AK_Hat_%s.label>%s's Hat</AK_Hat_%s.label>\n\n"
			"</LanguageData>", settings.agentName.English, settings.agentName.English, settings.agentName.English);
		printf("处理完成.\n");
		fclose(settings.outputFile.outputFileTranslate[f_hat]);
	}

//------------武器----------------------------------------------------------------------------
	printf("正在处理 WeaponDef.xml\n");
		fprintf(settings.outputFile.outputFile[f_weapon],
			"\t<ThingDef ParentName=\"AK_WeaponBase_%s\">\n"
			"\t\t<defName>AK_Weapon_%s</defName>\n"
			"\t\t<label>%s%s</label>\n"
			"\t\t<graphicData>\n"
			"\t\t\t<texPath>Things/AK_Agents/Weapon/%s/%sW</texPath>\n"
			"\t\t\t<graphicClass>Graphic_Single</graphicClass>\n"
			"\t\t</graphicData>\n"
			"\t</ThingDef>\n\n", settings.agentType.Upper, settings.agentName.English, settings.agentName.Chinese, UTF8Type.weapon, settings.agentType.Upper, settings.agentName.English);
	printf("处理完成.\n");
	fprintf(settings.outputFile.outputFile[f_weapon], "</Defs>");
	fclose(settings.outputFile.outputFile[f_weapon]);

	printf("正在处理 WeaponDef.xml 的翻译文件\n");
		fprintf(settings.outputFile.outputFileTranslate[f_weapon],
			"\t<AK_Weapon_%s.label>%s's Weapon</AK_Weapon_%s.label>\n\n", settings.agentName.English, settings.agentName.English, settings.agentName.English);
	printf("处理完成.\n");
	fprintf(settings.outputFile.outputFileTranslate[f_weapon], "</LanguageData>");
	fclose(settings.outputFile.outputFileTranslate[f_weapon]);
	
//-----发型-------------------------------------------------
	printf("正在处理 HairDef.xml\n");
	fprintf(settings.outputFile.outputFile[f_hair],
			"\t<HairDef ParentName=\"AK_HairBaseF\">\n"
			"\t\t<defName>AK_Hair_%s</defName>\n"
			"\t\t<label>%s%s</label>\n"
			"\t\t<texPath>Things/Hair/%s/%s</texPath>\n"
			"\t</HairDef>\n\n", settings.agentName.English, settings.agentName.Chinese, UTF8Type.hair, settings.agentType.Upper, settings.agentName.English);
	printf("处理完成.\n");
	fprintf(settings.outputFile.outputFile[f_hair], "</Defs>");
	fclose(settings.outputFile.outputFile[f_hair]);
		
	printf("正在处理 HairDef.xml 的翻译文件\n");
		printf("\t正在处理 干员:%s\n", settings.agentName.English);
		fprintf(settings.outputFile.outputFileTranslate[f_hair],
			"\t<AK_Hair_%s.label>%s's Hair</AK_Hair_%s.label>\n\n", settings.agentName.English, settings.agentName.English, settings.agentName.English);
	printf("处理完成.\n");
	fprintf(settings.outputFile.outputFileTranslate[f_hair], "</LanguageData>");
	fclose(settings.outputFile.outputFileTranslate[f_hair]);


	//背景故事
	printf("正在处理 背景故事\n");
	if (settings.story.backstory[0] == 1) {
		fprintf(settings.outputFile.outputFile[f_backstory],
			"\t<StoriesRetold.SRBackstoryDef>\n"
			"\t\t<defName>AK_BackStory_%s_Child</defName>\n"
			"\t\t<slot>Childhood</slot>\n"
			"\t\t<title>%s</title>\n"
			"\t\t<baseDesc>%s</baseDesc>\n"
			"\t\t<bodyTypeGlobal>Thin</bodyTypeGlobal>\n"
			"\t</StoriesRetold.SRBackstoryDef>\n\n", settings.agentName.English, settings.story.childName, settings.story.childDesc);
	}
	if (settings.story.backstory[1] == 1) {
		fprintf(settings.outputFile.outputFile[f_backstory],
			"\t<StoriesRetold.SRBackstoryDef>\n"
			"\t\t<defName>AK_BackStory_%s_Adult</defName>\n"
			"\t\t<slot>Adulthood</slot>\n"
			"\t\t<title>%s</title>\n"
			"\t\t<baseDesc>%s</baseDesc>\n"
			"\t\t<bodyTypeGlobal>Thin</bodyTypeGlobal>\n"
			"\t</StoriesRetold.SRBackstoryDef>\n\n", settings.agentName.English, settings.story.adultName, settings.story.adultDesc);
	}
	printf("处理完成.\n");
	fprintf(settings.outputFile.outputFile[f_backstory], "</Defs>");

	//翻译。没做。
	fprintf(settings.outputFile.outputFileTranslate[f_backstory], "\n</LanguageData>");

	//特性
	printf("正在处理 特性\n");
	NewTraitGenerate(settings.outputFile.outputFile[f_trait], settings.outputFile.outputFile[f_thoughts], rbts_search_returnNode(settings.traitsRoot, "Personal", 1)->degree, settings.thought, settings.agentName.English);
	printf("处理完成.\n");
	fprintf(settings.outputFile.outputFile[f_trait], "</Defs>");
	fprintf(settings.outputFile.outputFile[f_thoughts], "</Defs>");

	//翻译。没做。
	fprintf(settings.outputFile.outputFileTranslate[f_trait], "\n</LanguageData>");
	fprintf(settings.outputFile.outputFileTranslate[f_thoughts], "\n</LanguageData>");
	return;
}
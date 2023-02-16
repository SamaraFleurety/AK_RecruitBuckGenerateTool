#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "TypeDef.h"
#include "PrintOperator.h"
#include "VoicePackProcess.h"

void printTraits(RBSTree rt) {
	if (!rt) return;
	printf("%s %d\n", rt->string, rt->degree);
	printTraits(rt->left);
	printTraits(rt->right);
	return;
}

void printOperator(Settings* settings) {
	printf("\nName: %s\n", settings->agentName.English);
	printf("帽子: %d\n", settings->has[hat]);
	printf("Type: %s\n", settings->agentType.Upper);
	char input = 'x';
	if (settings->generateMode == 1) {
		printf("是否确认开始重置立绘，Y/N\n");
		scanf("%c", &input);
		if (input != 'y' && input != 'Y') {
			printf("取消.\n", input);
			AnyKeyQuit();
		}
		return;
	}
	else {
		printf(
			"Age: %d\n"
			"BS: %d %d\n"
			"BodyType: %s\n"
			, settings->age, settings->story.backstory[0], settings->story.backstory[1], settings->bodyType
		);


#pragma region PrintSkill&Fire
#define _SS settings->skills
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
		printTraits(settings->traitsRoot->node);
		printf("是否确认开始新增此干员，Y/N\n");
		scanf("%c", &input);
		if (input != 'Y' && input != 'y') {
			printf("取消.\n");
			AnyKeyQuit();
		}
	}
	return;
}
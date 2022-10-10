#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <io.h>
#include "InputData.h"
#include "Output.h"
#include "TextureProcess.h"
#include "VoicePackProcess.h"
#include "xmlEndProcess.h"
#include "ANSI-UNICODE-UTF8.h"
#include "PrintOperator.h"

void CheckAll(Settings* settings) {
	if (!settings->debugOverride)FileEndCheck_All(settings);

	CreateTextureFolder(settings->agentType);
	if (!settings->debugOverride) CheckTexture(settings->has[hat]);

	printOperator(settings);

	return;
}

int main() {
	//我不知道为啥用settings指针会报错。别改。
	printf("Version.22-10-11\n");
	PrintVoiceCount();
	Settings settings = InitializeSettings();
	settings.debugOverride = false;
	CheckAll(&settings);

	if (!settings.debugOverride) MoveTexture(settings);

	if (!settings.debugOverride) FileEndRemove_All(&settings);
	AutoProcessVoicePack(&settings);
	
	FileProcess(settings);
	FileEndRestore_All(&settings);

	printf("执行完成.");
	system("Pause");
	return 0;
}
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include "ReadConfig.h"
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
	printf("Version.24-03-15\n");
	Settings* settings = InitializeSettings();
	settings->debugOverride = false;
	settings->enableHatFeature = false;
	AutoProcessVoiceFile(settings);
	CheckAll(settings);

	if (!settings->debugOverride) MoveTexture(settings);

	FileEndRemove_All(settings);
	AutoProcessVoicePack(settings);

	FileProcess(settings);
	FileEndRestore_All(settings);

	printf("ִ�����.");
	system("Pause");
	return 0;
}
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <io.h>
#include "InputData.h"
#include "Output.h"
#include "CheckFile.h"
#include "ANSI-UNICODE-UTF8.h"

int main() {
	//�Ҳ�֪��Ϊɶ��settingsָ��ᱨ����ġ�
	Settings settings = InitializeSettings();
	CheckAll(settings);

	FileProcess(settings);

	printf("ִ�����.");
	system("Pause");
	return 0;
}
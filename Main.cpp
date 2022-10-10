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
	//我不知道为啥用settings指针会报错。别改。
	Settings settings = InitializeSettings();
	CheckAll(settings);

	FileProcess(settings);

	printf("执行完成.");
	system("Pause");
	return 0;
}
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include "TypeDef.h"
#include "xmlEndProcess.h"

void FileEndCheck(FILE* targetFile) {
	char fileEnd[25] = "\0";

	fseek(targetFile, -7, SEEK_END);
	fgets(fileEnd, 20, targetFile);
	printf("\t得到:%s  ", fileEnd);
	if (strcmp(fileEnd, "</Defs>") == 0) {
		printf("文件尾正确\n");
	}
	else {
		printf("文件尾错误.按任意键退出.\n");
		AnyKeyQuit();
	}
}

void FileEndCheckTranslate(FILE* targetFile) {
	char fileEnd[25] = "\0";

	fseek(targetFile, -15, SEEK_END);
	fgets(fileEnd, 20, targetFile);
	//printf("\t得到:%s\n", fileEnd);
	if (strcmp(fileEnd, "</LanguageData>") == 0) {
		printf("文件尾正确\n");
	}
	else {
		printf("文件尾错误.按任意键退出.\n");
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
	rewind(targetFile);
}

void FileEndRestore(FILE* file) {
	fprintf(file, "</Defs>");
	fclose(file);
}

void FileEndRestoreTranslate(FILE* file) {
	fprintf(file, "</LanguageData>");
	fclose(file);
}

void FileEndCheck_All(Settings* settings) {
	int i = 0;
	if (!settings->has[hat]) ++i;
	for (; i < fileCnt; ++i) {
		printf("检测%d号文件尾 ", i);
		FileEndCheck(settings->outputFile.outputFile[i]);
		if (i == f_voice) continue; //voiceDef不需要翻译
		//printf("检测%d号翻译文件尾 ", i);
		//FileEndCheckTranslate(settings->outputFile.outputFileTranslate[i]);
	}
}

void FileEndRemove_All(Settings* settings) {
	int i = 0;
	if (!settings->has[hat]) ++i;
	for (; i < fileCnt; ++i) {
		FileEndRemove(settings->outputFile.outputFile[i]);
		if (i == f_voice) continue; //voiceDef不需要翻译
		if (i == f_operator) FileEndRemoveTranslate(settings->outputFile.outputFileTranslate[i]);
		//FileEndRemoveTranslate(settings->outputFile.outputFileTranslate[i]);
	}
}

void FileEndRestore_All(Settings* settings) {
	int i = 0;
	if (!settings->has[hat]) ++i;
	for (; i < fileCnt; ++i) {
		FileEndRestore(settings->outputFile.outputFile[i]);
		if (i == f_voice) continue; //voiceDef不需要翻译
		if (i == f_operator) FileEndRestoreTranslate(settings->outputFile.outputFileTranslate[i]);
	}
}
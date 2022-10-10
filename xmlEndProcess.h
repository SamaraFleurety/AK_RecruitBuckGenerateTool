#ifndef _xmlEndProcess_h
#define _xmlEndProcess_h

void FileEndRestore(FILE* file);

//检测xml文件尾
void FileEndCheck_All(Settings* settings);

//移除文件尾
void FileEndRemove_All(Settings* settings);

//恢复文件尾
void FileEndRestore_All(Settings* settings);

#endif

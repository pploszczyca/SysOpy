#ifndef block_library_h
#define block_library_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct rowMergedFile{
    int n_rows;
    char **rows;
};

struct rowMergedFile ** makeMainTable(size_t n_elements);
FILE* openFileToRead(char *nameOfTheFile);
int getFileLines(FILE *fp);
void addFileToArray(FILE *fileToAdd, struct rowMergedFile **arrayOfFiles, int index, int isEven);
void addFilesToArray(FILE *firstFile, FILE *secondFile, struct rowMergedFile **arrayOfFiles, int index);
FILE * mergeArrayToTemporaryFile(struct rowMergedFile **arrayOfFiles, int index);
void readFromTemporaryFileToArray(FILE* temp_file, struct rowMergedFile **arrayOfFiles, int index);
void print3DArray(struct rowMergedFile ** array, int n_array);
void removeOneLineFromBlock(struct rowMergedFile ** array, int blockIndex, int lineIndex);
void removeOneBlock(struct rowMergedFile ** array, int blockIndex);
char ** splitFileNames(char **notSplitFileNames, int n_fileNames);
char **splitTwoFileNames(char *notSplitFileNames);
#endif //block_library_h
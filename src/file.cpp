/**
 * Definitions for the file.h Header File
 */

#pragma once
#include "file.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

/**
 * Takes the filepath as input and returns a pointer to buffer which
 * holds the content of file
 * 
 * If return NULL, then some problem occured while retrieving file.
 * Possibly file doesn't exists.
 */
file::FileData *file::openFile(char *filePath) 
{
    // Gather Stats about the file
    struct stat buf;
    if (stat(filePath, &buf) == -1) 
    {
        return NULL;
    }

    // Check if it's a regular File
    if (!(buf.st_mode & S_IFREG))
    {
        return NULL;
    }

    // Open File for reading 
    FILE *filePointer = fopen(filePath, "rb");
    if (filePointer == NULL) 
    {
        return NULL;
    }

    // Allocate the size of file to store file in buffer
    int fileSize = buf.st_size;
    char *fileBuffer = (char *)malloc(fileSize);
    if (fileBuffer == NULL) 
    {
        fclose(filePointer);
        return NULL;
    }

    // Read the entire file
    size_t result = fread(fileBuffer, 1, fileSize, filePointer);
    if (result != fileSize) 
    {
        fclose(filePointer);
        free(fileBuffer);
        return NULL;
    }
    fclose(filePointer);

    // Store the content of buffer in FileData structure
    file::FileData *currFile = (file::FileData *)(sizeof(file::FileData));
    if (currFile == NULL) 
    {
        free(fileBuffer);
        return NULL;
    }
    currFile -> data = fileBuffer;
    currFile -> sizeOfFile = fileSize;
    return currFile;
}

/** 
 * deallocates the file buffer and currfile structure
 */
void file::closeFile(file::FileData *currFile) 
{
    free(currFile -> data);
    free(currFile);
}
/**
 * Header file for file handling library
 */
#pragma once
#include <stdio.h>
#include <string>

namespace file
{
    struct FileData 
    {
        char *data;
        int sizeOfFile;
    };

    FileData *openFile(char *);
    void closeFile(FileData *);
} // namespace file
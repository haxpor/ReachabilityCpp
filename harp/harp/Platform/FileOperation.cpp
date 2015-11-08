//
//  FileOperation.cpp
//  ZombieHero
//
//  Created by haxpor on 2/3/14.
//
//

#include "FileOperation.h"
#include "cocos2d.h"
#include <sys/stat.h>

using namespace harp;

bool FileOperation::write(const char* path, const char* buf, int size)
{
    FILE *fp = fopen(path, "wb");
    if(fp == NULL)
    {
        CCLOG("Error open file for writing at %s", path);
        return false;
    }
    
    // write with the whole chunk with size
    size_t wBytes = fwrite(buf, sizeof(unsigned char), size, fp);
    fclose(fp);
    
    CCLOG("Write file to %s", path);
    CCLOG("Written content (%d bytes)", wBytes);
    
    return true;
}

bool FileOperation::read(const char* path, char* outBuf, int size, size_t &readSize)
{
    FILE *fp = fopen(path, "rb");
    if(fp == NULL)
    {
        CCLOG("Error open file for reading at %s", path);
        return false;
    }
    
    // read the whole chunk with size
    readSize = fread(outBuf, sizeof(unsigned char), size, fp);
    fclose(fp);
    
    CCLOG("Read file at %s", path);
    CCLOG("Read content (%d bytes)", readSize);
    
    return true;
}

bool FileOperation::exist(const char* path)
{
    struct stat info;
    int ret = stat(path, &info);
    if(ret == -1)
    {
        CCLOG("File at %s doesn't exist.", path);
        return false;
    }
    
    // file exists
    return true;
}

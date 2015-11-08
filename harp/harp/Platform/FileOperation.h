//
//  FileOperation.h
//  ZombieHero
//
//  Created by haxpor on 2/3/14.
//
//

#ifndef __ZombieHero__FileOperation__
#define __ZombieHero__FileOperation__

#include <string>
#include "CoreMacros.h"

NS_S_HARPENGINE

class FileOperation
{
public:
    static bool write(const char* path, const char* buf, int size);
    static bool read(const char* path, char* outBuf, int size, size_t &readSize);
    
    static bool exist(const char* path);
};

NS_E_HARPENGINE

#endif /* defined(__ZombieHero__FileOperation__) */

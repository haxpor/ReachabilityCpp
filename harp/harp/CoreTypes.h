//
//  CoreTypes.h
//  ZombieHero
//
//  Created by haxpor on 4/22/13.
//
//

#ifndef ZombieHero_CoreTypes_h
#define ZombieHero_CoreTypes_h

// for scene management
#define BUFFERSCENE_BUFFERLAYER_STATE 0

/*
 Dialogue settings working with .zhd config file.
 */
struct DialogueSetting
{
    char *code;
    char *dtextCode;
    int portraitPosition;
};

#endif

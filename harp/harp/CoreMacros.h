//
//  CoreSettings.h
//  ZombieHero
//
//  Created by haxpor on 10/22/12.
//
//

#ifndef ZombieHero_Setting_h
#define ZombieHero_Setting_h

#include "CoreTypes.h"

// Structures and Enum
// - Device type is based on screen resolution
enum DeviceType
{
    DEVICE_UNKNOWN,
    iPadRetina,
    iPad,
    iPhone3GS,
    iPhone4,
    iPhone5,
    iPhone6,
    iPhone6Plus
};

enum Direction {
    LEFT,
    RIGHT
};

// --- Debug ---
#if COCOS2D_DEBUG == 1
	#define IS_DEBUG 1
	#define IS_SHOW_STATS 1     // cocos2d-x stats
#else
	#define IS_DEBUG 0
	#define IS_SHOW_STATS 0
#endif

// - Steering AI debug (based on IS_DEBUG, if it turns off then ignore following setting)
#define IS_STEERINGAI_SHOW_GIZMO_DEBUG 0
#define IS_STEERINGAI_SHOW_WALLS_DEBUG 0
#define IS_STEERINGAI_SHOW_FLASHTAG_DEBUG 0
#define IS_STEERINGAI_SHOW_WANDER_DEBUG 0

// --- Game ---
#define BACKGROUND_LAYER -1
#define CHARACTER_SHADOW_LAYER 5000
#define CHARACTER_LAYER 5001
#define OBJECT_LAYER 10000
#define EFFECT_LAYER 10001
#define UI_LAYER 10002
#define TOPMOST_LAYER 10003

#define CHARACTER_SCALE_FACTOR 1.8f // safe value is between 1.0-2.0
#define CHARACTER_ASSET_SCALE_FACTOR 2.0f
#define CHARACTER_SCALE_ASSET_RATIO (CHARACTER_SCALE_FACTOR / CHARACTER_ASSET_SCALE_FACTOR)

// --- Namespace for HarpEngine ---
#define NS_S_HARPENGINE namespace harp {
#define NS_E_HARPENGINE }

// --- Macros ---
#define HarpConvertX(x)                 harp::Core::sharedCore()->convertPointX(x)
#define HarpConvertY(y)                 harp::Core::sharedCore()->convertPointY(y)
#define HarpConvertLength(length)       harp::Core::sharedCore()->convertPointX(length)
#define HarpConvertSize(size)           harp::Core::sharedCore()->convertPointX(size)

// --- SYSTEM ---
#define SMALL_CHARS_BUFFER 1024
#define MEDIUM_CHARS_BUFFER 2048
#define LARGE_CHARS_BUFFER 4096

#define ENFORCE_NONPENETRATION_CONSTRAINT 1
#define ENFORCER_NONPENETRATION_UPDATE_FREQ 1/5.0f
/* Value between 0.0-1.0 used to multiply with result to apply in calculation. */
#define ENFORCER_NONPENETRATION_EFFICIENT_MULTIPLIER 0.3f

// --- Directives ---
/**
 Non-virtual version of getter/setter functions. It's the same as CC_PROPERTY in cocos2d-x but without virtual keyword.
 */
#define HARP_PROPERTY_NONVIRTUAL(varType, varName, funcName)\
protected: varType varName;\
public: varType get##funcName(void);\
public: void set##funcName(varType var);

/**
 Non-virtual version of getter/setter functions. It's the same as CC_SYNTHESIZE in cocos2d-x but without virtual, and const keyword.
 */
#define HARP_SYNTHESIZE_NONVIRTUAL(varType, varName, funcName)\
protected: varType varName;\
public: varType get##funcName(void) { return varName; }\
public: void set##funcName(varType var) { varName = var; }

// --- Utils / Wrappers ---
#define HarpiCloudEnabled (APP_ICLOUD_ENABLED == 1 && harp::CloudDocument::isiCloudAvailable())

#endif

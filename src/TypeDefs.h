#pragma once

#include <string>
#include <list>
#include <unordered_map>
#include <map>
#include <vector>
#include "common.h"

using ItemID = uint16_t;
using ItemUID = uint64_t;
using NpcID = uint32_t;

//-------------------------------------------MACROS----------------------------------------------------------
#define foreach(x, y) for (decltype((y).begin()) x = (y).begin(); x != (y).end(); ++x)
#define foreach_a(x, y) for (decltype(&(y)[0]) x = &(y)[0]; x < &(y)[0] + (sizeof(y) / sizeof((y)[0])); x++)

//--------------------------------------------TIME-----------------------------------------------------------

struct Date
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
};

//#define SECONDS(x)			(x * CLOCKS_PER_SEC)
//#define MINUTES(x)			(x * SECONDS(60))
//#define HOURS(x)			(x * MINUTES(60))
//#define DAYS(x)				(x * HOURS(24))

// Time
// #define _ms					_s / 1000
// #define _s					* CLOCKS_PER_SEC
// #define _m					_s * 60
// #define _h					_m * 60
// #define _d					_h * 24

// time_t
#define MINUTE(x) x * 60
#define HOUR(x) x *MINUTE(60)
#define DAY(x) x *HOUR(24)

enum DaysOfWeek
{
    SUNDAY,
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY
};

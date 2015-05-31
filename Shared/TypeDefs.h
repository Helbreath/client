#ifndef TYPEDEFS_H
#define TYPEDEFS_H


#include <string>
 //typedef std::string string;
 typedef std::stringstream sstream;
 
 typedef std::ostream ostream;

#include <list>
#include <unordered_map>
#include <map>
#include <vector>
#include "common.h"

typedef uint16_t ItemID;
typedef uint64_t ItemUID;
typedef uint32_t NpcID;

//-------------------------------------------MACROS----------------------------------------------------------
#define foreach(x,y)		for(decltype(( y ).begin()) x = ( y ).begin(); x != ( y ).end(); ++x)
#define foreach_a(x,y)	for(decltype(&( y )[0]) x = &( y )[0]; x < &( y )[0] + (sizeof( y )/sizeof(( y )[0])); x++)

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
#define MINUTE(x)		x * 60
#define HOUR(x)	 	x * MINUTE(60)
#define DAY(x)		 	x * HOUR(24)

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

#endif // TYPEDEFS_H

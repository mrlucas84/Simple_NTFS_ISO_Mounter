#include "lv2.h"

uint64_t peekq(uint64_t addr);

#define	CFW_341			0
#define	CFW_355			1
#define	CFW_355_DEX		2
#define	CFW_421			3
#define	CFW_421_DEX		4
#define	CFW_430			5
#define	CFW_430_DEX		6
#define	CFW_431			7
#define	CFW_440			8
#define	CFW_441			9
#define	CFW_446			10
#define	CFW_450			11

extern bool bIsCFW[12];

void CheckFWVersion(void);
int fan_init(void);

int sys_game_get_temperature(int sel, uint32_t *temperature);

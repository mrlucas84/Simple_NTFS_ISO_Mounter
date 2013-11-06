#include "main.h"

bool bIsCFW[12];

#define FAN_SAFE_MODE

#define PEEK_SYSCALL	(6)
#define POKE_SYSCALL	(7)

uint64_t peekq(uint64_t addr)
{
	return Lv2Syscall1(PEEK_SYSCALL, addr);
}

static uint32_t peekq32(uint64_t addr) 
{
    uint32_t ret = (uint32_t) (peekq(addr) >> 32ULL);
    return ret;
}

static void pokeq(uint64_t addr, uint64_t val)
{
    Lv2Syscall2(POKE_SYSCALL, addr, val);
}

static void pokeq32(uint64_t addr, uint32_t val)
{
    uint32_t next = peekq(addr) & 0xffffffff;
    pokeq(addr, (((uint64_t) val) << 32) | next);
}

static inline void _poke(uint64_t addr, uint64_t val)
{
    pokeq(0x8000000000000000ULL + addr, val);
}

static inline void _poke32(uint64_t addr, uint32_t val)
{
    pokeq32(0x8000000000000000ULL + addr, val);
}

int sys_game_get_temperature(int sel, uint32_t *temperature)
{
    uint32_t temp;
	    
	Lv2Syscall2(383, (uint64_t) sel, (uint64_t) &temp);
    *temperature = (temp >> 24);

	return 0;
}

int sys_sm_set_fan_policy(uint8_t arg0, uint8_t arg1, uint8_t arg2)
{
    Lv2Syscall3(389, (uint64_t) arg0, (uint64_t) arg1, (uint64_t) arg2);

	return 0;
}
static int sys_set_leds(uint64_t color, uint64_t state)
{
    Lv2Syscall2(386, (uint64_t) color, (uint64_t) state);

	return 0;
}

#define LV2_SM_CMD_ADDR		((uint64_t)(0x8000000000000450ULL))
#define PAYLOAD_BASE		((uint64_t)(0x8000000000000f70ULL))

int is_firm_341(void)
{
	uint64_t nSyscallBase			= 0x80000000002EB128ULL;
	uint64_t nNewPokeSyscallAddress = 0x80000000001BB93CULL;
	uint32_t nNewPokeSyscall		= 813;

    uint64_t addr = peekq((nSyscallBase + nNewPokeSyscall * 8));
    // check address first
    if(addr < 0x8000000000000000ULL || addr > 0x80000000007FFFFFULL || (addr & 3)!=0)
        return 0;
    addr = peekq(addr);

    if(addr == nNewPokeSyscallAddress) return 1;

    return 0;
}

int is_firm_355(void)
{
	uint64_t nSyscallBase			= 0x8000000000346570ULL;
	uint64_t nNewPokeSyscallAddress = 0x8000000000195A68ULL;
	uint32_t nNewPokeSyscall		= 813;

    uint64_t addr = peekq((nSyscallBase + nNewPokeSyscall * 8));
    // check address first
    if(addr < 0x8000000000000000ULL || addr > 0x80000000007FFFFFULL || (addr & 3)!=0)
        return 0;
    addr = peekq(addr);

    if(addr == nNewPokeSyscallAddress) return 1;

    return 0;
}

int is_firm_355dex(void)
{
	uint64_t nSyscallBase			= 0x8000000000361578ULL;
	uint64_t nNewPokeSyscallAddress = 0x800000000019be24ULL;
	uint32_t nNewPokeSyscall		= 813;

    uint64_t addr = peekq((nSyscallBase + nNewPokeSyscall * 8));
    // check address first
    if(addr < 0x8000000000000000ULL || addr > 0x80000000007FFFFFULL || (addr & 3)!=0)
        return 0;
    addr = peekq(addr);

    if(addr == nNewPokeSyscallAddress) return 1;

    return 0;
}

int is_firm_421(void)
{
	uint64_t nSyscallBase			= 0x800000000035BCA8ULL;
	uint64_t nNewPokeSyscallAddress = 0x80000000001B65C8ULL;
	uint32_t nNewPokeSyscall		= 813;

    uint64_t addr = peekq((nSyscallBase + nNewPokeSyscall * 8));
    // check address first
    if(addr < 0x8000000000000000ULL || addr > 0x80000000007FFFFFULL || (addr & 3)!=0)
        return 0;
    addr = peekq(addr);

    if(addr == nNewPokeSyscallAddress) return 1;

    return 0;
}

int is_firm_421dex(void)
{
	uint64_t nSyscallBase			= 0x800000000037A1B0ULL;
	uint64_t nNewPokeSyscallAddress = 0x80000000001BC9B8ULL;
	uint32_t nNewPokeSyscall		= 813;

    uint64_t addr = peekq((nSyscallBase + nNewPokeSyscall * 8));
    // check address first
    if(addr < 0x8000000000000000ULL || addr > 0x80000000007FFFFFULL || (addr & 3)!=0)
        return 0;
    addr = peekq(addr);

    if(addr == nNewPokeSyscallAddress) return 1;

    return 0;
}

int is_firm_430(void)
{
	uint64_t nSyscallBase			= 0x800000000035DBE0ULL;
	uint64_t nNewPokeSyscallAddress = 0x80000000001B6950ULL;
	uint32_t nNewPokeSyscall		= 813;

    uint64_t addr = peekq((nSyscallBase + nNewPokeSyscall * 8));
    // check address first
    if(addr < 0x8000000000000000ULL || addr > 0x80000000007FFFFFULL || (addr & 3)!=0)
        return 0;
    addr = peekq(addr);

    if(addr == nNewPokeSyscallAddress) return 1;

    return 0;
}

int is_firm_430dex(void)
{
	uint64_t nSyscallBase			= 0x800000000037C068ULL;
	uint64_t nNewPokeSyscallAddress = 0x8000000000001510ULL;
	uint32_t nNewPokeSyscall		= 813;

    uint64_t addr = peekq((nSyscallBase + nNewPokeSyscall * 8));
    // check address first
    if(addr < 0x8000000000000000ULL || addr > 0x80000000007FFFFFULL || (addr & 3)!=0)
        return 0;
    addr = peekq(addr);

    if(addr == nNewPokeSyscallAddress) return 1;

    return 0;
}

int is_firm_431(void)
{
	uint64_t nSyscallBase			= 0x800000000035DBE0ULL;
	uint64_t nNewPokeSyscallAddress = 0x80000000001B6958ULL;
	uint32_t nNewPokeSyscall		= 813;

    uint64_t addr = peekq((nSyscallBase + nNewPokeSyscall * 8));
    // check address first
    if(addr < 0x8000000000000000ULL || addr > 0x80000000007FFFFFULL || (addr & 3)!=0)
        return 0;
    addr = peekq(addr);

    if(addr == nNewPokeSyscallAddress) return 1;

    return 0;
}

int is_firm_440(void)
{
	uint64_t nSyscallBase			= 0x800000000035E260ULL;
	uint64_t nNewPokeSyscallAddress = 0x80000000001B5934ULL;
	uint32_t nNewPokeSyscall		= 813;

    uint64_t addr = peekq((nSyscallBase + nNewPokeSyscall * 8));
    // check address first
    if(addr < 0x8000000000000000ULL || addr > 0x80000000007FFFFFULL || (addr & 3)!=0)
        return 0;
    addr = peekq(addr);

    if(addr == nNewPokeSyscallAddress) return 1;

    return 0;
}

int is_firm_441(void)
{
	uint64_t nSyscallBase			= 0x800000000035E260ULL;
	uint64_t nNewPokeSyscallAddress = 0x80000000001B593CULL;
	uint32_t nNewPokeSyscall		= 813;

    uint64_t addr = peekq((nSyscallBase + nNewPokeSyscall * 8));
    // check address first
    if(addr < 0x8000000000000000ULL || addr > 0x80000000007FFFFFULL || (addr & 3)!=0)
        return 0;
    addr = peekq(addr);

    if(addr == nNewPokeSyscallAddress) return 1;

    return 0;
}

int is_firm_446(void)
{
	uint64_t nSyscallBase			= 0x800000000035E860ULL;
	uint64_t nNewPokeSyscallAddress = 0x80000000001B5B9CULL;
	uint32_t nNewPokeSyscall		= 813;

    uint64_t addr = peekq((nSyscallBase + nNewPokeSyscall * 8));
    // check address first
    if(addr < 0x8000000000000000ULL || addr > 0x80000000007FFFFFULL || (addr & 3)!=0)
        return 0;
    addr = peekq(addr);

    if(addr == nNewPokeSyscallAddress) return 1;

    return 0;
}

int is_firm_450(void)
{
	uint64_t nSyscallBase			= 0x800000000035F0D0ULL;
	uint64_t nNewPokeSyscallAddress = 0x80000000001A6C78ULL;
	uint32_t nNewPokeSyscall		= 813;

    uint64_t addr = peekq((nSyscallBase + nNewPokeSyscall * 8));
    // check address first
    if(addr < 0x8000000000000000ULL || addr > 0x80000000007FFFFFULL || (addr & 3)!=0)
        return 0;
    addr = peekq(addr);

    if(addr == nNewPokeSyscallAddress) return 1;

    return 0;
}

int fan_init(void)
{
	int ret = 0;

	// 4.50 cex
	//if(bIsCFW[CFW_450])
	//{
 //       // enables sys_game_get_temperature
 //       pokeq32(0x800000000000C694ULL, 0x38600000); // sys 383

 //       // enables sys_sm_get_fan_policy
 //       pokeq32(0x8000000000009E28ULL, 0x38600001); // sys 409

 //       // enables sys_sm_set_fan_policy
 //       pokeq32(0x800000000000A324ULL, 0x38600001); // sys 389

 //       // enables sys_set_leds
 //       pokeq32(0x800000000000A3ECULL, 0x38600001); // sys 386

	//	ret = 1;
	//}

	if(bIsCFW[CFW_446]) {
        // enables sys_game_get_temperature
        pokeq32(0x800000000000C698ULL, 0x38600000); // sys 383

        // enables sys_sm_get_fan_policy
        pokeq32(0x8000000000009E28ULL, 0x38600001); // sys 409

        // enables sys_sm_set_fan_policy
        pokeq32(0x800000000000A324ULL, 0x38600001); // sys 389

        // enables sys_set_leds
        pokeq32(0x800000000000A3ECULL, 0x38600001); // sys 386
		
		ret = 1;
	}

#ifdef FAN_SAFE_MODE
	if(ret) 
	{
		uint64_t command;

		//if(mode == -1) mode = fan_mode;

		//if(!test_controlfan_compatibility()) return;
		static uint64_t payload_ctrl = (PAYLOAD_BASE + (peekq32(PAYLOAD_BASE + 4ULL ))) - 8ULL;

		//cur_fan_mode = mode;

		pokeq32(payload_ctrl + 4ULL, 0); // disabled

		command = peekq(LV2_SM_CMD_ADDR)>>56ULL;

		if(command == 0x55ULL) { // SM present!
			while(1) {
				pokeq(LV2_SM_CMD_ADDR, 0xFF00000000000000ULL); // get status
				sys_timer_usleep(2000);
				command = peekq(LV2_SM_CMD_ADDR);
				if((command>>48ULL) == 0x55FFULL && (command & 0xF) == 1) break; // SM Fan Control Stopped
			}
		}
		sys_timer_usleep(100000); // waits

		sys_set_leds(2, 0); // restore LEDS
		sys_set_leds(0, 0);
		sys_set_leds(1, 1);

		//sys_sm_set_fan_policy(0, 2, 0xCA);
	}
#endif

	return ret;
}

void CheckFWVersion(void)
{
	memset(&bIsCFW, 0, sizeof(bool) * sizeof(bIsCFW));

	if(is_firm_341())		{ bIsCFW[CFW_341]		= true; return; }
	if(is_firm_355())		{ bIsCFW[CFW_355]		= true; return; }
	if(is_firm_355dex())	{ bIsCFW[CFW_355_DEX]	= true; return; }
	if(is_firm_421())		{ bIsCFW[CFW_421]		= true; return; }
	if(is_firm_421dex())	{ bIsCFW[CFW_421_DEX]	= true; return; }
	if(is_firm_430())		{ bIsCFW[CFW_430]		= true; return; }
	if(is_firm_430dex())	{ bIsCFW[CFW_430_DEX]	= true; return; }
	if(is_firm_431())		{ bIsCFW[CFW_431]		= true; return; }
	if(is_firm_440())		{ bIsCFW[CFW_440]		= true; return; }
	if(is_firm_441())		{ bIsCFW[CFW_441]		= true; return; }
	if(is_firm_446())		{ bIsCFW[CFW_446]		= true; return; }
	if(is_firm_450())		{ bIsCFW[CFW_450]		= true; return; }
}

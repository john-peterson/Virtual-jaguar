#ifndef __JAGUAR_H__
#define __JAGUAR_H__

#include <stdint.h>
#include <cstddef>
#include "memory.h"							// For "UNKNOWN" enum

// Breakpoint struture
typedef struct BrkInfo
{
	bool Used;					// Allocated breakpoint
	bool Active;				// Active breakpoint
	char *Name;					// Functions's name
	char *Filename;				// Source filename
	char *LineSrc;				// Source code line
	size_t NumLine;				// Line number
	size_t Adr;					// Breakpoint address
	size_t HitCounts;			// Hit counts
}S_BrkInfo;

void JaguarSetScreenBuffer(uint32_t * buffer);
void JaguarSetScreenPitch(uint32_t pitch);
extern void JaguarInit(void);
extern void JaguarReset(void);
extern void JaguarDone(void);

// Memory functions
uint8_t JaguarReadByte(uint32_t offset, uint32_t who = UNKNOWN);
uint16_t JaguarReadWord(uint32_t offset, uint32_t who = UNKNOWN);
uint32_t JaguarReadLong(uint32_t offset, uint32_t who = UNKNOWN);
void JaguarWriteByte(uint32_t offset, uint8_t data, uint32_t who = UNKNOWN);
void JaguarWriteWord(uint32_t offset, uint16_t data, uint32_t who = UNKNOWN);
void JaguarWriteLong(uint32_t offset, uint32_t data, uint32_t who = UNKNOWN);

bool JaguarInterruptHandlerIsValid(uint32_t i);
void JaguarDasm(uint32_t offset, uint32_t qt);

void JaguarExecuteNew(void);
void	JaguarStepInto(void);
void	JaguarStepOver(int depth);

// Exports from JAGUAR.CPP

extern int32_t jaguarCPUInExec;
extern uint32_t jaguarMainROMCRC32, jaguarROMSize, jaguarRunAddress;
extern char * jaguarEepromsPath;
extern bool jaguarCartInserted;
extern bool bpmActive, bpmSaveActive;
extern size_t bpmHitCounts;
extern uint32_t bpmAddress1;
extern bool startM68KTracing;
extern S_BrkInfo *brkInfo;
extern size_t brkNbr;

// Various clock rates

#define M68K_CLOCK_RATE_PAL		13296950
#define M68K_CLOCK_RATE_NTSC	13295453
#define RISC_CLOCK_RATE_PAL		26593900
#define RISC_CLOCK_RATE_NTSC	26590906

// Stuff for IRQ handling

#define ASSERT_LINE		1
#define CLEAR_LINE		0

//Temp debug stuff (will go away soon, so don't depend on these)

void DumpMainMemory(void);
uint8_t * GetRamPtr(void);

#endif	// __JAGUAR_H__

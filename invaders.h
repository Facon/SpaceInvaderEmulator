#ifndef INVADERS_H
#define INVADERS_H

#include <iostream>
#include <SDL/SDL.h>
#include "core.h"
#include "driver.h"
#include "8080.h"
#include "utils.h"

void WritePort(Driver *drv,uchar addr, byte value);
byte ReadPort(Driver *drv, uchar addr);
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

class Invaders : public Driver
{
	private:
		// Intel's 8080A CPU
		I8080 cpu;
		
		// Memory
		static const unsigned short MEMORY_SIZE = 16384;
		byte memory[MEMORY_SIZE]; // From 0x0000 to 0x4000
		
		// Ports
		byte read_2;
		word shift_register;
		word shift_offset;
		
		// vertical interval (60 Hz)
		void Vblank();
		
		// Screen
		static const unsigned short SCREEN_X = 256;
		static const unsigned short SCREEN_Y = 224;
		SDL_Surface *screen;
		
	public:
		Invaders();
		
		// Getters
		byte GetMemory(word addr) const
		{return (addr < MEMORY_SIZE) ? memory[addr] : 0;}
		byte GetRead_2() const
		{return read_2;}
		word GetShift_register() const
		{return shift_register;}
		word GetShift_offset() const
		{return shift_offset;}
		
		// Setters
		void SetMemory(word addr, byte b)
		{
			if (addr < MEMORY_SIZE)
			{
				memory[addr] = b;
			}
		}
		void SetRead_2(byte b)
		{read_2 = b;}
		void SetShift_register(word w)
		{shift_register = w;}
		void SetShift_offset(word w)
		{shift_offset = w;}

		void Run();
		
		~Invaders();
};

#endif

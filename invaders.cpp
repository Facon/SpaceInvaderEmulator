#include "invaders.h"

Invaders::Invaders() : cpu(&memory[0], this, &WritePort, &ReadPort)
{
	/*
	Memory map:
		ROM
		$0000-$07ff:	invaders.h
		$0800-$0fff:	invaders.g
		$1000-$17ff:	invaders.f
		$1800-$1fff:	invaders.e
		
		RAM
		$2000-$23ff:	work RAM
		$2400-$3fff:	video RAM
		
		$4000-:		RAM mirror
	*/
	
	LoadRom(&memory[0], 0x0000, std::string("invaders.h"));
	LoadRom(&memory[0], 0x0800, std::string("invaders.g"));
	LoadRom(&memory[0], 0x1000, std::string("invaders.f"));
	LoadRom(&memory[0], 0x1800, std::string("invaders.e"));
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		exit(1);
	}
	
	atexit(SDL_Quit);
	
	screen = SDL_SetVideoMode(SCREEN_X, SCREEN_Y, 16, SDL_SWSURFACE);
	
	if (screen == NULL)
	{
		exit(1);
	}
	
	SDL_WM_SetCaption("Marcianitos al ataque!", 0);
}

void Invaders::Vblank()
{
	/*
	Video: 256(x)*224(y) @ 60Hz, vertical monitor. Colours are simulated with a plastic transparent overlay and a background picture.
	Video hardware is very simple: 7168 bytes 1bpp bitmap (32 bytes per scanline).
	*/
	
	/*TODO*/
	
	Uint32 pixel;
	byte data;
	unsigned short x, y;
	unsigned char offset;
	
	for (y = 0; (y < SCREEN_Y); y++)
	{
		data = memory[0x2400 + (x * y) + y];
		
		for (x = 0, offset = 0; (x < SCREEN_X); x++, offset++)
		{
			if (offset == 8)
			{offset = 0;}
			
			pixel = ((data >> offset) & 0x01) ? 0xFFFF : 0x0000;
			DrawPixel(screen, x, y, pixel);
		}
	}
	
	SDL_Flip(screen);
}

void Invaders::Run()
{	
	// Loop
	for (;;)
	{
		cpu.Run(28527);
		cpu.SetBus(0xcf);
		cpu.SetINT();
		// VBLANK
		// Draw Screen
		// ReadInput
		Vblank();
		cpu.Run(4839);
		cpu.SetBus(0xd7);
		cpu.SetINT();
	}
}

/*
Ports:
	Read 0
	BIT	0	coin (0 when active)
		1	P2 start button
		2	P1 start button
		3	?
		4	P1 shoot button
		5	P1 joystick left
		6	P1 joystick right
		7	?
	
	Read 1
	BIT	0,1	dipswitch number of lives (0:3,1:4,2:5,3:6)
		2	tilt 'button'
		3	dipswitch bonus life at 1:1000,0:1500
		4	P2 shoot button
		5	P2 joystick left
		6	P2 joystick right
		7	dipswitch coin info 1:off,0:on
	
	Read 2		shift register result
	
	Write 2		shift register result offset (bits 0,1,2)
	Write 3		sound related
	Write 4		fill shift register
	Write 5		sound related
	Write 6		strange 'debug' port? eg. it writes to this port when
			it writes text to the screen (0=a,1=b,2=c, etc)
	
	(write ports 3,5,6 can be left unemulated, read port 0=$01 and 1=$00
	will make the game run, but but only in attract mode)

I haven't looked into sound details.

16 bit shift register:
	f              0	bit
	xxxxxxxxyyyyyyyy
	
	Writing to port 4 shifts x into y, and the new value into x, eg.
	$0000,
	write $aa -> $aa00,
	write $ff -> $ffaa,
	write $12 -> $12ff, ..
	
	Writing to port 2 (bits 0,1,2) sets the offset for the 8 bit result, eg.
	offset 0:
	rrrrrrrr		result=xxxxxxxx
	xxxxxxxxyyyyyyyy
	
	offset 2:
	  rrrrrrrr		result=xxxxxxyy
	xxxxxxxxyyyyyyyy
	
	offset 7:
		   rrrrrrrr		result=xyyyyyyy
	xxxxxxxxyyyyyyyy
	
	Reading from port 2 returns said result.
*/

Invaders::~Invaders()
{SDL_FreeSurface(screen);}

void WritePort(Driver *drv, uchar addr, byte value)
{
	Invaders *drv_invaders = (Invaders*) drv;
	
	switch (addr)
	{
		
		case 2:
			//read_2 = value & 7;
			drv_invaders -> SetRead_2(value & 7);
			break;
		case 3:
			break; // sound
		case 4:
			//shift_register = (shift_register >> 8) | (value << 8);
			drv_invaders -> SetShift_register((drv_invaders -> GetShift_register() >> 8) | (value << 8));
			break;
		case 5:
			break; // sound
		default:
			break;
	}
}

byte ReadPort(Driver *drv, uchar addr)
{
	Invaders *drv_invaders = (Invaders*) drv;
	unsigned char ret = addr;

	switch (addr)
	{
		case 1:
			//ret=key&0x7f;
			ret = 0x01;
			break;
		case 2:
			//ret=dips|(key&0x70)|(key>>5&4);
			ret = 0x00;
			break;
		case 3:
			//ret = (shift_register << shift_offset) >> 8;
			ret = (drv_invaders -> GetShift_register() << drv_invaders -> GetShift_register()) >> 8;
			break;
		
		default:
			break;
	}

	return ret;
}

void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to set */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch(bpp)
	{
		case 1:
			*p = pixel;
			break;

		case 2:
			*(Uint16 *)p = pixel;
			break;

		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			{
				p[0] = (pixel >> 16) & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = pixel & 0xff;
			}
			else
			{
				p[0] = pixel & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = (pixel >> 16) & 0xff;
			}
			break;

		case 4:
			*(Uint32 *)p = pixel;
			break;
	}
}

#include <cstdio>
#include <iostream>
#include <string>
#include "core.h"

using namespace std;

/* Cambiar */

void LoadRom(byte *memory, word start_addr, string rom_name)
{
	FILE *file_rom;
	rom_name = string("rom/") + rom_name;
	file_rom = fopen(rom_name.c_str(), "r");
	
	if (file_rom == NULL)
	{
		cout << "Error. Not found " << rom_name << "." << endl;
	}
	else
	{
		unsigned short cont_bytes = 0;
		while(!feof(file_rom))
		{
			memory[start_addr + cont_bytes] = fgetc(file_rom);
			cont_bytes++;
		}
	}
	
	fclose(file_rom);
}

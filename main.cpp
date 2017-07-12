#include "chip8.hpp"
#include <string>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>

int main()
{
	char name[32];
	chip8 chip;
	
	std::cout << "Please enter ROM's name: ";
	std::cin >> name;
	bool isFileLoaded = chip.load(name);
	if (isFileLoaded)
	{
		for(;;)
		{
			system("clear");
			chip.cycle();
			//std::cout << chip.PC;
			//std::cin.ignore();
			for (int i = 0; i < 2048; i++)
			{
				if (!(i % 64))
					printf("\n");
				else
				{
					if (chip.screen[i] == 0)
						printf(" ");
					if (chip.screen[i] == 1)
						printf("*");
				}
			}
		}
	}
}
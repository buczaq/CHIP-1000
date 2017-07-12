#include "chip8.hpp"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>

chip8::chip8()
{
	initialize();
}

void chip8::initialize()
{
	srand(time(NULL));
	opcode = 0;
	for (int i = 0; i < 4096; i++)
		memory[i] = 0;
	for (int i = 0; i < 16; i++)
		V[i] = 0;
	I = 0;
	PC = 0x200;
	SP = 0;
	for (int i = 0; i < 16; i++)
		stack[i] = 0;
	delay_timer = 0;
	sound_timer = 0;
	for (int i = 0; i < 64*32; i++)
		screen[i] = 0;
	for (int i = 0; i < 16; i++)
		key[i] = 0;

	//loading font into memory
	for (int i = 0; i < 80; i++)
		memory[i] = fontset[i];
}

void chip8::cycle()
{
	opcode = (memory[PC] << 8) | memory[PC+1]; //opcode is 16 bits long
	
	//std::cout << opcode;

	//switch for decoding the opcode
	switch (opcode & 0xF000)
	{
		case 0x0000:
			switch (opcode & 0x00FF)
			{
				case 0x00E0:
					//TODO: clear the display
				system("clear");
				PC += 2;
				break;

				case 0x00EE:
					PC = stack[SP];
					SP--;
				break;

				default:
					//jump to a machine code routine, not used
				break;
			}
			break;

		case 0x1000:
			PC = opcode & 0x0FFF;
			break;

		case 0x2000:
			SP++;
			stack[SP] = PC;
			PC = opcode & 0x0FFF;
			break;

		case 0x3000:
			if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
				PC += 4;
			else
				PC += 2;
			break;

		case 0x4000:
			if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
				PC += 4;
			else
				PC += 2;
			break;

		case 0x5000:
			if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
				PC += 4;
			else
				PC += 2;
			break;

		case 0x6000:
			V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
			PC += 2;
			break;

		case 0x7000:
			V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
			PC += 2;
			break;

		case 0x8000:
			switch(opcode & 0x000F)
			{
				case 0x0000:
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
					PC += 2;
					break;

				case 0x0001:
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] | V[(opcode & 0x00F0) >> 4];
					PC += 2;
					break;

				case 0x0002:
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0) >> 4];
					PC += 2;
					break;

				case 0x0003:
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] ^ V[(opcode & 0x00F0) >> 4];
					PC += 2;
					break;

				case 0x0004:
					if ((V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4]) > 255)
					{
						V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4] - 255;
						V[0xF] = 1;
						PC += 2;
					}
					else
					{
						V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4];
						V[0xF] = 0;
						PC += 2;
					}
					break;

				case 0x0005:
					if ((V[(opcode & 0x0F00) >> 8] - V[(opcode & 0x00F0) >> 4]) >= 0)
					{
						V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] - V[(opcode & 0x00F0) >> 4];
						V[0xF] = 1;
						PC += 2;
					}
					else 
					{
						V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] - V[(opcode & 0x00F0) >> 4] + 255;
						V[0xF] = 0;
						PC += 2;
					}
					break;

				case 0x0006:
					if (V[(opcode & 0x0F00) >> 8] % 2)
						V[0xF] = 1;
					else V[0xF] = 0;
					V[(opcode & 0x0F00)] /= 2;
					PC += 2;
					break;

				case 0x0007:
					if ((V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8]) >= 0)
					{
						V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
						V[0xF] = 1;
					}
					else 
					{
						V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8] + 255;
						V[0xF] = 0;
					}
					PC += 2;
					break;

				case 0x000E:
					if (V[(opcode & 0x0F00) >> 8] > 127)
						V[0xF] = 1;
					else V[0xF] = 0;
					V[(opcode & 0x0F00)] *= 2;
					PC += 2;
					break;
			}
			break;

		case 0x9000:
			if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4])
				PC += 4;
			else
				PC += 2;
			break;

		case 0xA000:
			I = opcode & 0x0FFF;
			PC += 2;
			break;

		case 0xB000:
			PC = (opcode & 0x0FFF) + V[0];
			break;

		case 0xC000:
			V[(opcode & 0x0F00) >> 8] = (rand() % 256) & (opcode & 0x00FF);
			PC += 2;
			break;

		case 0xD000:
		{
			unsigned short x = V[(opcode & 0x0F00) >> 8];
			unsigned short y = V[(opcode & 0x00F0) >> 4];
			unsigned short memloc = I;
			unsigned char mask = 128;
			unsigned short coordinates = 0;
			V[0xF] = 0;
			for(unsigned short i = 0; i < (opcode & 0x000F); i++)
			{
				for (int j = 0; j< 8; j++)
				{
					if (x > 63)
						coordinates = (x - 63) + 64 * y;
					else if (y > 31)
						coordinates = x + (64 * y - 31);
					else
						coordinates = x + 64*y;

					if ((screen[coordinates] ^ (memory[memloc] & mask) >> (7 -j)) && !V[0xF])
						V[0xF] = 1;
					screen[coordinates] = screen[coordinates] ^ (memory[memloc] & mask) >> (7 -j);
					x++;
					mask /= 2;
				}
				memloc++;
				y++;
				x -= 8;
				mask = 128;
			}
		}
		PC += 2;
			break;

		case 0xE000:
			switch(opcode & 0x00F0)
			{
				case 0x0090:
					if (key[V[(opcode & 0x0F00) >> 8]] != 0)
						PC += 4;
					else
						PC += 2;
					break;

				case 0x00A0:
					if (key[V[(opcode & 0x0F00) >> 8]] == 0)
						PC += 4;
					else
						PC += 2;
					break;
			}
			break;

		case 0xF000:
			switch(opcode & 0x00F0)
			{
				case 0x0000:
					switch(opcode & 0x000F)
					{
						case 0x0007:
							V[(opcode & 0x0F00) >> 8] = delay_timer;
							PC += 2;
							break;

						case 0x000A:
							while(1)
							{
								for (int i = 0; i < 16; i++)
									if (key[i])
										break;
							}
							PC += 2;
							break;
					}
					break;

				case 0x0010:
					switch(opcode & 0x000F)
					{
						case 0x0005:
							delay_timer = V[(opcode & 0x0F00) >> 8];
							PC += 2;
							break;

						case 0x0008:
							sound_timer = V[(opcode & 0x0F00) >> 8];
							PC += 2;
							break;

						case 0x000E:
							I += V[(opcode & 0x0F00) >> 8];
							PC += 2;
							break;
					}
					break;

				case 0x0020:
					I = V[(opcode & 0x0F00) >> 8] * 5;
					PC += 2;
					break;

				case 0x0030:
				{
					unsigned char decimal = V[(opcode & 100) >> 8];
					memory[I] = decimal / 100;
					memory[I + 1] = (decimal - 100 * I) / 10;
					memory[I + 2] = (decimal - 100 * I) - 10 * (I + 1) + 1;
					PC += 2;
				}
					break;

				case 0x0050:
					for (int i = 0; i < (opcode & 0x0F00) >> 8; i++)
						memory[I + i] = V[i];
					PC += 2;
					break;

				case 0x0060:
					for (int i = 0; i < (opcode & 0x0F00) >> 8; i++)
						V[i] = memory[I + i];
					PC += 2;
					break;
			}
			break;

		default:
		printf("ERROR: wrong opcode!\n");
		PC += 2;
	}

if (delay_timer > 0)
	delay_timer--;

if (sound_timer > 0)
	sound_timer--;
}

bool chip8::load(const char * name)
{
	initialize();
	FILE * file = fopen(name, "rb");

	if (file == NULL)
	{
		printf ("ERROR: No such file!\n"); 
		return false;
	}

	fseek(file , 0 , SEEK_END);
	long size = ftell(file);
	rewind(file);
	
	char * buffer = (char*)malloc(sizeof(char) * size);
	fread (buffer, 1, size, file);

	if((4096-512) > size)
	{
		for(int i = 0; i < size; ++i)
			memory[i + 512] = buffer[i];
	}
	else
		printf("ERROR: ROM file is too big!\n");
	
	// Close file, free buffcer
	fclose(file);
	free(buffer);
	return true;
}
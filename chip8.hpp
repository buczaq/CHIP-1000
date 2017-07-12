#define ST_LOC 0x200	//starting location for programs (first 512B of memory are reserved for interpreter's code)

class chip8
{
public:
	unsigned char fontset[80] =
	{ 
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};

	unsigned short opcode;			//current operation code (16-bit)
	unsigned char memory[4096];		//memory (4096B)
	unsigned char V[16];			//registers (note: V[16] is a special register that is used as a flag)
	unsigned short I;				//(mostly) address storing register (16-bit)
	unsigned short PC;				//program counter; currently executing address (16-bit)
	unsigned char SP;				//stack pointer (8-bit)
	unsigned short stack[16];		//stack (16 positions)
	unsigned char delay_timer;		//delay timer (8-bit)
	unsigned char sound_timer;		//sound timer (8-bit)
	unsigned char screen[64*32];	//graphics (64x32px)
	unsigned char key[16];			//keys maps

	void initialize();
	chip8();
	void cycle();
	bool load(const char *);
private:
	/*
	unsigned short opcode;			//current operation code (16-bit)
	unsigned char memory[4096];		//memory (4096B)
	unsigned char V[16];			//registers (note: V[16] is a special register that is used as a flag)
	unsigned short I;				//(mostly) address storing register (16-bit)
	unsigned short PC;				//program counter; currently executing address (16-bit)
	unsigned char SP;				//stack pointer (8-bit)
	unsigned short stack[16];		//stack (16 positions)
	unsigned char delay_timer;		//delay timer (8-bit)
	unsigned char sound_timer;		//sound timer (8-bit)
	unsigned char screen[64 * 32];	//graphics (64x32px)
	unsigned char key[16];			//keys maps
	*/
};
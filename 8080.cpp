#include "8080.h"

/* Code based on Alessandro Scotti's code */

I8080::I8080(byte *memory, Driver *drv, Pf_WRITE_PORT_8 func1, Pf_READ_PORT_8 func2)
{
	Reset();
	this -> memory = memory;
	this -> drv = drv; 
	WritePort = func1;
	ReadPort = func2;
	running = INT = false;
}

const byte I8080::PSZ[256] =
{
	Zero|Parity, 0, 0, Parity, 0, Parity, Parity, 0, 0, Parity, Parity, 0, Parity, 0, 0, Parity, 
	0, Parity, Parity, 0, Parity, 0, 0, Parity, Parity, 0, 0, Parity, 0, Parity, Parity, 0, 
	0, Parity, Parity, 0, Parity, 0, 0, Parity, Parity, 0, 0, Parity, 0, Parity, Parity, 0, 
	Parity, 0, 0, Parity, 0, Parity, Parity, 0, 0, Parity, Parity, 0, Parity, 0, 0, Parity, 
	0, Parity, Parity, 0, Parity, 0, 0, Parity, Parity, 0, 0, Parity, 0, Parity, Parity, 0, 
	Parity, 0, 0, Parity, 0, Parity, Parity, 0, 0, Parity, Parity, 0, Parity, 0, 0, Parity, 
	Parity, 0, 0, Parity, 0, Parity, Parity, 0, 0, Parity, Parity, 0, Parity, 0, 0, Parity, 
	0, Parity, Parity, 0, Parity, 0, 0, Parity, Parity, 0, 0, Parity, 0, Parity, Parity, 0, 
	Sign, Sign|Parity, Sign|Parity, Sign, Sign|Parity, Sign, Sign, Sign|Parity, Sign|Parity,
	Sign, Sign, Sign|Parity, Sign, Sign|Parity, Sign|Parity, Sign, Sign|Parity, Sign, Sign,
	Sign|Parity, Sign, Sign|Parity, Sign|Parity, Sign, Sign, Sign|Parity, Sign|Parity, Sign,
	Sign|Parity, Sign, Sign, Sign|Parity, Sign|Parity, Sign, Sign, Sign|Parity, Sign, Sign|Parity,
	Sign|Parity, Sign, Sign, Sign|Parity, Sign|Parity, Sign, Sign|Parity, Sign, Sign, Sign|Parity,
	Sign, Sign|Parity, Sign|Parity, Sign, Sign|Parity, Sign, Sign, Sign|Parity, Sign|Parity,
	Sign, Sign, Sign|Parity, Sign, Sign|Parity, Sign|Parity, Sign, Sign|Parity, Sign, Sign,
	Sign|Parity, Sign, Sign|Parity, Sign|Parity, Sign, Sign, Sign|Parity, Sign|Parity, Sign,
	Sign|Parity, Sign, Sign, Sign|Parity, Sign, Sign|Parity, Sign|Parity, Sign, Sign|Parity,
	Sign, Sign, Sign|Parity, Sign|Parity, Sign, Sign, Sign|Parity, Sign, Sign|Parity, Sign|Parity,
	Sign, Sign, Sign|Parity, Sign|Parity, Sign, Sign|Parity, Sign, Sign, Sign|Parity, Sign|Parity,
	Sign, Sign, Sign|Parity, Sign, Sign|Parity, Sign|Parity, Sign, Sign|Parity, Sign, Sign,
	Sign|Parity, Sign, Sign|Parity, Sign|Parity, Sign, Sign, Sign|Parity, Sign|Parity, Sign,
	Sign|Parity, Sign, Sign, Sign|Parity
};

const uchar I8080::cycles_table[256] =
{
	/*                       8080's Cycles Table                         */
	/*     0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F */
	/*0*/  4, 10,  7,  5,  5,  5,  7,  4,  0, 10,  7,  5,  5,  5,  7,  4,
	/*1*/  0, 10,  7,  5,  5,  5,  7,  4,  0, 10,  7,  5,  5,  5,  7,  4,
	/*2*/  0, 10, 16,  5,  5,  5,  7,  4,  0, 10, 16,  5,  5,  5,  7,  4,
	/*3*/  0, 10, 13,  5, 10, 10, 10,  4,  0, 10, 13,  5,  5,  5,  7,  4,
	/*4*/  5,  5,  5,  5,  5,  5,  7,  5,  5,  5,  5,  5,  5,  5,  7,  5,
	/*5*/  5,  5,  5,  5,  5,  5,  7,  5,  5,  5,  5,  5,  5,  5,  7,  5,
	/*6*/  5,  5,  5,  5,  5,  5,  7,  5,  5,  5,  5,  5,  5,  5,  7,  5,
	/*7*/  7,  7,  7,  7,  7,  7,  7,  7,  5,  5,  5,  5,  5,  5,  7,  5,
	/*8*/  4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4,
	/*9*/  4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4,
	/*A*/  4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4,
	/*B*/  4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4,
	/*C*/  5, 10, 10, 10, 11, 11,  7, 11,  5, 10, 10,  0, 11, 17,  7, 11,
	/*D*/  5, 10, 10, 10, 11, 11,  7, 11,  5,  0, 10, 10, 11,  0,  7, 11,
	/*E*/  5, 10, 10, 18, 11, 11,  7, 11,  5,  5, 10,  4, 11,  0,  7, 11,
	/*F*/  5, 10, 10,  4, 11, 11,  7, 11,  5,  5, 10,  4, 11,  0,  7, 11
};

const OPTABLE I8080::instruction[256] =
{
	&I8080::opcode_00, &I8080::opcode_01, &I8080::opcode_02, &I8080::opcode_03, &I8080::opcode_04,
	&I8080::opcode_05, &I8080::opcode_06, &I8080::opcode_07, &I8080::opcode_08,	&I8080::opcode_09,
	&I8080::opcode_0A, &I8080::opcode_0B, &I8080::opcode_0C, &I8080::opcode_0D, &I8080::opcode_0E,
	&I8080::opcode_0F, &I8080::opcode_10, &I8080::opcode_11, &I8080::opcode_12, &I8080::opcode_13,
	&I8080::opcode_14, &I8080::opcode_15, &I8080::opcode_16, &I8080::opcode_17,	&I8080::opcode_18,
	&I8080::opcode_19, &I8080::opcode_1A, &I8080::opcode_1B, &I8080::opcode_1C, &I8080::opcode_1D,
	&I8080::opcode_1E, &I8080::opcode_1F, &I8080::opcode_20, &I8080::opcode_21, &I8080::opcode_22,
	&I8080::opcode_23, &I8080::opcode_24, &I8080::opcode_25, &I8080::opcode_26, &I8080::opcode_27,
	&I8080::opcode_28, &I8080::opcode_29, &I8080::opcode_2A, &I8080::opcode_2B, &I8080::opcode_2C,
	&I8080::opcode_2D, &I8080::opcode_2E, &I8080::opcode_2F, &I8080::opcode_30, &I8080::opcode_31,
	&I8080::opcode_32, &I8080::opcode_33, &I8080::opcode_34, &I8080::opcode_35, &I8080::opcode_36,
	&I8080::opcode_37, &I8080::opcode_38, &I8080::opcode_39, &I8080::opcode_3A, &I8080::opcode_3B,
	&I8080::opcode_3C, &I8080::opcode_3D, &I8080::opcode_3E, &I8080::opcode_3F, &I8080::opcode_40,
	&I8080::opcode_41, &I8080::opcode_42, &I8080::opcode_43, &I8080::opcode_44, &I8080::opcode_45,
	&I8080::opcode_46, &I8080::opcode_47, &I8080::opcode_48, &I8080::opcode_49, &I8080::opcode_4A,
	&I8080::opcode_4B, &I8080::opcode_4C, &I8080::opcode_4D, &I8080::opcode_4E, &I8080::opcode_4F,
	&I8080::opcode_50, &I8080::opcode_51, &I8080::opcode_52, &I8080::opcode_53, &I8080::opcode_54,
	&I8080::opcode_55, &I8080::opcode_56, &I8080::opcode_57, &I8080::opcode_58, &I8080::opcode_59,
	&I8080::opcode_5A, &I8080::opcode_5B, &I8080::opcode_5C, &I8080::opcode_5D, &I8080::opcode_5E,
	&I8080::opcode_5F, &I8080::opcode_60, &I8080::opcode_61, &I8080::opcode_62, &I8080::opcode_63,
	&I8080::opcode_64, &I8080::opcode_65, &I8080::opcode_66, &I8080::opcode_67, &I8080::opcode_68,
	&I8080::opcode_69, &I8080::opcode_6A, &I8080::opcode_6B, &I8080::opcode_6C, &I8080::opcode_6D,
	&I8080::opcode_6E, &I8080::opcode_6F, &I8080::opcode_70, &I8080::opcode_71, &I8080::opcode_72,
	&I8080::opcode_73, &I8080::opcode_74, &I8080::opcode_75, &I8080::opcode_76, &I8080::opcode_77,
	&I8080::opcode_78, &I8080::opcode_79, &I8080::opcode_7A, &I8080::opcode_7B, &I8080::opcode_7C,
	&I8080::opcode_7D, &I8080::opcode_7E, &I8080::opcode_7F, &I8080::opcode_80, &I8080::opcode_81,
	&I8080::opcode_82, &I8080::opcode_83, &I8080::opcode_84, &I8080::opcode_85, &I8080::opcode_86,
	&I8080::opcode_87, &I8080::opcode_88, &I8080::opcode_89, &I8080::opcode_8A, &I8080::opcode_8B,
	&I8080::opcode_8C, &I8080::opcode_8D, &I8080::opcode_8E, &I8080::opcode_8F,	&I8080::opcode_90,
	&I8080::opcode_91, &I8080::opcode_92, &I8080::opcode_93, &I8080::opcode_94, &I8080::opcode_95,
	&I8080::opcode_96, &I8080::opcode_97, &I8080::opcode_98, &I8080::opcode_99, &I8080::opcode_9A,
	&I8080::opcode_9B, &I8080::opcode_9C, &I8080::opcode_9D, &I8080::opcode_9E, &I8080::opcode_9F,
	&I8080::opcode_A0, &I8080::opcode_A1, &I8080::opcode_A2, &I8080::opcode_A3, &I8080::opcode_A4,
	&I8080::opcode_A5, &I8080::opcode_A6, &I8080::opcode_A7, &I8080::opcode_A8, &I8080::opcode_A9,
	&I8080::opcode_AA, &I8080::opcode_AB, &I8080::opcode_AC, &I8080::opcode_AD, &I8080::opcode_AE,
	&I8080::opcode_AF, &I8080::opcode_B0, &I8080::opcode_B1, &I8080::opcode_B2, &I8080::opcode_B3,
	&I8080::opcode_B4, &I8080::opcode_B5, &I8080::opcode_B6, &I8080::opcode_B7,	&I8080::opcode_B8,
	&I8080::opcode_B9, &I8080::opcode_BA, &I8080::opcode_BB, &I8080::opcode_BC, &I8080::opcode_BD,
	&I8080::opcode_BE, &I8080::opcode_BF, &I8080::opcode_C0, &I8080::opcode_C1, &I8080::opcode_C2,
	&I8080::opcode_C3, &I8080::opcode_C4, &I8080::opcode_C5, &I8080::opcode_C6, &I8080::opcode_C7,
	&I8080::opcode_C8, &I8080::opcode_C9, &I8080::opcode_CA, &I8080::opcode_CB, &I8080::opcode_CC,
	&I8080::opcode_CD, &I8080::opcode_CE, &I8080::opcode_CF, &I8080::opcode_D0, &I8080::opcode_D1,
	&I8080::opcode_D2, &I8080::opcode_D3, &I8080::opcode_D4, &I8080::opcode_D5, &I8080::opcode_D6,
	&I8080::opcode_D7, &I8080::opcode_D8, &I8080::opcode_D9, &I8080::opcode_DA, &I8080::opcode_DB,
	&I8080::opcode_DC, &I8080::opcode_DD, &I8080::opcode_DE, &I8080::opcode_DF,	&I8080::opcode_E0,
	&I8080::opcode_E1, &I8080::opcode_E2, &I8080::opcode_E3, &I8080::opcode_E4, &I8080::opcode_E5,
	&I8080::opcode_E6, &I8080::opcode_E7, &I8080::opcode_E8, &I8080::opcode_E9, &I8080::opcode_EA,
	&I8080::opcode_EB, &I8080::opcode_EC, &I8080::opcode_ED, &I8080::opcode_EE, &I8080::opcode_EF,
	&I8080::opcode_F0, &I8080::opcode_F1, &I8080::opcode_F2, &I8080::opcode_F3, &I8080::opcode_F4,
	&I8080::opcode_F5, &I8080::opcode_F6, &I8080::opcode_F7, &I8080::opcode_F8, &I8080::opcode_F9,
	&I8080::opcode_FA, &I8080::opcode_FB, &I8080::opcode_FC, &I8080::opcode_FD, &I8080::opcode_FE,
	&I8080::opcode_FF
};

void I8080::Reset()
{
    B = 0; 
    C = 0;
    D = 0; 
    E = 0;
    H = 0;
    L = 0;
    A = 0;
    F = 0;
    PC = 0;
    SP = 0;
}

void I8080::SetPSZTable()
{F = PSZ[A];}

void I8080::SetFlagsPSZ()
{F = (F & ~(Parity | Sign | Zero)) | PSZ[A];}

byte I8080::ReadByte(word addr)
{return memory[addr];}

void I8080::WriteByte(word addr, byte b)
{memory[addr] = b;}

word I8080::ReadWord(word addr)
{return memory[addr] | (word)(memory[addr + 1] << 8);}

void I8080::WriteWord(word addr, word w)
{
	memory[addr] = (w & 0xFF);
	memory[addr + 1] = (w >> 8);
}

word I8080::NextWord()
{
    word x = ReadWord(PC);
    PC += 2;
	
    return x;
}

byte I8080::DecByte(byte b)
{
	F = (F & ~(Zero | Sign | HalfCarry | Overflow));
	if ((b & 0x0F) == 0 ) F |= HalfCarry;
	b--;
	if (b == 0x7F) F |= Overflow;
	if (b & 0x80) F |= Sign;
	if (b == 0) F |= Zero;

	return b;
}

byte I8080::IncByte(byte b)
{
	b++;
	F &= ~(Zero | Sign | HalfCarry | Overflow);
	if (!(b & 0x0F)) F |= HalfCarry;
	if (b == 0x80) F |= Overflow;
	if (b & 0x80) F |= Sign;
	if (b == 0) F |= Zero;

	return b;
}

void I8080::AddByte(byte operand, byte carry)
{
	word x = A + operand;
	
	x += (carry) ? 1 : 0;
	
	F = 0;
	
    if ((x & 0xFF) == 0) F |= Zero;
    if (x & 0x80) F |= Sign;
    if (x >= 0x100) F |= Carry;
	
	// Halfcarry = ((accumulator+operand+halfcarry) ^ accumulator ^ operand) & 0x10
	// Check 8080 documentation
	if ((A^operand^x) & 0x10) F |= HalfCarry;
	
	// Overflow = ~(A ^ op) & (x ^ op) & 0x80
	if (~(A^operand) & (x^operand) & 0x80) F |= Overflow;

    A = x;
}

byte I8080::SubByte(byte operand, byte carry)
{
    byte x = A - operand;

	x -= (carry) ? 1 : 0;

    F = 0;
    if (x == 0) F |= Zero;
    if (x & 0x80) F |= Sign;
    if ((x >= A) && (operand | carry)) F |= Carry;

	// Halfcarry = ((accumulator+operand+halfcarry) ^ accumulator ^ operand) & 0x10
	// Check 8080 documentation
    if ((A^operand^x) & 0x10) F |= HalfCarry;

	// Overflow = ~(A ^ op) & (x ^ op) & 0x80
    if ((A^operand) & (x^A) & 0x80) F |= Overflow;

    return x;
}

void I8080::CallSub(word addr) 
{
	SP -= 2;
	WriteWord(SP, PC);
	PC = addr; // Revisar
}

void I8080::RetSub()
{
	PC = ReadWord(SP);
	SP += 2;
}

void I8080::Interruption()
{
	// Support only for RST
	cycles -= cycles_table[bus];
	(this ->* instruction[bus])();
	INT = false;
}

void I8080::Run(int n_cycles)
{
	running = true;
	cycles = n_cycles;
	
	while((cycles > 0) && (running))
	{
		IR = memory[PC++];
		if (INTE && INT)
		{
			cycles -= cycles_table[IR];
			(this ->* instruction[IR])();
			Interruption();
			INTE = false;
		}
		else
		{
			cycles -= cycles_table[IR];
			(this ->* instruction[IR])();
		}
	}
	
	if (!running)
	{
		cycles = 0;
	}
}

/*
8080's instructions implementation

Undocumented I8080 opcodes TODO:
08h, 10h, 18h, 20h, 28h, 30h, 38h  -  NOP
0CBh                               -  JMP
0D9h                               -  RET
0DDh, 0EDh, 0FDh                   -  CALL
*/

void I8080::opcode_00() // NOP
{}

void I8080::opcode_01() // LXI B
{
	C = ReadByte(PC++);
	B = ReadByte(PC++);
}

void I8080::opcode_02() // STAX B
{WriteByte(BC, A);}

void I8080::opcode_03() // INX B
{BC++;}

void I8080::opcode_04() // INR B
{B = IncByte(B);}

void I8080::opcode_05() // DCR B
{B = DecByte(B);}

void I8080::opcode_06() // MVI B
{B = ReadByte(PC++);}

void I8080::opcode_07() // RLC
{
    A = (A << 1) | (A >> 7);
    F &= ~(HalfCarry | Carry);
    if (A & 0x01) F |= Carry;
}

void I8080::opcode_08() // Unknown
{}

void I8080::opcode_09() // DAD B
{
    word x  = HL + BC;

    F &= (Sign | Zero | Parity);
    if ((HL + BC) > 0xFFFF ) F |= Carry;
    if (((HL & 0xFFF) + (BC & 0xFFF)) > 0xFFF ) F |= HalfCarry;

    L = x & 0xFF;
    H = x >> 8;
}

void I8080::opcode_0A() // LDAX B
{A = ReadByte(BC);}

void I8080::opcode_0B() // DCX B
{BC--;}

void I8080::opcode_0C() // INR C
{C = IncByte(C);}

void I8080::opcode_0D() // DCR C
{C = DecByte(C);}

void I8080::opcode_0E() // MVI C
{C = ReadByte(PC++);}

void I8080::opcode_0F() // RRC
{
	A = (A >> 1) | (A << 7);
    F &= ~(HalfCarry | Carry);
    if (A & 0x80 ) F |= Carry;
}

void I8080::opcode_10() // Unknown
{}

void I8080::opcode_11() // LXI D
{
    E = ReadByte(PC++);
    D = ReadByte(PC++);
}

void I8080::opcode_12() // STAX D
{WriteByte(DE, A);}

void I8080::opcode_13() // INX D
{DE++;}

void I8080::opcode_14() // INR D
{D = IncByte(D);}

void I8080::opcode_15() // DCR D
{D = DecByte(D);}

void I8080::opcode_16() // MVI D
{D = ReadByte(PC++);}

void I8080::opcode_17() // RAL
{
    byte a = A;

    A <<= 1;
    if (F & Carry) A |= 0x01;
    F &= ~(HalfCarry | Carry);
    if (a & 0x80) F |= Carry;
}

void I8080::opcode_18() // Unknown
{}

void I8080::opcode_19() // DAD D
{
    word x  = HL + DE;

    F &= (Sign | Zero | Parity);
    if ((HL + DE) > 0xFFFF) F |= Carry;
    if (((HL & 0xFFF) + (DE & 0xFFF)) > 0xFFF ) F |= HalfCarry;

    L = x & 0xFF;
    H = x >> 8;
}

void I8080::opcode_1A() // LDAX D
{A = ReadByte(DE);}

void I8080::opcode_1B() // DCX D
{DE--;}

void I8080::opcode_1C() // INR E
{E = IncByte(E);}

void I8080::opcode_1D() // DCR E
{E = DecByte(E);}

void I8080::opcode_1E() // MVI E
{E = ReadByte(PC++);}

void I8080::opcode_1F() // RAR
{
    byte a = A;

    A >>= 1;
    if (F & Carry) A |= 0x80;
    F &= ~(HalfCarry | Carry);
    if (a & 0x01) F |= Carry;
}

void I8080::opcode_20() // Unknown
{}

void I8080::opcode_21() // LXI H
{
    L = ReadByte(PC++);
    H = ReadByte(PC++);
}

void I8080::opcode_22() // SHLD
{
    word x = NextWord();

    WriteByte(x, L);
    WriteByte(x + 1, H);
}

void I8080::opcode_23() // INX H
{HL++;}

void I8080::opcode_24() // INR H
{H = IncByte(H);}

void I8080::opcode_25() // DCR H
{H = DecByte(H);}

void I8080::opcode_26() // MVI H
{H = ReadByte(PC++);}

void I8080::opcode_27() // DAA
{
	if (((A & 0x0F) > 9) || (F & HalfCarry))
	{
		A += 0x06;
		F |= HalfCarry;
	}
	else
	{
		F &= ~HalfCarry;
	}

	if ((A > 0x9F) || (F & Carry))
	{
		A += 0x60;
		F |= Carry;
	}
	else
	{
		F &= ~Carry;
	}

	SetFlagsPSZ();
}

void I8080::opcode_28() // Unknown
{}

void I8080::opcode_29() // DAD H
{
    word RP = HL;
    word x  = HL + RP;

    F &= (Sign | Zero | Parity);
    if ((HL + RP) > 0xFFFF) F |= Carry;
    if (((HL & 0xFFF) + (RP & 0xFFF)) > 0xFFF) F |= HalfCarry;

    L = x & 0xFF;
    H = x >> 8;
}

void I8080::opcode_2A() // LHLD
{
    word x = NextWord();

    L = ReadByte(x);
    H = ReadByte(x + 1);
}

void I8080::opcode_2B() // DCX HL
{HL--;}

void I8080::opcode_2C() // INR L
{L = IncByte(L);}

void I8080::opcode_2D() // DCR L
{L = DecByte(L);}

void I8080::opcode_2E() // MVI L
{L = ReadByte(PC++);}

void I8080::opcode_2F() // CMA
{
    A ^= 0xFF;
    F |= HalfCarry;
}

void I8080::opcode_30() // Unknown - SIM in 8085
{}

void I8080::opcode_31() // LXI SP
{SP = NextWord();}

void I8080::opcode_32() // STA
{WriteByte(NextWord(), A);}

void I8080::opcode_33() // INX SP
{SP++;}

void I8080::opcode_34() // INR M
{WriteByte(HL, IncByte(ReadByte(HL)));}

void I8080::opcode_35() // DCR M
{WriteByte(HL, DecByte(ReadByte(HL)));}

void I8080::opcode_36() // MVI M
{WriteByte(HL, ReadByte(PC++));}

void I8080::opcode_37() // STC
{
	F |= Carry;
}

void I8080::opcode_38() // Unknown
{}

void I8080::opcode_39() // DAD SP
{
    word RP = SP;
    word x  = HL + RP;

    F &= (Sign | Zero | Parity);
    if ((HL + RP) > 0xFFFF) F |= Carry;
    if (((HL & 0xFFF) + (RP & 0xFFF)) > 0xFFF) F |= HalfCarry;

    L = x & 0xFF;
    H = x >> 8;
}

void I8080::opcode_3A() // LDA
{A = ReadByte(NextWord());}

void I8080::opcode_3B() // DCX SP
{SP--;}

void I8080::opcode_3C() // INR A
{A = IncByte(A);}

void I8080::opcode_3D() // DCR A
{A = DecByte(A);}

void I8080::opcode_3E() // MVI A
{A = ReadByte(PC++);}

void I8080::opcode_3F() // CMF
{F ^= Carry;}

void I8080::opcode_40() // MOV B, B
{}

void I8080::opcode_41() // MOV B, C
{B = C;}

void I8080::opcode_42() // MOV B, D
{B = D;}

void I8080::opcode_43() // MOV B, E
{B = E;}

void I8080::opcode_44() // MOV B, H
{B = H;}

void I8080::opcode_45() // MOV B, L
{B = L;}

void I8080::opcode_46() // MOV B, M
{B = ReadByte(HL);}

void I8080::opcode_47() // MOV B, A
{B = A;}

void I8080::opcode_48() // MOV C, B
{C = B;}

void I8080::opcode_49() // MOV C, C
{}

void I8080::opcode_4A() // MOV C, D
{C = D;}

void I8080::opcode_4B() // MOV C, E
{C = E;}

void I8080::opcode_4C() // MOV C, H
{C = H;}

void I8080::opcode_4D() // MOV C, L
{C = L;}

void I8080::opcode_4E() // MOV C, M
{C = ReadByte(HL);}

void I8080::opcode_4F() // MOV C, A
{C = A;}

void I8080::opcode_50() // MOV D, B
{D = B;}

void I8080::opcode_51() // MOV D, C
{D = C;}

void I8080::opcode_52() // MOV D, D
{}

void I8080::opcode_53() // MOV D, E
{D = E;}

void I8080::opcode_54() // MOV D, H
{D = H;}

void I8080::opcode_55() // MOV D, L
{D = L;}

void I8080::opcode_56() // MOV D, M
{D = ReadByte(HL);}

void I8080::opcode_57() // MOV D, A
{D = A;}

void I8080::opcode_58() // MOV E, B
{E = B;}

void I8080::opcode_59() // MOV E, C
{E = C;}

void I8080::opcode_5A() // MOV E, D
{E = D;}

void I8080::opcode_5B() // MOV E, E
{}

void I8080::opcode_5C() // MOV E, H
{E = H;}

void I8080::opcode_5D() // MOV E, L
{E = L;}

void I8080::opcode_5E() // MOV E, M
{E = ReadByte(HL);}

void I8080::opcode_5F() // MOV E, A 
{E = A;}

void I8080::opcode_60() // MOV H, B
{H = B;}

void I8080::opcode_61() // MOV H, C
{H = C;}

void I8080::opcode_62() // MOV H, D
{H = D;}

void I8080::opcode_63() // MOV H, E
{H = E;}

void I8080::opcode_64() // MOV H, H
{}

void I8080::opcode_65() // MOV H, L
{H = L;}

void I8080::opcode_66() // MOV H, M
{H = ReadByte(HL);}

void I8080::opcode_67() // MOV H, A
{H = A;}

void I8080::opcode_68() // MOV L, B
{L = B;}

void I8080::opcode_69() // MOV L, C
{L = C;}

void I8080::opcode_6A() // MOV L, D
{L = D;}

void I8080::opcode_6B() // MOV L, E
{L = E;}

void I8080::opcode_6C() // MOV L, H
{L = H;}

void I8080::opcode_6D() // MOV L, L
{}

void I8080::opcode_6E() // MOV L, M
{L = ReadByte(HL);}

void I8080::opcode_6F() // MOV L, A
{L = A;}

void I8080::opcode_70() // MOV M, B
{WriteByte(HL, B);}

void I8080::opcode_71() // MOV M, C
{WriteByte(HL, C);}

void I8080::opcode_72() // MOV M, D
{WriteByte(HL, D);}

void I8080::opcode_73() // MOV M, E
{WriteByte(HL, E);}

void I8080::opcode_74() // MOV M, H
{WriteByte(HL, H);}

void I8080::opcode_75() // MOV M, L
{WriteByte(HL, L);}

void I8080::opcode_76() // HALT
{
	//REVISAR!!!! 
	//iflags_ |= FlagHalted;
	running = false;
}

void I8080::opcode_77() // MOV M, A
{WriteByte(HL, A);}

void I8080::opcode_78() // MOV A, B
{A = B;}

void I8080::opcode_79() // MOV A, C
{A = C;}

void I8080::opcode_7A() // MOV A, D
{A = D;}

void I8080::opcode_7B() // MOV A, E
{A = E;}

void I8080::opcode_7C() // MOV A, H
{A = H;}

void I8080::opcode_7D() // MOV A, L
{A = L;}

void I8080::opcode_7E() // MOV A, M
{A = ReadByte(HL);}

void I8080::opcode_7F() // MOV A, A
{}

void I8080::opcode_80() // ADD B
{AddByte(B, 0);}

void I8080::opcode_81() // ADD C
{AddByte(C, 0);}

void I8080::opcode_82() // ADD D
{AddByte(D, 0);}

void I8080::opcode_83() // ADD E
{AddByte(E, 0);}

void I8080::opcode_84() // ADD H
{AddByte(H, 0);}

void I8080::opcode_85() // ADD L
{AddByte(L, 0);}

void I8080::opcode_86() // ADD M
{AddByte(ReadByte(HL), 0);}

void I8080::opcode_87() // ADD A
{AddByte(A, 0);}

void I8080::opcode_88() // ADC B
{AddByte(B, F & Carry);}

void I8080::opcode_89() // ADC C
{AddByte(C, F & Carry);}

void I8080::opcode_8A() // ADC D
{AddByte(D, F & Carry);}

void I8080::opcode_8B() // ADC E
{AddByte(E, F & Carry);}

void I8080::opcode_8C() // ADC H
{AddByte(H, F & Carry);}

void I8080::opcode_8D() // ADC L
{AddByte(L, F & Carry);}

void I8080::opcode_8E() // ADC M
{AddByte(ReadByte(HL), F & Carry);}

void I8080::opcode_8F() // ADC A
{AddByte(A, F & Carry);}

void I8080::opcode_90() // SUB B
{A = SubByte(B, 0);}

void I8080::opcode_91() // SUB C
{A = SubByte(C, 0);}

void I8080::opcode_92() // SUB D
{A = SubByte(D, 0);}

void I8080::opcode_93() // SUB E
{A = SubByte(E, 0);}

void I8080::opcode_94() // SUB H
{A = SubByte(H, 0);}

void I8080::opcode_95() // SUB L
{A = SubByte(L, 0);}

void I8080::opcode_96() // SUB M
{A = SubByte(ReadByte(HL), 0);}

void I8080::opcode_97() // SUB A
{A = SubByte(B, 0);}

void I8080::opcode_98() // SBB B
{A = SubByte(B, F & Carry);}

void I8080::opcode_99() // SBB C
{A = SubByte(C, F & Carry);}

void I8080::opcode_9A() // SBB D
{A = SubByte(D, F & Carry);}

void I8080::opcode_9B() // SBB E
{A = SubByte(E, F & Carry);}

void I8080::opcode_9C() // SBB H
{A = SubByte(H, F & Carry);}

void I8080::opcode_9D() // SBB L
{A = SubByte(L, F & Carry);}

void I8080::opcode_9E() // SBB M
{A = SubByte(ReadByte(HL), F & Carry);}

void I8080::opcode_9F() // SBB A
{A = SubByte(A, F & Carry);}

void I8080::opcode_A0() // ANA B
{
    A &= B;
    SetPSZTable();
}

void I8080::opcode_A1() // ANA C
{
    A &= C;
    SetPSZTable();
}

void I8080::opcode_A2() // ANA D
{
    A &= D;
    SetPSZTable();
}

void I8080::opcode_A3() // ANA E
{
    A &= E;
    SetPSZTable();
}

void I8080::opcode_A4() // ANA H
{
    A &= H;
    SetPSZTable();
}

void I8080::opcode_A5() // ANA L
{
    A &= L;
    SetPSZTable();
}

void I8080::opcode_A6() // ANA M
{
    A &= ReadByte(HL);
    SetPSZTable();
}

void I8080::opcode_A7() // ANA A
{SetPSZTable();}

void I8080::opcode_A8() // XRA B
{
	A ^= B;
	SetPSZTable();
}

void I8080::opcode_A9() // XRA C
{
	A ^= C;
	SetPSZTable();
}

void I8080::opcode_AA() // XRA D
{
	A ^= D;
	SetPSZTable();
}

void I8080::opcode_AB() // XRA E
{
	A ^= E;
	SetPSZTable();
}

void I8080::opcode_AC() // XRA H
{
	A ^= H;
	SetPSZTable();
}

void I8080::opcode_AD() // XRA L
{
	A ^= L;
	SetPSZTable();
}

void I8080::opcode_AE() // XRA M
{
	A ^= ReadByte(HL);
	SetPSZTable();
}

void I8080::opcode_AF() // XRA A
{
	A = 0;
	SetPSZTable();
}

void I8080::opcode_B0() // ORA B
{
	A ^= B;
	SetPSZTable();
}

void I8080::opcode_B1() // ORA C
{
	A |= C;
	SetPSZTable();
}

void I8080::opcode_B2() // ORA D
{
	A |= D;
	SetPSZTable();
}

void I8080::opcode_B3() // ORA E
{
	A |= E;
	SetPSZTable();
}

void I8080::opcode_B4() // ORA H
{
	A |= H;
	SetPSZTable();
}

void I8080::opcode_B5() // ORA L
{
	A |= L;
	SetPSZTable();
}

void I8080::opcode_B6() // ORA M
{
	A |= ReadByte(HL);
	SetPSZTable();
}

void I8080::opcode_B7() // ORA A
{SetPSZTable();}

void I8080::opcode_B8() // CMP B
{SubByte(B, 0);}

void I8080::opcode_B9() // CMP C
{SubByte(C, 0);}

void I8080::opcode_BA() // CMP D
{SubByte(D, 0);}

void I8080::opcode_BB() // CMP E
{SubByte(E, 0);}

void I8080::opcode_BC() // CMP H
{SubByte(H, 0);}

void I8080::opcode_BD() // CMP L
{SubByte(L, 0);}

void I8080::opcode_BE() // CMP M
{SubByte(ReadByte(HL), 0);}

void I8080::opcode_BF() // CMP A
{SubByte(A, 0);}

void I8080::opcode_C0() // RNZ
{
    if (!(F & Zero))
	{
        RetSub();
        cycles -= 6;
    }
}

void I8080::opcode_C1() // POP B
{
    C = ReadByte(SP++);
    B = ReadByte(SP++);
}

void I8080::opcode_C2() // JNZ nn
{
	if (!(F & Zero))
	{
		PC = NextWord();

		cycles -= 5;
    }
}

void I8080::opcode_C3() // JMP nn
{PC = ReadWord(PC);}

void I8080::opcode_C4() // CNZ nn
{
    word nn = NextWord();

    if (!(F & Zero))
	{
        CallSub(nn);
        cycles -= 7;
    }
}

void I8080::opcode_C5() // PUSH B
{
	WriteByte(--SP, B);
	WriteByte(--SP, C);
}

void I8080::opcode_C6() // ADI n
{AddByte(ReadByte(PC++), 0);}

void I8080::opcode_C7() // RST 0
{CallSub(0x00);}

void I8080::opcode_C8() // RZ
{
    if (F & Zero)
	{
        RetSub();
        cycles -= 6;
    }
}

void I8080::opcode_C9() // RET
{RetSub();}

void I8080::opcode_CA() // JZ nn
{
	if (F & Zero)
	{
		PC = NextWord();
		cycles -= 5;
	}
}

void I8080::opcode_CB() // Unknown
{}

void I8080::opcode_CC() // CZ nn
{
	word nn = NextWord();

	if (F & Zero)
	{
		CallSub(nn);
		cycles -= 7;
	}
}

void I8080::opcode_CD() // CALL nn
{CallSub(NextWord());}

void I8080::opcode_CE() // ACI n
{AddByte(ReadByte(PC++), F & Carry);}

void I8080::opcode_CF() // RST 1
{CallSub(0x08);}

void I8080::opcode_D0() // RNC
{
	if (!(F & Carry))
	{
		RetSub();
		cycles -= 6;
	}
}

void I8080::opcode_D1() // POP D
{
	E = ReadByte(SP++);
	D = ReadByte(SP++);
}

void I8080::opcode_D2() // JNC nn
{
	if (!(F & Carry))
	{
		PC = NextWord();
		cycles -= 5;
	}
}

void I8080::opcode_D3() // OUT n
{WritePort(drv, ReadByte(PC++), A);}

void I8080::opcode_D4() // CNC nn
{
	word nn = NextWord();

	if (!(F & Carry))
	{
		CallSub(nn);
		cycles -= 7;
	}
}

void I8080::opcode_D5() // PUSH D
{
	WriteByte(--SP, D);
	WriteByte(--SP, E);
}

void I8080::opcode_D6() // SUI n
{
	A = SubByte(ReadByte(PC++), 0);
}

void I8080::opcode_D7() // RST 2
{
	CallSub(0x10);
}

void I8080::opcode_D8() // RC
{
	if (F & Carry)
	{
		RetSub();
		cycles -= 6;
	}
}

void I8080::opcode_D9() // Unknown
{}

void I8080::opcode_DA() // JC nn
{
	if (F & Carry)
	{
		PC = NextWord();
		cycles -= 5;
	}
}

void I8080::opcode_DB() // IN n
{A = ReadPort(drv, ReadByte(PC++));}

void I8080::opcode_DC() // CC nn
{
	word nn = NextWord();
	
	if (F & Carry)
	{
		CallSub(nn);
		cycles -= 7;
	}
}

void I8080::opcode_DD() // Unknown
{}

void I8080::opcode_DE() // SBI n
{A = SubByte(ReadByte(PC++), F & Carry );}

void I8080::opcode_DF() // RST 3
{CallSub(0x18);}

void I8080::opcode_E0() // RPO
{
	if (!(F & Parity))
	{
		RetSub();
		cycles -= 6;
	}
}

void I8080::opcode_E1() // POP H
{
	L = ReadByte(SP++);
	H = ReadByte(SP++);
}

void I8080::opcode_E2() // JPO nn
{
	if (!(F & Parity))
	{
		PC = NextWord();
		cycles -= 5;
	}
}

void I8080::opcode_E3() // XTHL
{
	byte x;

	x = ReadByte(SP); WriteByte(SP, L); L = x; // SP <-> L
	x = ReadByte(SP + 1); WriteByte(SP + 1, H); H = x; // SP + 1 <-> H
}

void I8080::opcode_E4() // CPO nn
{
	word nn = NextWord();

    if (!(F & Parity))
	{
        CallSub(nn);
        cycles -= 7;
    }
}

void I8080::opcode_E5() // PUSH H
{
    WriteByte(--SP, H);
    WriteByte(--SP, L);
}

void I8080::opcode_E6() // ANI n
{
	A &= ReadByte(PC++);
	SetPSZTable();
}

void I8080::opcode_E7() // RST 4
{CallSub(0x20);}

void I8080::opcode_E8() // RPE
{
	if (F & Parity)
	{
		RetSub();
		cycles -= 6;
	}
}

void I8080::opcode_E9() // PCHL
{PC = HL;}

void I8080::opcode_EA() // JPE nn
{
	if (F & Parity)
	{
		PC = NextWord();
		cycles -= 5;
	}
}

void I8080::opcode_EB() // XCHG
{
	byte x;

	x = D; D = H; H = x; // D <-> H
	x = E; E = L; L = x; // E <-> L
}

void I8080::opcode_EC() // CPE nn
{
	word nn = NextWord();

	if (F & Parity)
	{
		CallSub(nn);
		cycles -= 7;
	}
}

void I8080::opcode_ED() // Unknown
{}

void I8080::opcode_EE() // XRI n
{
    A ^= ReadByte(PC++);
    SetPSZTable();
}

void I8080::opcode_EF() // RST 5
{CallSub(0x28);}

void I8080::opcode_F0() // RP
{
    if (!(F & Sign))
	{
		RetSub();
		cycles -= 6;
    }
}

void I8080::opcode_F1() // POP PSW
{
	// Program Status Word in stack to A and Flags
	F = ReadByte(SP++);
	A = ReadByte(SP++);
}

void I8080::opcode_F2() // JP nn
{
    if (!(F & Sign))
	{
		PC = NextWord();
		cycles -= 5;
    }
}

void I8080::opcode_F3() // DI
{INTE = false;}

void I8080::opcode_F4() // CP nn
{
    word nn = NextWord();

    if (!(F & Sign))
	{
        CallSub(nn);
        cycles -= 7;
    }
}

void I8080::opcode_F5() // PUSH PSW
{
	// Program Status Word to stack
	WriteByte(--SP, A);
	WriteByte(--SP, F);
}

void I8080::opcode_F6() // ORI n
{
    A |= ReadByte(PC++);
    SetPSZTable();
}

void I8080::opcode_F7() // RST  6
{CallSub(0x30);}

void I8080::opcode_F8() // RM
{
    if (F & Sign)
	{
        RetSub();
        cycles -= 6;
    }
}

void I8080::opcode_F9() // SPHL
{SP = HL;}

void I8080::opcode_FA() // JM nn
{
    if (F & Sign)
	{
        PC = NextWord();
        cycles -= 5;
    }

}

void I8080::opcode_FB() // EI
{INTE = true;}

void I8080::opcode_FC() // CM nn
{   
	word nn = NextWord();

	if (F & Sign)
	{
		CallSub(nn);
		cycles -= 7;
	}
}

void I8080::opcode_FD() // Unknown
{}

void I8080::opcode_FE() // CPI n
{SubByte(ReadByte(PC++), 0);}

void I8080::opcode_FF() // RST  7
{CallSub(0x38);}

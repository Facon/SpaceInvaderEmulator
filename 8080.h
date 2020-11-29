#ifndef I8080_H
#define I8080_H

#include <string>
#include "core.h"
#include "driver.h"

using namespace std;

class I8080;

// function pointer for opcodes
typedef void (I8080::*OPTABLE)();

class I8080
{
	protected:
		enum Flags
		{
			Carry     = 0x01,
			// Bit 1 is 1
			Parity    = 0x04, Overflow = Parity,    // Same bit used for parity and overflow
			// Bit 3 is 0
			HalfCarry = 0x10, AuxCarry = HalfCarry,
			// Bit 5 is 0
			Zero      = 0x40,
			Sign      = 0x80
		};
		
		bool INT, INTE, running;

		// Accumulator + General Purpose Registers + Flags
		union
		{
			struct
			{
				#ifdef BIGENDIAN
				byte A, F, B, C, D, E, H, L; // "Correct" order
				#else
				byte F, A, C, B, D, E, L, H;
				#endif
			};
			
			struct
			{
				word AF, BC, DE, HL;
			};
		};
		
		byte IR; // Instruction Register
		word PC; // Program Counter
		word SP; // Stack Pointer
		
		byte bus; // For interruption
		byte bus_aux[2]; // For 2+ byte instruction

		byte *memory; // Pointer for memory map

		int cycles; // Actual cycles
		
		// Parity, sign, zero table, it takes all possible combinations
		static const byte PSZ[256];
		
		Driver *drv;

		void SetPSZTable();
		
		// Read the content of addr and return it
		byte ReadByte(word addr);

		// Write b in addr address
		void WriteByte(word addr, byte b);
		
		// Read the content of addr and addr + 1 and return it in a word
		word ReadWord(word addr);
		
		// Write w in addr and addr + 1 address
		void WriteWord(word addr, word w);
		
		// b--, flags checking
		byte DecByte(byte b);
		
		// b++, flags checking
		byte IncByte(byte b);
		
		// A += operand, with carry, flags checking
		void AddByte(byte operand, byte carry);
		
		// A -= operand, with carry, flags checking
		byte SubByte(byte operand, byte carry);
		
		// Return a word from PC and PC + 1
		word NextWord();
		
		// Sets parity, zero and sign according to accumulator value
		void SetFlagsPSZ();
		
		// Calls management
		void CallSub(word addr);
		
		// Return of Calls in Stack
		void RetSub();
		
		// Reads one byte from port
		Pf_READ_PORT_8 ReadPort;
		
		// Write one byte to port
		Pf_WRITE_PORT_8 WritePort;
		
		static const OPTABLE instruction[256];

		void opcode_00(); // NOP
		void opcode_01(); // LXI B
		void opcode_02(); // STAX B
		void opcode_03(); // INX B
		void opcode_04(); // INR B
		void opcode_05(); // DCR B
		void opcode_06(); // MVI B
		void opcode_07(); // RLC
		void opcode_08(); // Unknown
		void opcode_09(); // DAD B
		void opcode_0A(); // LDAX B
		void opcode_0B(); // DCX B
		void opcode_0C(); // INR C
		void opcode_0D(); // DCR C
		void opcode_0E(); // MVI C
		void opcode_0F(); // RRC
		void opcode_10(); // Unknown
		void opcode_11(); // LXI D
		void opcode_12(); // STAX D
		void opcode_13(); // INX D
		void opcode_14(); // INR D
		void opcode_15(); // DCR D
		void opcode_16(); // MVI D
		void opcode_17(); // RAL
		void opcode_18(); // Unknown
		void opcode_19(); // DAD D
		void opcode_1A(); // LDAX D
		void opcode_1B(); // DCX D
		void opcode_1C(); // INR E
		void opcode_1D(); // DCR E
		void opcode_1E(); // MVI E
		void opcode_1F(); // RAR
		void opcode_20(); // Unknown
		void opcode_21(); // LXI H
		void opcode_22(); // SHLD
		void opcode_23(); // INX H
		void opcode_24(); // INR H
		void opcode_25(); // DCR H
		void opcode_26(); // MVI H
		void opcode_27(); // DAA
		void opcode_28(); // Unknown
		void opcode_29(); // DAD H
		void opcode_2A(); // LHLD
		void opcode_2B(); // DCX HL
		void opcode_2C(); // INR L
		void opcode_2D(); // DCR L
		void opcode_2E(); // MVI L
		void opcode_2F(); // CMA
		void opcode_30(); // Unknown - SIM in 8085
		void opcode_31(); // LXI SP
		void opcode_32(); // STA
		void opcode_33(); // INX SP
		void opcode_34(); // INR M
		void opcode_35(); // DCR M
		void opcode_36(); // MVI M
		void opcode_37(); // STC
		void opcode_38(); // Unknown
		void opcode_39(); // DAD SP
		void opcode_3A(); // LDA
		void opcode_3B(); // DCX SP
		void opcode_3C(); // INR A
		void opcode_3D(); // DCR A
		void opcode_3E(); // MVI A
		void opcode_3F(); // CMF
		void opcode_40(); // MOV B, B
		void opcode_41(); // MOV B, C
		void opcode_42(); // MOV B, D
		void opcode_43(); // MOV B, E
		void opcode_44(); // MOV B, H
		void opcode_45(); // MOV B, L
		void opcode_46(); // MOV B, M
		void opcode_47(); // MOV B, A
		void opcode_48(); // MOV C, B
		void opcode_49(); // MOV C, C
		void opcode_4A(); // MOV C, D
		void opcode_4B(); // MOV C, E
		void opcode_4C(); // MOV C, H
		void opcode_4D(); // MOV C, L
		void opcode_4E(); // MOV C, M
		void opcode_4F(); // MOV C, A
		void opcode_50(); // MOV D, B
		void opcode_51(); // MOV D, C
		void opcode_52(); // MOV D, D
		void opcode_53(); // MOV D, E
		void opcode_54(); // MOV D, H
		void opcode_55(); // MOV D, L
		void opcode_56(); // MOV D, M
		void opcode_57(); // MOV D, A
		void opcode_58(); // MOV E, B
		void opcode_59(); // MOV E, C
		void opcode_5A(); // MOV E, D
		void opcode_5B(); // MOV E, E
		void opcode_5C(); // MOV E, H
		void opcode_5D(); // MOV E, L
		void opcode_5E(); // MOV E, M
		void opcode_5F(); // MOV E, A
		void opcode_60(); // MOV H, B
		void opcode_61(); // MOV H, C
		void opcode_62(); // MOV H, D
		void opcode_63(); // MOV H, E
		void opcode_64(); // MOV H, H
		void opcode_65(); // MOV H, L
		void opcode_66(); // MOV H, M
		void opcode_67(); // MOV H, A
		void opcode_68(); // MOV L, B
		void opcode_69(); // MOV L, C
		void opcode_6A(); // MOV L, D
		void opcode_6B(); // MOV L, E
		void opcode_6C(); // MOV L, H
		void opcode_6D(); // MOV L, L
		void opcode_6E(); // MOV L, M
		void opcode_6F(); // MOV L, A
		void opcode_70(); // MOV M, B
		void opcode_71(); // MOV M, C
		void opcode_72(); // MOV M, D
		void opcode_73(); // MOV M, E
		void opcode_74(); // MOV M, H
		void opcode_75(); // MOV M, L
		void opcode_76(); // HALT
		void opcode_77(); // MOV M, A
		void opcode_78(); // MOV A, B
		void opcode_79(); // MOV A, C
		void opcode_7A(); // MOV A, D
		void opcode_7B(); // MOV A, E
		void opcode_7C(); // MOV A, H
		void opcode_7D(); // MOV A, L
		void opcode_7E(); // MOV A, M
		void opcode_7F(); // MOV A, A
		void opcode_80(); // ADD B
		void opcode_81(); // ADD C
		void opcode_82(); // ADD D
		void opcode_83(); // ADD E
		void opcode_84(); // ADD H
		void opcode_85(); // ADD L
		void opcode_86(); // ADD M
		void opcode_87(); // ADD A
		void opcode_88(); // ADC B
		void opcode_89(); // ADC C
		void opcode_8A(); // ADC D
		void opcode_8B(); // ADC E
		void opcode_8C(); // ADC H
		void opcode_8D(); // ADC L
		void opcode_8E(); // ADC M
		void opcode_8F(); // ADC A
		void opcode_90(); // SUB B
		void opcode_91(); // SUB C
		void opcode_92(); // SUB D
		void opcode_93(); // SUB E
		void opcode_94(); // SUB H
		void opcode_95(); // SUB L
		void opcode_96(); // SUB M
		void opcode_97(); // SUB A
		void opcode_98(); // SBB B
		void opcode_99(); // SBB C
		void opcode_9A(); // SBB D
		void opcode_9B(); // SBB E
		void opcode_9C(); // SBB H
		void opcode_9D(); // SBB L
		void opcode_9E(); // SBB M
		void opcode_9F(); // SBB A
		void opcode_A0(); // ANA B
		void opcode_A1(); // ANA C
		void opcode_A2(); // ANA D
		void opcode_A3(); // ANA E
		void opcode_A4(); // ANA H
		void opcode_A5(); // ANA L
		void opcode_A6(); // ANA M
		void opcode_A7(); // ANA A
		void opcode_A8(); // XRA B
		void opcode_A9(); // XRA C
		void opcode_AA(); // XRA D
		void opcode_AB(); // XRA E
		void opcode_AC(); // XRA H
		void opcode_AD(); // XRA L
		void opcode_AE(); // XRA M
		void opcode_AF(); // XRA A
		void opcode_B0(); // ORA B
		void opcode_B1(); // ORA C
		void opcode_B2(); // ORA D
		void opcode_B3(); // ORA E
		void opcode_B4(); // ORA H
		void opcode_B5(); // ORA L
		void opcode_B6(); // ORA M
		void opcode_B7(); // ORA A
		void opcode_B8(); // CMP B
		void opcode_B9(); // CMP C
		void opcode_BA(); // CMP D
		void opcode_BB(); // CMP E 
		void opcode_BC(); // CMP H
		void opcode_BD(); // CMP L
		void opcode_BE(); // CMP M
		void opcode_BF(); // CMP A
		void opcode_C0(); // RNZ
		void opcode_C1(); // POP B
		void opcode_C2(); // JNZ nn
		void opcode_C3(); // JMP nn
		void opcode_C4(); // CNZ nn
		void opcode_C5(); // PUSH B
		void opcode_C6(); // ADI n
		void opcode_C7(); // RST 0
		void opcode_C8(); // RZ
		void opcode_C9(); // RET
		void opcode_CA(); // JZ nn
		void opcode_CB(); // Unknown
		void opcode_CC(); // CZ nn
		void opcode_CD(); // CALL nn
		void opcode_CE(); // ACI n
		void opcode_CF(); // RST 1
		void opcode_D0(); // RNC
		void opcode_D1(); // POP D
		void opcode_D2(); // JNC nn
		void opcode_D3(); // OUT n
		void opcode_D4(); // CNC nn
		void opcode_D5(); // PUSH D
		void opcode_D6(); // SUI n
		void opcode_D7(); // RST 2
		void opcode_D8(); // RC
		void opcode_D9(); // Unknown
		void opcode_DA(); // JC nn
		void opcode_DB(); // IN n
		void opcode_DC(); // CC nn
		void opcode_DD(); // Unknown
		void opcode_DE(); // SBI n
		void opcode_DF(); // RST 3
		void opcode_E0(); // RPO
		void opcode_E1(); // POP H
		void opcode_E2(); // JPO nn
		void opcode_E3(); // XTHL
		void opcode_E4(); // CPO nn
		void opcode_E5(); // PUSH H
		void opcode_E6(); // ANI n
		void opcode_E7(); // RST 4
		void opcode_E8(); // RPE
		void opcode_E9(); // PCHL
		void opcode_EA(); // JPE nn
		void opcode_EB(); // XCHG
		void opcode_EC(); // CPE nn
		void opcode_ED(); // Unknown
		void opcode_EE(); // XRI n
		void opcode_EF(); // RST 5
		void opcode_F0(); // RP
		void opcode_F1(); // POP PSW
		void opcode_F2(); // JP nn
		void opcode_F3(); // DI
		void opcode_F4(); // CP nn
		void opcode_F5(); // PUSH PSW
		void opcode_F6(); // ORI n
		void opcode_F7(); // RST  6
		void opcode_F8(); // RM
		void opcode_F9(); // SPHL
		void opcode_FA(); // JM nn
		void opcode_FB(); // EI
		void opcode_FC(); // CM nn
		void opcode_FD(); // Unknown
		void opcode_FE(); // CPI n
		void opcode_FF(); // RST 7

	public:		
		// Constructor
		// Arguments -> Memory + Function Pointers
		I8080(byte *memory, Driver *drv, Pf_WRITE_PORT_8 func1, Pf_READ_PORT_8 func2);
		
		// Getters
		byte GetA() const
		{return A;}
		byte GetB() const
		{return B;}
		byte GetC() const
		{return C;}
		byte GetD() const
		{return D;}
		byte GetE() const
		{return E;}
		byte GetH() const
		{return H;}
		byte GetL() const
		{return L;}
		byte GetFlags() const
		{return F;}
		byte GetBus() const
		{return bus;}
		byte GetIR() const
		{return IR;}
		word GetPC() const
		{return PC;}
		word GetAF() const
		{return AF;}
		word GetBC() const
		{return BC;}
		word GetHL() const
		{return HL;}
		word GetDE() const
		{return DE;}
		
		// Setters
		void SetA(const byte b)
		{A = b;}
		void SetB(const byte b)
		{B = b;}
		void SetC(const byte b)
		{C = b;}
		void SetD(const byte b)
		{D = b;}
		void SetE(const byte b)
		{E = b;}
		void SetH(const byte b)
		{H = b;}
		void SetL(const byte b)
		{L = b;}
		void SetFlags(const byte b)
		{F = b;}
		void SetBus(const byte b)
		{bus = b;}
		void SetIR(const byte b)
		{IR = b;}
		void SetPC(const word w)
		{PC = w;}
		void SetINT()
		{INT = true;}
		
		void Run(int n_cycles);
		void Reset();
		void Interruption();
		string Debug(byte opcode);
		
		static const uchar cycles_table[256];
};

#endif

#include <string>
#include "8080.h"

string I8080::Debug(byte opcode)
{
	string ret;		
	
	switch (opcode)
	{
		case 0x00:
			return "0x00 - NOP";
			break;
		case 0x01:
			return "0x01 - LXI B";
			break;
		case 0x02:
			return "0x02 - STAX B";
			break;
		case 0x03:
			return "0x03 - INX B";
			break;
		case 0x04:
			return "0x04 - INR B";
			break;
		case 0x05:
			return "0x05 - DCR B";
			break;
		case 0x06:
			return "0x06 - MVI B";
			break;
		case 0x07:
			return "0x07 - RLC";
			break;
		case 0x08:
			return "0x08 - Unknown";
			break;
		case 0x09:
			return "0x09 - DAD B";
			break;
		case 0x0A:
			return "0x0A - LDAX B";
			break;
		case 0x0B:
			return "0x0B - DCX B";
			break;
		case 0x0C:
			return "0x0C - INR C";
			break;
		case 0x0D:
			return "0x0D - DCR C";
			break;
		case 0x0E:
			return "0x0E - MVI C";
			break;
		case 0x0F:
			return "0x0F - RRC";
			break;
		case 0x10:
			return "0x10 - Unknown";
			break;
		case 0x11:
			return "0x11 - LXI D";
			break;
		case 0x12:
			return "0x12 - STAX D";
			break;
		case 0x13:
			return "0x13 - INX D";
			break;
		case 0x14:
			return "0x14 - INR D";
			break;
		case 0x15:
			return "0x15 - DCR D";
			break;
		case 0x16:
			return "0x16 - MVI D";
			break;
		case 0x17:
			return "0x17 - RAL";
			break;
		case 0x18:
			return "0x18 - Unknown";
			break;
		case 0x19:
			return "0x19 - DAD D";
			break;
		case 0x1A:
			return "0x1A - LDAX D";
			break;
		case 0x1B:
			return "0x1B - DCX D";
			break;
		case 0x1C:
			return "0x1C - INR E";
			break;
		case 0x1D:
			return "0x1D - DCR E";
			break;
		case 0x1E:
			return "0x1E - MVI E";
			break;
		case 0x1F:
			return "0x1F - RAR";
			break;
		case 0x20:
			return "0x20 - Unknown";
			break;
		case 0x21:
			return "0x21 - LXI H";
			break;
		case 0x22:
			return "0x22 - SHLD";
			break;
		case 0x23:
			return "0x23 - INX H";
			break;
		case 0x24:
			return "0x24 - INR H";
			break;
		case 0x25:
			return "0x25 - DCR H";
			break;
		case 0x26:
			return "0x26 - MVI H";
			break;
		case 0x27:
			return "0x27 - DAA";
			break;
		case 0x28:
			return "0x28 - Unknown";
			break;
		case 0x29:
			return "0x29 - DAD H";
			break;
		case 0x2A:
			return "0x2A - LHLD";
			break;
		case 0x2B:
			return "0x2B - DCX HL";
			break;
		case 0x2C:
			return "0x2C - INR L";
			break;
		case 0x2D:
			return "0x2D - DCR L";
			break;
		case 0x2E:
			return "0x2E - MVI L";
			break;
		case 0x2F:
			return "0x2F - CMA";
			break;
		case 0x30:
			return "0x30 - SIM in 8085";
			break;
		case 0x31:
			return "0x31 - LXI SP";
			break;
		case 0x32:
			return "0x32 - STA";
			break;
		case 0x33:
			return "0x33 - INX SP";
			break;
		case 0x34:
			return "0x34 - INR M";
			break;
		case 0x35:
			return "0x35 - DCR M";
			break;
		case 0x36:
			return "0x36 - MVI M";
			break;
		case 0x37:
			return "0x37 - STC";
			break;
		case 0x38:
			return "0x38 - Unknown";
			break;
		case 0x39:
			return "0x39 - DAD SP";
			break;
		case 0x3A:
			return "0x3A - LDA";
			break;
		case 0x3B:
			return "0x3B - DCX SP";
			break;
		case 0x3C:
			return "0x3C - INR A";
			break;
		case 0x3D:
			return "0x3D - DCR A";
			break;
		case 0x3E:
			return "0x3E - MVI A";
			break;
		case 0x3F:
			return "0x3F - CMF";
			break;
		case 0x40:
			return "0x40 - MOV B, B";
			break;
		case 0x41:
			return "0x41 - MOV B, C";
			break;
		case 0x42:
			return "0x42 - MOV B, D";
			break;
		case 0x43:
			return "0x43 - MOV B, E";
			break;
		case 0x44:
			return "0x44 - MOV B, H";
			break;
		case 0x45:
			return "0x45 - MOV B, L";
			break;
		case 0x46:
			return "0x46 - MOV B, M";
			break;
		case 0x47:
			return "0x47 - MOV B, A";
			break;
		case 0x48:
			return "0x48 - MOV C, B";
			break;
		case 0x49:
			return "0x49 - MOV C, C";
			break;
		case 0x4A:
			return "0x4A - MOV C, D";
			break;
		case 0x4B:
			return "0x4B - MOV C, E";
			break;
		case 0x4C:
			return "0x4C - MOV C, H";
			break;
		case 0x4D:
			return "0x4D - MOV C, L";
			break;
		case 0x4E:
			return "0x4E - MOV C, M";
			break;
		case 0x4F:
			return "0x48 - MOV C, A";
			break;
		case 0x50:
			return "0x50 - MOV D, B";
			break;
		case 0x51:
			return "0x51 - MOV D, C";
			break;
		case 0x52:
			return "0x52 - MOV D, D";
			break;
		case 0x53:
			return "0x53 - MOV D, E";
			break;
		case 0x54:
			return "0x54 - MOV D, H";
			break;
		case 0x55:
			return "0x55 - MOV D, L";
			break;
		case 0x56:
			return "0x56 - MOV D, M";
			break;
		case 0x57:
			return "0x57 - MOV D, A";
			break;
		case 0x58:
			return "0x58 - MOV E, B";
			break;
		case 0x59:
			return "0x59 - MOV E, C";
			break;
		case 0x5A:
			return "0x5A - MOV E, D";
			break;
		case 0x5B:
			return "0x5B - MOV E, E";
			break;
		case 0x5C:
			return "0x5C - MOV E, H";
			break;
		case 0x5D:
			return "0x5D - MOV E, L";
			break;
		case 0x5E:
			return "0x5E - MOV E, M";
			break;
		case 0x5F:
			return "0x5F - MOV E, A";
			break;
		case 0x60:
			return "0x60 - MOV H, B";
			break;
		case 0x61:
			return "0x61 - MOV H, C";
			break;
		case 0x62:
			return "0x62 - MOV H, D";
			break;
		case 0x63:
			return "0x63 - MOV H, E";
			break;
		case 0x64:
			return "0x64 - MOV H, H";
			break;
		case 0x65:
			return "0x65 - MOV H, L";
			break;
		case 0x66:
			return "0x66 - MOV H, M";
			break;
		case 0x67:
			return "0x67 - MOV H, A";
			break;
		case 0x68:
			return "0x68 - MOV L, B";
			break;
		case 0x69:
			return "0x69 - MOV L, C";
			break;
		case 0x6A:
			return "0x6A - MOV L, D";
			break;
		case 0x6B:
			return "0x6B - MOV L, E";
			break;
		case 0x6C:
			return "0x6C - MOV L, H";
			break;
		case 0x6D:
			return "0x6D - MOV L, L";
			break;
		case 0x6E:
			return "0x6E - MOV L, M";
			break;
		case 0x6F:
			return "0x6F - MOV L, A";
			break;
		case 0x70:
			return "0x70 - MOV M, B";
			break;
		case 0x71:
			return "0x71 - MOV M, C";
			break;
		case 0x72:
			return "0x72 - MOV M, D";
			break;
		case 0x73:
			return "0x73 - MOV M, E";
			break;
		case 0x74:
			return "0x74 - MOV M, H";
			break;
		case 0x75:
			return "0x75 - MOV M, L";
			break;
		case 0x76:
			return "0x76 - HALT";
			break;
		case 0x77:
			return "0x77 - MOV M, A";
			break;
		case 0x78:
			return "0x78 - MOV A, B";
			break;
		case 0x79:
			return "0x79 - MOV A, C";
			break;
		case 0x7A:
			return "0x7A - MOV A, D";
			break;
		case 0x7B:
			return "0x7B - MOV A, E";
			break;
		case 0x7C:
			return "0x7C - MOV A, H";
			break;
		case 0x7D:
			return "0x7D - MOV A, L";
			break;
		case 0x7E:
			return "0x7E - MOV A, M";
			break;
		case 0x7F:
			return "0x7F - MOV A, A";
			break;
		case 0x80:
			return "0x80 - ADD B";
			break;
		case 0x81:
			return "0x81 - ADD C";
			break;
		case 0x82:
			return "0x82 - ADD D";
			break;
		case 0x83:
			return "0x83 - ADD E";
			break;
		case 0x84:
			return "0x84 - ADD H";
			break;
		case 0x85:
			return "0x85 - ADD L";
			break;
		case 0x86:
			return "0x86 - ADD M";
			break;
		case 0x87:
			return "0x87 - ADD A";
			break;
		case 0x88:
			return "0x88 - ADC B";
			break;
		case 0x89:
			return "0x89 - ADC C";
			break;
		case 0x8A:
			return "0x8A - ADC D";
			break;
		case 0x8B:
			return "0x8B - ADC E";
			break;
		case 0x8C:
			return "0x8C - ADC H";
			break;
		case 0x8D:
			return "0x8D - ADC L";
			break;
		case 0x8E:
			return "0x8E - ADC M";
			break;
		case 0x8F:
			return "0x8F - ADC A";
			break;
		case 0x90:
			return "0x90 - SUB B";
			break;
		case 0x91:
			break;
		case 0x92:
			break;
		case 0x93:
			break;
		case 0x94:
			break;
		case 0x95:
			break;
		case 0x96:
			break;
		case 0x97:
			break;
		case 0x98:
			break;
		case 0x99:
			break;
		case 0x9A:
			break;
		case 0x9B:
			break;
		case 0x9C:
			break;
		case 0x9D:
			break;
		case 0x9E:
			break;
		case 0x9F:
			break;
		case 0xA0:
			break;
		case 0xA1:
			break;
		case 0xA2:
			break;
		case 0xA3:
			break;
		case 0xA4:
			break;
		case 0xA5:
			break;
		case 0xA6:
			break;
		case 0xA7:
			break;
		case 0xA8:
			break;
		case 0xA9:
			break;
		case 0xAA:
			break;
		case 0xAB:
			break;
		case 0xAC:
			break;
		case 0xAD:
			break;
		case 0xAE:
			break;
		case 0xAF:
			break;
		case 0xB0:
			break;
		case 0xB1:
			break;
		case 0xB2:
			break;
		case 0xB3:
			break;
		case 0xB4:
			break;
		case 0xB5:
			break;
		case 0xB6:
			break;
		case 0xB7:
			break;
		case 0xB8:
			break;
		case 0xB9:
			break;
		case 0xBA:
			break;
		case 0xBB:
			break;
		case 0xBC:
			break;
		case 0xBD:
			break;
		case 0xBE:
			break;
		case 0xBF:
			break;
		case 0xC0:
			break;
		case 0xC1:
			break;
		case 0xC2:
			break;
		case 0xC3:
			break;
		case 0xC4:
			break;
		case 0xC5:
			break;
		case 0xC6:
			break;
		case 0xC7:
			break;
		case 0xC8:
			break;
		case 0xC9:
			break;
		case 0xCA:
			break;
		case 0xCB:
			break;
		case 0xCC:
			break;
		case 0xCD:
			break;
		case 0xCE:
			break;
		case 0xCF:
			break;
		case 0xD0:
			break;
		case 0xD1:
			break;
		case 0xD2:
			break;
		case 0xD3:
			break;
		case 0xD4:
			break;
		case 0xD5:
			break;
		case 0xD6:
			break;
		case 0xD7:
			break;
		case 0xD8:
			break;
		case 0xD9:
			break;
		case 0xDA:
			break;
		case 0xDB:
			break;
		case 0xDC:
			break;
		case 0xDD:
			break;
		case 0xDE:
			break;
		case 0xDF:
			break;
		case 0xE0:
			break;
		case 0xE1:
			break;
		case 0xE2:
			break;
		case 0xE3:
			break;
		case 0xE4:
			break;
		case 0xE5:
			break;
		case 0xE6:
			break;
		case 0xE7:
			break;
		case 0xE8:
			break;
		case 0xE9:
			break;
		case 0xEA:
			break;
		case 0xEB:
			break;
		case 0xEC:
			break;
		case 0xED:
			break;
		case 0xEE:
			break;
		case 0xEF:
			break;
		case 0xF0:
			break;
		case 0xF1:
			break;
		case 0xF2:
			break;
		case 0xF3:
			break;
		case 0xF4:
			break;
		case 0xF5:
			break;
		case 0xF6:
			break;
		case 0xF7:
			break;
		case 0xF8:
			break;
		case 0xF9:
			break;
		case 0xFA:
			break;
		case 0xFB:
			break;
		case 0xFC:
			break;
		case 0xFD:
			break;
		case 0xFE:
			break;
		case 0xFF:
			break;
		case default:
			return "Opcode Unknown";
	}
}

#ifndef CORE_H
#define CORE_H

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int dword;

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

// Needed
class Driver;
class CPU; // Not implemented yet

// function pointer for ports
//                            generic class address value
typedef void (*Pf_WRITE_PORT_8)(Driver*, uchar, byte); // 8 Bits
typedef byte (*Pf_READ_PORT_8)(Driver*, uchar);
typedef void (*Pf_WRITE_PORT_16)(Driver*, uchar, byte); // 16 Bits
typedef byte (*Pf_READ_PORT_16)(Driver*, uchar);
typedef void (*Pf_WRITE_PORT_32)(Driver*, uchar, byte); // 32 Bits
typedef byte (*Pf_READ_PORT_32)(Driver*, uchar);

#endif

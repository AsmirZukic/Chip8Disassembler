#include <iostream>
#include <fstream>
#include <ios>
#include <stdint.h>


void logicalOperators(uint8_t lastnib, std::uint8_t *code)
{
    switch(lastnib)
    {
        case 0: printf("%-10s V%01X,V%01X", "MOV.", code[0]&0xf, code[1]>>4); break;
        case 1: printf("%-10s V%01X,V%01X", "OR.", code[0]&0xf, code[1]>>4); break;
        case 2: printf("%-10s V%01X,V%01X", "AND.", code[0]&0xf, code[1]>>4); break;
        case 3: printf("%-10s V%01X,V%01X", "XOR.", code[0]&0xf, code[1]>>4); break;
        case 4: printf("%-10s V%01X,V%01X", "ADD.", code[0]&0xf, code[1]>>4); break;
        case 5: printf("%-10s V%01X,V%01X,V%01X", "SUB.", code[0]&0xf, code[0]&0xf, code[1]>>4); break;
        case 6: printf("%-10s V%01X,V%01X", "SHR.", code[0]&0xf, code[1]>>4); break;
        case 7: printf("%-10s V%01X,V%01X,V%01X", "SUB.", code[0]&0xf, code[1]>>4, code[1]>>4); break;
        case 0xe: printf("%-10s V%01X,V%01X", "SHL.", code[0]&0xf, code[1]>>4); break;
        default: printf("UNKNOWN 8"); break;
    }
}

void specialOperations(std::uint8_t *code)
{
    switch(code[1])
    {
        case 0xe0: printf("%-10s", "CLS"); break;
        case 0xee: printf("%-10s", "RLS"); break;
        default: printf("Unkown 0 "); break;
    }
}

void keyboardOperations(std::uint8_t *code)
{
    switch(code[1])
    {
        case 0x9E: printf("%-10s V%01X", "SKIPKEY.Y", code[0]&0xf); break;
        case 0xA1: printf("%-10s V%01X", "SKIPKEY.N", code[0]&0xf); break;
        default: printf("UNKNOWN E"); break;
    }
}

void otherOperations(std::uint8_t *code)
{

    switch(code[1])
    {
        case 0x07: printf("%-10s V%01X,DELAY", "MOV", code[0]&0xf); break;
        case 0x0a: printf("%-10s V%01X", "KEY", code[0]&0xf); break;
        case 0x15: printf("%-10s DELAY,V%01X", "MOV", code[0]&0xf); break;
        case 0x18: printf("%-10s SOUND,V%01X", "MOV", code[0]&0xf); break;
        case 0x1e: printf("%-10s I,V%01X", "ADI", code[0]&0xf); break;
        case 0x29: printf("%-10s I,V%01X", "SPRITECHAR", code[0]&0xf); break;
        case 0x33: printf("%-10s (I),V%01X", "MOVBCD", code[0]&0xf); break;
        case 0x55: printf("%-10s (I),V0-V%01X", "MOVM", code[0]&0xf); break;
        case 0x65: printf("%-10s V0-V%01X,(I)", "MOVM", code[0]&0xf); break;
        default: printf("UNKNOWN F"); break;
    }
}

void DisassembleChip8op(std::uint8_t *buffer, int pc)
{
    std::uint8_t *code = &buffer[pc];
    std::uint8_t firstnib = (code[0] >> 4);

    printf("%04x %02x %02x ", pc, code[0], code[1]);

    switch(firstnib)
    {
        case 0x00: specialOperations(code); break;
        case 0x01: printf("%-10s $%01x%02x", "JUMP", code[0]&0x0f, code[1]); break;
        case 0x02: printf("%-10s $%01x%02x", "CALL", code[0]&0x0f, code[1]); break;
        case 0x03: printf("%-10s V%01X, #$%02x", "SKIP.EQ", code[0]&0x0f, code[1]); break;
        case 0x04: printf("%-10s V%01X, #$%02x", "SKIP.NE", code[0]&0x0f, code[1]); break;
        case 0x05: printf("%-10s V%01X, #$%02x", "SKIP.EQ", code[0]&0x0f, code[1]>>4); break;
        case 0x06: printf("%-10s V%01X, #$%02x", "MVI", code[0]&0x0f, code[1]); break;
        case 0x07: printf("%-10s V%01X,#$%02x", "ADI", code[0]&0xf, code[1]); break;
        case 0x08: logicalOperators(code[1]>>4, code); break;
        case 0x09: printf("%-10s V%01X,V%01X", "SKIP.NE", code[0]&0xf, code[1]>>4); break;
        case 0x0a: printf("%-10s I,#$%01x%02x", "MVI", code[0]&0xf, code[1]); break;
        case 0x0b: printf("%-10s $%01x%02x(V0)", "JUMP", code[0]&0xf, code[1]); break;
	case 0x0c: printf("%-10s V%01X,#$%02X", "RNDMSK", code[0]&0xf, code[1]); break;
	case 0x0d: printf("%-10s V%01X,V%01X,#$%01x", "SPRITE", code[0]&0xf, code[1]>>4, code[1]&0xf); break;
	case 0x0e: keyboardOperations(code); break;
	case 0x0f: otherOperations(code); break;
    }
}

int main( int argc, char* argv[])
{
    std::fstream file(argv[1], std::ios::in | std::ios::binary | std::ios::ate );

    if(!file.is_open())
    {
        std::cout << "Could not open file";
        return 1;
    }

    const std::streampos START_ADDRESS = 200;
    std::streampos size = file.tellg() + START_ADDRESS;
    std::uint8_t* buffer = new std::uint8_t[size];

    file.seekg(0, std::ios::beg);
    file.read((char*)buffer, size);
    file.close();
    
    int programCounter = 0x200;

    while( programCounter < size)
    {
        DisassembleChip8op(buffer, programCounter);
        programCounter += 2;
        std::cout << std::endl;
    }

    delete[] buffer;


    return 0;
}

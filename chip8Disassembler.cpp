#include <iostream>
#include <fstream>
#include <ios>

void DisassembleChip8op( unsigned char *buffer, int pc)
{
    unsigned char *code = &buffer[pc];
    unsigned char firstnib = (code[0] >> 4);

    printf("%04x %02x %02x ", pc, code[0], code[1]);

    switch(firstnib)
    {
        case 0x00: printf("0 not handled yet\n"); break;
        case 0x01: printf("1 not handled yet\n"); break;
        case 0x02: printf("2 not handled yet\n"); break;
        case 0x03: printf("3 not handled yet\n"); break;
        case 0x04: printf("4 not handled yet\n"); break;
        case 0x05: printf("5 not handled yet\n"); break;
        case 0x06: 
            {
                unsigned char reg = code[0] & 0x0f;
                printf("$-10s V#01X, #$%02x", "MVI", reg, code[1]);
                break;
            }
        case 0x07: printf("7 not handled yet\n"); break;
        case 0x08: printf("8 not handled yet\n"); break;
        case 0x09: printf("9 not handled yet\n"); break;
        case 0x0a: 
            {
                unsigned char addresshi = code[0] & 0x0f;
                printf("%-10s I,#$%01x%02x", "MVI", addresshi, code[1]);
                break;
            }
        case 0x0b: printf("b not handled yet\n"); break;
        case 0x0c: printf("c not handled yet\n"); break;
        case 0x0d: printf("d not handled yet\n"); break;
        case 0x0e: printf("e not handled yet\n"); break;
        case 0x0f: printf("f not handled yet\n"); break;
    }

}

int main( int argc, char* argv[])
{
    //Open the file as astream of binary and move the file pointer to the end
    std::fstream file(argv[1], std::ios::in | std::ios::binary | std::ios::ate );

    //Early termination
    if(!file.is_open())
    {
        std::cout << "Could not open file" << std::endl;
        return 1;
    }

    //Get size of file and alocate a buffer to hold the contents
    //Memory untill 0x200 is reserved so we add 200 to the size
    std::streampos size = file.tellg() + (std::streamoff)200;
    unsigned char* buffer = new unsigned char[size];

    //Go back to the beggining of the file and fill the buffer
    file.seekg(0, std::ios::beg);
    file.read((char*)buffer, size);
    file.close();
    

    int pc = 0x200;

    while( pc < size)
    {
        DisassembleChip8op(buffer, pc);
        pc += 2;
    }


    delete[] buffer;


    return 0;
}
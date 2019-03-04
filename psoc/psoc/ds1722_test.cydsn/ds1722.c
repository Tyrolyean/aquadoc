
#include <stdint.h>
#include <endian.h>
#include <stdbool.h>

inline uint8_t get_ds1722_address(bool read, uint8_t address){
    if(read){
        return (~(1 << 8)) & address;
    }else{
        return (1 << 8) | address;
    }
}


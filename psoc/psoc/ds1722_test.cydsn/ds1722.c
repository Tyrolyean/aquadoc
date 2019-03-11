
#include <stdint.h>
#include <stdbool.h>

#include "ds1722.h"

inline uint8_t get_ds1722_address(bool read, uint8_t address){
    if(read){
        return (~(1 << 8)) & address;
    }else{
        return (1 << 8) | address;
    }
}


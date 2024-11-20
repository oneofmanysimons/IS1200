#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"

volatile int* portD = (volatile int*)0xbf8860d0; //can find memory address on page 72 in pic32 family

int getsw(void)
{
  return ((*portD & 0x0f00)>>8); // return SW4-SW1 values as 4 least significant bits
}

int getbtns(void)
{
  return ((*portD & 0x0e0)>>5); // return BTN4-BTN2 values as 3 least significant bits
}

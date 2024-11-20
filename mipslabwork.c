/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int mytime = 0x5957;

char textstring[] = "text, more text, and even more text!";
int timeoutcount = 0;

/* Interrupt Service Routine */
void user_isr( void )
{
     volatile int* portE = (volatile int*) 0xbf886110;//initilise memory address
    if(IFS(0) & 0x80000){
        (*portE)++;
        IFSCLR(0) = 0x80000;
    }

                if(timeoutcount == 10){
                 time2string( textstring, mytime );
                display_string( 3, textstring );
                display_update();
                tick( &mytime );
                timeoutcount = 0;
            }
            IFSCLR(0) = 0x100;
            timeoutcount++;
}

/* Lab-specific initialization goes here */
void labinit( void )
{
    volatile int* trisE = (volatile int*) 0xbf886100;//initilise memory address
    *trisE = *trisE & 0x00;//set bit 7 to 0 to 0
    volatile int* portE = (volatile int*) 0xbf886110;//initilise memory address
    *portE = 0;//clear portE
    volatile int* trisD = (volatile int*) 0xbf8860c0;
    *trisD = *trisD | 0x0fe0;//set bit 11 to 5 to 1 and do not change any other bit

    /* Initialization of Timer2 */
    T2CON = 0;//set all bits in Timer2 to 0
    TMR2 = 0;		//set timer value to 0 before start

    PR2 = ((80000000/256)/10); //set period time to 31250
    T2CONSET = 0x8070;//start Timer2 and set prescale value to 256

    IEC(0) = (1 << 19);
    IPCSET(4) = (3 << 28);
    enable_interrupt();

  return;
}

/* This function is called repetitively from the main program */
void labwork( void )
{
   // volatile int* portE = (volatile int*) 0xbf886110;//initilise memory address
/*
    if((getbtns() & 0x4)){
    mytime = (mytime & 0xf000); //get byte X9:57
    mytime = mytime|(getsw()<<12); //move them to 4 least significant bits
    //display_string( 3, textstring );//update display
    }
    if((getbtns() & 0x2)){
    mytime = mytime & 0xf0ff; //get byte 5X:57
    mytime = mytime|(getsw()<<8); //move them to 4 least significant bits
    //display_string( 3, textstring );//update display
    }
    if((getbtns() & 0x1)){
    mytime = mytime & 0xff0f; //get byte 59:X7
    mytime = mytime |(getsw()<<4); //move them to 4 least significant bits
    //display_string( 3, textstring );//update display
    }

    if(IFS(0) & 0x100){
            timeoutcount++; //incrament timeoutcount
    IFSCLR(0) = 0x100;//reset the event flag
    }

    if(timeoutcount >= 10){
          //delay( 1000 );
  time2string( textstring, mytime );
  display_string( 3, textstring );
  display_update();
  tick( &mytime );
  (*portE)++; //incrament the current bit value in portE after tick
  display_image(96, icon);
  timeoutcount = 0;//reset timeoutcount
    }
    */
     prime = nextprime( prime );
 display_string( 0, itoaconv( prime ) );
 display_update();
}

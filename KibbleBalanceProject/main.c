// Ilias Mastoras 05/08/2018

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "Laser.h" //This includes the Laser struct
#include "utils/uartstdio.h" // This include statement helps us use the UART functions.
#include "inits/inits.h" // This include statement helps us use the initialization functions.
void outputFun(char*, char*,char*); // This function handles all output for this program.
bool checkChoice(char [], char); // This function checks for valid inputed choice.
void printSelectionMenu(); // This prints a menu of choices to the screen.
void printLaserMenu(char*);
void initsConfirmation();
void clearPuttyTerminal();
void printTaskMenu();
void terminateFunction();
char InputFun();  //InputFun - Gets input .

void PinGenConfig();       // This function configures the pins and the generators.

void laserCommands(char*);

void delay(int);

Laser pd0LaserA;  // .
Laser pd1LaserB;  // This configures the PWM generator 0, using PD1.

int main(void){
    initializeAll(); //Initializes all the important hardware. This function is in inits file.
    initsConfirmation();
    printTaskMenu();

    initializeFreqDuty(&pd0LaserA); // for laser A
    initializeFreqDuty(&pd1LaserB); // for laser B

    int delayTime = 90000; //local delayTime for loop.
    char arrTask[3] = {'S','C','E'};// This array is for the select task menu. You can add more char to compare, if more tasks are added.
    char arr[5] = {'A','B','C','X','E'};
    char laserChoice[1], taskChoice;
    bool wait = false, taskWait = false;

    while(1){
       taskWait = true;
       wait = true;

       while(taskWait){
           printTaskMenu();
           bool check = true;
           while(check){
               taskChoice = toupper(InputFun()); // Get the input from the user,convert it to
                                                  // upper case and then store it to taskChoice.
               check = checkChoice(arrTask,taskChoice);
               if(check){
                   printTaskMenu();
                   delay(delayTime);
                   outputFun("\n(aaa999)","  Invalid Entry, Please try again!","");
               } else {
                   if (taskChoice == 'S'){
                       clearPuttyTerminal();
                       taskWait = false;
                   }
                   if(taskChoice =='C'){
                       clearPuttyTerminal();
                   }
                   if(taskChoice =='E'){
                       terminateFunction();
                   }
               }
          }

       }// end of while(taskWait)

        while(wait){
            printSelectionMenu();
            bool check = true;

            while(check){
                laserChoice[0] = toupper(InputFun());

                check = checkChoice(arr,laserChoice[0]);
                if(check){
                    printSelectionMenu();
                    delay(delayTime);
                    outputFun("\n(aaa999)","  Invalid Entry, Please try again!","");
                }
            }

            // You execute the command based on the input choice.
            if( (laserChoice[0] == 'A') || (laserChoice[0] == 'B') ){
                clearPuttyTerminal();
                laserCommands( laserChoice);
            }
            if(laserChoice[0] =='C'){
                clearPuttyTerminal();
            }
            if(laserChoice[0] =='X'){
                clearPuttyTerminal();
                wait = false;
            }
            if(laserChoice[0] =='E'){
               terminateFunction();
            }
        }//end of while(wait)
    }//end of while(1)


}//end of main


// This function controls the Freq Command.
void laserCommands(char* laserChoice){
    int delayTime = 90000; //local delayTime for loop.
    char choice;
    bool wait = true;
    char freq;
    int32_t freqTotal;
    char duty;
    int32_t dutyTotal;
    char arr[4] = {'F','D','X','E'};
    char buffer[5];
    bool back;
    wait = true;
    int j;
    int count;
    int decades;
    int pass;


    printLaserMenu(laserChoice);

    bool check = true;
    while(check){
        choice = toupper(InputFun());
        check = checkChoice(arr,choice);
        if(check){
            printLaserMenu(laserChoice);
            delay(delayTime);
            outputFun("\n(aaa999)","  Invalid Entry, Please try again!","");
        }
    }

    while(wait){
        if(choice =='F'){
            outputFun("\n(aaa007)  Type the Frequency for laser",laserChoice," and PRESS ENTER.( between 0Hz and 1000Hz)");

            freqTotal = 0;
            count = 4;
            decades = 1000;
            pass = 1;

            for(j=0; j<=4; j++){
                freq =  InputFun();

                if(freq == 13){ // 13 is the ASCII number of Enter. '\n' doesn't work.
                    break;
                } else if(isdigit(freq)){
                    freqTotal += (freq -'0') * decades;
                    decades /= 10;
                    count--;
                } else {
                    clearPuttyTerminal();
                    printLaserMenu(laserChoice);
                    delay(delayTime);
                    outputFun("\n(aaa999)","  Invalid Entry, Please try again!","");
                    choice = 'F';
                    pass = 0;
                    break;
                }
            }

            if(pass == 1){
                while(count>0){
                    freqTotal /= 10;
                    count--;
                }
                ltoa(freqTotal,buffer);
                outputFun("\n(aaa090)  You entered ",buffer,"Hz");


                if((freqTotal < 0) || (freqTotal > 1000)){
                    clearPuttyTerminal();
                    printLaserMenu(laserChoice);
                    delay(delayTime);
                    outputFun("\n(aaa999)","  Invalid Entry, Please try again!","");
                }else {
                    if(laserChoice[0] == 'A'){
                        back = setFrequency(&pd0LaserA, laserChoice[0], freqTotal);
                    } else {
                        back = setFrequency(&pd1LaserB,laserChoice[0], freqTotal);
                    }
                        if(back){
                            outputFun("(aaa111)  Laser's", laserChoice," frequency was changed successfully.\n");
                       }
                   wait = false;       //Exit frequency loop if input is valid
               }
            }// end if pass
        }// end if F

        if(choice =='D'){
            outputFun("\n(aaa008)  Type the Duty Cycle for laser",laserChoice," and PRESS ENTER.( between 0% and 100% )");

            dutyTotal = 0;
            count = 3;
            decades = 100;
            pass = 1;

            for(j=0; j<=3; j++){
                duty = InputFun();

                if(duty == 13){ // 13 is the ASCII number of Enter. '\n' doesn't work.
                    break;
                } else if(isdigit(duty)){
                    dutyTotal += (duty -'0') * decades;
                    decades /= 10;
                    count--;
                } else {
                    clearPuttyTerminal();
                    printLaserMenu(laserChoice);
                    delay(delayTime);
                    outputFun("\n(aaa999)","  Invalid Entry, Please try again!","");
                    choice = 'D';
                    pass = 0;
                    break;
                }
            }//end for loop


            if(pass == 1){
                while(count>0){
                   dutyTotal /= 10;
                   count--;
                }

                ltoa(dutyTotal,buffer);
                outputFun("\n(aaa090)  You entered ",buffer,"%");

                    if((dutyTotal < 0) || (dutyTotal > 100)){
                        clearPuttyTerminal();
                        printLaserMenu(laserChoice);
                        delay(delayTime);
                        outputFun("\n(aaa999)","  Invalid Entry, Please try again!","");
                    } else {
                        if(laserChoice[0] == 'A'){
                            back = setDutyCycle(&pd0LaserA, laserChoice[0], dutyTotal);
                        } else {
                            back = setDutyCycle(&pd1LaserB, laserChoice[0], dutyTotal);
                        }

                        if(back){
                          outputFun("(aaa222)  Laser's", laserChoice," duty cycle was changed successfully.\n");
                        }

                       wait = false;       //Exit duty loop if input is valid
                   }
            }// end if pass
        }//end if D

        if(choice == 'X'){
            clearPuttyTerminal();
            wait = false;
        }

        if(choice == 'E'){
            terminateFunction();
        }
   }// end while(wait)
}//end laserCommands method



//OutputFun - Handles output messages with identifiers
void outputFun(char* ident, char* str, char* optional ){
    UARTprintf("%s%s%s%s%s", ident, " ", str,optional, "\n");
}

//InputFun - Gets input .
char InputFun(){
  return UARTgetc();
}

bool checkChoice(char arr[], char c){
int i;
 for(i=0; i<sizeof(arr); i++){
        if(arr[i]==c)
            return false;
    }
    clearPuttyTerminal();
    return true;
}

void printTaskMenu(){
    clearPuttyTerminal();
    outputFun("(aaa300)   --- S E L E C T ","T A S K ---","\n");
    outputFun("(aaa039)","  Press s to", " set the Laser's.");
    outputFun("(aaa012)","  Press c ","- for clearing the screen");
    outputFun("(aaa300)","  Press e ","- for Exit");
}

//This function prints out the MENU
void printSelectionMenu(){
    outputFun("(aaa100)   --- S E L E C T I O N ","M E N U ---","\n");
    outputFun("(aaa009)","  Which Laser you want to", " operate with?\n");
    outputFun("(aaa010)","  Press a ","- for Laser number 1");
    outputFun("(aaa011)","  Press b ","- for Laser number 2");
    outputFun("(aaa012)","  Press c ","- for clearing the screen");
    outputFun("(aaa250)","  Press x ","- Go back to Task Menu");
    outputFun("(aaa300)","  Press e ","- for Exit");
}

void printLaserMenu(char* laserChoice){
    char freqBuffer[5];
    char dutyBuffer[5];

    if( laserChoice[0] == 'A'){ // laserChoice is a pointer and works like a string of chars. It is like an array with one element.
           ltoa(pd0LaserA.frequency,freqBuffer);
           ltoa(pd0LaserA.dutyCycle,dutyBuffer);
       } else {
           ltoa(pd1LaserB.frequency,freqBuffer);
           ltoa(pd1LaserB.dutyCycle,dutyBuffer);
       }

    outputFun("(aaa101)  --- L A S E R  -",laserChoice," -  M E N U ---");
    outputFun("\t\t\t\t\t(aaa050)  Laser",laserChoice," Current Status");
    outputFun("\t\t\t\t\t(aaa051)    Frequency:  ",freqBuffer," Hz");
    outputFun("\t\t\t\t\t(aaa052)    Duty Cycle: ",dutyBuffer," %\n");

    outputFun("(aaa020)", "f - Change PWM Frequency","");
    outputFun("(aaa021)", "d - Change PWM Duty Cycle","");
    outputFun("(aaa250)", "x - Go Back to Main Menu","");
    outputFun("(aaa300)", "e - Exit","");
}

void initsConfirmation(){
    UARTEchoSet(false); //This disables the display of anything typed during the initialization.

    int delayTime = 3500000; //local delayTime for loop.
    outputFun("\033\143","",""); // \033\143 this command clears the Putty terminal. This may not work for other terminals.
    delay(delayTime);
    outputFun("(aaa000)", "  UART initialized...","");
    delay(delayTime);
    outputFun("(aaa001)", "  Clock initialized...","");
    delay(delayTime);
    outputFun("(aaa002)", "  Timer initialized...","");
    delay(delayTime);
    outputFun("(aaa003)", "  Timer0IntHandler initialized...","");
    delay(delayTime);
    outputFun("(aaa004)", "  Interrupt initialized...","");
    delay(delayTime);
    outputFun("(aaa005)", "  GPIO and PWM's initialized...","\n");
    delay(delayTime);
    outputFun("(aaa006)", "  Initialization Completed.!","");
    delay(delayTime*2);
    clearPuttyTerminal();

    UARTEchoSet(true); // This enables the display of anything that will be typed from now on.
    UARTFlushRx(); // This clears the input buffer from anything typed (but not displayed) during initialization.
}

void terminateFunction(){
    int delayTime = 900000;
    UARTEchoSet(false); //This disables the display of anything typed during the initialization.
    clearPuttyTerminal();
    int count = 0;
        do{
            UARTprintf("%s",". ");
            delay(delayTime);
            count++;
          }while(count < 5);

    clearPuttyTerminal();
    outputFun("\n(aaa500)","System Terminated.","\a");
    delay(delayTime);
    exit(0);
}


void clearPuttyTerminal(){
    // \033\143 this command clears the Putty terminal.This may not work for other terminals.
    outputFun("\033\143","","");
}


void delay(int time){
    int i;
    for( i=0; i<time; i++){
        //delay
    }
}

















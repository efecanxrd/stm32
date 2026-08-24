#include <stdio.h>
#include <stdint.h>

//Checkout course.docx for theory.

int main() {
    //CONST
    uint8_t const data2 = 10; //cannot edit from now on
    //data2 = 50; Compile-time error

    uint8_t *ptr = (uint8_t*) &data2;
    *ptr = 50; //you can change with like that with its address

    uint8_t const *pData = (uint8_t*) 0x400000000;
    //Here the pointer pData is modifiable but the data pointed by the pData cannot be modifiable
    //So we can say that pData is a pointer pointing to read-only data.

    uint8_t *const pData = (uint8_t*) 0x400000000;
    //Here is the pointer pData is read-only but the data pointer can be modifiable
    //So we can say that pData is a read only pointer pointing to modifiable data

    uint8_t const *const pData = (uint8_t) 0x40000000;
    //Here is the pointer pData is read-only and the data pointed by the pData is also read only
    //So we can say that pData is read-only pointer pointing to read only data


    //VOLATILE
    uint8_t volatile data1; 
    /*  
    data1 will not be affected by ANY optimization procedures. even if it is not used
    */
   
    uint8_t volatile *pStatusReg; //pStatusReg is a non-volatile pointer to volatile data
    //Use this syntaxx whenever you are accessing memory mapped registers within MCU
    
    //rarely used ones:
    uint8_t *volatile pStatusReg2; //pStatusReg2 is a volatile pointer to non-volatile data
    uint8_t volatile *volatile pStatusReg3; //pStatusReg3 is a volatile pointer to volatile data

    //The keywords const and volatile can be applied to any declaration; like structures, unions, enumerated types etc.

    uint8_t volatile *const pReg = (uint8_t *) 0x40000000; //constant pointer and volatile data
    uint8_t const volatile *const pReg = (uint8_t*) 0x40000000; //data pointed by address is volatile but programmer cannot change address

    return 0;
}

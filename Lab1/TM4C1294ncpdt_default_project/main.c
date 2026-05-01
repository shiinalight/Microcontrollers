/** Default Project for TM4C1294ncpdt
 *
 * File:    main.c
 * Author:
 * Date:
 * Version:
 *
 * Blinks the onboard LED using a busy-wait loop as delay
 */

#include "inc/tm4c1294ncpdt.h"
#include <stdint.h>
#include <stdio.h>

/*******************************************************************************************/
// Keypad layout
char keys[4][4] = {
    {'1', '2', '3', 'F'},
    {'4', '5', '6', 'E'},
    {'7', '8', '9', 'D'},
    {'A', '0', 'B', 'C'}
};

/*******************************************************************************************/
// Simple delay
void wait(int ticks) {
    for (int i = 0; i < ticks; i++);
}

/*******************************************************************************************/
// Configure keypad (Port M)
void configure_keypad() {
    SYSCTL_RCGCGPIO_R |= (0x1 << 11);                  // enable clock for Port M
    while (!(SYSCTL_PRGPIO_R & (0x1 << 11)));          // wait until ready

    GPIO_PORTM_DIR_R = 0x0F;                           // PM0-PM3 = output (X lines)
    GPIO_PORTM_DEN_R = 0xFF;                           // PM0-PM7 digital enable
    GPIO_PORTM_DATA_R |= 0x0F;                         // all X HIGH
}

/*******************************************************************************************/
// Configure display (Port K, PK0–PK3)
void configure_display() {
    SYSCTL_RCGCGPIO_R |= (0x1 << 9);                   // enable clock for Port K
    while (!(SYSCTL_PRGPIO_R & (0x1 << 9)));           // wait until ready

    GPIO_PORTK_DIR_R |= 0x0F;                          // PK0–PK3 output
    GPIO_PORTK_DEN_R |= 0x0F;                          // digital enable
}

/*******************************************************************************************/
// Scan keypad
char scan_keypad(void)
{
    int col, row;
    int count = 0;
    char detected = 0;

    for (col = 0; col < 4; col++)
    {
        GPIO_PORTM_DATA_R |= 0x0F;                     // all columns HIGH
        GPIO_PORTM_DATA_R &= ~(1 << col);              // one column LOW

        wait(1000);                                    // allow signal to settle

        uint8_t y = (GPIO_PORTM_DATA_R >> 4) & 0x0F;   // read rows

        for (row = 0; row < 4; row++)
        {
            if ((y & (1 << row)) == 0)                 // active LOW
            {
                detected = keys[row][col];
                count++;
            }
        }
    }

    GPIO_PORTM_DATA_R |= 0x0F;                         // reset columns

    if (count == 0)
        return 0;                                      // no key

    if (count > 1)
        return '*';                                    // multiple keys

    return detected;
}

/*******************************************************************************************/
// Convert key to BCD and display
void display_key(char key)
{
    uint8_t value;

    if (key >= '0' && key <= '9')
        value = key - '0';
    else if (key >= 'A' && key <= 'F')
        value = key - 'A' + 10;
    else
        return;

    GPIO_PORTK_DATA_R = value;                         // output BCD
}

/*******************************************************************************************/
int main(void)
{
    char key;
    char last = 0;

    configure_keypad();
    configure_display();

    while(1)
    {
        key = scan_keypad();

        // print/display only once per press
        if (key != 0 && key != last)
        {
            if (key == '*')
            {
                printf("Error: multiple keys\n");
                GPIO_PORTK_DATA_R = 14;   // show 'E'
            }
            else
            {
                printf("Key: %c\n", key);
                display_key(key);
            }

            last = key;
        }

        // reset when released
        if (key == 0)
        {
            last = 0;
        }
    }
}
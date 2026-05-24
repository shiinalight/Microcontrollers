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

/*******************************/

void PortD()
{
    SYSCTL_RCGCGPIO_R |= 0x08;

    while ((SYSCTL_PRGPIO_R & 0x08) == 0)
        ;
    GPIO_PORTD_AHB_DIR_R |= 0x01;  /* PD0 = Trigger OUTPUT */
    GPIO_PORTD_AHB_DIR_R &= ~0x02; /* PD1 = Echo INPUT */
    GPIO_PORTD_AHB_DEN_R |= 0x03;  /* Digital enable */
    GPIO_PORTD_AHB_DATA_R &= ~0x01;
}

void PortM(void)
{
    SYSCTL_RCGCGPIO_R |= (1 << 11);

    while ((SYSCTL_PRGPIO_R & (1 << 11)) == 0)
        ;
    GPIO_PORTM_DIR_R = 0xFF; // PM0 - PM7 OUTPUT
    GPIO_PORTM_DEN_R = 0xFF; // Digital enable
}

void PortL(void)
{
    SYSCTL_RCGCGPIO_R |= (1 << 10);

    while ((SYSCTL_PRGPIO_R & (1 << 10)) == 0)
        ;
    GPIO_PORTL_DIR_R &= ~0x03; // PL0 and PL1 input
    GPIO_PORTL_DEN_R |= 0x03;
    GPIO_PORTL_DEN_R |= 0x03; // Digital enable
    GPIO_PORTL_DATA_R &= ~0x02;
}

void GPIO(void)
{
    PortD();
    // PortM();
    // PortL();
}

void Timer0()
{
    SYSCTL_RCGCTIMER_R |= 0x01;
    while ((SYSCTL_PRTIMER_R & 0x01) == 0)
        ;
    // TIMER0_CTL_R = 0x00;
    TIMER0_CFG_R = 0x00;  // 32
    TIMER0_TAMR_R = 0x01; // Start timer
}

void sleep(int ms)
{
    Timer0();
    TIMER0_CTL_R = 0x00;
    TIMER0_TAILR_R = (16000 * ms) - 1;
    TIMER0_ICR_R = 0x01;
    TIMER0_CTL_R |= 0x01;
    while ((TIMER0_RIS_R & 0x01) == 0)
    {
    }
    TIMER0_ICR_R = 0x01;
}

void display_rect_signal(void)
{
    GPIO_PORTD_AHB_DATA_R |= 0x01; // PD0 HIGH
    sleep(10);

    GPIO_PORTD_AHB_DATA_R &= ~0x01; // PD0 LOW
    sleep(10);
}

float Calculate_Distance(unsigned int time_us)
{
    float distance;
    distance = (time_us * 0.0343f) / 2.0f;
    return distance;
}

void trigger_ultrasonic_sensor()
{

    GPIO_PORTD_AHB_DATA_R |= 0x01; // Trigger HIGH
    sleep(1);
    GPIO_PORTD_AHB_DATA_R &= ~0x01; // Trigger LOW
}

unsigned int Measure_Echo_Time(void)
{
    unsigned int count;
    int timeout = 0;

    /* Wait for Echo HIGH */
    while ((GPIO_PORTL_DATA_R & 0x02) == 0)
    {
        timeout++;

        if (timeout > 1000000)
        {
            printf("No Echo Signal\n");
            return 0;
        }
    }

    /* Configure timer */
    TIMER0_CTL_R = 0x00;
    TIMER0_CFG_R = 0x00;
    TIMER0_TAMR_R = 0x02;
    TIMER0_TAILR_R = 0xFFFFFFFF;
    TIMER0_ICR_R = 0x01;

    TIMER0_CTL_R |= 0x01;

    /* Wait while Echo is HIGH */
    while ((GPIO_PORTL_DATA_R & 0x02) != 0)
    {
    }

    TIMER0_CTL_R = 0x00;

    count = 0xFFFFFFFF - TIMER0_TAR_R;
    count = count / 16;

    return count;
}

void Display_Bars(float distance)
{
    if (distance < 20)
    {
        GPIO_PORTM_DATA_R = 0x01;
    }
    else if (distance < 40)
    {
        GPIO_PORTM_DATA_R = 0x0F;
    }
    else
    {
        GPIO_PORTM_DATA_R = 0xFF;
    }

    sleep(2);

    GPIO_PORTM_DATA_R = 0x00;
}

void Display_HAW(void)
{
    uint8_t H[5] = {0x81, 0x81, 0xFF, 0x81, 0x81};

    uint8_t A[5] = {0x7E, 0x81, 0xFF, 0x81, 0x81};

    uint8_t W[5] = {0x81, 0x81, 0x91, 0x91, 0x6E};

    /* ===== H ===== */

    for (int i = 0; i < 5; i++)
    {
        GPIO_PORTM_DATA_R = H[i];

        sleep(2);
    }

    GPIO_PORTM_DATA_R = 0x00;

    sleep(2);

    /* ===== A ===== */

    for (int i = 0; i < 5; i++)
    {
        GPIO_PORTM_DATA_R = A[i];

        sleep(2);
    }

    GPIO_PORTM_DATA_R = 0x00;

    sleep(2);

    /* ===== W ===== */

    for (int i = 0; i < 5; i++)
    {
        GPIO_PORTM_DATA_R = W[i];

        sleep(2);
    }

    GPIO_PORTM_DATA_R = 0x00;
}

/*******************************/

int main(void)
{

    // GPIO();
    PortD();
    PortL();
    PortM();

    while (1)
    {
        trigger_ultrasonic_sensor();

        unsigned int echo_time = Measure_Echo_Time();

        float distance = Calculate_Distance(echo_time);

        printf("Echo Time = %u us, Distance = %.2f cm\n", echo_time, distance);

        // Display_Bars(distance);
        Display_HAW();

        sleep(1000);

        // Display_Bars();
    }

    // Trigger_Ultrasonic();

    // unsigned int t = (Measure_Echo_Time() * 1540) / 100;
    // printf("Echo Time %d cm\n", t);

    // float d = Calculate_Distance(t);
    // sleep(1000);
    // Display_Distance(d);
}
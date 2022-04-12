/**
* \file main.c
*
* \brief Main source file.
*
    (c) 2022 Microchip Technology Inc. and its subsidiaries.
    
    Subject to your compliance with these terms, you may use Microchip software and any
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party
    license terms applicable to your use of third party software (including open source software) that
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS
    SOFTWARE.
*/

#include <avr/io.h>
#include <avr/interrupt.h>

/* Default fuses configuration:
- BOD disabled
- Oscillator in High-Frequency Mode
- UPDI pin active(WARNING: DO NOT CHANGE!)
- RESET pin used as GPIO
- CRC disabled
- MVIO enabled for dual supply
- Watchdog Timer disabled
*/
FUSES =
{
.BODCFG = ACTIVE_DISABLE_gc | LVL_BODLEVEL0_gc | SAMPFREQ_128Hz_gc | SLEEP_DISABLE_gc,
.BOOTSIZE = 0x0,
.CODESIZE = 0x0,
.OSCCFG = CLKSEL_OSCHF_gc,
.SYSCFG0 = CRCSEL_CRC16_gc | CRCSRC_NOCRC_gc | RSTPINCFG_GPIO_gc | UPDIPINCFG_UPDI_gc,
.SYSCFG1 = MVSYSCFG_DUAL_gc | SUT_0MS_gc,
.WDTCFG = PERIOD_OFF_gc | WINDOW_OFF_gc,
};

volatile uint16_t signal_frequency = 0;

void PORT_Init (void);
void ZCD0_Init (void);
void TCB0_Init (void);
void EVSYS_Init (void);

void PORT_Init (void)
{
    /* Disable digital input buffer and pull-up resistor on PC2*/
    PORTC.PIN2CTRL = PORT_ISC_INPUT_DISABLE_gc;
    
    /* SET PA7 as an output*/
    PORTA.OUTSET |= PIN7_bm;
    PORTA.DIRSET |= PIN7_bm;    
}

/* Initialization of the Zero Cross Detector */
void ZCD0_Init (void)
{
    ZCD3.CTRLA = ZCD_ENABLE_bm          /* Enable the ZCD*/
               | ZCD_OUTEN_bm;          /*Enable the output of the ZCD*/
}

/*Initialize TCB in pulse width-frequency measurement mode, input from ZCD through Event System */
void TCB0_Init (void)
{
    TCB0.CTRLB = TCB_CNTMODE_FRQ_gc;    /* Input Capture Frequency */
    TCB0.EVCTRL = TCB_CAPTEI_bm;        /* Event Input Enable: enabled */    
    TCB0.INTCTRL = TCB_CAPT_bm;         /* Capture or Timeout: enabled */

    /* CLK_PER/2 - This is needed to be able to measure up to 40Hz with a 4MHz system clock */
    TCB0.CTRLA = TCB_CLKSEL_DIV2_gc        
               | TCB_ENABLE_bm          /* Enable: enabled */
               | TCB_RUNSTDBY_bm;       /* Run Standby: enabled */
}

/* Enable event generation from ZCD to TCB */
void EVSYS_Init (void)
{
    /* Zero-cross detector 0 out linked to Event Channel 0 */
    EVSYS.CHANNEL0 = EVSYS_CHANNEL0_ZCD3_gc;
    /* TCB uses Event Channel 0 */
    EVSYS.USERTCB0CAPT = EVSYS_USER_CHANNEL0_gc;
}

int main(void)
{
    TCB0_Init();
    EVSYS_Init();
    ZCD0_Init();
    PORT_Init();

    while(1)
    {
    }
}

ISR(TCB0_INT_vect)
{
    /* The frequency measurement is stored in the CCMP register */
    signal_frequency = TCB0.CCMP;
}

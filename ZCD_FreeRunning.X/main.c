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


void ZCD_Init(void);
void PORT_Init(void);

/* Initialization of the Zero Cross Detector */
void ZCD_Init(void)
{ 
	ZCD3.CTRLA = ZCD_ENABLE_bm	/* Enable the ZCD */
	           | ZCD_OUTEN_bm;	/*Enable the output of the ZCD */
}

void PORT_Init(void)
{
    /* Disable digital input buffer and pull-up resistor on PC2*/
    PORTC.PIN2CTRL = PORT_ISC_INPUT_DISABLE_gc;
    
    /* SET PA7 as an output*/
    PORTA.DIRSET = PIN7_bm;
    PORTA.OUTSET = PIN7_bm;
}

int main(void)
{
	ZCD_Init();
    PORT_Init();
	
    while(1) 
    {
        ;
    }
}

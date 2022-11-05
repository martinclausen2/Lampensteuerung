/*
Controls Status LED connected to OCA (blue), OCB (green), OCC (red)
*/

#ifndef __StatusLED_H_GUARD
#define __StatusLED_H_GUARD 1

unsigned char LEDFlashCount;		/* Number of flashes currently required, should not exceed 0x7F ... */
unsigned char LEDFlashSeqCounter;	/* Number of flashes * 2 (on & off) to be produced in this sequence */
unsigned char LEDLimitFlashTimer;	/* Software timer to decouple flashing frequency from calling frequency */
unsigned char LEDStandbyTimer;
__bit LEDTempOn;			/* Flag to indicate active temperature control, required for LED control */

#define LEDFlashMaxSeq	8	/* max. number of flashes in a squence, should not exceed 0x7F ... */
#define LEDFlashMaxSeqSteps LEDFlashMaxSeq << 1
#define LEDmaxFlashTimer 6	/* cycles to execute before next flash toggel happens */
#define LEDmaxLimitFlashTimer 4	/* cycles to execute before LED status is restored */
#define LEDmaxStandyTimer 0x3F	/* cycles to execute before LED brightness is dimed */
#define flashColorMask	0x07

#define BlueLEDPort 	P2_6
#define GreenLEDPort 	P1_6
#define RedLEDPort 	P1_7

#define LEDBlank	0
#define LEDWhite	1
#define LEDRed		2
#define LEDBlue		3
#define LEDGreen	4
#define LEDReddim	9

__code unsigned char colorTable[12][3] = { { 0x00, 0x00, 0x00},	/* off */

					{ 0x7F, 0x7F, 0x7F},	/* white */

					{ 0x00, 0x00, 0xFF},	/* red */
					{ 0xFF, 0x00, 0x00},	/* blue */
					{ 0x00, 0xFF, 0x00},	/* green */

					{ 0x5F, 0x00, 0xBF},
					{ 0x5F, 0x7F, 0x00},
					{ 0x5F, 0x3F, 0x9F},
					{ 0x5F, 0x8F, 0x4F},

					{ 0x00, 0x00, 0x0F},	/* red very dim */
					{ 0x00, 0x00, 0x3F},	/* red dim */
					{ 0x00, 0x00, 0xFF},	/* red */
			 	        };

__code unsigned char flashColorTable[8] = {LEDBlue, LEDBlank, LEDBlue, LEDBlank, LEDBlue, LEDBlank, LEDRed, LEDBlank};

void LEDOff();

void LEDOn();

void LEDCancel();

void LEDOptions();

void LEDFadeLightOut();

void LEDSetupOptions(unsigned char i);

void LEDValue(unsigned char i);

void LEDSetupStandby();

void LEDStandby();

void LEDOvertemp();

void LEDSetupLimit();

void LEDLimit();

void LEDSetColor(unsigned char i);

#endif
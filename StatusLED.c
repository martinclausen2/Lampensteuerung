/*
Controls Status LED connected to OCA (blue), OCB (green), OCC (red)
*/

#include "StatusLED.h"

void LEDOff()
{
	LEDSetColor(LEDBlank);
}

void LEDOn()
{
	LEDSetColor(LEDWhite);
}

void LEDCancel()
{
	LEDSetColor(LEDGreen);
}

// indicate value by color
void LEDValue(unsigned char i)		//i should not exceed colorTable length - 5
{
	LEDSetColor(i+5);		//add offset reserved for other status
}

// functions to indicate current option
void LEDSetupOptions(unsigned char i)	//i should not exceed LEDmax Flash
{
	LEDSetColor(flashColorTable[0]);
	LEDFlashSeqCounter = 0;
	LEDFlashCount = i*2+1;
}

void LEDOptions()		//flashes LED to indicate current option
{
	unsigned char static LEDFlashTimer;	//Software timer to decouple flashing frequency from calling frequency
	if (LEDFlashTimer)
		{
		--LEDFlashTimer;
		}
	else
		{
		LEDFlashTimer = LEDmaxFlashTimer;
		if (LEDFlashSeqCounter <= (LEDFlashCount))
			{
			LEDSetColor(flashColorTable[LEDFlashSeqCounter & flashColorMask]);
			}
		if (LEDFlashSeqCounter == LEDFlashMaxSeqSteps)
			{
			LEDFlashSeqCounter = 0;
			}
		else
			{
			++LEDFlashSeqCounter;		//upcounting required for proper reading of flash color table
			}
		}
}

// functions to indicate standby and dim the status led according to surrounding brightness

void LEDSetupStandby()					//call once before entering standby, then call LEDStandby repeatedly
{
	LEDSetColor(LEDRed);
	LEDStandbyTimer = LEDmaxStandyTimer;
}

void LEDStandby()
{
	if (LEDStandbyTimer)
		{
		--LEDStandbyTimer;
		}
		else
		{
			if (ExtBrightness > 0xFFFF0)	// must be equal to 0xFFFF << 4
				{
				OCRCL = 0xFF;
				OCRCH = 0xFF;
				}
			else
				{
				//dim red LED along with external brightness
				OCRCL = (ExtBrightness >> 4) & 0x00FF;
				OCRCH = (ExtBrightness >> 12) & 0x00FF;
				}
			TCR21 = PLLSetting;	//Set PLL prescaler and start CCU register update
		}
}

// functions to indicate overtemperture and temperature derating

void LEDOverTemp()			//flashes red LED to indicate overtemperature
{
	unsigned char static LEDFlashTimer;	//Software timer to decouple flashing frequency from calling frequency
	__bit static Status;
	LEDTempOn = 1;
	if (LEDFlashTimer)
		{
		--LEDFlashTimer;
		}
	else
		{
		LEDFlashTimer = LEDmaxFlashTimer;
		Status = !Status;
		RedLEDPort = Status;
		}
}

void LEDTempDerating()		//flashes green & blue LED to indicate temperature derating
				//flashes will appear as red / white if the basic color is white
{
	unsigned char static LEDFlashTimer;	//Software timer to decouple flashing frequency from calling frequency
	__bit static Status;
	LEDTempOn = 1;
	if (LEDFlashTimer)
		{
		--LEDFlashTimer;
		}
	else
		{
		LEDFlashTimer = LEDmaxFlashTimer;
		Status = !Status;
		GreenLEDPort = Status;
		BlueLEDPort = Status;
		}
}

void LEDTempReset()
{
	if (LEDTempOn)
		{
		RedLEDPort = 1;
		GreenLEDPort = 1;
		BlueLEDPort = 1;
		LEDTempOn = 0;
		}
}

// two functions to indicate lower and upper limit of brightness setting

void LEDSetupLimit()
{
	if (maxLimit == limit)
		{
		LEDLimitFlashTimer = LEDmaxLimitFlashTimer;
		BlueLEDPort = 0;
		RedLEDPort = 0;
		}
	else if (minLimit == limit)
		{
		LEDLimitFlashTimer = LEDmaxLimitFlashTimer;
		BlueLEDPort = 0;
		GreenLEDPort = 0;
		}
}

void LEDLimit()		//blanks LED to indicate brightness limit
{
	if (LEDLimitFlashTimer)
		{
		--LEDLimitFlashTimer;
		if (0==LEDLimitFlashTimer)	//Restore LED status
			{
			BlueLEDPort = 1;
			GreenLEDPort = 1;
			RedLEDPort = 1;
			}
		}
}

// function to set PWM for status led

void LEDSetColor(unsigned char i)
{
	unsigned int tempPWM;
	tempPWM = colorTable[i][0];	//blue
	tempPWM *= tempPWM;

	OCRAL = tempPWM & 0x00FF;
	OCRAH = (tempPWM >> 8) & 0x00FF;

	tempPWM = colorTable[i][1];	//green
	tempPWM *= tempPWM;

	OCRBL = tempPWM & 0x00FF;
	OCRBH = (tempPWM >> 8) & 0x00FF;

	tempPWM = colorTable[i][2];	//red
	tempPWM *= tempPWM;

	OCRCL = tempPWM & 0x00FF;
	OCRCH = (tempPWM >> 8) & 0x00FF;

	TCR21 = PLLSetting;	//Set PLL prescaler and start CCU register update
}

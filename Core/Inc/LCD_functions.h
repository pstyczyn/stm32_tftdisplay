/*
 * LCD_functions.h
 *
 *  Created on: Jan 12, 2025
 *      Author: patry
 */


#ifndef INC_LCD_FUNCTIONS_H_
#define INC_LCD_FUNCTIONS_H_
#include <my_functions.h>
typedef enum {FALSE=0, TRUE=!FALSE} bool;
typedef struct lcd_adres
{
	uint8_t ZERO;
	uint8_t SOFTWARERESET;
	uint8_t SLEEPOUT;
	uint8_t DISPLAYOFF;
	uint8_t DISPLAYON;
	uint8_t INTERFEJSON;
	uint8_t BIT_12_ON_PIXEL;
	uint8_t BIT_16_ON_PIXEL;
	uint8_t BIT_18_ON_PIXEL;
	uint8_t CASET;
	uint8_t RASET;
	uint8_t MEMWRITE;
	uint8_t X0_SET;
	uint8_t Y0_SET;
	uint8_t X_END_SET;
	uint8_t Y_END_SET;
	uint8_t RM;
	uint8_t GM;
	uint8_t BM;
	uint8_t Mbyte;
	uint8_t Mbyte2;
}adres_t;


void CS_ON(void)
{
	HAL_GPIO_WritePin(GPIOA, CS_Pin, SET);
}
void CS_OFF(void)
{
	HAL_GPIO_WritePin(GPIOA, CS_Pin, RESET);
}
void DC_ON(void)
{
	HAL_GPIO_WritePin(GPIOA, DC_Pin, SET);
}
void DC_OFF(void)
{
	HAL_GPIO_WritePin(GPIOA, DC_Pin, RESET);
}
void RESET_ON(void)
{
	HAL_GPIO_WritePin(GPIOC, RESET_Pin, RESET);
}
void RESET_OFF(void)
{
	HAL_GPIO_WritePin(GPIOC, RESET_Pin, SET);
}

void LCD_komenda(uint8_t komenda)
{
	DC_OFF();
	CS_OFF();
	HAL_SPI_Transmit(&hspi3, &komenda, 1, HAL_MAX_DELAY);
	CS_ON();
}
void LCD_dana(uint8_t dana)
{
	DC_ON();
	CS_OFF();
	HAL_SPI_Transmit(&hspi3, &dana, 1, HAL_MAX_DELAY);
	CS_ON();
}

void LCD_init(adres_t *ADRES)
{
	RESET_ON();
	HAL_Delay(50);
	RESET_OFF();
	HAL_Delay(50);
	LCD_komenda(ADRES->SOFTWARERESET);
	HAL_Delay(130);
	LCD_komenda(ADRES->SLEEPOUT);
	HAL_Delay(130);
	LCD_komenda(ADRES->INTERFEJSON);
	LCD_dana(ADRES->BIT_16_ON_PIXEL);
	LCD_komenda(ADRES->DISPLAYON);
	HAL_Delay(100);
}

void LCD_set_adressing(adres_t *ADRES)
{
	LCD_komenda(ADRES->CASET);
	LCD_dana(ADRES->ZERO);
	LCD_dana(ADRES->X0_SET);
	LCD_dana(ADRES->ZERO);
	LCD_dana(ADRES->X_END_SET);
	LCD_komenda(ADRES->RASET);
	LCD_dana(ADRES->ZERO);
	LCD_dana(ADRES->Y0_SET);
	LCD_dana(ADRES->ZERO);
	LCD_dana(ADRES->Y_END_SET);
	LCD_komenda(ADRES->MEMWRITE);
}
void LCD_printM(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<=ADRES->Y_END_SET;wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<=ADRES->X_END_SET;kolumna++)
		{
			if(kolumna==(ADRES->X0_SET+2) && wiersz>=(ADRES->Y0_SET+1) && wiersz<(ADRES->Y_END_SET+1))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+10) && wiersz>=(ADRES->Y0_SET+1) && wiersz<(ADRES->Y_END_SET+1))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(wiersz==(ADRES->Y0_SET+2) && (kolumna==(ADRES->X0_SET+3) || kolumna==(ADRES->X0_SET+9)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(wiersz==(ADRES->Y0_SET+3) && (kolumna==(ADRES->X0_SET+4) || kolumna==(ADRES->X0_SET+8)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(wiersz==(ADRES->Y0_SET+4) && (kolumna==(ADRES->X0_SET+5) || kolumna==(ADRES->X0_SET+7)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(wiersz==(ADRES->Y0_SET+5) && kolumna==(ADRES->X0_SET+6))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();

}

void LCD_printE(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<=ADRES->Y_END_SET;wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<=ADRES->X_END_SET;kolumna++)
		{
			if(kolumna==(ADRES->X0_SET+1) && wiersz>=(ADRES->Y0_SET+1) && wiersz<(ADRES->Y_END_SET+1))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna>=(ADRES->X0_SET+1) && kolumna<(ADRES->X_END_SET+1) && wiersz==(ADRES->Y0_SET+8))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna>=(ADRES->X0_SET+1) && kolumna<(ADRES->X_END_SET+1) && wiersz==(ADRES->Y0_SET+1))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna>=(ADRES->X0_SET+1) && kolumna<(ADRES->X_END_SET+1) && wiersz==(ADRES->Y_END_SET-1))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();

}

void LCD_printN(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<=ADRES->Y_END_SET;wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<=ADRES->X_END_SET;kolumna++)
		{
			if(kolumna==(ADRES->X0_SET+1) && wiersz>=(ADRES->Y0_SET+1) && wiersz<(ADRES->Y0_SET+14))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+10) && wiersz>=(ADRES->Y0_SET+1) && wiersz<(ADRES->Y0_SET+14))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+2) && wiersz==(ADRES->Y0_SET+2))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+3) && wiersz==(ADRES->Y0_SET+4))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+4) && wiersz==(ADRES->Y0_SET+6))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+5) && wiersz==(ADRES->Y0_SET+8))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+6) && wiersz==(ADRES->Y0_SET+10))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+7) && wiersz==(ADRES->Y0_SET+12))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+8) && wiersz==(ADRES->Y0_SET+14))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}

		}
	}
	CS_ON();
}

void LCD_printU(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<=ADRES->Y_END_SET;wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<=ADRES->X_END_SET;kolumna++)
		{
			if(kolumna==(ADRES->X0_SET+1) && wiersz>=(ADRES->Y0_SET+1) && wiersz<(ADRES->Y_END_SET-3))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X_END_SET) && wiersz>=(ADRES->Y0_SET+1) && wiersz<(ADRES->Y_END_SET-3))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna>=(ADRES->X0_SET+3) && kolumna<(ADRES->X_END_SET-3) && wiersz==(ADRES->Y_END_SET-1))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+2) || kolumna==(ADRES->X_END_SET-1)) && wiersz==(ADRES->Y_END_SET-3))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+3) || kolumna==(ADRES->X_END_SET-2)) && wiersz==(ADRES->Y_END_SET-2))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();
}

void LCD_printA(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{
			if((kolumna==(ADRES->X0_SET+2) || kolumna==(ADRES->X0_SET+10)) && (wiersz==(ADRES->Y0_SET+13) || wiersz==(ADRES->Y0_SET+14)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+3) || kolumna==(ADRES->X0_SET+9)) && (wiersz==(ADRES->Y0_SET+9) || wiersz==(ADRES->Y0_SET+10)||  wiersz==(ADRES->Y0_SET+11)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+4) || kolumna==(ADRES->X0_SET+8)) && (wiersz==(ADRES->Y0_SET+4) || wiersz==(ADRES->Y0_SET+5) || wiersz==(ADRES->Y0_SET+7)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+5) || kolumna==(ADRES->X0_SET+7)) && (wiersz==(ADRES->Y0_SET+2) || wiersz==(ADRES->Y0_SET+3)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+6) && (wiersz==(ADRES->Y0_SET+0) || wiersz==(ADRES->Y0_SET+1)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+5) && kolumna<=(ADRES->X0_SET+8)) && wiersz==(ADRES->Y0_SET+7))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();
}

void LCD_printB(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{
			if(kolumna==(ADRES->X0_SET+2)  && (wiersz<ADRES->Y_END_SET))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+3) &&  kolumna<=(ADRES->X0_SET+6)) && (wiersz==(ADRES->Y0_SET+0) || wiersz==(ADRES->Y0_SET+1) || wiersz==(ADRES->Y0_SET+7) || wiersz==(ADRES->Y0_SET+8) || wiersz==(ADRES->Y0_SET+13)|| wiersz==(ADRES->Y0_SET+14)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+7) && (wiersz==(ADRES->Y0_SET+1) || wiersz==(ADRES->Y0_SET+7) || wiersz==(ADRES->Y0_SET+9) || wiersz==(ADRES->Y0_SET+14)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+8) && (wiersz==(ADRES->Y0_SET+2) || wiersz==(ADRES->Y0_SET+6) || wiersz==(ADRES->Y0_SET+10) || wiersz==(ADRES->Y0_SET+13)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+9) && (wiersz==(ADRES->Y0_SET+3) || wiersz==(ADRES->Y0_SET+4) || wiersz==(ADRES->Y0_SET+11) || wiersz==(ADRES->Y0_SET+12)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}}
	}
	CS_ON();
}

void LCD_printC(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{
			if(kolumna==(ADRES->X0_SET+2)  && (wiersz>=(ADRES->Y0_SET+5) && wiersz<=(ADRES->Y0_SET+7)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+7) &&  kolumna<=(ADRES->X0_SET+10)) && (wiersz==(ADRES->Y0_SET+0) || wiersz==(ADRES->Y0_SET+12)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+3) && (wiersz==(ADRES->Y0_SET+4) || wiersz==(ADRES->Y0_SET+8)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+4) && (wiersz==(ADRES->Y0_SET+3) || wiersz==(ADRES->Y0_SET+9)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+5) && (wiersz==(ADRES->Y0_SET+2) || wiersz==(ADRES->Y0_SET+10)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+6) && (wiersz==(ADRES->Y0_SET+1) || wiersz==(ADRES->Y0_SET+11)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();
}

void LCD_printD(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{
			if(kolumna==(ADRES->X0_SET+2) && wiersz<(ADRES->Y0_SET+15))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+3) && (wiersz==(ADRES->Y0_SET+0) || wiersz==(ADRES->Y0_SET+14)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+4) && (wiersz==(ADRES->Y0_SET+0) || wiersz==(ADRES->Y0_SET+1) || wiersz==(ADRES->Y0_SET+13) || wiersz==(ADRES->Y0_SET+14)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+5) && (wiersz==(ADRES->Y0_SET+1) || wiersz==(ADRES->Y0_SET+2) || wiersz==(ADRES->Y0_SET+12) || wiersz==(ADRES->Y0_SET+13)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+6) && (wiersz==(ADRES->Y0_SET+2) || wiersz==(ADRES->Y0_SET+3) || wiersz==(ADRES->Y0_SET+11) || wiersz==(ADRES->Y0_SET+12)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+7) && (wiersz==(ADRES->Y0_SET+3) || wiersz==(ADRES->Y0_SET+4) || wiersz==(ADRES->Y0_SET+10) || wiersz==(ADRES->Y0_SET+11)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+8) && (wiersz>=(ADRES->Y0_SET+4) && wiersz<=(ADRES->Y0_SET+10)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+9) && (wiersz>(ADRES->Y0_SET+4) && wiersz<(ADRES->Y0_SET+10)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();
}

void LCD_printF(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{
			if(kolumna==(ADRES->X0_SET+2) && wiersz<ADRES->Y_END_SET)
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+2)&&kolumna<=(ADRES->X_END_SET)) && (wiersz==(ADRES->Y0_SET+0) || wiersz==(ADRES->Y0_SET+5)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();
}

void LCD_printG(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{
			if(kolumna==(ADRES->X0_SET+2) && (wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+12)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+5) && kolumna<=(ADRES->X0_SET+10)) && wiersz==(ADRES->Y0_SET+0))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+5) && kolumna<=(ADRES->X0_SET+10)) && wiersz==(ADRES->Y0_SET+6))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+5) && kolumna<=(ADRES->X0_SET+10)) && wiersz==(ADRES->Y0_SET+14))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+10) && (wiersz>=(ADRES->Y0_SET+6) && wiersz<=(ADRES->Y0_SET+14)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+3) && (wiersz==(ADRES->Y0_SET+2) || wiersz==(ADRES->Y0_SET+13)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+4) && (wiersz==(ADRES->Y0_SET+1) || wiersz==(ADRES->Y0_SET+14)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();
}

void LCD_printH(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{
			if(kolumna==(ADRES->X0_SET+2) && (wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+12)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+5) && kolumna<=(ADRES->X0_SET+10)) && wiersz==(ADRES->Y0_SET+0))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+5) && kolumna<=(ADRES->X0_SET+10)) && wiersz==(ADRES->Y0_SET+6))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+5) && kolumna<=(ADRES->X0_SET+10)) && wiersz==(ADRES->Y0_SET+14))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+10) && (wiersz>=(ADRES->Y0_SET+6) && wiersz<=(ADRES->Y0_SET+14)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+3) && (wiersz==(ADRES->Y0_SET+2) || wiersz==(ADRES->Y0_SET+13)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+4) && (wiersz==(ADRES->Y0_SET+1) || wiersz==(ADRES->Y0_SET+14)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();
}

void LCD_printI(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{
			if(kolumna==(ADRES->X0_SET+6) && (wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+14)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+8)) && wiersz==(ADRES->Y0_SET+0))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+8)) && wiersz==(ADRES->Y0_SET+14))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();
}

void LCD_printJ(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{
			if(kolumna==(ADRES->X0_SET+8) && (wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+13)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+5) && kolumna<=(ADRES->X0_SET+10)) && wiersz==(ADRES->Y0_SET+0))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+7)) && wiersz==(ADRES->Y0_SET+14))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+2) && (wiersz>=(ADRES->Y0_SET+9) && wiersz<=(ADRES->Y0_SET+12)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+3) && wiersz==(ADRES->Y0_SET+13))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();
}

void LCD_printK(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{
			if(kolumna==(ADRES->X0_SET+3) && (wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+14)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(((kolumna>=(ADRES->X0_SET+2) && kolumna<=(ADRES->X0_SET+4)) || (kolumna>=(ADRES->X0_SET+8) && kolumna<=(ADRES->X0_SET+10))) && wiersz==(ADRES->Y0_SET+0))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(((kolumna>=(ADRES->X0_SET+2) && kolumna<=(ADRES->X0_SET+4)) || (kolumna>=(ADRES->X0_SET+8) && kolumna<=(ADRES->X0_SET+10))) && wiersz==(ADRES->Y0_SET+14))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+4) && (wiersz==(ADRES->Y0_SET+6) || wiersz==(ADRES->Y0_SET+8)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+5) && (wiersz==(ADRES->Y0_SET+5) || wiersz==(ADRES->Y0_SET+9)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+6) && (wiersz==(ADRES->Y0_SET+4) || wiersz==(ADRES->Y0_SET+10)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+7) && (wiersz==(ADRES->Y0_SET+3) || wiersz==(ADRES->Y0_SET+11)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+8) && (wiersz==(ADRES->Y0_SET+2) || wiersz==(ADRES->Y0_SET+12)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+9) && (wiersz==(ADRES->Y0_SET+1) || wiersz==(ADRES->Y0_SET+13)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();
}

void LCD_printL(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{
			if(kolumna==(ADRES->X0_SET+3) && (wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+14)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+2) && kolumna<=(ADRES->X0_SET+10)) && wiersz==(ADRES->Y0_SET+14))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+2) && kolumna<=(ADRES->X0_SET+4)) && wiersz==(ADRES->Y0_SET+0))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+10) && (wiersz>=(ADRES->Y0_SET+11) && wiersz<=(ADRES->Y0_SET+14)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();
}

void LCD_printO(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{
			if((kolumna==(ADRES->X0_SET+2) || kolumna==(ADRES->X0_SET+10))  && (wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+11)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+5) && kolumna<=(ADRES->X0_SET+7)) && (wiersz==(ADRES->Y0_SET+0) || wiersz==(ADRES->Y0_SET+14)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+3)) && (wiersz==(ADRES->Y0_SET+2) || wiersz==(ADRES->Y0_SET+12)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+4)) && (wiersz==(ADRES->Y0_SET+1) || wiersz==(ADRES->Y0_SET+13)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+9)) && (wiersz==(ADRES->Y0_SET+2) || wiersz==(ADRES->Y0_SET+12)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+8)) && (wiersz==(ADRES->Y0_SET+1) || wiersz==(ADRES->Y0_SET+13)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();
}

void LCD_printP(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{
			if(kolumna==(ADRES->X0_SET+2) && (wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+14)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+6)) && wiersz==(ADRES->Y0_SET+0))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+6)) && wiersz==(ADRES->Y0_SET+7))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+7)) && (wiersz==(ADRES->Y0_SET+1) || wiersz==(ADRES->Y0_SET+6)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+8)) && (wiersz==(ADRES->Y0_SET+2) || wiersz==(ADRES->Y0_SET+5)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+9)) && (wiersz==(ADRES->Y0_SET+3) || wiersz==(ADRES->Y0_SET+4)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();
}

void LCD_printR(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{
			if(kolumna==(ADRES->X0_SET+2) && (wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+14)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+6)) && wiersz==(ADRES->Y0_SET+0))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+6)) && wiersz==(ADRES->Y0_SET+7))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+6)) && wiersz==(ADRES->Y0_SET+8))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+7)) && (wiersz==(ADRES->Y0_SET+1) || wiersz==(ADRES->Y0_SET+6) || wiersz==(ADRES->Y0_SET+11)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+8)) && (wiersz==(ADRES->Y0_SET+2) || wiersz==(ADRES->Y0_SET+5) || wiersz==(ADRES->Y0_SET+12)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+9)) && (wiersz==(ADRES->Y0_SET+3) || wiersz==(ADRES->Y0_SET+4) || wiersz==(ADRES->Y0_SET+13)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+5)) && (wiersz==(ADRES->Y0_SET+9)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+6)) && (wiersz==(ADRES->Y0_SET+10)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+10)) && (wiersz==(ADRES->Y0_SET+14)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();
}

void LCD_printS(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{
			if(kolumna==(ADRES->X0_SET+2) && ((wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+5)) || (wiersz>=(ADRES->Y0_SET+13) && wiersz<=(ADRES->Y0_SET+14))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+5) && kolumna<=(ADRES->X0_SET+9)) && wiersz==(ADRES->Y0_SET+0))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+8)) && wiersz==(ADRES->Y0_SET+7))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+7)) && wiersz==(ADRES->Y0_SET+14))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+10)) && (wiersz>=(ADRES->Y0_SET+9) && wiersz<=(ADRES->Y0_SET+11) || wiersz==(ADRES->Y0_SET+0) || wiersz==(ADRES->Y0_SET+1)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+3)) && (wiersz==(ADRES->Y0_SET+2) || wiersz==(ADRES->Y0_SET+6)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+4)) && (wiersz==(ADRES->Y0_SET+1) || wiersz==(ADRES->Y0_SET+7)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+8)) && (wiersz==(ADRES->Y0_SET+13)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+9)) && (wiersz==(ADRES->Y0_SET+8) || wiersz==(ADRES->Y0_SET+12)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();
}

void LCD_printT(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{
			if(kolumna==(ADRES->X0_SET+6) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+14))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+2) && kolumna<=(ADRES->X0_SET+10)) && wiersz==(ADRES->Y0_SET+0))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+2) || kolumna==(ADRES->X0_SET+10)) && wiersz==(ADRES->Y0_SET+1))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();
}

void LCD_printW(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{
			if(kolumna==(ADRES->X0_SET+2) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+14))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+10) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+14))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+3) || kolumna==(ADRES->X0_SET+9)) && wiersz==(ADRES->Y0_SET+13))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+4) || kolumna==(ADRES->X0_SET+8)) && wiersz==(ADRES->Y0_SET+12))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+5) || kolumna==(ADRES->X0_SET+7)) && wiersz==(ADRES->Y0_SET+11))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+6)) && wiersz==(ADRES->Y0_SET+10))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();
}

void LCD_printY(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{
			if(kolumna==(ADRES->X0_SET+6) && ((wiersz>=(ADRES->Y0_SET+4) && wiersz<=(ADRES->Y0_SET+14))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+5) && kolumna<=(ADRES->X0_SET+7)) && wiersz==(ADRES->Y0_SET+14))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+10) || kolumna==(ADRES->X0_SET+2)) && wiersz==(ADRES->Y0_SET+0))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+9) || kolumna==(ADRES->X0_SET+3)) && wiersz==(ADRES->Y0_SET+1))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+5) || kolumna==(ADRES->X0_SET+7)) && wiersz==(ADRES->Y0_SET+3))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+4) || kolumna==(ADRES->X0_SET+8)) && wiersz==(ADRES->Y0_SET+2))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();
}

void LCD_printZ(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{
			if(kolumna==(ADRES->X0_SET+2) && ((wiersz>=(ADRES->Y0_SET+11) && wiersz<=(ADRES->Y0_SET+14)) || wiersz==(ADRES->Y0_SET+0)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+10)) && wiersz==(ADRES->Y0_SET+0))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+10)) && wiersz==(ADRES->Y0_SET+14))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+10) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+3))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+9)) && wiersz==(ADRES->Y0_SET+4))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+8)) && wiersz==(ADRES->Y0_SET+5))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+7)) && wiersz==(ADRES->Y0_SET+6))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+6)) && wiersz==(ADRES->Y0_SET+7))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+5)) && wiersz==(ADRES->Y0_SET+8))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+4)) && wiersz==(ADRES->Y0_SET+9))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+3)) && wiersz==(ADRES->Y0_SET+10))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();
}

void LCD_print1(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{
			if(kolumna==(ADRES->X0_SET+9) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+14))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+7) && kolumna<=(ADRES->X0_SET+10)) && wiersz==(ADRES->Y0_SET+14))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+6)) && wiersz==(ADRES->Y0_SET+3))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+7)) && wiersz==(ADRES->Y0_SET+2))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+8)) && wiersz==(ADRES->Y0_SET+1))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();
}

void LCD_print2(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{
			if(kolumna==(ADRES->X0_SET+3) && ((wiersz>=(ADRES->Y0_SET+11) && wiersz<=(ADRES->Y0_SET+14) || wiersz==(ADRES->Y0_SET+2))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+5) && kolumna<=(ADRES->X0_SET+7)) && wiersz==(ADRES->Y0_SET+0))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+10)) && wiersz==(ADRES->Y0_SET+13))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+9) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+5) || wiersz==(ADRES->Y0_SET+13))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+4)) && (wiersz==(ADRES->Y0_SET+1) || wiersz==(ADRES->Y0_SET+10)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+8)) && (wiersz==(ADRES->Y0_SET+1) || wiersz==(ADRES->Y0_SET+6)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+5)) && wiersz==(ADRES->Y0_SET+9))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+6)) && wiersz==(ADRES->Y0_SET+8))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+7)) && wiersz==(ADRES->Y0_SET+7))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();
}

void LCD_print3(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{
			if(kolumna==(ADRES->X0_SET+7) && ((wiersz==(ADRES->Y0_SET+1) || wiersz==(ADRES->Y0_SET+6) || wiersz==(ADRES->Y0_SET+8) || wiersz==(ADRES->Y0_SET+13))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+6)) && wiersz==(ADRES->Y0_SET+0))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+6)) && wiersz==(ADRES->Y0_SET+7))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+6)) && wiersz==(ADRES->Y0_SET+14))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+8) && ((wiersz==(ADRES->Y0_SET+2) || wiersz==(ADRES->Y0_SET+5) || wiersz==(ADRES->Y0_SET+9) || wiersz==(ADRES->Y0_SET+12))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+9) && ((wiersz==(ADRES->Y0_SET+3) || wiersz==(ADRES->Y0_SET+4) || wiersz==(ADRES->Y0_SET+10) || wiersz==(ADRES->Y0_SET+111))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();
}

void LCD_print4(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{
			if(kolumna==(ADRES->X0_SET+8) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+14))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+2) && kolumna<=(ADRES->X0_SET+10)) && wiersz==(ADRES->Y0_SET+7))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+3)) && wiersz==(ADRES->Y0_SET+6))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+4)) && wiersz==(ADRES->Y0_SET+5))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+5)) && wiersz==(ADRES->Y0_SET+4))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+6)) && wiersz==(ADRES->Y0_SET+3))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+7)) && wiersz==(ADRES->Y0_SET+2))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+8) && kolumna<=(ADRES->X0_SET+10)) && wiersz==(ADRES->Y0_SET+14))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();
}

void LCD_print5(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{
			if(kolumna==(ADRES->X0_SET+2) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+6))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+2) && kolumna<=(ADRES->X0_SET+10)) && wiersz==(ADRES->Y0_SET+0))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+2) && kolumna<=(ADRES->X0_SET+8)) && wiersz==(ADRES->Y0_SET+7))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+2) && kolumna<=(ADRES->X0_SET+8)) && wiersz==(ADRES->Y0_SET+14))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+9)) && (wiersz==(ADRES->Y0_SET+8) || wiersz==(ADRES->Y0_SET+13)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+10) && ((wiersz>=(ADRES->Y0_SET+9) && wiersz<=(ADRES->Y0_SET+12))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();
}

void LCD_print6(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{
			if(kolumna==(ADRES->X0_SET+2) && ((wiersz>=(ADRES->Y0_SET+6) && wiersz<=(ADRES->Y0_SET+11))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+5) && kolumna<=(ADRES->X0_SET+8)) && wiersz==(ADRES->Y0_SET+7))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+2) && kolumna<=(ADRES->X0_SET+8)) && wiersz==(ADRES->Y0_SET+14))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+7) && kolumna<=(ADRES->X0_SET+10)) && wiersz==(ADRES->Y0_SET+0))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+3)) && (wiersz==(ADRES->Y0_SET+4) || wiersz==(ADRES->Y0_SET+9) || wiersz==(ADRES->Y0_SET+12)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+4)) && (wiersz==(ADRES->Y0_SET+3) || wiersz==(ADRES->Y0_SET+8) || wiersz==(ADRES->Y0_SET+13)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+5)) && (wiersz==(ADRES->Y0_SET+2)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+6)) && (wiersz==(ADRES->Y0_SET+1)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+9)) && (wiersz==(ADRES->Y0_SET+8) || wiersz==(ADRES->Y0_SET+13)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();
}

void LCD_print_dot(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{
			if((kolumna>=(ADRES->X0_SET+2) && kolumna<=(ADRES->X0_SET+10)) && ((wiersz>=(ADRES->Y0_SET+42) && wiersz<=(ADRES->Y0_SET+50))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}

			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();
}

void LCD_print_myslnik(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{

			if((kolumna>=(ADRES->X0_SET+1) && kolumna<=(ADRES->X0_SET+9)) && wiersz==(ADRES->Y0_SET+7))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();
}

void LCD_print8H(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{
			if((kolumna>=(ADRES->X0_SET+10) && kolumna<=(ADRES->X0_SET+21)) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+4))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+10) && kolumna<=(ADRES->X0_SET+21)) && ((wiersz>=(ADRES->Y0_SET+21) && wiersz<=(ADRES->Y0_SET+28))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+10) && kolumna<=(ADRES->X0_SET+21)) && ((wiersz>=(ADRES->Y0_SET+47) && wiersz<=(ADRES->Y0_SET+50))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+2) && kolumna<=(ADRES->X0_SET+5)) && ((wiersz>=(ADRES->Y0_SET+8) && wiersz<=(ADRES->Y0_SET+17))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+2) && kolumna<=(ADRES->X0_SET+5)) && ((wiersz>=(ADRES->Y0_SET+32) && wiersz<=(ADRES->Y0_SET+43))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+25) && kolumna<=(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+8) && wiersz<=(ADRES->Y0_SET+17))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+25) && kolumna<=(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+32) && wiersz<=(ADRES->Y0_SET+43))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+2)) && ((wiersz>=(ADRES->Y0_SET+31) && wiersz<=(ADRES->Y0_SET+43))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+7) && wiersz<=(ADRES->Y0_SET+18)) || (wiersz>=(ADRES->Y0_SET+30) && wiersz<=(ADRES->Y0_SET+44))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+4)) && ((wiersz>=(ADRES->Y0_SET+6) && wiersz<=(ADRES->Y0_SET+19)) || (wiersz>=(ADRES->Y0_SET+29) && wiersz<=(ADRES->Y0_SET+45))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+5)) && ((wiersz>=(ADRES->Y0_SET+5) && wiersz<=(ADRES->Y0_SET+20)) || (wiersz>=(ADRES->Y0_SET+28) && wiersz<=(ADRES->Y0_SET+46))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+6)) && ((wiersz>=(ADRES->Y0_SET+4) && wiersz<=(ADRES->Y0_SET+7)) || (wiersz>=(ADRES->Y0_SET+18) && wiersz<=(ADRES->Y0_SET+21))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+6)) && ((wiersz>=(ADRES->Y0_SET+27) && wiersz<=(ADRES->Y0_SET+31)) || (wiersz>=(ADRES->Y0_SET+44) && wiersz<=(ADRES->Y0_SET+47))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+7)) && ((wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+6)) || (wiersz>=(ADRES->Y0_SET+19) && wiersz<=(ADRES->Y0_SET+22))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+7)) && ((wiersz>=(ADRES->Y0_SET+26) && wiersz<=(ADRES->Y0_SET+30)) || (wiersz>=(ADRES->Y0_SET+45) && wiersz<=(ADRES->Y0_SET+48))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+8)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+5)) || (wiersz>=(ADRES->Y0_SET+20) && wiersz<=(ADRES->Y0_SET+23))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+8)) && ((wiersz>=(ADRES->Y0_SET+20) && wiersz<=(ADRES->Y0_SET+29)) || (wiersz>=(ADRES->Y0_SET+46) && wiersz<=(ADRES->Y0_SET+49))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+9)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+4)) || (wiersz>=(ADRES->Y0_SET+21) && wiersz<=(ADRES->Y0_SET+28))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+9)) && ((wiersz>=(ADRES->Y0_SET+47) && wiersz<=(ADRES->Y0_SET+50))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+22)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+4)) || (wiersz>=(ADRES->Y0_SET+20) && wiersz<=(ADRES->Y0_SET+29))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+22)) && ((wiersz>=(ADRES->Y0_SET+46) && wiersz<=(ADRES->Y0_SET+50))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+5)) || (wiersz>=(ADRES->Y0_SET+19) && wiersz<=(ADRES->Y0_SET+23))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+25) && wiersz<=(ADRES->Y0_SET+30)) || (wiersz>=(ADRES->Y0_SET+45) && wiersz<=(ADRES->Y0_SET+49))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+24)) && ((wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+6)) || (wiersz>=(ADRES->Y0_SET+18) && wiersz<=(ADRES->Y0_SET+22))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+24)) && ((wiersz>=(ADRES->Y0_SET+26) && wiersz<=(ADRES->Y0_SET+31)) || (wiersz>=(ADRES->Y0_SET+44) && wiersz<=(ADRES->Y0_SET+48))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+25)) && ((wiersz>=(ADRES->Y0_SET+4) && wiersz<=(ADRES->Y0_SET+7)) || (wiersz>=(ADRES->Y0_SET+17) && wiersz<=(ADRES->Y0_SET+21))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+25)) && ((wiersz>=(ADRES->Y0_SET+27) && wiersz<=(ADRES->Y0_SET+32)) || (wiersz>=(ADRES->Y0_SET+43) && wiersz<=(ADRES->Y0_SET+47))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+26)) && ((wiersz>=(ADRES->Y0_SET+5) && wiersz<=(ADRES->Y0_SET+7)) || (wiersz>=(ADRES->Y0_SET+17) && wiersz<=(ADRES->Y0_SET+20))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+26)) && ((wiersz>=(ADRES->Y0_SET+28) && wiersz<=(ADRES->Y0_SET+46))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+27)) && ((wiersz>=(ADRES->Y0_SET+6) && wiersz<=(ADRES->Y0_SET+19)) || (wiersz>=(ADRES->Y0_SET+29) && wiersz<=(ADRES->Y0_SET+45))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+7) && wiersz<=(ADRES->Y0_SET+18)) || (wiersz>=(ADRES->Y0_SET+30) && wiersz<=(ADRES->Y0_SET+44))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+8) && wiersz<=(ADRES->Y0_SET+17)) || (wiersz>=(ADRES->Y0_SET+31) && wiersz<=(ADRES->Y0_SET+43))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();
}

void LCD_print1H(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{
			if(kolumna==(ADRES->X0_SET+25) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+50))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+26) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+50))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+27) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+50))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+28) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+50))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+13) && kolumna<=(ADRES->X0_SET+30)) && wiersz==(ADRES->Y0_SET+49))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+13) && kolumna<=(ADRES->X0_SET+30)) && wiersz==(ADRES->Y0_SET+50))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+13) && kolumna<=(ADRES->X0_SET+30)) && wiersz==(ADRES->Y0_SET+48))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+13) && kolumna<=(ADRES->X0_SET+30)) && wiersz==(ADRES->Y0_SET+46))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+13) && kolumna<=(ADRES->X0_SET+30)) && wiersz==(ADRES->Y0_SET+47))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+6) && kolumna<=(ADRES->X0_SET+13)) && wiersz==(ADRES->Y0_SET+18))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+7) && kolumna<=(ADRES->X0_SET+14)) && wiersz==(ADRES->Y0_SET+17))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+8) && kolumna<=(ADRES->X0_SET+15)) && wiersz==(ADRES->Y0_SET+16))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+9) && kolumna<=(ADRES->X0_SET+16)) && wiersz==(ADRES->Y0_SET+15))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+10) && kolumna<=(ADRES->X0_SET+17)) && wiersz==(ADRES->Y0_SET+14))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+11) && kolumna<=(ADRES->X0_SET+18)) && wiersz==(ADRES->Y0_SET+13))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+12) && kolumna<=(ADRES->X0_SET+19)) && wiersz==(ADRES->Y0_SET+12))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+13) && kolumna<=(ADRES->X0_SET+20)) && wiersz==(ADRES->Y0_SET+11))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+14) && kolumna<=(ADRES->X0_SET+21)) && wiersz==(ADRES->Y0_SET+10))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+15) && kolumna<=(ADRES->X0_SET+22)) && wiersz==(ADRES->Y0_SET+9))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+16) && kolumna<=(ADRES->X0_SET+23)) && wiersz==(ADRES->Y0_SET+8))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+17) && kolumna<=(ADRES->X0_SET+24)) && wiersz==(ADRES->Y0_SET+7))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+18) && kolumna<=(ADRES->X0_SET+25)) && wiersz==(ADRES->Y0_SET+6))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+19) && kolumna<=(ADRES->X0_SET+24)) && wiersz==(ADRES->Y0_SET+5))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+20) && kolumna<=(ADRES->X0_SET+24)) && wiersz==(ADRES->Y0_SET+4))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+21) && kolumna<=(ADRES->X0_SET+24)) && wiersz==(ADRES->Y0_SET+3))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+22) && kolumna<=(ADRES->X0_SET+24)) && wiersz==(ADRES->Y0_SET+2))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+23) && kolumna<=(ADRES->X0_SET+24)) && wiersz==(ADRES->Y0_SET+1))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();
}

void LCD_print2H(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{
			if((kolumna>=(ADRES->X0_SET+2) && kolumna<=(ADRES->X0_SET+30)) && ((wiersz>=(ADRES->Y0_SET+44) && wiersz<=(ADRES->Y0_SET+50))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+2) && kolumna<=(ADRES->X0_SET+11)) && ((wiersz>=(ADRES->Y0_SET+40) && wiersz<=(ADRES->Y0_SET+43))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+25) && kolumna<=(ADRES->X0_SET+30)) && ((wiersz>=(ADRES->Y0_SET+11) && wiersz<=(ADRES->Y0_SET+18))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+14) && kolumna<=(ADRES->X0_SET+20)) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+5))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+2) && kolumna<=(ADRES->X0_SET+6)) && ((wiersz>=(ADRES->Y0_SET+13) && wiersz<=(ADRES->Y0_SET+15))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+10) && wiersz<=(ADRES->Y0_SET+12))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+4)) && ((wiersz>=(ADRES->Y0_SET+9) && wiersz<=(ADRES->Y0_SET+12)) || wiersz==(ADRES->Y0_SET+39)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+5)) && ((wiersz>=(ADRES->Y0_SET+8) && wiersz<=(ADRES->Y0_SET+12)) || (wiersz>=(ADRES->Y0_SET+38) && wiersz<=(ADRES->Y0_SET+39))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+6)) && ((wiersz>=(ADRES->Y0_SET+7) && wiersz<=(ADRES->Y0_SET+12)) || (wiersz>=(ADRES->Y0_SET+37) && wiersz<=(ADRES->Y0_SET+39))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+7)) && ((wiersz>=(ADRES->Y0_SET+6) && wiersz<=(ADRES->Y0_SET+12)) || (wiersz>=(ADRES->Y0_SET+36) && wiersz<=(ADRES->Y0_SET+39))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+8)) && ((wiersz>=(ADRES->Y0_SET+5) && wiersz<=(ADRES->Y0_SET+11)) || (wiersz>=(ADRES->Y0_SET+35) && wiersz<=(ADRES->Y0_SET+39))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+9)) && ((wiersz>=(ADRES->Y0_SET+4) && wiersz<=(ADRES->Y0_SET+10)) || (wiersz>=(ADRES->Y0_SET+34) && wiersz<=(ADRES->Y0_SET+39))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+10)) && ((wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+9)) || (wiersz>=(ADRES->Y0_SET+33) && wiersz<=(ADRES->Y0_SET+39))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+11)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+8)) || (wiersz>=(ADRES->Y0_SET+32) && wiersz<=(ADRES->Y0_SET+39))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+12)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+7)) || (wiersz>=(ADRES->Y0_SET+31) && wiersz<=(ADRES->Y0_SET+39))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+13)) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+6)) || (wiersz>=(ADRES->Y0_SET+30) && wiersz<=(ADRES->Y0_SET+38))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+21)) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+6)) || (wiersz>=(ADRES->Y0_SET+22) && wiersz<=(ADRES->Y0_SET+30))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+22)) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+7)) || (wiersz>=(ADRES->Y0_SET+21) && wiersz<=(ADRES->Y0_SET+29))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+8)) || (wiersz>=(ADRES->Y0_SET+20) && wiersz<=(ADRES->Y0_SET+28))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+24)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+9)) || (wiersz>=(ADRES->Y0_SET+19) && wiersz<=(ADRES->Y0_SET+27))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+25)) && ((wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+10)) || (wiersz>=(ADRES->Y0_SET+18) && wiersz<=(ADRES->Y0_SET+26))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+26)) && ((wiersz>=(ADRES->Y0_SET+4) && wiersz<=(ADRES->Y0_SET+11)) || (wiersz>=(ADRES->Y0_SET+17) && wiersz<=(ADRES->Y0_SET+25))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+27)) && ((wiersz>=(ADRES->Y0_SET+5) && wiersz<=(ADRES->Y0_SET+24))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+6) && wiersz<=(ADRES->Y0_SET+23))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+6) && wiersz<=(ADRES->Y0_SET+22))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+30)) && ((wiersz>=(ADRES->Y0_SET+7) && wiersz<=(ADRES->Y0_SET+21))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+14)) && ((wiersz>=(ADRES->Y0_SET+29) && wiersz<=(ADRES->Y0_SET+37))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+15)) && ((wiersz>=(ADRES->Y0_SET+28) && wiersz<=(ADRES->Y0_SET+36))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+16)) && ((wiersz>=(ADRES->Y0_SET+27) && wiersz<=(ADRES->Y0_SET+35))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+17)) && ((wiersz>=(ADRES->Y0_SET+26) && wiersz<=(ADRES->Y0_SET+34))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+18)) && ((wiersz>=(ADRES->Y0_SET+25) && wiersz<=(ADRES->Y0_SET+33))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+19)) && ((wiersz>=(ADRES->Y0_SET+24) && wiersz<=(ADRES->Y0_SET+32))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+20)) && ((wiersz>=(ADRES->Y0_SET+23) && wiersz<=(ADRES->Y0_SET+31))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();
}

void LCD_print3H(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{
			if((kolumna>=(ADRES->X0_SET+10) && kolumna<=(ADRES->X0_SET+20)) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+5))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+25) && kolumna<=(ADRES->X0_SET+30)) && ((wiersz>=(ADRES->Y0_SET+10) && wiersz<=(ADRES->Y0_SET+20))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+14) && kolumna<=(ADRES->X0_SET+22)) && ((wiersz>=(ADRES->Y0_SET+23) && wiersz<=(ADRES->Y0_SET+27))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+25) && kolumna<=(ADRES->X0_SET+30)) && ((wiersz>=(ADRES->Y0_SET+31) && wiersz<=(ADRES->Y0_SET+40))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+9) && kolumna<=(ADRES->X0_SET+19)) && ((wiersz>=(ADRES->Y0_SET+46) && wiersz<=(ADRES->Y0_SET+50))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+2)) && ((wiersz>=(ADRES->Y0_SET+6) && wiersz<=(ADRES->Y0_SET+13) || (wiersz>=(ADRES->Y0_SET+39) && wiersz<=(ADRES->Y0_SET+48)))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+5) && wiersz<=(ADRES->Y0_SET+12) || (wiersz>=(ADRES->Y0_SET+40) && wiersz<=(ADRES->Y0_SET+49)))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+4)) && ((wiersz>=(ADRES->Y0_SET+4) && wiersz<=(ADRES->Y0_SET+11) || (wiersz>=(ADRES->Y0_SET+41) && wiersz<=(ADRES->Y0_SET+50)))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+5)) && ((wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+10) || (wiersz>=(ADRES->Y0_SET+42) && wiersz<=(ADRES->Y0_SET+50)))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+6)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+9) || (wiersz>=(ADRES->Y0_SET+43) && wiersz<=(ADRES->Y0_SET+50)))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+7)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+8) || (wiersz>=(ADRES->Y0_SET+44) && wiersz<=(ADRES->Y0_SET+50)))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+8)) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+7) || (wiersz>=(ADRES->Y0_SET+45) && wiersz<=(ADRES->Y0_SET+50)))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+9)) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+6) || (wiersz>=(ADRES->Y0_SET+46) && wiersz<=(ADRES->Y0_SET+50)))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+22)) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+7) || (wiersz>=(ADRES->Y0_SET+43) && wiersz<=(ADRES->Y0_SET+50)))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+8) || (wiersz>=(ADRES->Y0_SET+22) && wiersz<=(ADRES->Y0_SET+28)) || (wiersz>=(ADRES->Y0_SET+42) && wiersz<=(ADRES->Y0_SET+50)))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+20)) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+5) || (wiersz>=(ADRES->Y0_SET+45) && wiersz<=(ADRES->Y0_SET+50)))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+21)) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+6) || (wiersz>=(ADRES->Y0_SET+44) && wiersz<=(ADRES->Y0_SET+50)))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+24)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+9) || (wiersz>=(ADRES->Y0_SET+21) && wiersz<=(ADRES->Y0_SET+29)) || (wiersz>=(ADRES->Y0_SET+41) && wiersz<=(ADRES->Y0_SET+50)))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+25)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+50))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+26)) && ((wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+24) || (wiersz>=(ADRES->Y0_SET+26) && wiersz<=(ADRES->Y0_SET+49)))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+27)) && ((wiersz>=(ADRES->Y0_SET+4) && wiersz<=(ADRES->Y0_SET+23) || (wiersz>=(ADRES->Y0_SET+27) && wiersz<=(ADRES->Y0_SET+48)))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+5) && wiersz<=(ADRES->Y0_SET+22) || (wiersz>=(ADRES->Y0_SET+28) && wiersz<=(ADRES->Y0_SET+48)))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+6) && wiersz<=(ADRES->Y0_SET+23) || (wiersz>=(ADRES->Y0_SET+27) && wiersz<=(ADRES->Y0_SET+47)))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+30)) && ((wiersz>=(ADRES->Y0_SET+7) && wiersz<=(ADRES->Y0_SET+20) || (wiersz>=(ADRES->Y0_SET+30) && wiersz<=(ADRES->Y0_SET+46)))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();
}

void LCD_print4H(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{
			if((kolumna>=(ADRES->X0_SET+27) && kolumna<=(ADRES->X0_SET+30)) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+50))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+0) && kolumna<=(ADRES->X0_SET+30)) && ((wiersz>=(ADRES->Y0_SET+24) && wiersz<=(ADRES->Y0_SET+27))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+21) && kolumna<=(ADRES->X0_SET+26)) && ((wiersz>=(ADRES->Y0_SET+48) && wiersz<=(ADRES->Y0_SET+50))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+2)) && ((wiersz==(ADRES->Y0_SET+23))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+22) && wiersz<=(ADRES->Y0_SET+23))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+4)) && ((wiersz>=(ADRES->Y0_SET+21) && wiersz<=(ADRES->Y0_SET+23))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+5)) && ((wiersz>=(ADRES->Y0_SET+20) && wiersz<=(ADRES->Y0_SET+23))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+6)) && ((wiersz>=(ADRES->Y0_SET+19) && wiersz<=(ADRES->Y0_SET+23))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+7)) && ((wiersz>=(ADRES->Y0_SET+18) && wiersz<=(ADRES->Y0_SET+22))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+8)) && ((wiersz>=(ADRES->Y0_SET+17) && wiersz<=(ADRES->Y0_SET+21))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+9)) && ((wiersz>=(ADRES->Y0_SET+16) && wiersz<=(ADRES->Y0_SET+20))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+10)) && ((wiersz>=(ADRES->Y0_SET+15) && wiersz<=(ADRES->Y0_SET+19))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+11)) && ((wiersz>=(ADRES->Y0_SET+14) && wiersz<=(ADRES->Y0_SET+18))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+12)) && ((wiersz>=(ADRES->Y0_SET+13) && wiersz<=(ADRES->Y0_SET+17))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+13)) && ((wiersz>=(ADRES->Y0_SET+12) && wiersz<=(ADRES->Y0_SET+16))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+14)) && ((wiersz>=(ADRES->Y0_SET+11) && wiersz<=(ADRES->Y0_SET+15))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+15)) && ((wiersz>=(ADRES->Y0_SET+10) && wiersz<=(ADRES->Y0_SET+14))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+16)) && ((wiersz>=(ADRES->Y0_SET+9) && wiersz<=(ADRES->Y0_SET+13))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+17)) && ((wiersz>=(ADRES->Y0_SET+8) && wiersz<=(ADRES->Y0_SET+12))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+18)) && ((wiersz>=(ADRES->Y0_SET+7) && wiersz<=(ADRES->Y0_SET+11))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+19)) && ((wiersz>=(ADRES->Y0_SET+6) && wiersz<=(ADRES->Y0_SET+10))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+20)) && ((wiersz>=(ADRES->Y0_SET+5) && wiersz<=(ADRES->Y0_SET+9))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+21)) && ((wiersz>=(ADRES->Y0_SET+4) && wiersz<=(ADRES->Y0_SET+8))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+22)) && ((wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+7))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+6))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+24)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+5))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+25)) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+4))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+26)) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+3))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();
}

void LCD_print5H(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{
			if((kolumna>=(ADRES->X0_SET+2) && kolumna<=(ADRES->X0_SET+30)) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+3))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+2) && kolumna<=(ADRES->X0_SET+5)) && ((wiersz>=(ADRES->Y0_SET+4) && wiersz<=(ADRES->Y0_SET+21))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+2) && kolumna<=(ADRES->X0_SET+18)) && ((wiersz>=(ADRES->Y0_SET+22) && wiersz<=(ADRES->Y0_SET+25))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+27) && kolumna<=(ADRES->X0_SET+30)) && ((wiersz>=(ADRES->Y0_SET+34) && wiersz<=(ADRES->Y0_SET+39))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+2) && kolumna<=(ADRES->X0_SET+19)) && ((wiersz>=(ADRES->Y0_SET+47) && wiersz<=(ADRES->Y0_SET+50))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+19)) && ((wiersz>=(ADRES->Y0_SET+22) && wiersz<=(ADRES->Y0_SET+26))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+21)) && ((wiersz>=(ADRES->Y0_SET+24) && wiersz<=(ADRES->Y0_SET+28)) || (wiersz>=(ADRES->Y0_SET+46) && wiersz<=(ADRES->Y0_SET+49))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+22)) && ((wiersz>=(ADRES->Y0_SET+25) && wiersz<=(ADRES->Y0_SET+29)) || (wiersz>=(ADRES->Y0_SET+45) && wiersz<=(ADRES->Y0_SET+48))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+26) && wiersz<=(ADRES->Y0_SET+30)) || (wiersz>=(ADRES->Y0_SET+44) && wiersz<=(ADRES->Y0_SET+47))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+24)) && ((wiersz>=(ADRES->Y0_SET+27) && wiersz<=(ADRES->Y0_SET+31)) || (wiersz>=(ADRES->Y0_SET+43) && wiersz<=(ADRES->Y0_SET+46))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+25)) && ((wiersz>=(ADRES->Y0_SET+28) && wiersz<=(ADRES->Y0_SET+32)) || (wiersz>=(ADRES->Y0_SET+42) && wiersz<=(ADRES->Y0_SET+45))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+26)) && ((wiersz>=(ADRES->Y0_SET+29) && wiersz<=(ADRES->Y0_SET+33)) || (wiersz>=(ADRES->Y0_SET+41) && wiersz<=(ADRES->Y0_SET+44))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+27)) && ((wiersz>=(ADRES->Y0_SET+30) && wiersz<=(ADRES->Y0_SET+34)) || (wiersz>=(ADRES->Y0_SET+40) && wiersz<=(ADRES->Y0_SET+42))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+20)) && ((wiersz>=(ADRES->Y0_SET+23) && wiersz<=(ADRES->Y0_SET+27)) || (wiersz>=(ADRES->Y0_SET+46) && wiersz<=(ADRES->Y0_SET+49))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+32) && wiersz<=(ADRES->Y0_SET+41))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+33) && wiersz<=(ADRES->Y0_SET+40))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+30)) && ((wiersz>=(ADRES->Y0_SET+34) && wiersz<=(ADRES->Y0_SET+39))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();
}

void LCD_print6H(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{
			if((kolumna>=(ADRES->X0_SET+16) && kolumna<=(ADRES->X0_SET+30)) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+3))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+2) && kolumna<=(ADRES->X0_SET+6)) && ((wiersz>=(ADRES->Y0_SET+14) && wiersz<=(ADRES->Y0_SET+44))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+14) && kolumna<=(ADRES->X0_SET+22)) && ((wiersz>=(ADRES->Y0_SET+23) && wiersz<=(ADRES->Y0_SET+26))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+27) && kolumna<=(ADRES->X0_SET+30)) && ((wiersz>=(ADRES->Y0_SET+31) && wiersz<=(ADRES->Y0_SET+43))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+10) && kolumna<=(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+47) && wiersz<=(ADRES->Y0_SET+50))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+19)) && ((wiersz>=(ADRES->Y0_SET+22) && wiersz<=(ADRES->Y0_SET+26))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+13) && wiersz<=(ADRES->Y0_SET+45))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+4)) && ((wiersz>=(ADRES->Y0_SET+12) && wiersz<=(ADRES->Y0_SET+46))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+5)) && ((wiersz>=(ADRES->Y0_SET+11) && wiersz<=(ADRES->Y0_SET+47))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+6)) && ((wiersz>=(ADRES->Y0_SET+10) && wiersz<=(ADRES->Y0_SET+48))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+7)) && ((wiersz>=(ADRES->Y0_SET+9) && wiersz<=(ADRES->Y0_SET+13)) || (wiersz>=(ADRES->Y0_SET+30) && wiersz<=(ADRES->Y0_SET+49))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+8)) && ((wiersz>=(ADRES->Y0_SET+8) && wiersz<=(ADRES->Y0_SET+12)) || (wiersz>=(ADRES->Y0_SET+29) && wiersz<=(ADRES->Y0_SET+32))|| (wiersz>=(ADRES->Y0_SET+46) && wiersz<=(ADRES->Y0_SET+49))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+9)) && ((wiersz>=(ADRES->Y0_SET+7) && wiersz<=(ADRES->Y0_SET+11)) || (wiersz>=(ADRES->Y0_SET+28) && wiersz<=(ADRES->Y0_SET+31))|| (wiersz>=(ADRES->Y0_SET+47) && wiersz<=(ADRES->Y0_SET+50))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+10)) && ((wiersz>=(ADRES->Y0_SET+6) && wiersz<=(ADRES->Y0_SET+10)) || (wiersz>=(ADRES->Y0_SET+27) && wiersz<=(ADRES->Y0_SET+30))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+11)) && ((wiersz>=(ADRES->Y0_SET+5) && wiersz<=(ADRES->Y0_SET+9)) || (wiersz>=(ADRES->Y0_SET+26) && wiersz<=(ADRES->Y0_SET+29))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+12)) && ((wiersz>=(ADRES->Y0_SET+4) && wiersz<=(ADRES->Y0_SET+8)) || (wiersz>=(ADRES->Y0_SET+25) && wiersz<=(ADRES->Y0_SET+28))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+13)) && ((wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+7)) || (wiersz>=(ADRES->Y0_SET+24) && wiersz<=(ADRES->Y0_SET+27))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+14)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+6)) || (wiersz>=(ADRES->Y0_SET+23) && wiersz<=(ADRES->Y0_SET+26))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+15)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+5)) || (wiersz>=(ADRES->Y0_SET+22) && wiersz<=(ADRES->Y0_SET+25))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+16)) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+4))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+23) && wiersz<=(ADRES->Y0_SET+27))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+24)) && ((wiersz>=(ADRES->Y0_SET+24) && wiersz<=(ADRES->Y0_SET+28)) || (wiersz>=(ADRES->Y0_SET+46) && wiersz<=(ADRES->Y0_SET+50))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+25)) && ((wiersz>=(ADRES->Y0_SET+25) && wiersz<=(ADRES->Y0_SET+29)) || (wiersz>=(ADRES->Y0_SET+45) && wiersz<=(ADRES->Y0_SET+49))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+26)) && ((wiersz>=(ADRES->Y0_SET+26) && wiersz<=(ADRES->Y0_SET+30)) || (wiersz>=(ADRES->Y0_SET+44) && wiersz<=(ADRES->Y0_SET+48))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+27)) && ((wiersz>=(ADRES->Y0_SET+27) && wiersz<=(ADRES->Y0_SET+31)) || (wiersz>=(ADRES->Y0_SET+43) && wiersz<=(ADRES->Y0_SET+47))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+28) && wiersz<=(ADRES->Y0_SET+32)) || (wiersz>=(ADRES->Y0_SET+42) && wiersz<=(ADRES->Y0_SET+46))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+29) && wiersz<=(ADRES->Y0_SET+33)) || (wiersz>=(ADRES->Y0_SET+41) && wiersz<=(ADRES->Y0_SET+45))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+30)) && ((wiersz>=(ADRES->Y0_SET+30) && wiersz<=(ADRES->Y0_SET+34)) || (wiersz>=(ADRES->Y0_SET+40) && wiersz<=(ADRES->Y0_SET+44))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();
}

void LCD_print7H(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{
			if((kolumna>=(ADRES->X0_SET+2) && kolumna<=(ADRES->X0_SET+30)) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+4))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+27) && kolumna<=(ADRES->X0_SET+30)) && ((wiersz>=(ADRES->Y0_SET+5) && wiersz<=(ADRES->Y0_SET+22))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+2)) && ((wiersz>=(ADRES->Y0_SET+47) && wiersz<=(ADRES->Y0_SET+50))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+46) && wiersz<=(ADRES->Y0_SET+50))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+4)) && ((wiersz>=(ADRES->Y0_SET+45) && wiersz<=(ADRES->Y0_SET+50))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+5)) && ((wiersz>=(ADRES->Y0_SET+44) && wiersz<=(ADRES->Y0_SET+49))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+6)) && ((wiersz>=(ADRES->Y0_SET+43) && wiersz<=(ADRES->Y0_SET+48))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+7)) && ((wiersz>=(ADRES->Y0_SET+42) && wiersz<=(ADRES->Y0_SET+47))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+8)) && ((wiersz>=(ADRES->Y0_SET+41) && wiersz<=(ADRES->Y0_SET+46))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+9)) && ((wiersz>=(ADRES->Y0_SET+40) && wiersz<=(ADRES->Y0_SET+45))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+10)) && ((wiersz>=(ADRES->Y0_SET+39) && wiersz<=(ADRES->Y0_SET+44))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+11)) && ((wiersz>=(ADRES->Y0_SET+37) && wiersz<=(ADRES->Y0_SET+43))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+12)) && ((wiersz>=(ADRES->Y0_SET+36) && wiersz<=(ADRES->Y0_SET+42))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+13)) && ((wiersz>=(ADRES->Y0_SET+35) && wiersz<=(ADRES->Y0_SET+41))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+14)) && ((wiersz>=(ADRES->Y0_SET+34) && wiersz<=(ADRES->Y0_SET+40))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+15)) && ((wiersz>=(ADRES->Y0_SET+33) && wiersz<=(ADRES->Y0_SET+39))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+16)) && ((wiersz>=(ADRES->Y0_SET+32) && wiersz<=(ADRES->Y0_SET+38))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+17)) && ((wiersz>=(ADRES->Y0_SET+31) && wiersz<=(ADRES->Y0_SET+37))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+18)) && ((wiersz>=(ADRES->Y0_SET+30) && wiersz<=(ADRES->Y0_SET+36))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+19)) && ((wiersz>=(ADRES->Y0_SET+29) && wiersz<=(ADRES->Y0_SET+35))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+20)) && ((wiersz>=(ADRES->Y0_SET+28) && wiersz<=(ADRES->Y0_SET+34))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+21)) && ((wiersz>=(ADRES->Y0_SET+27) && wiersz<=(ADRES->Y0_SET+33))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+22)) && ((wiersz>=(ADRES->Y0_SET+26) && wiersz<=(ADRES->Y0_SET+32))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+25) && wiersz<=(ADRES->Y0_SET+31))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+24)) && ((wiersz>=(ADRES->Y0_SET+24) && wiersz<=(ADRES->Y0_SET+30))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+25)) && ((wiersz>=(ADRES->Y0_SET+23) && wiersz<=(ADRES->Y0_SET+29))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+26)) && ((wiersz>=(ADRES->Y0_SET+22) && wiersz<=(ADRES->Y0_SET+28))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+27)) && ((wiersz>=(ADRES->Y0_SET+21) && wiersz<=(ADRES->Y0_SET+27))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+20) && wiersz<=(ADRES->Y0_SET+26))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+19) && wiersz<=(ADRES->Y0_SET+25))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+30)) && ((wiersz>=(ADRES->Y0_SET+18) && wiersz<=(ADRES->Y0_SET+24))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();
}

void LCD_print0H(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{
			if((kolumna>=(ADRES->X0_SET+11) && kolumna<=(ADRES->X0_SET+21)) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+4))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+11) && kolumna<=(ADRES->X0_SET+21)) && ((wiersz>=(ADRES->Y0_SET+46) && wiersz<=(ADRES->Y0_SET+50))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+2) && kolumna<=(ADRES->X0_SET+5)) && ((wiersz>=(ADRES->Y0_SET+10) && wiersz<=(ADRES->Y0_SET+40))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+27) && kolumna<=(ADRES->X0_SET+30)) && ((wiersz>=(ADRES->Y0_SET+10) && wiersz<=(ADRES->Y0_SET+40))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+2)) && ((wiersz>=(ADRES->Y0_SET+9) && wiersz<=(ADRES->Y0_SET+41))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+8) && wiersz<=(ADRES->Y0_SET+9)) || (wiersz>=(ADRES->Y0_SET+41) && wiersz<=(ADRES->Y0_SET+42))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+4)) && ((wiersz>=(ADRES->Y0_SET+7) && wiersz<=(ADRES->Y0_SET+9)) || (wiersz>=(ADRES->Y0_SET+41) && wiersz<=(ADRES->Y0_SET+43))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+5)) && ((wiersz>=(ADRES->Y0_SET+6) && wiersz<=(ADRES->Y0_SET+9)) || (wiersz>=(ADRES->Y0_SET+41) && wiersz<=(ADRES->Y0_SET+44))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+6)) && ((wiersz>=(ADRES->Y0_SET+5) && wiersz<=(ADRES->Y0_SET+9)) || (wiersz>=(ADRES->Y0_SET+41) && wiersz<=(ADRES->Y0_SET+45))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+7)) && ((wiersz>=(ADRES->Y0_SET+4) && wiersz<=(ADRES->Y0_SET+8)) || (wiersz>=(ADRES->Y0_SET+42) && wiersz<=(ADRES->Y0_SET+46))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+8)) && ((wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+7)) || (wiersz>=(ADRES->Y0_SET+43) && wiersz<=(ADRES->Y0_SET+47))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+9)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+6)) || (wiersz>=(ADRES->Y0_SET+44) && wiersz<=(ADRES->Y0_SET+48))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+10)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+5)) || (wiersz>=(ADRES->Y0_SET+45) && wiersz<=(ADRES->Y0_SET+49))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+22)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+5)) || (wiersz>=(ADRES->Y0_SET+45) && wiersz<=(ADRES->Y0_SET+49))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+6)) || (wiersz>=(ADRES->Y0_SET+44) && wiersz<=(ADRES->Y0_SET+48))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+24)) && ((wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+7)) || (wiersz>=(ADRES->Y0_SET+43) && wiersz<=(ADRES->Y0_SET+47))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+25)) && ((wiersz>=(ADRES->Y0_SET+4) && wiersz<=(ADRES->Y0_SET+8)) || (wiersz>=(ADRES->Y0_SET+42) && wiersz<=(ADRES->Y0_SET+46))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+26)) && ((wiersz>=(ADRES->Y0_SET+5) && wiersz<=(ADRES->Y0_SET+9)) || (wiersz>=(ADRES->Y0_SET+41) && wiersz<=(ADRES->Y0_SET+45))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+27)) && ((wiersz>=(ADRES->Y0_SET+6) && wiersz<=(ADRES->Y0_SET+9)) || (wiersz>=(ADRES->Y0_SET+41) && wiersz<=(ADRES->Y0_SET+44))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+7) && wiersz<=(ADRES->Y0_SET+9)) || (wiersz>=(ADRES->Y0_SET+41) && wiersz<=(ADRES->Y0_SET+43))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+8) && wiersz<=(ADRES->Y0_SET+9)) || (wiersz>=(ADRES->Y0_SET+41) && wiersz<=(ADRES->Y0_SET+42))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+30)) && ((wiersz==(ADRES->Y0_SET+9) || wiersz==(ADRES->Y0_SET+41))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();
}

void LCD_print9H(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{
			if((kolumna>=(ADRES->X0_SET+10) && kolumna<=(ADRES->X0_SET+21)) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+4))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+10) && kolumna<=(ADRES->X0_SET+21)) && ((wiersz>=(ADRES->Y0_SET+21) && wiersz<=(ADRES->Y0_SET+24))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+2) && kolumna<=(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+47) && wiersz<=(ADRES->Y0_SET+50))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+25) && kolumna<=(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+8) && wiersz<=(ADRES->Y0_SET+45))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+2) && kolumna<=(ADRES->X0_SET+5)) && ((wiersz>=(ADRES->Y0_SET+8) && wiersz<=(ADRES->Y0_SET+17))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+7) && wiersz<=(ADRES->Y0_SET+18))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+4)) && ((wiersz>=(ADRES->Y0_SET+6) && wiersz<=(ADRES->Y0_SET+19))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+5)) && ((wiersz>=(ADRES->Y0_SET+5) && wiersz<=(ADRES->Y0_SET+20))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+6)) && ((wiersz>=(ADRES->Y0_SET+4) && wiersz<=(ADRES->Y0_SET+7)) || (wiersz>=(ADRES->Y0_SET+18) && wiersz<=(ADRES->Y0_SET+21))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+7)) && ((wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+6)) || (wiersz>=(ADRES->Y0_SET+19) && wiersz<=(ADRES->Y0_SET+22))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+8)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+5)) || (wiersz>=(ADRES->Y0_SET+20) && wiersz<=(ADRES->Y0_SET+23))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+9)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+4)) || (wiersz>=(ADRES->Y0_SET+21) && wiersz<=(ADRES->Y0_SET+24))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+22)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+4)) || (wiersz>=(ADRES->Y0_SET+20) && wiersz<=(ADRES->Y0_SET+24))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+5)) || (wiersz>=(ADRES->Y0_SET+19) && wiersz<=(ADRES->Y0_SET+23))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+24)) && ((wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+6)) || (wiersz>=(ADRES->Y0_SET+18) && wiersz<=(ADRES->Y0_SET+22))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+24)) && ((wiersz>=(ADRES->Y0_SET+46) && wiersz<=(ADRES->Y0_SET+50))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+25)) && ((wiersz>=(ADRES->Y0_SET+4) && wiersz<=(ADRES->Y0_SET+7)) || (wiersz>=(ADRES->Y0_SET+17) && wiersz<=(ADRES->Y0_SET+21))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+25)) && ((wiersz>=(ADRES->Y0_SET+45) && wiersz<=(ADRES->Y0_SET+49))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+26)) && ((wiersz>=(ADRES->Y0_SET+5) && wiersz<=(ADRES->Y0_SET+7)) || (wiersz>=(ADRES->Y0_SET+17) && wiersz<=(ADRES->Y0_SET+20))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+26)) && ((wiersz>=(ADRES->Y0_SET+44) && wiersz<=(ADRES->Y0_SET+48))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+27)) && ((wiersz>=(ADRES->Y0_SET+6) && wiersz<=(ADRES->Y0_SET+19)) || (wiersz>=(ADRES->Y0_SET+29) && wiersz<=(ADRES->Y0_SET+45))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+27)) && ((wiersz>=(ADRES->Y0_SET+43) && wiersz<=(ADRES->Y0_SET+47))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+7) && wiersz<=(ADRES->Y0_SET+18)) || (wiersz>=(ADRES->Y0_SET+30) && wiersz<=(ADRES->Y0_SET+44))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+8) && wiersz<=(ADRES->Y0_SET+17)) || (wiersz>=(ADRES->Y0_SET+31) && wiersz<=(ADRES->Y0_SET+43))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}
		}
	}
	CS_ON();
}

void LCD_print_segment(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES,unsigned int num)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	ADRES->RM=12;
	ADRES->GM=18;
	ADRES->BM=0;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword2[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	switch(num)
	{
		case 0:
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+9) && kolumna<=(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+2))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+26) && kolumna<=(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+23))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+26) && kolumna<=(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+27) && wiersz<=(ADRES->Y0_SET+47))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+9) && kolumna<=(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+48) && wiersz<=(ADRES->Y0_SET+50))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+6)) && ((wiersz>=(ADRES->Y0_SET+27) && wiersz<=(ADRES->Y0_SET+47))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+6)) && ((wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+23))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+9) && kolumna<=(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+24) && wiersz<=(ADRES->Y0_SET+26))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
		break;
		case 1:
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+9) && kolumna<=(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+2))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+26) && kolumna<=(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+23))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+26) && kolumna<=(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+27) && wiersz<=(ADRES->Y0_SET+47))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+9) && kolumna<=(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+48) && wiersz<=(ADRES->Y0_SET+50))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+6)) && ((wiersz>=(ADRES->Y0_SET+27) && wiersz<=(ADRES->Y0_SET+47))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+6)) && ((wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+23))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+9) && kolumna<=(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+24) && wiersz<=(ADRES->Y0_SET+26))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
			break;
		case 2:
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+9) && kolumna<=(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+2))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+26) && kolumna<=(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+23))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+26) && kolumna<=(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+27) && wiersz<=(ADRES->Y0_SET+47))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+9) && kolumna<=(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+48) && wiersz<=(ADRES->Y0_SET+50))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+6)) && ((wiersz>=(ADRES->Y0_SET+27) && wiersz<=(ADRES->Y0_SET+47))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+6)) && ((wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+23))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+9) && kolumna<=(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+24) && wiersz<=(ADRES->Y0_SET+26))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
		break;
		case 3:
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+9) && kolumna<=(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+2))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+26) && kolumna<=(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+23))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+26) && kolumna<=(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+27) && wiersz<=(ADRES->Y0_SET+47))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+9) && kolumna<=(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+48) && wiersz<=(ADRES->Y0_SET+50))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+6)) && ((wiersz>=(ADRES->Y0_SET+27) && wiersz<=(ADRES->Y0_SET+47))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+6)) && ((wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+23))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+9) && kolumna<=(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+24) && wiersz<=(ADRES->Y0_SET+26))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
		break;
		case 4:
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+9) && kolumna<=(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+2))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+26) && kolumna<=(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+23))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+26) && kolumna<=(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+27) && wiersz<=(ADRES->Y0_SET+47))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+9) && kolumna<=(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+48) && wiersz<=(ADRES->Y0_SET+50))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+6)) && ((wiersz>=(ADRES->Y0_SET+27) && wiersz<=(ADRES->Y0_SET+47))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+6)) && ((wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+23))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+9) && kolumna<=(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+24) && wiersz<=(ADRES->Y0_SET+26))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
		break;
		case 5:
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+9) && kolumna<=(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+2))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+26) && kolumna<=(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+23))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+26) && kolumna<=(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+27) && wiersz<=(ADRES->Y0_SET+47))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+9) && kolumna<=(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+48) && wiersz<=(ADRES->Y0_SET+50))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+6)) && ((wiersz>=(ADRES->Y0_SET+27) && wiersz<=(ADRES->Y0_SET+47))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+6)) && ((wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+23))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+9) && kolumna<=(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+24) && wiersz<=(ADRES->Y0_SET+26))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
		break;
		case 6:
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+9) && kolumna<=(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+2))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+26) && kolumna<=(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+23))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+26) && kolumna<=(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+27) && wiersz<=(ADRES->Y0_SET+47))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+9) && kolumna<=(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+48) && wiersz<=(ADRES->Y0_SET+50))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+6)) && ((wiersz>=(ADRES->Y0_SET+27) && wiersz<=(ADRES->Y0_SET+47))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+6)) && ((wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+23))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+9) && kolumna<=(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+24) && wiersz<=(ADRES->Y0_SET+26))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
		break;
		case 7:
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+9) && kolumna<=(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+2))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+26) && kolumna<=(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+23))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+26) && kolumna<=(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+27) && wiersz<=(ADRES->Y0_SET+47))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+9) && kolumna<=(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+48) && wiersz<=(ADRES->Y0_SET+50))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+6)) && ((wiersz>=(ADRES->Y0_SET+27) && wiersz<=(ADRES->Y0_SET+47))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+6)) && ((wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+23))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+9) && kolumna<=(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+24) && wiersz<=(ADRES->Y0_SET+26))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
		break;
		case 8:
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+9) && kolumna<=(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+2))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+26) && kolumna<=(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+23))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+26) && kolumna<=(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+27) && wiersz<=(ADRES->Y0_SET+47))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+9) && kolumna<=(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+48) && wiersz<=(ADRES->Y0_SET+50))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+6)) && ((wiersz>=(ADRES->Y0_SET+27) && wiersz<=(ADRES->Y0_SET+47))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+6)) && ((wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+23))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+9) && kolumna<=(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+24) && wiersz<=(ADRES->Y0_SET+26))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
		break;
		case 9:
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+9) && kolumna<=(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+0) && wiersz<=(ADRES->Y0_SET+2))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+26) && kolumna<=(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+23))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+26) && kolumna<=(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+27) && wiersz<=(ADRES->Y0_SET+47))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+9) && kolumna<=(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+48) && wiersz<=(ADRES->Y0_SET+50))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+6)) && ((wiersz>=(ADRES->Y0_SET+27) && wiersz<=(ADRES->Y0_SET+47))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+6)) && ((wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+23))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+9) && kolumna<=(ADRES->X0_SET+23)) && ((wiersz>=(ADRES->Y0_SET+24) && wiersz<=(ADRES->Y0_SET+26))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
		break;
		default: break;
	}
	CS_ON();
}

void LCD_print_tank(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES,float num)
{
	unsigned int kolumna=0,wiersz=0;
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	ADRES->RM=31;
	ADRES->GM=0;
	ADRES->BM=0;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword2[]={ADRES->Mbyte,ADRES->Mbyte2};
	ADRES->RM=12;
	ADRES->GM=18;
	ADRES->BM=0;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t Mword3[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	if(num<0.1)
	{
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+1)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+66)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+65) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword3, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+64))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
	}
	else if(num>=0.1 && num<0.2)
	{
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+1)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+66)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+63) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword3, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+62))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
	}
	else if(num>=0.2 && num<0.3)
	{
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+1)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+66)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+61) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword3, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+60))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
	}
	else if(num>=0.3 && num<0.4)
	{
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+1)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+66)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+59) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword3, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+58))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
	}
	else if(num>=0.4 && num<0.5)
	{
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+1)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+66)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+57) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword3, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+56))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
	}
	else if(num>=0.5 && num<0.6)
	{
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+1)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+66)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+55) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword3, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+54))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
	}
	else if(num>=0.6 && num<0.7)
	{
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+1)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+66)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+53) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword3, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+52))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
	}
	else if(num>=0.7 && num<0.8)
	{
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+1)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+66)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+51) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword3, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+50))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
	}
	else if(num>=0.8 && num<0.9)
	{
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+1)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+66)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+49) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword3, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+48))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
	}
	else if(num>=0.9 && num<1.0)
	{
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+1)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+66)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+47) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword3, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+46))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
	}
	else if(num>=1.0 && num<1.1)
	{
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+1)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+66)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+45) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword3, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+44))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
	}
	else if(num>=1.1 && num<1.2)
	{
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+1)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+66)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+43) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword3, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+42))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
	}
	else if(num>=1.2 && num<1.3)
	{
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+1)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+66)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+41) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword3, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+40))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
	}
	else if(num>=1.3 && num<1.4)
	{
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+1)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+66)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+39) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword3, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+38))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
	}
	else if(num>=1.4 && num<1.5)
	{
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+1)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+66)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+37) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword3, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+36))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
	}
	else if(num>=1.5 && num<1.6)
	{
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+1)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+66)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+35) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword3, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+34))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
	}
	else if(num>=1.6 && num<1.7)
	{
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+1)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+66)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+33) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword3, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+32))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
	}
	else if(num>=1.7 && num<1.8)
	{
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+1)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+66)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+31) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword3, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+30))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
	}
	else if(num>=1.8 && num<1.9)
	{
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+1)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+66)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+29) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword3, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+28))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
	}
	else if(num>=1.9 && num<2.0)
	{
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+1)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+66)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+27) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword3, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+26))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
	}
	else if(num>=2.0 && num<2.1)
	{
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+1)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+66)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+25) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword3, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+24))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
	}
	else if(num>=2.1 && num<2.2)
	{
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+1)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+66)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+23) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword3, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+22))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
	}
	else if(num>=2.2 && num<2.3)
	{
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+1)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+66)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+21) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword3, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+20))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
	}
	else if(num>=2.3 && num<2.4)
	{
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+1)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+66)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+19) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword3, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+18))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
	}
	else if(num>=2.4 && num<2.5)
	{
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+1)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+66)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+17) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword3, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+16))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
	}
	else if(num>=2.5 && num<2.6)
	{
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+1)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+66)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+15) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword3, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+14))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
	}
	else if(num>=2.6 && num<2.7)
	{
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+1)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+66)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+15) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword3, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+14))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
	}
	else if(num>=2.7 && num<2.8)
	{
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+1)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+66)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+13) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword3, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+12))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
	}
	else if(num>=2.8 && num<2.9)
	{
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+1)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+66)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+11) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword3, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+10))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
	}
	else if(num>=2.9 && num<3.0)
	{
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+1)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+66)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+9) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword3, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+8))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
	}
	else if(num>=3.0 && num<3.1)
	{
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+1)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+66)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+7) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword3, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+6))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
	}
	else if(num>=3.1 && num<3.2)
	{
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+1)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+66)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+5) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword3, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+4))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
	}
	else if(num>=3.2 && num<3.3)
	{
		for(wiersz=ADRES->Y0_SET;wiersz<=(ADRES->Y_END_SET);wiersz++)
		{
			for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
			{
				if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+1)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+3) && kolumna<=(ADRES->X0_SET+29)) && (wiersz==(ADRES->Y0_SET+66)))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+29)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna==(ADRES->X0_SET+3)) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+66))))
				{
					HAL_SPI_Transmit(&hspi3, Mword3, 2, HAL_MAX_DELAY);
				}
				else if((kolumna>=(ADRES->X0_SET+4) && kolumna<=(ADRES->X0_SET+28)) && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+2))))
				{
					HAL_SPI_Transmit(&hspi3, Mword2, 2, HAL_MAX_DELAY);
				}
				else
				{
					HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
				}
			}
		}
	}
	CS_ON();
}



void LCD_printdioda(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES, bool on)
{
	unsigned int kolumna=0,wiersz=0;
	if(on)
	{
		ADRES->RM=0;
		ADRES->GM=14;
		ADRES->BM=0;
		ADRES->Mbyte=0x00;
		ADRES->Mbyte2=0x00;
		ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
		ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	}
	else
	{
		ADRES->RM=17;
		ADRES->GM=0;
		ADRES->BM=0;
		ADRES->Mbyte=0x00;
		ADRES->Mbyte2=0x00;
		ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
		ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	}
	uint8_t Mword[]={ADRES->Mbyte,ADRES->Mbyte2};
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	DC_ON();
	CS_OFF();
	for(wiersz=ADRES->Y0_SET;wiersz<(ADRES->Y_END_SET);wiersz++)
	{
		for(kolumna=ADRES->X0_SET;kolumna<(ADRES->X_END_SET+1);kolumna++)
		{
			if(kolumna==(ADRES->X0_SET+2) && ((wiersz>=(ADRES->Y0_SET+5) && wiersz<=(ADRES->Y0_SET+17))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+20) && ((wiersz>=(ADRES->Y0_SET+5) && wiersz<=(ADRES->Y0_SET+17))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+2) && kolumna<=(ADRES->X0_SET+20)) && wiersz==(ADRES->Y0_SET+17))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna>=(ADRES->X0_SET+7) && kolumna<=(ADRES->X0_SET+15)) && wiersz==(ADRES->Y0_SET+0))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+6) || kolumna==(ADRES->X0_SET+16)) && (wiersz==(ADRES->Y0_SET+1)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+5) || kolumna==(ADRES->X0_SET+17)) && (wiersz==(ADRES->Y0_SET+2)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+4) || kolumna==(ADRES->X0_SET+18)) && (wiersz==(ADRES->Y0_SET+3)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+3) || kolumna==(ADRES->X0_SET+19)) && (wiersz==(ADRES->Y0_SET+4)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+7)) && (wiersz>=(ADRES->Y0_SET+18) && wiersz<=(ADRES->Y0_SET+20)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+16)) && (wiersz>=(ADRES->Y0_SET+18) && wiersz<=(ADRES->Y0_SET+20)))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+3))  && ((wiersz>=(ADRES->Y0_SET+5) && wiersz<=(ADRES->Y0_SET+16))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+4))  && ((wiersz>=(ADRES->Y0_SET+4) && wiersz<=(ADRES->Y0_SET+16))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+5))  && ((wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+16))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+6))  && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+16))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+7))  && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+16))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+8))  && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+16))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+9))  && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+16))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+10))  && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+16))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+19))  && ((wiersz>=(ADRES->Y0_SET+5) && wiersz<=(ADRES->Y0_SET+16))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+18))  && ((wiersz>=(ADRES->Y0_SET+4) && wiersz<=(ADRES->Y0_SET+16))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+17))  && ((wiersz>=(ADRES->Y0_SET+3) && wiersz<=(ADRES->Y0_SET+16))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+16))  && ((wiersz>=(ADRES->Y0_SET+2) && wiersz<=(ADRES->Y0_SET+16))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+15))  && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+16))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+14))  && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+16))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+13))  && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+16))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if((kolumna==(ADRES->X0_SET+12))  && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+16))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else if(kolumna==(ADRES->X0_SET+11) && ((wiersz>=(ADRES->Y0_SET+1) && wiersz<=(ADRES->Y0_SET+16))))
			{
				HAL_SPI_Transmit(&hspi3, Mword, 2, HAL_MAX_DELAY);
			}
			else
			{
				HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
			}


		}
	}
	CS_ON();
}

void LCD_printMENU(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	ADRES->RM=0;
	ADRES->GM=63;
	ADRES->BM=31;
	ADRES->Mbyte=0x00;
	ADRES->Mbyte2=0x00;
	ADRES->Mbyte=((ADRES->Mbyte^ADRES->RM)<<3)^(ADRES->GM&0x38);
	ADRES->Mbyte2=((ADRES->Mbyte2^(ADRES->GM&0x07))<<5)^ADRES->BM;
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	//LCD_komenda(ADRES->DISPLAYOFF);
	// MENU
	ADRES->X0_SET=0x00;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	ADRES->Y0_SET=0x00;
	ADRES->Y_END_SET=ADRES->Y0_SET+15;
	LCD_set_adressing(ADRES);
	LCD_printM(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printE(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printN(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printU(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=127;
	LCD_set_adressing(ADRES);
	DC_ON();
	CS_OFF();
	for(int i=0;i<(((ADRES->X_END_SET)-(ADRES->X0_SET-1))*16);i++)
	{
		HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
	}
	CS_ON();



	// 1. L - STAN DIOD
	ADRES->X0_SET=0x00;
	ADRES->X_END_SET=ADRES->X0_SET+20;
	ADRES->Y0_SET=15;
	ADRES->Y_END_SET=ADRES->Y0_SET+15;
	LCD_set_adressing(ADRES);
	LCD_printD(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+13;
	LCD_set_adressing(ADRES);
	LCD_print_myslnik(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printS(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printT(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printA(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+20;
	LCD_set_adressing(ADRES);
	LCD_printN(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+18;
	ADRES->X_END_SET=ADRES->X0_SET+13;
	LCD_set_adressing(ADRES);
	LCD_printD(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printI(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;;
	LCD_set_adressing(ADRES);
	LCD_printO(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=127;
	LCD_set_adressing(ADRES);
	LCD_printD(R,G,B,ADRES);



	// 2. A - STAN ADC1
	ADRES->X0_SET=0x00;
	ADRES->X_END_SET=ADRES->X0_SET+20;
	ADRES->Y0_SET=30;
	ADRES->Y_END_SET=ADRES->Y0_SET+15;
	LCD_set_adressing(ADRES);
	LCD_printA(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+13;
	LCD_set_adressing(ADRES);
	LCD_print_myslnik(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printS(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printT(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printA(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+20;
	LCD_set_adressing(ADRES);
	LCD_printN(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+18;
	ADRES->X_END_SET=ADRES->X0_SET+13;
	LCD_set_adressing(ADRES);
	LCD_printA(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printD(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;;
	LCD_set_adressing(ADRES);
	LCD_printC(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=127;
	LCD_set_adressing(ADRES);
	LCD_print1(R,G,B,ADRES);




	// 3. B - STAN ADC2
	ADRES->X0_SET=0x00;
	ADRES->X_END_SET=ADRES->X0_SET+20;
	ADRES->Y0_SET=45;
	ADRES->Y_END_SET=ADRES->Y0_SET+15;
	LCD_set_adressing(ADRES);
	LCD_printB(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+13;
	LCD_set_adressing(ADRES);
	LCD_print_myslnik(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printS(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printT(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printA(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+20;
	LCD_set_adressing(ADRES);
	LCD_printN(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+18;
	ADRES->X_END_SET=ADRES->X0_SET+13;
	LCD_set_adressing(ADRES);
	LCD_printA(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printD(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;;
	LCD_set_adressing(ADRES);
	LCD_printC(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=127;
	LCD_set_adressing(ADRES);
	LCD_print2(R,G,B,ADRES);




	// 4. S - SEGMENT
	ADRES->X0_SET=0x00;
	ADRES->X_END_SET=ADRES->X0_SET+20;
	ADRES->Y0_SET=60;
	ADRES->Y_END_SET=ADRES->Y0_SET+15;
	LCD_set_adressing(ADRES);
	LCD_printS(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+13;
	LCD_set_adressing(ADRES);
	LCD_print_myslnik(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printS(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printE(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printG(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printM(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printE(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printN(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=127;
	LCD_set_adressing(ADRES);
	LCD_printT(R,G,B,ADRES);



	// 5. L - LINIJKA
	ADRES->X0_SET=0x00;
	ADRES->X_END_SET=ADRES->X0_SET+20;
	ADRES->Y0_SET=75;
	ADRES->Y_END_SET=ADRES->Y0_SET+15;
	LCD_set_adressing(ADRES);
	LCD_printL(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+13;
	LCD_set_adressing(ADRES);
	LCD_print_myslnik(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printL(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printI(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printN(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printI(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printJ(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printK(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=127;
	LCD_set_adressing(ADRES);
	LCD_printA(R,G,B,ADRES);
	ADRES->Y0_SET=90;
	ADRES->Y_END_SET=159;
	ADRES->X0_SET=0;
	ADRES->X_END_SET=127;
	LCD_set_adressing(ADRES);
	DC_ON();
	CS_OFF();
	for(uint32_t i=0;i<((159-90)*127);i++)
	{
		HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
	}
	CS_ON();
	//LCD_komenda(ADRES->DISPLAYON);
}

void LCD_print_tryb_diody(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES)
{
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};

	// 1 dioda
	ADRES->X0_SET=0x00;
	ADRES->X_END_SET=ADRES->X0_SET+20;
	ADRES->Y0_SET=0x00;
	ADRES->Y_END_SET=ADRES->Y0_SET+25;
	LCD_set_adressing(ADRES);
	LCD_print1(1,1,1,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+21;
	ADRES->X_END_SET=127;
	LCD_set_adressing(ADRES);
	if(HAL_GPIO_ReadPin(GPIOC, Czerwony_1_Pin)==SET)
	{
		LCD_printdioda(1,1,1,ADRES,TRUE);
	}
	else
	{
		LCD_printdioda(1,1,1,ADRES,FALSE);
	}

	// 2 dioda
	ADRES->X0_SET=0x00;
	ADRES->X_END_SET=ADRES->X0_SET+20;
	ADRES->Y0_SET=25;
	ADRES->Y_END_SET=ADRES->Y0_SET+25;
	LCD_set_adressing(ADRES);
	LCD_print2(1,1,1,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+21;
	ADRES->X_END_SET=127;
	LCD_set_adressing(ADRES);
	if(HAL_GPIO_ReadPin(GPIOC, Zolty_1_Pin)==SET)
	{
		LCD_printdioda(1,1,1,ADRES,TRUE);
	}
	else
	{
		LCD_printdioda(1,1,1,ADRES,FALSE);
	}

	// 3 dioda
	ADRES->X0_SET=0x00;
	ADRES->X_END_SET=ADRES->X0_SET+20;
	ADRES->Y0_SET=50;
	ADRES->Y_END_SET=ADRES->Y0_SET+25;
	LCD_set_adressing(ADRES);
	LCD_print3(1,1,1,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+21;
	ADRES->X_END_SET=127;
	LCD_set_adressing(ADRES);
	if(HAL_GPIO_ReadPin(GPIOC, Zielony_1_Pin)==SET)
	{
		LCD_printdioda(1,1,1,ADRES,TRUE);
	}
	else
	{
		LCD_printdioda(1,1,1,ADRES,FALSE);
	}

	// 4 dioda
	ADRES->X0_SET=0x00;
	ADRES->X_END_SET=ADRES->X0_SET+20;
	ADRES->Y0_SET=75;
	ADRES->Y_END_SET=ADRES->Y0_SET+25;
	LCD_set_adressing(ADRES);
	LCD_print4(1,1,1,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+21;
	ADRES->X_END_SET=127;
	LCD_set_adressing(ADRES);
	if(HAL_GPIO_ReadPin(GPIOC, Czerwony_2_Pin)==SET)
	{
		LCD_printdioda(1,1,1,ADRES,TRUE);
	}
	else
	{
		LCD_printdioda(1,1,1,ADRES,FALSE);
	}

	// 5 dioda
	ADRES->X0_SET=0x00;
	ADRES->X_END_SET=ADRES->X0_SET+20;
	ADRES->Y0_SET=100;
	ADRES->Y_END_SET=ADRES->Y0_SET+25;
	LCD_set_adressing(ADRES);
	LCD_print5(1,1,1,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+21;
	ADRES->X_END_SET=127;
	LCD_set_adressing(ADRES);
	if(HAL_GPIO_ReadPin(GPIOB, Zolty_2_Pin)==SET)
	{
		LCD_printdioda(1,1,1,ADRES,TRUE);
	}
	else
	{
		LCD_printdioda(1,1,1,ADRES,FALSE);
	}

	// 6 dioda
	ADRES->X0_SET=0x00;
	ADRES->X_END_SET=ADRES->X0_SET+20;
	ADRES->Y0_SET=125;
	ADRES->Y_END_SET=159;
	LCD_set_adressing(ADRES);
	LCD_print6(1,1,1,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+21;
	ADRES->X_END_SET=127;
	LCD_set_adressing(ADRES);
	if(HAL_GPIO_ReadPin(GPIOB, Zielony_2_Pin)==SET)
	{
		LCD_printdioda(1,1,1,ADRES,TRUE);
	}
	else
	{
		LCD_printdioda(1,1,1,ADRES,FALSE);
	}
}

void LCD_print_tryb_adc_numbers(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES, unsigned int num_1, unsigned int num2, bool ADC)
{
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	// 1 dioda
	if(ADC)
	{
		ADRES->X0_SET=0x00;
		ADRES->X_END_SET=ADRES->X0_SET+40;
		ADRES->Y0_SET=0x00;
		ADRES->Y_END_SET=ADRES->Y0_SET+60;
		LCD_set_adressing(ADRES);
		switch(num_1)
		{
		case 0: 	LCD_print0H(1,1,1,ADRES);
					break;
		case 1: 	LCD_print1H(1,1,1,ADRES);
					break;
		case 2: 	LCD_print2H(1,1,1,ADRES);
					break;
		case 3: 	LCD_print3H(1,1,1,ADRES);
					break;
		default:	 break;
		}

		ADRES->X0_SET=ADRES->X_END_SET;
		ADRES->X_END_SET=ADRES->X0_SET+15;
		LCD_set_adressing(ADRES);
		LCD_print_dot(1,1,1,ADRES);
		ADRES->X0_SET=ADRES->X_END_SET;
		ADRES->X_END_SET=127;
		LCD_set_adressing(ADRES);
		switch(num2)
		{
		case 0: 	LCD_print0H(1,1,1,ADRES);
					break;
		case 1: 	LCD_print1H(1,1,1,ADRES);
					break;
		case 2: 	LCD_print2H(1,1,1,ADRES);
					break;
		case 3: 	LCD_print3H(1,1,1,ADRES);
					break;
		case 4: 	LCD_print4H(1,1,1,ADRES);
					break;
		case 5: 	LCD_print5H(1,1,1,ADRES);
					break;
		case 6: 	LCD_print6H(1,1,1,ADRES);
					break;
		case 7: 	LCD_print7H(1,1,1,ADRES);
					break;
		case 8: 	LCD_print8H(1,1,1,ADRES);
					break;
		case 9: 	LCD_print9H(1,1,1,ADRES);
					break;
		default:	 break;
		}


		ADRES->X0_SET=0x00;
		ADRES->X_END_SET=ADRES->X0_SET+11;
		ADRES->Y0_SET=60;
		ADRES->Y_END_SET=ADRES->Y0_SET+20;
		LCD_set_adressing(ADRES);
		LCD_printS(1,1,1,ADRES);
		ADRES->X0_SET=ADRES->X0_SET+11;
		ADRES->X_END_SET=ADRES->X0_SET+10;
		LCD_set_adressing(ADRES);
		LCD_printT(1,1,1,ADRES);
		ADRES->X0_SET=ADRES->X0_SET+11;
		ADRES->X_END_SET=ADRES->X0_SET+10;
		LCD_set_adressing(ADRES);
		LCD_printA(1,1,1,ADRES);
		ADRES->X0_SET=ADRES->X0_SET+11;
		ADRES->X_END_SET=ADRES->X0_SET+20;
		LCD_set_adressing(ADRES);
		LCD_printN(1,1,1,ADRES);
		ADRES->X0_SET=ADRES->X0_SET+18;
		ADRES->X_END_SET=ADRES->X0_SET+13;
		LCD_set_adressing(ADRES);
		LCD_printA(1,1,1,ADRES);
		ADRES->X0_SET=ADRES->X0_SET+11;
		ADRES->X_END_SET=ADRES->X0_SET+10;
		LCD_set_adressing(ADRES);
		LCD_printD(1,1,1,ADRES);
		ADRES->X0_SET=ADRES->X0_SET+11;
		ADRES->X_END_SET=ADRES->X0_SET+10;;
		LCD_set_adressing(ADRES);
		LCD_printC(1,1,1,ADRES);
		ADRES->X0_SET=ADRES->X0_SET+11;
		ADRES->X_END_SET=127;
		LCD_set_adressing(ADRES);
		LCD_print1(1,1,1,ADRES);
		ADRES->X0_SET=0;
		ADRES->X_END_SET=127;
		ADRES->Y0_SET=80;
		ADRES->Y_END_SET=159;
		LCD_set_adressing(ADRES);
		DC_ON();
		CS_OFF();
		for(uint32_t i=0;i<((159-60)*127);i++)
		{
			HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
		}
		CS_ON();
	}
	else
	{
		ADRES->X0_SET=0x00;
		ADRES->X_END_SET=ADRES->X0_SET+40;
		ADRES->Y0_SET=0x00;
		ADRES->Y_END_SET=ADRES->Y0_SET+60;
		LCD_set_adressing(ADRES);
		switch(num_1)
		{
		case 0: 	LCD_print0H(1,1,1,ADRES);
					break;
		case 1: 	LCD_print1H(1,1,1,ADRES);
					break;
		case 2: 	LCD_print2H(1,1,1,ADRES);
					break;
		case 3: 	LCD_print3H(1,1,1,ADRES);
					break;
		default:	 break;
		}

		ADRES->X0_SET=ADRES->X_END_SET;
		ADRES->X_END_SET=ADRES->X0_SET+15;
		LCD_set_adressing(ADRES);
		LCD_print_dot(1,1,1,ADRES);
		ADRES->X0_SET=ADRES->X_END_SET;
		ADRES->X_END_SET=127;
		LCD_set_adressing(ADRES);
		switch(num2)
		{
		case 0: 	LCD_print0H(1,1,1,ADRES);
					break;
		case 1: 	LCD_print1H(1,1,1,ADRES);
					break;
		case 2: 	LCD_print2H(1,1,1,ADRES);
					break;
		case 3: 	LCD_print3H(1,1,1,ADRES);
					break;
		case 4: 	LCD_print4H(1,1,1,ADRES);
					break;
		case 5: 	LCD_print5H(1,1,1,ADRES);
					break;
		case 6: 	LCD_print6H(1,1,1,ADRES);
					break;
		case 7: 	LCD_print7H(1,1,1,ADRES);
					break;
		case 8: 	LCD_print8H(1,1,1,ADRES);
					break;
		case 9: 	LCD_print9H(1,1,1,ADRES);
					break;
		default:	 break;
		}


		ADRES->X0_SET=0x00;
		ADRES->X_END_SET=ADRES->X0_SET+11;
		ADRES->Y0_SET=60;
		ADRES->Y_END_SET=ADRES->Y0_SET+20;
		LCD_set_adressing(ADRES);
		LCD_printS(1,1,1,ADRES);
		ADRES->X0_SET=ADRES->X0_SET+11;
		ADRES->X_END_SET=ADRES->X0_SET+10;
		LCD_set_adressing(ADRES);
		LCD_printT(1,1,1,ADRES);
		ADRES->X0_SET=ADRES->X0_SET+11;
		ADRES->X_END_SET=ADRES->X0_SET+10;
		LCD_set_adressing(ADRES);
		LCD_printA(1,1,1,ADRES);
		ADRES->X0_SET=ADRES->X0_SET+11;
		ADRES->X_END_SET=ADRES->X0_SET+20;
		LCD_set_adressing(ADRES);
		LCD_printN(1,1,1,ADRES);
		ADRES->X0_SET=ADRES->X0_SET+18;
		ADRES->X_END_SET=ADRES->X0_SET+13;
		LCD_set_adressing(ADRES);
		LCD_printA(1,1,1,ADRES);
		ADRES->X0_SET=ADRES->X0_SET+11;
		ADRES->X_END_SET=ADRES->X0_SET+10;
		LCD_set_adressing(ADRES);
		LCD_printD(1,1,1,ADRES);
		ADRES->X0_SET=ADRES->X0_SET+11;
		ADRES->X_END_SET=ADRES->X0_SET+10;;
		LCD_set_adressing(ADRES);
		LCD_printC(1,1,1,ADRES);
		ADRES->X0_SET=ADRES->X0_SET+11;
		ADRES->X_END_SET=127;
		LCD_set_adressing(ADRES);
		LCD_print2(1,1,1,ADRES);
		ADRES->X0_SET=0;
		ADRES->X_END_SET=127;
		ADRES->Y0_SET=80;
		ADRES->Y_END_SET=159;
		LCD_set_adressing(ADRES);
		DC_ON();
		CS_OFF();
		for(uint32_t i=0;i<((159-60)*127);i++)
		{
			HAL_SPI_Transmit(&hspi3, word, 2, HAL_MAX_DELAY);
		}
		CS_ON();
	}

}


void LCD_print_tryb_segment(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES,unsigned int *num)
{
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	// 1 segment
	ADRES->X0_SET=0x00;
	ADRES->X_END_SET=ADRES->X0_SET+40;
	ADRES->Y0_SET=0x00;
	ADRES->Y_END_SET=ADRES->Y0_SET+60;
	LCD_set_adressing(ADRES);
	LCD_print_segment(1, 1, 1, ADRES, *num);
	ADRES->X0_SET=ADRES->X_END_SET;
	ADRES->X_END_SET=ADRES->X0_SET+15;
	LCD_set_adressing(ADRES);
	LCD_print_dot(1,1,1,ADRES);
	ADRES->X0_SET=ADRES->X_END_SET;
	ADRES->X_END_SET=127;
	LCD_set_adressing(ADRES);
	LCD_print_segment(1, 1, 1, ADRES,*(num+1));
	ADRES->X0_SET=0x00;
	ADRES->X_END_SET=ADRES->X0_SET+11;
	ADRES->Y0_SET=60;
	ADRES->Y_END_SET=ADRES->Y0_SET+20;
	LCD_set_adressing(ADRES);
	LCD_printS(1,1,1,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printT(1,1,1,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printA(1,1,1,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+20;
	LCD_set_adressing(ADRES);
	LCD_printN(1,1,1,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+18;
	ADRES->X_END_SET=ADRES->X0_SET+13;
	LCD_set_adressing(ADRES);
	LCD_printA(1,1,1,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printD(1,1,1,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;;
	LCD_set_adressing(ADRES);
	LCD_printC(1,1,1,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=127;
	LCD_set_adressing(ADRES);
	LCD_print1(1,1,1,ADRES);




	// 2 segment
	ADRES->X0_SET=0x00;
	ADRES->X_END_SET=ADRES->X0_SET+40;
	ADRES->Y0_SET=80;
	ADRES->Y_END_SET=ADRES->Y0_SET+60;
	LCD_set_adressing(ADRES);
	LCD_print_segment(1, 1, 1, ADRES, *(num+2));
	ADRES->X0_SET=ADRES->X_END_SET;
	ADRES->X_END_SET=ADRES->X0_SET+15;
	LCD_set_adressing(ADRES);
	LCD_print_dot(1,1,1,ADRES);
	ADRES->X0_SET=ADRES->X_END_SET;
	ADRES->X_END_SET=127;
	LCD_set_adressing(ADRES);
	LCD_print_segment(1, 1, 1, ADRES, *(num+3));
	ADRES->X0_SET=0x00;
	ADRES->X_END_SET=ADRES->X0_SET+11;
	ADRES->Y0_SET=140;
	ADRES->Y_END_SET=159;
	LCD_set_adressing(ADRES);
	LCD_printS(1,1,1,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printT(1,1,1,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printA(1,1,1,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+20;
	LCD_set_adressing(ADRES);
	LCD_printN(1,1,1,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+18;
	ADRES->X_END_SET=ADRES->X0_SET+13;
	LCD_set_adressing(ADRES);
	LCD_printA(1,1,1,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printD(1,1,1,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;;
	LCD_set_adressing(ADRES);
	LCD_printC(1,1,1,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=127;
	LCD_set_adressing(ADRES);
	LCD_print2(1,1,1,ADRES);


}

void LCD_print_tryb_tank(uint8_t R, uint8_t G, uint8_t B,adres_t *ADRES,float *num)
{
	uint8_t fbyte=0x00;
	uint8_t sbyte=0x00;
	fbyte=((fbyte^R)<<3)^(G&0x38);
	sbyte=((sbyte^(G&0x07))<<5)^B;
	uint8_t word[]={fbyte,sbyte};
	// 1 segment
	ADRES->X0_SET=0x00;
	ADRES->X_END_SET=ADRES->X0_SET+20;
	ADRES->Y0_SET=0x00;
	ADRES->Y_END_SET=ADRES->Y0_SET+25;
	LCD_set_adressing(ADRES);
	LCD_printA(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printD(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printC(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+20;
	LCD_set_adressing(ADRES);
	LCD_print1(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+20;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printA(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printD(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=ADRES->X0_SET+10;
	LCD_set_adressing(ADRES);
	LCD_printC(R,G,B,ADRES);
	ADRES->X0_SET=ADRES->X0_SET+11;
	ADRES->X_END_SET=127;
	LCD_set_adressing(ADRES);
	LCD_print2(R,G,B,ADRES);


	// 2 segment
	ADRES->X0_SET=0x00;
	ADRES->X_END_SET=ADRES->X0_SET+50;
	ADRES->Y0_SET=25;
	ADRES->Y_END_SET=159;
	LCD_set_adressing(ADRES);
	LCD_print_tank(1, 1, 1, ADRES, *num);
	ADRES->X0_SET=ADRES->X0_SET+51;
	ADRES->X_END_SET=127;
	LCD_set_adressing(ADRES);
	LCD_print_tank(1, 1, 1, ADRES, *(num+1));

}


#endif /* INC_LCD_FUNCTIONS_H_ */

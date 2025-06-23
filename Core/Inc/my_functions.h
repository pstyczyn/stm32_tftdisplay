/*
 * my_functions.h
 *
 *  Created on: Jan 5, 2025
 *      Author: patry
 */


#ifndef INC_MY_FUNCTIONS_H_
#define INC_MY_FUNCTIONS_H_

#include "LCD_functions.h"
typedef struct pinport{
	uint16_t pin;
}pin_t;

static const pin_t seg_0[]={A_Pin, B_Pin,C_Pin,D_Pin,E_Pin,F_Pin};
static const pin_t seg_1[]={B_Pin, C_Pin};
static const pin_t seg_2[]={A_Pin,B_Pin,G_Pin,E_Pin,D_Pin};
static const pin_t seg_3[]={A_Pin,B_Pin,G_Pin,C_Pin,D_Pin};
static const pin_t seg_4[]={F_Pin,G_Pin,B_Pin,C_Pin};
static const pin_t seg_5[]={A_Pin,F_Pin,G_Pin,C_Pin,D_Pin};
static const pin_t seg_6[]={A_Pin,F_Pin,G_Pin,E_Pin,C_Pin,D_Pin};
static const pin_t seg_7[]={A_Pin,B_Pin,C_Pin};
static const pin_t seg_8[]={A_Pin,B_Pin,C_Pin,D_Pin,E_Pin,F_Pin,G_Pin};
static const pin_t seg_9[]={A_Pin,F_Pin,B_Pin,G_Pin,C_Pin,D_Pin};

static const pin_t seg2_0[]={Ad2_Pin,Bd2_Pin,Cd2_Pin,Dd2_Pin,Ed2_Pin,Fd2_Pin};
static const pin_t seg2_1[]={Bd2_Pin,Cd2_Pin};
static const pin_t seg2_2[]={Ad2_Pin,Bd2_Pin,Gd2_Pin,Ed2_Pin,Dd2_Pin};
static const pin_t seg2_3[]={Ad2_Pin,Bd2_Pin,Gd2_Pin,Cd2_Pin,Dd2_Pin};
static const pin_t seg2_4[]={Fd2_Pin,Gd2_Pin,Bd2_Pin,Cd2_Pin};
static const pin_t seg2_5[]={Ad2_Pin,Fd2_Pin,Gd2_Pin,Cd2_Pin,Dd2_Pin};
static const pin_t seg2_6[]={Ad2_Pin,Fd2_Pin,Gd2_Pin,Ed2_Pin,Cd2_Pin,Dd2_Pin};
static const pin_t seg2_7[]={Ad2_Pin,Bd2_Pin,Cd2_Pin};
static const pin_t seg2_8[]={Ad2_Pin,Bd2_Pin,Cd2_Pin,Dd2_Pin,Ed2_Pin,Fd2_Pin,Gd2_Pin};
static const pin_t seg2_9[]={Ad2_Pin,Fd2_Pin,Bd2_Pin,Gd2_Pin,Cd2_Pin,Dd2_Pin};

static const pin_t all[]={Ad2_Pin,Bd2_Pin,Cd2_Pin,Dd2_Pin,Ed2_Pin,Fd2_Pin,Gd2_Pin};
static const pin_t all2[]={A_Pin,B_Pin,C_Pin,D_Pin,E_Pin,F_Pin,G_Pin};



bool odebrano=FALSE;
bool tOFF=FALSE;
bool lineON=FALSE;
bool aDC1=FALSE;
bool aDC2=FALSE;
bool aDC1_wyslano=FALSE;
bool aDC2_wyslano=FALSE;
uint8_t dSTATE=0x00;


unsigned int bufRxIndex=0;
unsigned int rozmiar=0;
unsigned int tryb=0;
unsigned int num1=0;
float number=0.0;
long int napiecie=0;
float fnapiecie=0.0;
long int napiecie2=0;
float fnapiecie2=0.0;


unsigned char buforRx[17]={0};
unsigned char diody[]={"Tryb wlaczania pojedynczych diod\n"};
unsigned char led1_on[]={"Zapalono diode czerwona"};
unsigned char led2_on[]={"Zapalono diode zolta"};
unsigned char led3_on[]={"Zapalono diode zielona"};
unsigned char led1_off[]={"Zgaszono diode czerwona"};
unsigned char led2_off[]={"Zgaszono diode zolta"};
unsigned char led3_off[]={"Zgaszono diode zielona"};
unsigned char all_off[]={"Zgaszono wszystkie diody"};
unsigned char tsegment[]={"Wlaczono tryb segmentu"};
unsigned char line[]={"Wlaczono tryb wyswietlania linijki"};
unsigned char dadc[]={"Wlaczono tryb wyswietlania pojedynczego ADC"};
unsigned char led[]={"Nie ma akcji przypisanej do tego przycisku\n"};
unsigned char powitanie[]={"Czynnosci, ktore mozesz wykonac to:\n"
		"1. Jezeli chcesz wlaczyc diody to uzyj przycisku D a potem\n"
		" przyciskow od 1 do 6\n"
		"2. Jezeli chcesz dana diode wylaczyc uzyj po raz drugi tego samego przycisku.\n"
		"3. Jezeli chcesz wszystkie diody wylaczyc za jednym razem uzyj R.\n"
		"4. Jezeli chcesz wlaczyc wyswietlanie aktualnej wartosci ADC1 uzyj B.\n"
		"5. Jezeli chcesz wlaczyc wyswietlanie aktualnej wartosci ADC2 uzyj A.\n"
		"6. Jezeli chcesz wlaczyc tryb segmentu wcisnij S.\n"
		"7. Jezeli chcesz wlaczyc linijke obu ADC uzyj L.\n"
		"8. Wcisnij M by ponownie wyswietlic menu.\n"};




void UART_decision(unsigned char c, adres_t *ADRES, unsigned int *num)
{
	  switch(c)
	  {
		  case 49:if(HAL_GPIO_ReadPin(GPIOC, Czerwony_1_Pin)==RESET)
		  	  	  {
			  	  	  HAL_UART_Transmit_IT(&huart2, &led1_on[bufRxIndex], 1);
		  	  	  }
				  else
				  {
					  HAL_UART_Transmit_IT(&huart2, &led1_off[bufRxIndex], 1);
				  }
				  tryb=1;
				  break;
		  case 50:if(HAL_GPIO_ReadPin(GPIOC, Zolty_1_Pin)==RESET)
				  {
					  HAL_UART_Transmit_IT(&huart2, &led2_on[bufRxIndex], 1);
				  }
				  else
				  {
					  HAL_UART_Transmit_IT(&huart2, &led2_off[bufRxIndex], 1);
				  }
		  	  	  tryb=2;
				  break;
		  case 51:if(HAL_GPIO_ReadPin(GPIOC, Zielony_1_Pin)==RESET)
				  {
					  HAL_UART_Transmit_IT(&huart2, &led3_on[bufRxIndex], 1);
				  }
				  else
				  {
					  HAL_UART_Transmit_IT(&huart2, &led3_off[bufRxIndex], 1);
				  }
		  	  	  tryb=3;
				  break;
		  case 52:if(HAL_GPIO_ReadPin(GPIOC, Czerwony_2_Pin)==RESET)
				  {
					  HAL_UART_Transmit_IT(&huart2, &led1_on[bufRxIndex], 1);
				  }
				  else
				  {
					  HAL_UART_Transmit_IT(&huart2, &led1_off[bufRxIndex], 1);
				  }
				  tryb=1;
				  break;
		  case 53:if(HAL_GPIO_ReadPin(GPIOB, Zolty_2_Pin)==RESET)
				  {
					  HAL_UART_Transmit_IT(&huart2, &led2_on[bufRxIndex], 1);
				  }
				  else
				  {
					  HAL_UART_Transmit_IT(&huart2, &led2_off[bufRxIndex], 1);
				  }
				  tryb=2;
				  break;
		  case 54:if(HAL_GPIO_ReadPin(GPIOB, Zielony_2_Pin)==RESET)
				  {
					  HAL_UART_Transmit_IT(&huart2, &led3_on[bufRxIndex], 1);
				  }
				  else
				  {
					  HAL_UART_Transmit_IT(&huart2, &led3_off[bufRxIndex], 1);
				  }
				  tryb=3;
				  break;
		  case 65:allsegment(FALSE); // A
		  	  	  aDC1_wyslano=FALSE;
		  	  	  aDC1=TRUE;
		  	  	  aDC2=FALSE;
		  	  	  lineON=FALSE;
		  	  	  tryb=5;
		  	  	  HAL_UART_Transmit_IT(&huart2, &dadc[bufRxIndex], 1);
		  	  	  break;
		  case 66:allsegment(FALSE); // B
		  	  	  aDC1_wyslano=FALSE;
				  aDC1=FALSE;
				  aDC2=TRUE;
				  lineON=FALSE;
				  tryb=5;
				  HAL_UART_Transmit_IT(&huart2, &dadc[bufRxIndex], 1);
				  break;
		  case 68:allsegment(FALSE); //D
		  	  	  LCD_print_tryb_diody(1,1,1,ADRES);
		  	  	  odebrano=FALSE;
		  	  	  bufRxIndex=0;
		  	  	  HAL_UART_Transmit(&huart2, &diody, sizeof(diody)-1, HAL_MAX_DELAY);
		  	  	  break;
		  case 76:allsegment(FALSE); // L
				  aDC1=FALSE;
				  aDC2=FALSE;
				  aDC1_wyslano=FALSE;
				  lineON=TRUE;
				  tryb=6;
				  HAL_UART_Transmit_IT(&huart2, &line[bufRxIndex], 1);
				  break;
		  case 77:odebrano=FALSE; // M
		  	  	  bufRxIndex=0;
		  	  	  aDC1_wyslano=FALSE;
		  	  	  tryb=0;
		  	  	  LCD_printMENU(1,1,1,ADRES);
		  	  	  HAL_UART_Transmit(&huart2, &powitanie, sizeof(powitanie)-1, HAL_MAX_DELAY);
		  	  	  HAL_UART_Receive_IT(&huart2, &buforRx[bufRxIndex], 1);
		  	  	  break;
		  case 82: 						// R
			  	  HAL_GPIO_WritePin(GPIOC, Czerwony_1_Pin | Zolty_1_Pin | Zielony_1_Pin | Czerwony_2_Pin, RESET);
				  HAL_GPIO_WritePin(GPIOB, Zolty_2_Pin | Zielony_2_Pin, RESET);
				  tryb=4;
				  dSTATE=0x00;
				  HAL_UART_Transmit_IT(&huart2, &all_off[bufRxIndex], 1);
				  break;
		  case 83:allsegment(FALSE); // S
			  	  tryb=7;
				  aDC1_wyslano=FALSE;
				  aDC1=FALSE;
				  aDC2=FALSE;
				  lineON=FALSE;
		  	  	  HAL_UART_Transmit_IT(&huart2, &tsegment[bufRxIndex], 1);
		  	  	  break;
		  default:HAL_GPIO_WritePin(GPIOC, Czerwony_1_Pin | Zolty_1_Pin | Zielony_1_Pin | Czerwony_2_Pin, RESET);
				  HAL_GPIO_WritePin(GPIOB, Zolty_2_Pin | Zielony_2_Pin, RESET);
				  tryb=0;
				  dSTATE=0x00;
				  HAL_UART_Transmit_IT(&huart2, &led[bufRxIndex], 1);
				  break;
	  }
}

void UART_transmit(unsigned int n)
{
	switch(n)
	{
	case 1: if(led1_on[bufRxIndex]==0x00 || led1_off[bufRxIndex]==0x00)
			{
				led1_on[bufRxIndex]=0x0A;
				led1_off[bufRxIndex]=0x0A;
			}
			if(bufRxIndex==(sizeof(led1_on)/sizeof(char)) || bufRxIndex==(sizeof(led1_off)/sizeof(char)))
			{
				if(buforRx[0]==49)
				{
					if((dSTATE&0x01)==0x00)
					{
						HAL_GPIO_WritePin(GPIOC, Czerwony_1_Pin, SET);
						dSTATE=dSTATE^0x01;
					}
					else if((dSTATE&0x01)==0x01)
					{
						HAL_GPIO_WritePin(GPIOC, Czerwony_1_Pin, RESET);
						dSTATE=dSTATE^0x01;
					}
				}
				else
				{
					if((dSTATE&0x08)==0x00)
					{
						HAL_GPIO_WritePin(GPIOC, Czerwony_2_Pin, SET);
						dSTATE=dSTATE^0x08;
					}
					else if((dSTATE&0x08)==0x08)
					{
					    HAL_GPIO_WritePin(GPIOC, Czerwony_2_Pin, RESET);
					    dSTATE=dSTATE^0x08;
					}
				}
			bufRxIndex=0;
			odebrano=FALSE;
			HAL_UART_Receive_IT(&huart2, &buforRx[bufRxIndex], 1);
			}
			break;
	case 2: if(led2_on[bufRxIndex]==0x00 || led2_off[bufRxIndex]==0x00)
			{
				led2_on[bufRxIndex]=0x0A;
				led2_off[bufRxIndex]=0x0A;
			}
			if(bufRxIndex==(sizeof(led2_on)/sizeof(char))||bufRxIndex==(sizeof(led2_off)/sizeof(char)))
			{
				if(buforRx[0]==50)
				{
					if((dSTATE&0x02)==0x00)
					{
						dSTATE=dSTATE|0x02;
						HAL_GPIO_WritePin(GPIOC, Zolty_1_Pin, SET);
					}
					else if((dSTATE&0x02)==0x02)
					{
						dSTATE=dSTATE^0x02;
						HAL_GPIO_WritePin(GPIOC, Zolty_1_Pin, RESET);
					}
				}
				else
				{
					if((dSTATE&0x10)==0x00)
					{
						dSTATE=dSTATE|0x10;
						HAL_GPIO_WritePin(GPIOB, Zolty_2_Pin, SET);
					}
					else if((dSTATE&0x10)==0x10)
					{
						dSTATE=dSTATE^0x10;
						HAL_GPIO_WritePin(GPIOB, Zolty_2_Pin, RESET);
					}
				}
			bufRxIndex=0;
			odebrano=FALSE;
			HAL_UART_Receive_IT(&huart2, &buforRx[bufRxIndex], 1);
			}
			break;
	case 3: if(led3_on[bufRxIndex]==0x00||led3_off[bufRxIndex]==0x00)
			{
				led3_on[bufRxIndex]=0x0A;
				led3_off[bufRxIndex]=0x0A;
			}
			if(bufRxIndex==(sizeof(led3_on)/sizeof(char))||bufRxIndex==(sizeof(led3_off)/sizeof(char)))
			{
				if(buforRx[0]==51)
				{
					if((dSTATE&0x04)==0x00)
					{
						dSTATE=dSTATE|0x04;
						HAL_GPIO_WritePin(GPIOC, Zielony_1_Pin, SET);
					}
					else if((dSTATE&0x04)==0x04)
					{
						dSTATE=dSTATE^0x04;
						HAL_GPIO_WritePin(GPIOC, Zielony_1_Pin, RESET);
					}
				}
				else
				{
					if((dSTATE&0x20)==0x00)
					{
						dSTATE=dSTATE|0x20;
						HAL_GPIO_WritePin(GPIOB, Zielony_2_Pin, SET);
					}
					else if((dSTATE&0x20)==0x20)
					{
						dSTATE=dSTATE^0x20;
						HAL_GPIO_WritePin(GPIOB, Zielony_2_Pin, RESET);
					}
				}
			bufRxIndex=0;
			odebrano=FALSE;
			HAL_UART_Receive_IT(&huart2, &buforRx[bufRxIndex], 1);
			}
			break;
	case 4: if(all_off[bufRxIndex]==0x00)
			{
				all_off[bufRxIndex]=0x0A;
			}
			if(bufRxIndex==(sizeof(all_off)/sizeof(char)))
			{
				bufRxIndex=0;
				odebrano=FALSE;
				HAL_UART_Receive_IT(&huart2, &buforRx[bufRxIndex], 1);
			}
			break;
	case 5: if(dadc[bufRxIndex]==0x00)
			{
				dadc[bufRxIndex]=0x0A;
			}
			if(bufRxIndex==(sizeof(dadc)/sizeof(char)))
			{
				bufRxIndex=0;
				odebrano=FALSE;
				aDC1_wyslano=TRUE;
				HAL_UART_Receive_IT(&huart2, &buforRx[bufRxIndex], 1);
			}
			break;
	case 6: if(line[bufRxIndex]==0x00)
			{
				line[bufRxIndex]=0x0A;
			}
			if(bufRxIndex==(sizeof(line)/sizeof(char)))
			{
				bufRxIndex=0;
				odebrano=FALSE;
				HAL_UART_Receive_IT(&huart2, &buforRx[bufRxIndex], 1);
			}
			break;
	case 7: if(tsegment[bufRxIndex]==0x00)
			{
				tsegment[bufRxIndex]=0x0A;
			}
			if(bufRxIndex==(sizeof(tsegment)/sizeof(char)))
			{
				bufRxIndex=0;
				odebrano=FALSE;
				HAL_UART_Receive_IT(&huart2, &buforRx[bufRxIndex], 1);
			}
			break;
	default:if(led[bufRxIndex]==0x00)
			{
				led[bufRxIndex]=0x0A;
			}
			if(bufRxIndex==(sizeof(led)/sizeof(char)))
			{
				bufRxIndex=0;
				odebrano=FALSE;
				HAL_UART_Receive_IT(&huart2, &buforRx[bufRxIndex], 1);
			}break;
	}
}

void segment1(unsigned int n, bool action)
{
	int j=0;
	switch(n)
	{
	case 0:
		if(action)
		{
			for(j=0;j<(sizeof(seg_0))/sizeof(uint16_t);j++)
			{
				HAL_GPIO_WritePin(GPIOA, seg_0[j].pin, GPIO_PIN_SET);
			}
		}
		else
		{
			for(j=0;j<(sizeof(seg_0))/sizeof(uint16_t);j++)
			{
				HAL_GPIO_WritePin(GPIOA, seg_0[j].pin, GPIO_PIN_RESET);
			}
		}break;
	case 1:
			if(action)
			{
				for(j=0;j<(sizeof(seg_1))/sizeof(uint16_t);j++)
				{
					HAL_GPIO_WritePin(GPIOA, seg_1[j].pin, GPIO_PIN_SET);
				}
			}
			else
			{
				for(j=0;j<(sizeof(seg_1))/sizeof(uint16_t);j++)
				{
					HAL_GPIO_WritePin(GPIOA, seg_1[j].pin, GPIO_PIN_RESET);
				}

			}break;
	case 2:
		if(action)
		{
			for(j=0;j<(sizeof(seg_2))/sizeof(uint16_t);j++)
			{
				HAL_GPIO_WritePin(GPIOA, seg_2[j].pin, GPIO_PIN_SET);
			}
		}
		else
		{
			for(j=0;j<(sizeof(seg_2))/sizeof(uint16_t);j++)
			{
				HAL_GPIO_WritePin(GPIOA, seg_2[j].pin, GPIO_PIN_RESET);
			}
		}break;
	case 3:
		if(action)
		{
			for(j=0;j<(sizeof(seg_3))/sizeof(uint16_t);j++)
			{
				HAL_GPIO_WritePin(GPIOA, seg_3[j].pin, GPIO_PIN_SET);
			}
		}
		else
		{
			for(j=0;j<(sizeof(seg_3))/sizeof(uint16_t);j++)
			{
				HAL_GPIO_WritePin(GPIOA, seg_3[j].pin, GPIO_PIN_RESET);
			}
		}break;
	case 4:
		if(action)
		{
			for(j=0;j<(sizeof(seg_4))/sizeof(uint16_t);j++)
			{
				HAL_GPIO_WritePin(GPIOA, seg_4[j].pin, GPIO_PIN_SET);
			}
		}
		else
		{
			for(j=0;j<(sizeof(seg_4))/sizeof(uint16_t);j++)
			{
				HAL_GPIO_WritePin(GPIOA, seg_4[j].pin, GPIO_PIN_RESET);
			}
		}break;
	case 5:
		if(action)
		{
			for(j=0;j<(sizeof(seg_5))/sizeof(uint16_t);j++)
			{
				HAL_GPIO_WritePin(GPIOA, seg_5[j].pin, GPIO_PIN_SET);
			}
		}
		else
		{
			for(j=0;j<(sizeof(seg_5))/sizeof(uint16_t);j++)
			{
				HAL_GPIO_WritePin(GPIOA, seg_5[j].pin, GPIO_PIN_RESET);
			}
		}break;
	case 6:
		if(action)
		{
			for(j=0;j<(sizeof(seg_6))/sizeof(uint16_t);j++)
			{
				HAL_GPIO_WritePin(GPIOA, seg_6[j].pin, GPIO_PIN_SET);
			}
		}
		else
		{
			for(j=0;j<(sizeof(seg_6))/sizeof(uint16_t);j++)
			{
				HAL_GPIO_WritePin(GPIOA, seg_6[j].pin, GPIO_PIN_RESET);
			}
		}break;
	case 7:
		if(action)
		{
			for(j=0;j<(sizeof(seg_7))/sizeof(uint16_t);j++)
			{
				HAL_GPIO_WritePin(GPIOA, seg_7[j].pin, GPIO_PIN_SET);
			}
		}
		else
		{
			for(j=0;j<(sizeof(seg_7))/sizeof(uint16_t);j++)
			{
				HAL_GPIO_WritePin(GPIOA, seg_7[j].pin, GPIO_PIN_RESET);
			}
		}break;
	case 8:
		if(action)
		{
			for(j=0;j<(sizeof(seg_8))/sizeof(uint16_t);j++)
			{
				HAL_GPIO_WritePin(GPIOA, seg_8[j].pin, GPIO_PIN_SET);
			}
		}
		else
		{
			for(j=0;j<(sizeof(seg_8))/sizeof(uint16_t);j++)
			{
				HAL_GPIO_WritePin(GPIOA, seg_8[j].pin, GPIO_PIN_RESET);
			}
		}break;
	case 9:
		if(action)
		{
			for(j=0;j<(sizeof(seg_9))/sizeof(uint16_t);j++)
			{
				HAL_GPIO_WritePin(GPIOA, seg_9[j].pin, GPIO_PIN_SET);
			}
		}
		else
		{
			for(j=0;j<(sizeof(seg_9))/sizeof(uint16_t);j++)
			{
				HAL_GPIO_WritePin(GPIOA, seg_9[j].pin, GPIO_PIN_RESET);
			}
		}break;

	default: break;
	}
}

void segment2(unsigned int n, bool action)
{
	int j=0;
	switch(n)
	{
	case 0:
		if(action)
		{
			for(j=0;j<(sizeof(seg2_0))/sizeof(uint16_t);j++)
			{
				HAL_GPIO_WritePin(GPIOB, seg2_0[j].pin, GPIO_PIN_SET);
			}
		}
		else
		{
			for(j=0;j<(sizeof(seg2_0))/sizeof(uint16_t);j++)
			{
				HAL_GPIO_WritePin(GPIOB, seg2_0[j].pin, GPIO_PIN_RESET);
			}
		}break;
	case 1:
			if(action)
			{
				for(j=0;j<(sizeof(seg2_1))/sizeof(uint16_t);j++)
				{
					HAL_GPIO_WritePin(GPIOB, seg2_1[j].pin, GPIO_PIN_SET);
				}
			}
			else
			{
				for(j=0;j<(sizeof(seg2_1))/sizeof(uint16_t);j++)
				{
					HAL_GPIO_WritePin(GPIOB, seg2_1[j].pin, GPIO_PIN_RESET);
				}
			}break;
	case 2:
		if(action)
		{
			for(j=0;j<(sizeof(seg2_2))/sizeof(uint16_t);j++)
			{
				HAL_GPIO_WritePin(GPIOB, seg2_2[j].pin, GPIO_PIN_SET);
			}
		}
		else
		{
			for(j=0;j<(sizeof(seg2_2))/sizeof(uint16_t);j++)
			{
				HAL_GPIO_WritePin(GPIOB, seg2_2[j].pin, GPIO_PIN_RESET);
			}
		}break;
	case 3:
		if(action)
		{
			for(j=0;j<(sizeof(seg2_3))/sizeof(uint16_t);j++)
			{
				HAL_GPIO_WritePin(GPIOB, seg2_3[j].pin, GPIO_PIN_SET);
			}
		}
		else
		{
			for(j=0;j<(sizeof(seg2_3))/sizeof(uint16_t);j++)
			{
				HAL_GPIO_WritePin(GPIOB, seg2_3[j].pin, GPIO_PIN_RESET);
			}
		}break;
	case 4:
		if(action)
		{
			for(j=0;j<(sizeof(seg2_4))/sizeof(uint16_t);j++)
			{
				HAL_GPIO_WritePin(GPIOB, seg2_4[j].pin, GPIO_PIN_SET);
			}
		}
		else
		{
			for(j=0;j<(sizeof(seg2_4))/sizeof(uint16_t);j++)
			{
				HAL_GPIO_WritePin(GPIOB, seg2_4[j].pin, GPIO_PIN_RESET);
			}
		}break;
	case 5:
		if(action)
		{
			for(j=0;j<(sizeof(seg2_5))/sizeof(uint16_t);j++)
			{
				HAL_GPIO_WritePin(GPIOB, seg2_5[j].pin, GPIO_PIN_SET);
			}
		}
		else
		{
			for(j=0;j<(sizeof(seg2_5))/sizeof(uint16_t);j++)
			{
				HAL_GPIO_WritePin(GPIOB, seg2_5[j].pin, GPIO_PIN_RESET);
			}
		}break;
	case 6:
		if(action)
		{
			for(j=0;j<(sizeof(seg2_6))/sizeof(uint16_t);j++)
			{
				HAL_GPIO_WritePin(GPIOB, seg2_6[j].pin, GPIO_PIN_SET);
			}
		}
		else
		{
			for(j=0;j<(sizeof(seg2_6))/sizeof(uint16_t);j++)
			{
				HAL_GPIO_WritePin(GPIOB, seg2_6[j].pin, GPIO_PIN_RESET);
			}
		}break;
	case 7:
		if(action)
		{
			for(j=0;j<(sizeof(seg2_7))/sizeof(uint16_t);j++)
			{
				HAL_GPIO_WritePin(GPIOB, seg2_7[j].pin, GPIO_PIN_SET);
			}
		}
		else
		{
			for(j=0;j<(sizeof(seg2_7))/sizeof(uint16_t);j++)
			{
				HAL_GPIO_WritePin(GPIOB, seg2_7[j].pin, GPIO_PIN_RESET);
			}
		}break;
	case 8:
		if(action)
		{
			for(j=0;j<(sizeof(seg2_8))/sizeof(uint16_t);j++)
			{
				HAL_GPIO_WritePin(GPIOB, seg2_8[j].pin, GPIO_PIN_SET);
			}
		}
		else
		{
			for(j=0;j<(sizeof(seg2_8))/sizeof(uint16_t);j++)
			{
				HAL_GPIO_WritePin(GPIOB, seg2_8[j].pin, GPIO_PIN_RESET);
			}
		}break;
	case 9:
		if(action)
		{
			for(j=0;j<(sizeof(seg2_9))/sizeof(uint16_t);j++)
			{
				HAL_GPIO_WritePin(GPIOB, seg2_9[j].pin, GPIO_PIN_SET);
			}
		}
		else
		{
			for(j=0;j<(sizeof(seg2_9))/sizeof(uint16_t);j++)
			{
				HAL_GPIO_WritePin(GPIOB, seg2_9[j].pin, GPIO_PIN_RESET);
			}
		}break;

	default: ;break;
	}
}

void allsegment(bool action)
{
	int j=0;
	if(!action)
	{
		for(j=0;j<(sizeof(all))/sizeof(uint16_t);j++)
		{
			HAL_GPIO_WritePin(GPIOB, all[j].pin, GPIO_PIN_RESET);
		}
	}
	else
	{
		for(j=0;j<(sizeof(all))/sizeof(uint16_t);j++)
		{
			HAL_GPIO_WritePin(GPIOB, all[j].pin, GPIO_PIN_SET);
		}
	}
	if(!action)
	{
		for(j=0;j<(sizeof(all2))/sizeof(uint16_t);j++)
		{
			HAL_GPIO_WritePin(GPIOA, all2[j].pin, GPIO_PIN_RESET);
		}
	}
	else
	{
		for(j=0;j<(sizeof(all2))/sizeof(uint16_t);j++)
		{
			HAL_GPIO_WritePin(GPIOA, all2[j].pin, GPIO_PIN_SET);
		}
	}
}

void linijka1(float poziom1)
{
	allsegment(FALSE);
	if(poziom1>=0 && poziom1<=0.471)
	{
		HAL_GPIO_WritePin(GPIOA, A_Pin, SET);
	}
	else if(poziom1>0.471 && poziom1<=0.942)
	{
		HAL_GPIO_WritePin(GPIOA, A_Pin | B_Pin, SET);
	}
	else if(poziom1>0.942 && poziom1<=1.413)
	{
		HAL_GPIO_WritePin(GPIOA, A_Pin | B_Pin | C_Pin, SET);
	}
	else if(poziom1>1.413 && poziom1<=1.884)
	{
		HAL_GPIO_WritePin(GPIOA, A_Pin | B_Pin | C_Pin | D_Pin, SET);
	}
	else if(poziom1>1.884 && poziom1<=2.355)
	{
		HAL_GPIO_WritePin(GPIOA, A_Pin | B_Pin | C_Pin | D_Pin | E_Pin, SET);
	}
	else if(poziom1>2.355 && poziom1<=2.826)
	{
		HAL_GPIO_WritePin(GPIOA, A_Pin | B_Pin | C_Pin | D_Pin | E_Pin | F_Pin, SET);
	}
	else if(poziom1>2.826)
	{
		HAL_GPIO_WritePin(GPIOA, A_Pin | B_Pin | C_Pin | D_Pin | E_Pin | F_Pin | G_Pin, SET);
	}
	else
	{
		allsegment(FALSE);
	}
}

void linijka2(float poziom2)
{
	allsegment(FALSE);
	if(poziom2>=0 && poziom2<=0.471)
	{
		HAL_GPIO_WritePin(GPIOB, Ad2_Pin, SET);
	}
	else if(poziom2>0.471 && poziom2<=0.942)
	{
		HAL_GPIO_WritePin(GPIOB, Ad2_Pin | Bd2_Pin, SET);
	}
	else if(poziom2>0.942 && poziom2<=1.413)
	{
		HAL_GPIO_WritePin(GPIOB, Ad2_Pin | Bd2_Pin | Cd2_Pin, SET);
	}
	else if(poziom2>1.413 && poziom2<=1.884)
	{
		HAL_GPIO_WritePin(GPIOB, Ad2_Pin | Bd2_Pin | Cd2_Pin | Dd2_Pin, SET);
	}
	else if(poziom2>1.884 && poziom2<=2.355)
	{
		HAL_GPIO_WritePin(GPIOB, Ad2_Pin | Bd2_Pin | Cd2_Pin | Dd2_Pin | Ed2_Pin, SET);
	}
	else if(poziom2>2.355 && poziom2<=2.826)
	{
		HAL_GPIO_WritePin(GPIOB, Ad2_Pin | Bd2_Pin | Cd2_Pin | Dd2_Pin | Ed2_Pin | Fd2_Pin, SET);
	}
	else if(poziom2>2.826)
	{
		HAL_GPIO_WritePin(GPIOB, Ad2_Pin | Bd2_Pin | Cd2_Pin | Dd2_Pin | Ed2_Pin | Fd2_Pin | Gd2_Pin, SET);
	}
	else
	{
		allsegment(FALSE);
	}
}




#endif /* INC_MY_FUNCTIONS_H_ */

/*
 * rtc.c
 *
 *  Created on: 15.12.2017
 *      Author: gwarchol
 */

#include "rtc.h"

unsigned long int total_rtc_ticks = 0;

volatile struct Time_s s_TimeStructVar;
struct AlarmTime_s s_AlarmStructVar;
struct Date_s s_DateStructVar;
struct AlarmDate_s s_AlarmDateStructVar;



void rtc_init(void){




	NVIC_RTC_Configuration();

	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
	  {
	RTC_Configuration();
	  }else{
		  if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
		    {
		      printf("\r\n\n Power On Reset occurred....");
		    }
		    /* Check if the Pin Reset flag is set */
		    else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
		    {
		      printf("\r\n\n External Reset occurred....");
		    }

		    printf("\r\n No need to configure RTC....");
		    /* Wait for RTC registers synchronization */
		    RTC_WaitForSynchro();

		    /* Enable the RTC Second */
		    RTC_ITConfig(RTC_IT_SEC, ENABLE);
		    /* Wait until last write operation on RTC registers has finished */
		    RTC_WaitForLastTask();

	  }

	  RCC_ClearFlag();
}






void RTC_Configuration(void)
{
  /* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);

  /* Reset Backup Domain */
  BKP_DeInit();

  /* Enable LSE */
  RCC_LSEConfig(RCC_LSE_ON);
  /* Wait till LSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}

  /* Select LSE as RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

  /* Enable RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC registers synchronization */
  RTC_WaitForSynchro();

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Enable the RTC Second */
  RTC_ITConfig(RTC_IT_SEC, ENABLE);

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Set RTC prescaler: set RTC period to 1sec */
  RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
}

void NVIC_RTC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  /* Enable the RTC Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

bool x = false;
void RTC_IRQHandler(void)
{
  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
  {
    /* Clear the RTC Second interrupt */
    RTC_ClearITPendingBit(RTC_IT_SEC);

    /* Toggle LED1 */
    //STM_EVAL_LEDToggle(LED1);

    total_rtc_ticks = RTC_GetCounter();


    switch(x){
    case true:
    	turnOnOffLed(true);
    	x = false;
    	break;
    case false:
    	turnOnOffLed(false);
    	x = true;
    	break;
    }



    /* Enable time update */
    //TimeDisplay = 1;

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

  }
}


int getTotalRtcTicks(){
	return total_rtc_ticks;
}



void read_timer(unsigned char *data_buff){
	uint32_t timevar;
	timevar = RTC_GetCounter();
	timevar = timevar % 86400;
	s_TimeStructVar.u8_HourHigh = (uint8_t)(timevar / 3600 ) / 10;
	s_TimeStructVar.u8_HourLow = (uint8_t)(timevar / 3600 ) % 10;
	s_TimeStructVar.u8_MinHigh = (uint8_t)((timevar % 3600 ) / 60) / 10;
	s_TimeStructVar.u8_MinLow = (uint8_t)((timevar % 3600 ) / 60) % 10;
	s_TimeStructVar.u8_SecHigh = ((uint8_t)(timevar % 60 ) % 60) / 10;
	s_TimeStructVar.u8_SecLow = ((uint8_t)(timevar % 60 ) % 60) % 10;
	sprintf(data_buff, "%d%d:%d%d:%d%d",
			s_TimeStructVar.u8_HourHigh,
			s_TimeStructVar.u8_HourLow,
			s_TimeStructVar.u8_MinHigh,
			s_TimeStructVar.u8_MinLow,
			s_TimeStructVar.u8_SecHigh,
			s_TimeStructVar.u8_SecLow);
}

void read_date(unsigned char *data_buff){
	s_DateStructVar.u8_Month = BKP_ReadBackupRegister(BKP_DR2);
	s_DateStructVar.u8_Day = BKP_ReadBackupRegister(BKP_DR3);
	s_DateStructVar.u16_Year = BKP_ReadBackupRegister(BKP_DR4);
}

void save_date(unsigned char day, unsigned char month, unsigned short year){

	s_DateStructVar.u8_Day = day;
	s_DateStructVar.u8_Month =  month;
	s_DateStructVar.u16_Year = year;
	BKP_WriteBackupRegister(BKP_DR2,s_DateStructVar.u8_Month);
	BKP_WriteBackupRegister(BKP_DR3,s_DateStructVar.u8_Day);
	BKP_WriteBackupRegister(BKP_DR4,s_DateStructVar.u16_Year);
}







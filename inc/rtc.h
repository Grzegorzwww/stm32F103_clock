/*
 * rtc.h
 *
 *  Created on: 15.12.2017
 *      Author: gwarchol
 */

#ifndef RTC_H_
#define RTC_H_

#include "stm32f10x.h"
#include "config.h"

void RTC_Configuration(void);
void NVIC_RTC_Configuration(void);
int getTotalRtcTicks();

void read_time(unsigned char *data_buff);
void read_date(unsigned char *data_buff);
void save_date(unsigned char day, unsigned char month, unsigned short year);


struct Time_s
{
  u8 u8_SecLow;
  u8 u8_SecHigh;
  u8 u8_MinLow;
  u8 u8_MinHigh;
  u8 u8_HourLow;
  u8 u8_HourHigh;
};


struct AlarmTime_s
{
  u8 u8_SecLow;
  u8 u8_SecHigh;
  u8 u8_MinLow;
  u8 u8_MinHigh;
  u8 u8_HourLow;
  u8 u8_HourHigh;
};

struct Date_s
{
  u8 u8_Month;
  u8 u8_Day;
  u16 u16_Year;
};

struct AlarmDate_s
{
  u8 u8_Month;
  u8 u8_Day;
  u16 u16_Year;
};
#endif /* RTC_H_ */

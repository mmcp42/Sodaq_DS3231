// Sodaq_DS3231 Class is a modified version of DS3231.
// DS3231 Class is by Seeed Technology Inc(http://www.seeedstudio.com) and used
// in Seeeduino Stalker v2.1 for battery management(MCU power saving mode)
// & to generate timestamp for data logging. DateTime Class is a modified
// version supporting day-of-week.

// Original DateTime Class and its utility code is by Jean-Claude Wippler at JeeLabs
// http://jeelabs.net/projects/cafe/wiki/RTClib 
// Released under MIT License http://opensource.org/licenses/mit-license.php

#ifndef SODAQ_DS3231_H
#define SODAQ_DS3231_H

#include <stdint.h>

#define DS3231_ADDRESS	      0x68 //I2C Slave address

/* DS3231 Registers. Refer Sec 8.2 of application manual */
#define DS3231_SEC_REG        0x00  
#define DS3231_MIN_REG        0x01  
#define DS3231_HOUR_REG       0x02
#define DS3231_WDAY_REG       0x03
#define DS3231_MDAY_REG       0x04
#define DS3231_MONTH_REG      0x05
#define DS3231_YEAR_REG       0x06

#define DS3231_AL1SEC_REG     0x07
#define DS3231_AL1MIN_REG     0x08
#define DS3231_AL1HOUR_REG    0x09
#define DS3231_AL1WDAY_REG    0x0A

#define DS3231_AL2MIN_REG     0x0B
#define DS3231_AL2HOUR_REG    0x0C
#define DS3231_AL2WDAY_REG    0x0D

#define DS3231_CONTROL_REG          0x0E
#define DS3231_STATUS_REG           0x0F
#define DS3231_AGING_OFFSET_REG     0x0F
#define DS3231_TMP_UP_REG           0x11
#define DS3231_TMP_LOW_REG          0x12

#define EverySecond     0x01
#define EveryMinute     0x02
#define EveryHour       0x03




// Simple general-purpose date/time class (no TZ / DST / leap second handling!)
class DateTime {
public:
    DateTime (long t =0);
    DateTime (uint16_t year, uint8_t month, uint8_t date,
              uint8_t hour, uint8_t min, uint8_t sec, uint8_t wday);
    DateTime (const char* date, const char* time);

    uint8_t second() const      { return ss; }
    uint8_t minute() const      { return mm; } 
    uint8_t hour() const        { return hh; }
 
    uint8_t date() const        { return d; }
    uint8_t month() const       { return m; }    /*Jan=1 Feb=2 Mar=3 Apr=4 May=5 Jun=6 Jul=7 Aug=8 Sep=9 Oct=10 Nov=11 Dec=12 */
    uint16_t year() const       { return 2000 + yOff; }		// Notice the 2000 !

    uint8_t dayOfWeek() const   { return wday;}  /*Su=0 Mo=1 Tu=2 We=3 Th=4 Fr=5 Sa=6 */

    // 32-bit time as seconds since 1/1/2000
    uint32_t get() const;
    // 32-bit number of seconds since Unix epoch (1970-01-01)
    uint32_t getEpoch() const;

protected:
    uint8_t yOff, m, d, hh, mm, ss, wday;
};

// RTC DS3231 chip connected via I2C and uses the Wire library.
// Only 24 Hour time format is supported in this implementation
class Sodaq_DS3231 {
public:
    uint8_t begin(void);
    uint8_t readRegister(uint8_t regaddress);
    void writeRegister(uint8_t regaddress, uint8_t value);

    void setDateTime(const DateTime& dt);  //Changes the date-time
    void setEpoch(uint32_t ts); // Set the RTC using timestamp (seconds since epoch)
    DateTime now();            //Gets the current date-time

    //Decides the /INT pin's output setting
    //periodicity can be any of following defines: EverySecond, EveryMinute, EveryHour 
    void enableInterrupts(uint8_t periodicity);
    void enableInterrupts(uint8_t hh24, uint8_t mm,uint8_t ss);
    void disableInterrupts();
    void clearINTStatus();

    void convertTemperature();
    float getTemperature();
};

extern Sodaq_DS3231 rtc;

#endif

#include "RTC.h"
#include "hardware/i2c.h"
#include <cstdint>
#include "pico/stdlib.h"
#include "pico/types.h"
#include "stdio.h"
namespace iu
{
    namespace Time
    {
        TimePoint::TimePoint(const Date& date)
            :m_point(0)
        {
            toTimePoint(date.hour, date.minute, date.second);
        }

        RTC::RTC(i2c_inst_t& i2c):m_i2cInstance(i2c)
        {
            uint8_t d = 0b0000100;
            WriteRegister(Register::CTRL_STATUS1, d);
        }

        void RTC::GetDate(Date& date)
        {
            ReadRegister(Register::SECONDS, date.second);
            ReadRegister(Register::MINUTES, date.minute);
            ReadRegister(Register::HOURS, date.hour);
            ReadRegister(Register::DAYS, date.day);
            ReadRegister(Register::DOTW, date.dotw);
            ReadRegister(Register::CENTURY_MONTHS, date.month);
            ReadRegister(Register::YEARS, date.year, false);


            date.second = ((date.second & 0b01110000) >> 4) * 10 + (date.second & 0b00001111);
            date.minute = ((date.minute & 0b01110000) >> 4) * 10 + (date.minute & 0b00001111);
            date.hour   = ((date.hour   & 0b00110000) >> 4) * 10 + (date.hour   & 0b00000111); 
            date.day    = ((date.day    & 0b00110000) >> 4) * 10 + (date.day    & 0b00001111);
            date.dotw   = (date.dotw    & 0b00000011);
            date.month  = ((date.month  & 0b00010000) >> 4) * 10 + (date.month  & 0b00001111);
            date.year   = ((date.year   & 0b11110000) >> 4) * 10 + (date.year   & 0b00001111);
        }

        void RTC::SetDate(Date &date)
        { 
            uint8_t data[7] ={
                (uint8_t)(((date.second / 10) << 4) | (date.second % 10)),
                (uint8_t)(((date.minute / 10) << 4) | (date.minute % 10)),
                (uint8_t)(((date.hour   / 10) << 4) | (date.hour   % 10)),
                (uint8_t)(((date.day    / 10) << 4) | (date.day    % 10)),
                date.dotw,
                (uint8_t)(((date.month  / 10) << 4) | (date.month  % 10)),
                (uint8_t)(((date.year   / 10) << 4) | (date.year   % 10))

            };

            for(uint8_t r = 0; r < 6; r++)
                WriteRegister((Register)(r + (uint8_t)Register::SECONDS), data[r]);
            WriteRegister(Register::YEARS, data[6], false);
            
        }

        void RTC::ReadRegister(Register&& reg, uint8_t& data, bool stop)
        {
            i2c_write_blocking(&m_i2cInstance, WRITE_ADDRESS, (uint8_t*)&reg, 1, true);
            i2c_read_blocking(&m_i2cInstance, READ_ADDRESS, &data, 1, stop);
        }

        void RTC::WriteRegister(Register&& reg, uint8_t& data, bool stop)
        {
            uint8_t write[2]= {(uint8_t)reg, data};
            i2c_write_blocking(&m_i2cInstance, WRITE_ADDRESS, write, 2, stop);
        }
    }
}
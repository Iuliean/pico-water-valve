#ifndef RTC_H
#define RTC_H

#include "hardware/i2c.h"
#include <cstdint>
#include "stdio.h"

namespace iu
{
    namespace Time
    {
        struct Date
        {
            uint8_t year;
            uint8_t month;
            uint8_t dotw;
            uint8_t day;
            uint8_t hour;
            uint8_t minute;
            uint8_t second;
        };
        
        class TimePoint
        {
        public:
            constexpr TimePoint(uint8_t hour, uint8_t minute)
                :m_point(0)
            {
                toTimePoint(hour, minute, 0);
            }
            
            TimePoint(const Date& date);

            inline bool operator>(const TimePoint& t)const
            {
                return m_point > t.m_point;
            }

            inline bool operator>=(const TimePoint& t)const
            {
                return m_point >= t.m_point;
            }

            inline bool operator<(const TimePoint& t)const
            {
                return m_point < t.m_point;
            }

            inline bool operator<=(const TimePoint& t)const
            {
                return m_point <= t.m_point;
            }

        private:
            constexpr void toTimePoint(uint8_t hour, uint8_t minute, uint8_t second)
            {
                m_point = hour * 3600 + minute * 60 + second;
            }
            size_t m_point;
        };

        class RTC
        {
        public:
            RTC(i2c_inst_t& i2c);

            void GetDate(Date& date);
            void SetDate(Date& date);
        private:
            enum class Register : uint8_t;

            void ReadRegister(Register&& reg, uint8_t& data, bool stop = true);
            void WriteRegister(Register&& reg, uint8_t& data, bool stop = true);

        private:
            i2c_inst_t& m_i2cInstance;
            static constexpr uint8_t READ_ADDRESS = 0x51;
            static constexpr uint8_t WRITE_ADDRESS = 0xd1;
            enum class Register: uint8_t
            {
                CTRL_STATUS1    = 0x00,
                CTRL_STATUS2    = 0x01,
                SECONDS         = 0x02,
                MINUTES         = 0x03,
                HOURS           = 0x04,
                DAYS            = 0x05,
                DOTW            = 0x06,
                CENTURY_MONTHS  = 0x07,
                YEARS           = 0x08,
                MINUTE_ALARM    = 0x09,
                HOUR_ALARM      = 0x0A,
                DAY_ALARM       = 0x0B,
                WEEKDAY_ALARM   = 0x0C,
                CLKOUT          = 0x0D,
                TIMER_CONTROL   = 0x0E,
                TIMER           = 0x0F
            };
        
        };
    }
}

#endif //RTC_H
#include "RTC.h"
#include "boards/pico.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include "stdio.h"
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <iostream>
#include "hardware/rtc.h"
#include "pico/time.h"
#include "pico/types.h"
#include "pico/util/datetime.h"
 
static constexpr uint8_t I2C_SDA = 0;
static constexpr uint8_t I2C_SCL = 1;
static constexpr uint8_t RELAY   = 16;
static constexpr int I2C_CLOCK_SPEED = 100000;

void init()
{

    
    stdio_init_all();
    i2c_init(&i2c0_inst, I2C_CLOCK_SPEED);
    
    gpio_init(RELAY);
    gpio_set_dir(RELAY, GPIO_OUT);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

}

void deinit()
{
    i2c_deinit(&i2c0_inst);
    gpio_deinit(RELAY);
}
int main()
{
    init();
    iu::Time::RTC rtc(i2c0_inst);
    iu::Time::Date date;
    
    using TimeAlarm = std::pair<iu::Time::TimePoint, iu::Time::TimePoint>;

    constexpr TimeAlarm TimeSheet[] = 
    {
        //Sunday
        {
            {0,0},
            {0,0}
        },

        //Monday
        {
            {17,55},
            {17,56}

        },

        //Tuesday
        {
            {17,57},
            {17,58}
        },

        //Wensday
        {
            {0,0},
            {0,0}
        },

        //Thursday
        {
            {0,0},
            {0,0}
        },

        //Friday
        {
            {0,0},
            {0,0}
        },

        //Saturday
        {
            {0,0},
            {0,0}           
        }

    };

    while(true)
    {
        rtc.GetDate(date);
        iu::Time::TimePoint t(date);
        
        printf("Y: %d M:%d Day:%d WeekDay:%d  %d:%d:%d\n",
        date.year, date.month, date.day, date.dotw, 
        date.hour, date.minute, date.second);

        auto& start = TimeSheet[date.dotw].first;
        auto& end   = TimeSheet[date.dotw].second;

        if(t > start && t < end)
        {
            gpio_put(RELAY, false);
        }
        else
        {
            gpio_put(RELAY, true);
        }
        sleep_ms(1000);
    }

    deinit();
    return 0;
}
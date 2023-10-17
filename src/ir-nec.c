#include "ir-nec.h"


uint8_t ir_nec_start  = 0;
uint16_t ir_nec_count = 0;
uint32_t ir_nec_data;
uint32_t ir_nec_safe_data;

uint8_t timerValue;
uint8_t msCount=0;
char pulseCount=0;

void ir_nec_timer_init(void)
{
    EA  = 1;
    ET0 = 1;
    TMOD |= 0x02; 
    TH0 = 0x00;
    TL0 = 0x00;
    TR0 = 1;
}

void ir_nec_signal_interrupt_init(void)
{
    EA  = 1; // 開啟硬體中斷機制
    EX1 = 1; // 開啟外部中斷 1
    IT1 = 1; // 設置 下緣觸發模式
}

void ir_nec_timer_interrupt_handler(void) __interrupt(1)
{
   // if(ir_nec_count<50)
        ir_nec_count++;
   
}

void ir_nec_signal_interrupt_handler(void) __interrupt(2)
{
   

    if (ir_nec_start) {
        if (ir_nec_count > 32) // 長度大於32bit
        {
            ir_nec_data  = 0;
            ir_nec_start = 1;
        }

        else if (ir_nec_count > 6) // bit長度大於六
        {
            ir_nec_data <<= 1;
            ir_nec_data |= 0x01;
        } else if (ir_nec_count > 3) // bit長度大於3
        {
            ir_nec_data <<= 1;
          //  ir_nec_data |=(uint32_t)1<<(31-ir_nec_count);
        }
        ir_nec_start++;
    } else {
        ir_nec_start = 1;
    }
    ir_nec_count = 0;
}

void ir_nec_init(void)
{
    ir_nec_timer_init();
    ir_nec_signal_interrupt_init();
}

uint8_t ir_nec_available(void)
{
    if (ir_nec_start == 34) {
        ir_nec_start = 0;
        ir_nec_safe_data = ir_nec_data;
        return 1;
    }
    return 0;
}

uint32_t ir_nec_get_code(void)
{
    return ir_nec_safe_data;
}

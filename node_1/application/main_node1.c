#include "can_driver.h"
#include <stm32l4xx.h>  

uint8_t 		leds_data[5] = {0x1,0x2,0x4,0x8,0x10};
uint8_t			led_index = 1;	

uint32_t 	rx_id;
bool 		rx_ext;
bool 		rx_rtr;
uint8_t 	rx_fmi;
uint8_t 	rx_len;
uint8_t 	rx_data[8];

uint32_t    tx_id = 0x123;
bool        exid = true;
bool        rtr = false;
uint8_t     dlc = 1;

//========================================================================
//                    can controle parametres :
//========================================================================
#define TTCM_ENABLE                         true
#define ABOM_ENABLE                         true
#define AWUM_ENABLE                         true
#define NART_ENABLE                         true
#define RFLM_ENABLE                         true
#define TXFP_ENABLE                         true

	
#define TTCM_DISABLE                        false
#define ABOM_DISABLE                        false
#define AWUM_DISABLE                        false
#define NART_DISABLE                        false
#define RFLM_DISABLE                        false
#define TXFP_DISABLE                        false

#define CAN_BTR_SJW_1tq                    ((uint32_t) 1)
#define CAN_BTR_BS1_13tq                   ((uint32_t) 13)
#define CAN_BTR_BS2_2tq                    ((uint32_t) 2)
#define CAN_BTR_PRESCALER                  ((uint32_t) 1)

#define SILM_ENABLE                         true   
#define LBKM_ENABLE                         true   
	
#define SILM_DISABLE                        false
#define LBKM_DISABLE                        false
	
//========================================================================
//                    filter pramaters :
//========================================================================

#define FILTER_BANK_NUMBER                  0

#define FILTER_SCALE_16BIT                  false
#define FILTER_SCALE_32BIT                  true

#define FILTER_MODE_MASK                    false
#define FILTER_MODE_LIST                    true

#define FIFO0                               0
#define FIFO1                               1

#define FILTER_ENABLE                       true
#define FILTER_DISABLE                      false


#define FILTER_REGISTER_1                           (0x00000120 << 3) | (1 << 2)       
#define FILTER_REGISTER_2                           (0x1FFFFFF0 << 3) | (1 << 2)     


void 		sw_init(void);
void 		CAN1_RX0_IRQHandler(void);
void 		CAN1_RX1_IRQHandler(void);
void        EXTI0_IRQHandler(void);


int main(void)
{
	sw_init();
    can_gpio_init(CAN1, CAN_GPIO_PB8_PB9);
	
    can_init( 
    CAN1,                                   // CAN controller selection
    TTCM_DISABLE,                           // Time Triggered Communication Mode disabled
    ABOM_DISABLE,                           // Automatic Bus-Off Management disabled
    AWUM_DISABLE,                           // Automatic WakeUp Mode disabled
    NART_DISABLE,                           // No Automatic Retransmission disabled
    RFLM_ENABLE,                            // Receive FIFO Locked Mode enabled
    TXFP_DISABLE,                           // Transmit FIFO Priority disabled
    CAN_BTR_SJW_1tq,                        // Synchronization Jump Width 
    CAN_BTR_BS1_13tq,                       // Bit Segment 1
    CAN_BTR_BS2_2tq,                        // Bit Segment 2
    CAN_BTR_PRESCALER,                      // Prescaler for setting CAN speed
    LBKM_DISABLE,                            // Loopback mode disabled 
    SILM_DISABLE                            // Silent mode disabled
    );

		
		
    can_filter_init(
        CAN1,                                 // CAN controller selection
        FILTER_BANK_NUMBER,                   // Filter bank number
        FILTER_SCALE_32BIT,                   // 32-bit filter scale
        FILTER_MODE_MASK,                     // Mask mode
        FILTER_REGISTER_1,                    // Filter ID (R1)
        FILTER_REGISTER_2,                    // Filter mask (just id = 0x12x) (R2)
        FIFO1,                                // Assign filter to FIFO1
        FILTER_ENABLE                         // Enable filter
    );
	
    while (1)
    {
		;
    }
}

void sw_init(void)
{
	RCC-> AHB2ENR |= RCC_AHB2ENR_GPIOCEN;       
	
	GPIOC->MODER &= ~GPIO_MODER_MODE13;	    

	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; 
	
	SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;      
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;   

	EXTI->FTSR1 |= EXTI_FTSR1_FT13;  
	EXTI->IMR1  |= EXTI_IMR1_IM13;   
	
	NVIC_EnableIRQ(EXTI15_10_IRQn);

}

void EXTI15_10_IRQHandler(void)
{
    if ((EXTI->PR1 & EXTI_PR1_PIF13) != 0)
    {
        can_transmit(CAN1, tx_id, exid, rtr, dlc, &leds_data[led_index]);
        
        led_index++;
        if (led_index >= 5U) 
        {
            led_index = 0U;
        }

        EXTI->PR1 |= EXTI_PR1_PIF13; 
    }
}

void CAN1_RX0_IRQHandler(void)
{
	can_receive(CAN1, 0, true, &rx_id, &rx_ext, &rx_rtr, &rx_fmi, &rx_len, rx_data);
}

void CAN1_RX1_IRQHandler(void)
{
	can_receive(CAN1, 1, true, &rx_id, &rx_ext, &rx_rtr, &rx_fmi, &rx_len, rx_data);
}

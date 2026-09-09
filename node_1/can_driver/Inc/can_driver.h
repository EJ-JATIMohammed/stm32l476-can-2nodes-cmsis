#ifndef CAN_DRIVER_H
#define CAN_DRIVER_H

#include <stdint.h>
#include <stm32l4xx.h>
#include <stdbool.h>

//===========================================================================================
//                                 Defines CAN Marcos:
//===========================================================================================
/*<-------------------------- can options selections status : ----------------------------->*/
#define CAN_GPIO_InitStatus_FAILED       ((uint8_t) 0x01U)      /*< CANx GPIO failed >*/
#define CAN_GPIO_InitStatus_SUCCESS      ((uint8_t) 0x00U)      /*< CANx GPIO success >*/

/*<---------------------------------- can init status : ----------------------------------->*/
#define CAN_InitStatus_FAILED            ((uint8_t) 0x01U)      /*< CANx inialization failed >*/
#define CAN_InitStatus_SUCCESS           ((uint8_t) 0x00U)      /*< CANx initialized success >*/

/*<-------------------------------- transmission status : --------------------------------->*/
#define TRANSMISSION_FAILED              ((int) -1)             /*< CANx inialization failed >*/

/*<------------------------------ filter configuration status : --------------------------->*/
#define FILTER_CONFIG_ERROR              ((int) -1)             /*< Filter configuration faild >*/
#define FILTER_CONFIG_SUCCESS            ((int) 0)              /*< Filter configured successfully >*/

/*<------------------------------ Reception status : --------------------------->*/
#define RECEPTION_FAILED                 ((int) -1)             /*< Filter configuration faild >*/
#define RECEPTION_SUCCESS                ((int) 0)              /*< Filter configured successfully >*/
	
//===========================================================================================
//                            CAN Structers And Enumerations:
//===========================================================================================
/*<----------------------------- CAN GPIO Options selection enumertion : ------------------>*/
typedef enum
{
	CAN_GPIO_PA11_PA12 = 0,                       /*<Rx = PA11, Tx = PA12>*/
	CAN_GPIO_PB8_PB9 = 1,                         /*<Rx = PB8, Tx = PB9>*/
	CAN_GPIO_PD0_PD1 = 2                          /*<Rx = PD0, Tx = PD1>*/
	
}CAN_GPIO_Option;

/*<----------------------------- CAN MailBox Options selection enumertion : ---------------->*/
typedef enum
{
	CAN_MailBox_0 = 0,                            /*<MailBox 0 is selected>*/
	CAN_MailBox_1 = 1,                            /*<MailBox 1 is selected>*/
	CAN_MailBox_2 = 2,                            /*<MailBox 2 is selected>*/
	CAN_NoMailBox = 3                             /*<MailBox ERROR selection>*/
	
}CAN_MailBox_Option;

//===================================================================================
//                             CAN Driver Functions :
//===================================================================================

int can_gpio_init(CAN_TypeDef* CANx,            /*<CANx Selection>*/
	                CAN_GPIO_Option option        /*<for CAN ports options selection>*/
                  );


int can_init(CAN_TypeDef* CANx,                 /*<CAN protocol contraler Selection>*/
	
             /*-------------- CAN functional options configuration: -----------------------------*/
             bool TTCM,                         /*<Time triggered communication mode>*/
						 bool ABOM,                         /*< Automatic bus-off management>*/
						 bool AWUM,                         /*< Automatic wakeup mode>*/
						 bool NART,                         /*< No automatic retransmission>*/
             bool RFLM,                         /*< Receive FIFO locked mode>*/
						 bool TXFP,                         /*< Transmit FIFO priority>*/

             /*-------------- Bit timing configuration: -----------------------------------------*/						 
             uint32_t SJW,                      /*< Synchronization Jump Width>*/
						 uint32_t BS1,                      /*< Bit segment 1>*/
						 uint32_t BS2,                      /*< Bit segment 2>*/
             uint32_t PRESCALER,                /*< Baud rate prescaler>*/ 

             /*-------------- Testing configuration: -------------------------------------------*/						 
             bool LOOPBACK,                     /*< for enable or Disable CAN Loop back Mode>*/
						 bool SILENT                        /*< for enable or Disable CAN Silent Mode>*/
							); 
						 

int can_transmit(CAN_TypeDef *CANx,             /*<CAN protocol contraler Selection>*/
	
	              /*-------------- Transmission parametrs: ---------------------------------------*/
	               uint32_t ID,                   /*< Message identifier>*/   
                 bool EXT,                      /*< Used for extend message ID>*/ 
								 bool RTR,                      /*< Remote transmission request (This node waits data from another node)>*/ 
                 uint8_t DLC,                   /*< Data length>*/ 
								 uint8_t *DATA);                /*< DATA Transmition>*/ 
								 

int can_filter_init(CAN_TypeDef* CANx,          /*< CAN protocol controller selection >*/
    
                    /*-------------- Filter parameters: --------------------------------------------*/
                    uint32_t  filter_number,    /*< Filter bank number >*/
                    bool      scale_32bit,      /*< Filter scale(32/16) bit >*/
                    bool      id_list_mode,     /*< Filter mode (identifier list mode/ false = mask mode) >*/
                    uint32_t  filter_register1, /*< First filter register R1>*/
                    uint32_t  filter_register2, /*< Second filter register R2 >*/
                    uint32_t  fifo,             /*< FIFO assignment ( 0 : FIFO0, 1 : FIFO1) >*/
                    bool      enable);          /*< Filter activation (enable / disable) >*/


												 
int can_receive(CAN_TypeDef* CANx,              /*< CAN protocol controller selection >*/

                /*-------------------------- Reception parameters: ---------------------------------*/
                uint8_t fifo,                   /*< FIFO number to read the message>*/
                bool release,                   /*< Release the FIFO automatically after reading >*/

                /*----------------------------- Data Reception : -----------------------------------*/
                uint32_t *id,                   /*< Identifier pointer>*/
                bool     *ext,                  /*< Indicating if ID is extended or standard >*/
                bool     *rtr,                  /*< Indicating if message is a remote request >*/
                uint8_t  *fmi,                  /*< Filter match index pointer >*/
                uint8_t  *length,               /*< Data length >*/
                uint8_t  *data);                /*< Pointer for read data >*/


#endif
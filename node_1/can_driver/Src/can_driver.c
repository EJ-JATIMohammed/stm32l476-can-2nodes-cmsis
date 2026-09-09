#include <can_driver.h>


void can_gpio_portA(void)
{
//================================================================
//                 CLOCK Configuration
//================================================================
	RCC->AHB2ENR  |= RCC_AHB2ENR_GPIOAEN;                             // Enable clock for GPIA

//================================================================
//                 Pins Configuration
//================================================================
	/*-------------- Mode Configuration ---------------------------*/
    GPIOA->MODER &= ~(GPIO_MODER_MODE11 | GPIO_MODER_MODE12); 
    GPIOA->MODER |=  (GPIO_MODER_MODE11_1 | GPIO_MODER_MODE12_1);   // PA11 and PA12 set to Alternate fct mode

	/*--------------- Alternate fct Configuration ------------------*/
    GPIOA->AFR[1] &= ~(GPIO_AFRH_AFSEL11);                         
    GPIOA->AFR[1] |=  (GPIO_AFRH_AFSEL11_0 | GPIO_AFRH_AFSEL11_3);  // Set PA11 to FA9 (CAN1_RX)

    GPIOA->AFR[1] &= ~(GPIO_AFRH_AFSEL12);                         
    GPIOA->AFR[1] |=  (GPIO_AFRH_AFSEL12_0 | GPIO_AFRH_AFSEL12_3);  // Set PA12 to FA9 (CAN1_TX)

	/*-------------------- Speed Configuration ---------------------*/	
    GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED11 | GPIO_OSPEEDR_OSPEED12);
    GPIOA->OSPEEDR |=  (GPIO_OSPEEDR_OSPEED11_1 | GPIO_OSPEEDR_OSPEED12_1); // Set PA11 and PA12 to High speed

	/*--------------- Internal Resistor Configuration --------------*/	
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD11 | GPIO_PUPDR_PUPD12);      // No pull-up / pull-down on PA12
    GPIOA->PUPDR |=  GPIO_PUPDR_PUPD11_0;                          // Enable pull-up on PA11 (CAN1_RX)
}

void can_gpio_portB(void)
{
//================================================================
//                 CLOCK Configuration
//================================================================
	RCC->AHB2ENR  |= RCC_AHB2ENR_GPIOBEN;      // Enable clock for GPIOB

//================================================================
//                 Pins Configuration
//================================================================
	/*-------------- Mode Configuration ---------------------------*/
    GPIOB->MODER &= ~(GPIO_MODER_MODE8 | GPIO_MODER_MODE9); 
    GPIOB->MODER |=  (GPIO_MODER_MODE8_1 | GPIO_MODER_MODE9_1);   // PB8 and PB9 set to Alternate fct mode

	/*--------------- Alternate fct Configuration ------------------*/
    GPIOB->AFR[1] &= ~(GPIO_AFRH_AFSEL8);                         
    GPIOB->AFR[1] |=  (GPIO_AFRH_AFSEL8_0 | GPIO_AFRH_AFSEL8_3);  // Set PB8 to AF9 (CAN1_RX)

    GPIOB->AFR[1] &= ~(GPIO_AFRH_AFSEL9);                         
    GPIOB->AFR[1] |=  (GPIO_AFRH_AFSEL9_0 | GPIO_AFRH_AFSEL9_3);  // Set PB9 to AF9 (CAN1_TX)12
	/*-------------------- Speed Configuration ---------------------*/	
    GPIOB->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED8 | GPIO_OSPEEDR_OSPEED9);
    GPIOB->OSPEEDR |=  (GPIO_OSPEEDR_OSPEED8_1 | GPIO_OSPEEDR_OSPEED9_1); // Set PB8 and PB9 to High speed

	/*--------------- Internal Resistor Configuration --------------*/	
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD8 | GPIO_PUPDR_PUPD9);        // No pull-up / pull-down on PB9
    GPIOB->PUPDR |=  GPIO_PUPDR_PUPD8_0;                           // Enable pull-up on PB8 (CAN1_RX)
}

void can_gpio_portD(void)
{
//================================================================
//                 CLOCK Configuration :
//================================================================
	RCC->AHB2ENR  |= RCC_AHB2ENR_GPIODEN;                            // Enable clock for GPIOD
	
//================================================================
//                 Pins Configuration :
//================================================================
	/*-------------- Mode Configuration ---------------------------*/
    GPIOD->MODER &= ~(GPIO_MODER_MODE0 | GPIO_MODER_MODE1); 
    GPIOD->MODER |=  (GPIO_MODER_MODE0_1 | GPIO_MODER_MODE1_1);   // PD0 and PD1 set to Alternate fct mode

	/*--------------- Alternate fct Configuration ------------------*/
    GPIOD->AFR[0] &= ~(GPIO_AFRL_AFSEL0);                         
    GPIOD->AFR[0] |=  (GPIO_AFRL_AFSEL0_0 | GPIO_AFRL_AFSEL0_3);  // Set PD0 to AF9 (CAN1_RX)
	
    GPIOD->AFR[0] &= ~(GPIO_AFRL_AFSEL1);                         
    GPIOD->AFR[0] |=  (GPIO_AFRL_AFSEL1_0 | GPIO_AFRL_AFSEL1_3);  // Set PD1 to AF9 (CAN1_TX)

	/*-------------------- Speed Configuration ---------------------*/	
    GPIOD->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED0 | GPIO_OSPEEDR_OSPEED1);
    GPIOD->OSPEEDR |=  (GPIO_OSPEEDR_OSPEED0_1 | GPIO_OSPEEDR_OSPEED1_1); // Set PD0 and PD1 to High speed

	/*--------------- Internal Resistor Configuration --------------*/	
    GPIOD->PUPDR &= ~(GPIO_PUPDR_PUPD0 | GPIO_PUPDR_PUPD1);       // No pull-up / pull-down on PD1
    GPIOD->PUPDR |=  GPIO_PUPDR_PUPD0_0;                          // Enable pull-up on PD0 (CAN1_RXD
}



int can_gpio_init(CAN_TypeDef* CANx,                  /*< CANx contraler Selection>*/        
	                CAN_GPIO_Option Option)             /*<for CAN ports options selection>*/
{
	int init_Status = CAN_GPIO_InitStatus_FAILED;       /*< Indicates the inialization satus>*/
	
	if(CANx == CAN1)
	{
    //================================================================
    //                 CAN CLOCK Configuration
    //================================================================
		RCC->APB1ENR1 |= RCC_APB1ENR1_CAN1EN;            /*< Enable clock for CAN1>*/
		
    //================================================================
    //                 CAN GPIO Configuration
    //================================================================
		
		switch(Option)
		{
			case CAN_GPIO_PA11_PA12:                       /*< CAN options portA inialization>*/
				can_gpio_portA();
		    init_Status = CAN_GPIO_InitStatus_SUCCESS; 
				break;
		
			case CAN_GPIO_PB8_PB9:                         /*< CAN options portB inialization>*/
				can_gpio_portB(); 
		    init_Status = CAN_GPIO_InitStatus_SUCCESS; 
				break;
			
			case CAN_GPIO_PD0_PD1:                         /*< CAN options portD inialization>*/
				can_gpio_portD();
		    init_Status = CAN_GPIO_InitStatus_SUCCESS; 
				break;
			
			default:                                      /*< CAN failde option has selected>*/
				init_Status = CAN_GPIO_InitStatus_FAILED;
		}
		
    NVIC_EnableIRQ(CAN1_RX0_IRQn); 
    NVIC_EnableIRQ(CAN1_RX1_IRQn); 

    CANx->IER |= (CAN_IER_FMPIE0 | CAN_IER_FMPIE1); 	
	}
	/*
	else if(CANx == CAN2)
	{
		RCC->APB1ENR1 |= RCC_APB1ENR1_CAN2EN;      // Enable clock for CAN2	
		...
	}
	*/

	
	return init_Status;
}


int can_init(CAN_TypeDef* CANx,        /*<CAN contraler Selection>*/
	
             /*-------------- CAN functional options configuration: -----------------------------*/
             bool TTCM,                /*<Time triggered communication mode>*/
						 bool ABOM,                /*< Automatic bus-off management>*/
						 bool AWUM,                /*< Automatic wakeup mode>*/
						 bool NART,                /*< No automatic retransmission>*/
             bool RFLM,                /*< Receive FIFO locked mode>*/
						 bool TXFP,                /*< Transmit FIFO priority>*/

             /*-------------- Bit timing configuration: -----------------------------------------*/						 
             uint32_t SJW,             /*< Synchronization Jump Width>*/
						 uint32_t BS1,             /*< Bit segment 1>*/
						 uint32_t BS2,             /*< Bit segment 2>*/
             uint32_t PRESCALER,       /*< Baud rate prescaler>*/ 

             /*-------------- Testing configuration: -------------------------------------------*/						 
             bool LOOPBACK,            /*< for enable or Disable CAN Loop back Mode>*/
						 bool SILENT               /*< for enable or Disable CAN Silent Mode>*/
							)
{ 
	int init_Status = CAN_InitStatus_FAILED;
	
	/* < Protect bit timing parameters > */
	if ((SJW < 1U || SJW > 4U)      ||
			(BS1 < 1U || BS1 > 16U)     ||
			(BS2 < 1U || BS2 > 8U)      ||
			(PRESCALER < 1U || PRESCALER > 1024U) )
	{
			return CAN_InitStatus_FAILED;
	}

	
//===================================================================
//           Start CAN initialization settings
//===================================================================
	 
 /* <Exit sleep mode> */
	CANx->MCR &= ~CAN_MCR_SLEEP;
	
 /* <Wait until sleep mode is exited> */
	while((CANx->MSR & CAN_MSR_SLAK) == CAN_MSR_SLAK)
	{
	}
	
 /* <Enter initialization mode> */
	CANx->MCR |= CAN_MCR_INRQ;
	
 /* <Wait until initialization mode is ready> */
	while((CANx->MSR & CAN_MSR_INAK) != CAN_MSR_INAK)
	{
	}
//===================================================================
//           Bit timing configuration
//===================================================================
	CANx->BTR = 0U;
	CANx->BTR   |= ((SJW -1) << CAN_BTR_SJW_Pos)        /* <Set Synchronization Jump Width> */
	            |((BS1 - 1) << CAN_BTR_TS1_Pos)         /* <Set Bit Segment 1> */
	            |((BS2 - 1) << CAN_BTR_TS2_Pos)         /* <Set Bit Segment 2> */
	            |((PRESCALER - 1) << CAN_BTR_BRP_Pos);  /* <Set the Baud Rate Prescaler> */

//===================================================================
//           CAN functional options settings :
//===================================================================
 /* <Time-Triggered Communication Mode> */
	if(TTCM)
	{
		CANx->MCR |= CAN_MCR_TTCM;
	}
	else
	{
		CANx->MCR &= ~CAN_MCR_TTCM;
	}
	
 /* <Automatic Bus-Off Management> */
	if(ABOM)
	{
		CANx->MCR |= CAN_MCR_ABOM;
	}
	else
	{
		CANx->MCR &= ~CAN_MCR_ABOM;
	}
	
 /* <Automatic Wake-Up Mode> */
		if(AWUM)
	{
		CANx->MCR |= CAN_MCR_AWUM;
	}
	else
	{
		CANx->MCR &= ~CAN_MCR_AWUM;
	}
	
 /* <No Automatic Retransmission> */
		if(NART)
	{
		CANx->MCR |= CAN_MCR_NART;
	}
	else
	{
		CANx->MCR &= ~CAN_MCR_NART;
	}
	
 /* <Receive FIFO Locked Mode> */
		if(RFLM)
	{
		CANx->MCR |= CAN_MCR_RFLM;
	}
	else
	{
		CANx->MCR &= ~CAN_MCR_RFLM;
	}
	
 /* <Transmit FIFO Priority> */
		if(TXFP)
	{
		CANx->MCR |= CAN_MCR_TXFP;
	}
	else
	{
		CANx->MCR &= ~CAN_MCR_TXFP;
	}

//===================================================================
//           CAN testing modes configuration :
//===================================================================
 /* <Loopback mode configuration> */
	if(LOOPBACK)
	{
		CANx->BTR |= CAN_BTR_LBKM;
	}
	else
	{
		CANx->BTR &= ~CAN_BTR_LBKM;
	}
	
 /* <Silent mode configuration> */
	if(SILENT)
	{
		CANx->BTR |= CAN_BTR_SILM;
	}
	else
	{
		CANx->BTR &= ~CAN_BTR_SILM;
	}

//===================================================================
//           Exit CAN initialization mode :
//===================================================================
 /* <Exit initialization mode> */
	CANx->MCR &= ~CAN_MCR_INRQ;
	
 /* <Wait until initialization mode is exited> */
	while((CANx->MSR & CAN_MSR_INAK) == CAN_MSR_INAK)
	{
	}
	init_Status = CAN_InitStatus_SUCCESS;
	
	return init_Status;
}

int can_transmit(CAN_TypeDef *CANx,       /*<CAN protocol contraler Selection>*/
	               uint32_t ID,             /*< Message identifier>*/   
                 bool EXT,                /*< Used for extend message ID>*/ 
								 bool RTR,                /*< Remote transmission request (This node waits data from another node)>*/ 
                 uint8_t DLC,             /*< Data length>*/ 
								 uint8_t *DATA)           /*< DATA Transmition>*/ 
{
//===================================================================
//           Searching for empty MailBox :
//===================================================================
	CAN_MailBox_Option empty_mailbox = CAN_NoMailBox;
	
	if((CANx->TSR & CAN_TSR_TME0) == CAN_TSR_TME0)
	{
		empty_mailbox = CAN_MailBox_0;
	}
	else if((CANx->TSR & CAN_TSR_TME1) == CAN_TSR_TME1)
	{
		empty_mailbox = CAN_MailBox_1;
	}
	else if((CANx->TSR & CAN_TSR_TME2) == CAN_TSR_TME2)
	{
		empty_mailbox = CAN_MailBox_2;
	}
	else
	{
		return TRANSMISSION_FAILED;           
	}
	
//===================================================================
//           MailBox Transmission configuration :
//===================================================================
	CAN_TxMailBox_TypeDef* Tx = &(CANx->sTxMailBox[empty_mailbox]); /*< Using pointer is important>*/
  
 /* < Set the Message ID> */
	Tx->TIR = 0;
	
	if(!EXT)                              /*<Standard ID>*/
	{
		  Tx->TIR &= ~CAN_TI0R_IDE;
			Tx->TIR |= (ID << CAN_TI0R_STID_Pos);
	}
	else                                  /*<Extend ID>*/
	{
		  Tx->TIR |= CAN_TI0R_IDE;
			Tx->TIR |= (ID << CAN_TI0R_EXID_Pos);
	}

 /* < Set the RTR> */
	if(RTR)
	{
			Tx->TIR |= CAN_TI0R_RTR;
	}
	else 
	{
			Tx->TIR &= ~CAN_TI0R_RTR;
	}
	
//	Tx->TIR |= (RTR << CAN_TI0R_RTR_Pos);

 /* < Fixing the data lenght (DLC)> */
	Tx->TDTR &= ~CAN_TDT0R_DLC;	   
	Tx->TDTR |= (DLC << CAN_TDT0R_DLC_Pos);	
	
 /* < Load data transmition > */
	if(!RTR)                      
	{
     Tx->TDLR = 0;
     Tx->TDHR = 0;
	
	  for(uint32_t i = 0; i < DLC ; i++)
	  {
		  if( i < 4)
		  {
				  Tx ->TDLR |= (*(DATA+i) << (8*i)); 
		  }
		  else
		  {
				  Tx ->TDHR |= (*(DATA+i) << (8*(i-4)));
		  }
	  }
  }

//===================================================================
//           Transmit message :
//===================================================================
 /* < Start transmition > */ 
	Tx-> TIR |= CAN_TI0R_TXRQ;
	
 /* < Wait until for the transmission is complete > */ 
   if(empty_mailbox == CAN_MailBox_0)
	 {
	 	 while((CANx->TSR & CAN_TSR_RQCP0) == 0U)
		 {
		 }
	 }
	 else if(empty_mailbox == CAN_MailBox_1)
	 {
	 	while((CANx->TSR & CAN_TSR_RQCP1) == 0U)
		{
		}
	 }
	 else if(empty_mailbox == CAN_MailBox_2)
	 {
	 	while((CANx->TSR & CAN_TSR_RQCP2) == 0U)
		{
		}
	 }
	 
	return empty_mailbox;
}


int can_filter_init(CAN_TypeDef* CANx,          /*< CAN protocol controller selection >*/
	
                    /*-------------- Filter parameters: --------------------------------------------*/
                    uint32_t  filter_number,    /*< Filter bank number >*/
                    bool      scale_32bit,      /*< Filter scale(32/16) bit >*/
                    bool      id_list_mode,     /*< Filter mode (identifier list mode/ false = mask mode) >*/
                    uint32_t  filter_register1, /*< First filter register R1>*/
                    uint32_t  filter_register2, /*< Second filter register R2 >*/
                    uint32_t  fifo,             /*< FIFO assignment ( 0 : FIFO0, 1 : FIFO1) >*/
                    bool      enable)           /*< Filter activation (enable / disable) >*/

{
		/*< Securazing the parameters>*/
	if((CANx != CAN1) || (filter_number > 27) || (fifo > 1))
	{
		return FILTER_CONFIG_ERROR;
	}
	
	/*< Enter in fitre inialization mode>*/
	CANx->FMR |= CAN_FMR_FINIT;   
	
  /*< Set up Scale>*/
	CANx->FS1R = (CANx->FS1R & ~(1 << filter_number)) | (scale_32bit << filter_number);

  /*< Set up Mode>*/
	CANx->FM1R = (CANx->FM1R & ~(1 << filter_number)) | (id_list_mode << filter_number);
	
  /*< Id setting and Mask settting>*/
	CANx->sFilterRegister[filter_number].FR1 = 0U;//Reset Register R1
	CANx->sFilterRegister[filter_number].FR2 = 0U;//Reset Register R2
	
	CANx->sFilterRegister[filter_number].FR1 = filter_register1;  
	CANx->sFilterRegister[filter_number].FR2 = filter_register2;  
	
 /*< Set up the Assegenement>*/
	CANx->FFA1R = (CANx->FFA1R & ~(1 << filter_number)) | (fifo << filter_number);
	
 /*< Activate the filter>*/
	CANx->FA1R = (CANx->FA1R & ~(1 << filter_number)) | (enable << filter_number);
	
 /*< exsit the inialization mode>*/
	CANx->FMR &= ~CAN_FMR_FINIT;
	
	return FILTER_CONFIG_SUCCESS;
}


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
                uint8_t  *data)                 /*< Pointer for read data >*/
{
	/* < Looking for message > */
	if(fifo == 0U)
	{
		if((CANx->RF0R & CAN_RF0R_FMP0) == 0U)
		{
			return RECEPTION_FAILED; 
    }			
	}
	else if(fifo == 1U)
	{
		if((CANx->RF1R & CAN_RF1R_FMP1) == 0U)
		{
			return RECEPTION_FAILED; 
    }		
	}
	else
	{
		return RECEPTION_FAILED;
	}
	
	/* < Get FIFO associated message > */
	CAN_FIFOMailBox_TypeDef* rx = &(CANx->sFIFOMailBox[fifo]);
	
	*ext = ((rx->RIR & CAN_RI0R_IDE) >> CAN_RI0R_IDE_Pos); 
	*rtr = ((rx->RIR & CAN_RI0R_RTR) >> CAN_RI0R_RTR_Pos); 
	
	if(*ext)
	{
		*id = (rx->RIR >> 3) & 0x1FFFFFFF;
	}
	else
	{
		*id = (rx->RIR >> 21) & 0x7FF;
	}
	
	*length = ((rx->RDTR & CAN_RDT0R_DLC) >> CAN_RDT0R_DLC_Pos); 
	*fmi = ((rx->RDTR & CAN_RDT0R_FMI) >> CAN_RDT0R_FMI_Pos); 
	
	/*< Read Data>*/
	if(0<= *length&& *length <= 8)
	{
		for(uint8_t i = 0; i < *length; i++)
		{
			if (i < 4)
			{
				data[i] = (rx->RDLR >> (8 * i)) & 0xFF;
			}
			else
			{
				data[i] = (rx->RDHR >> (8 * (i - 4))) & 0xFF;
			}
		}
  }
	
	/* < Release FIFO > */
	if(release == true)
	{
		if(fifo == 0)
		{
			CANx->RF0R |= CAN_RF0R_RFOM0;
		}
		else
		{
			CANx->RF1R |= CAN_RF1R_RFOM1;
		}
	}

	return RECEPTION_SUCCESS;
}


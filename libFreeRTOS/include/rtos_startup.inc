#ifdef __cplusplus
  #error "This file must be included from a C source file. extern C {} will not work."
#endif

#warning OLD STARTUP CODE, DO NOT USE!

#include "types.h"

/* init value for the stack pointer. defined in linker script */
extern void _sstack;
extern void _estack;
WaitResponseEx

void vPortSVCHandler();
void xPortPendSVHandler();
void xPortSysTickHandler();

// write from cmsis

/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define WEAK __attribute__ ((weak))



/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Reset_Handler(void) __attribute__((__interrupt__));
extern int main(void);
void __Init_Data(void);

/*******************************************************************************
*
*            Forward declaration of the default fault handlers.
*
*******************************************************************************/
void WEAK Reset_Handler(void);             // Reset Handler(void);
void WEAK NMI_Handler(void);               // NMI Handler(void);
void WEAK HardFault_Handler(void);         // Hard Fault Handler(void);
void WEAK MemManage_Handler(void);         // MPU Fault Handler(void);
void WEAK BusFault_Handler(void);          // Bus Fault Handler(void);
void WEAK UsageFault_Handler(void);        // Usage Fault Handler(void);
//void WEAK SVC_Handler(void);               // SVCall Handler(void);
void WEAK DebugMon_Handler(void);          // Debug Monitor Handler(void);
//void WEAK PendSV_Handler(void);            // PendSV Handler(void);
//void WEAK SysTick_Handler(void);           // SysTick Handler(void);

// External Interrupts
void WEAK WDT_IRQHandler(void);            // 16: Watchdog Timer
void WEAK TIMER0_IRQHandler(void);         // 17: Timer0
void WEAK TIMER1_IRQHandler(void);         // 18: Timer1
void WEAK TIMER2_IRQHandler(void);         // 19: Timer2
void WEAK TIMER3_IRQHandler(void);         // 20: Timer3
void WEAK UART0_IRQHandler(void);          // 21: UART0
void WEAK UART1_IRQHandler(void);          // 22: UART1
void WEAK UART2_IRQHandler(void);          // 23: UART2
void WEAK UART3_IRQHandler(void);          // 24: UART3
void WEAK PWM1_IRQHandler(void);           // 25: PWM1
void WEAK I2C0_IRQHandler(void);           // 26: I2C0
void WEAK I2C1_IRQHandler(void);           // 27: I2C1
void WEAK I2C2_IRQHandler(void);           // 28: I2C2
void WEAK SPI_IRQHandler(void);            // 29: SPI
void WEAK SSP0_IRQHandler(void);           // 30: SSP0
void WEAK SSP1_IRQHandler(void);           // 31: SSP1
void WEAK PLL0_IRQHandler(void);           // 32: PLL0 Lock (Main PLL)
void WEAK RTC_IRQHandler(void);            // 33: Real Time Clock
void WEAK EINT0_IRQHandler(void);          // 34: External Interrupt 0
void WEAK EINT1_IRQHandler(void);          // 35: External Interrupt 1
void WEAK EINT2_IRQHandler(void);          // 36: External Interrupt 2
void WEAK EINT3_IRQHandler(void);          // 37: External Interrupt 3
void WEAK ADC_IRQHandler(void);            // 38: A/D Converter
void WEAK BOD_IRQHandler(void);            // 39: Brown-Out Detect
void WEAK USB_IRQHandler(void);            // 40: USB
void WEAK CAN_IRQHandler(void);            // 41: CAN
void WEAK DMA_IRQHandler(void);            // 42: General Purpose DMA
void WEAK I2S_IRQHandler(void);            // 43: I2S
void WEAK ENET_IRQHandler(void);           // 44: Ethernet
void WEAK RIT_IRQHandler(void);            // 45: Repetitive Interrupt Timer
void WEAK MCPWM_IRQHandler(void);          // 46: Motor Control PWM
void WEAK QEI_IRQHandler(void);            // 47: Quadrature Encoder Interface
void WEAK PLL1_IRQHandler(void);           // 48: PLL1 Lock (USB PLL)

/* Private functions ---------------------------------------------------------*/
/******************************************************************************
*
* The minimal vector table for a Cortex M3.  Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
*
******************************************************************************/

__attribute__ ((section(".isr_vector")))
void (* const g_pfnVectors[])(void) =
{
  &_estack,                   /* The initial stack pointer */
	Reset_Handler,             // Reset Handler,
	NMI_Handler,               // NMI Handler,
	HardFault_Handler,         // Hard Fault Handler,
	MemManage_Handler,         // MPU Fault Handler,
	BusFault_Handler,          // Bus Fault Handler,
	UsageFault_Handler,        // Usage Fault Handler,
	0, 						   // CRC
	0,                         // Reserved
	0,                         // Reserved
	0,                         // Reserved
	vPortSVCHandler,               // SVCall Handler,
	DebugMon_Handler,          // Debug Monitor Handler,
	0,                         // Reserved
	xPortPendSVHandler,            // PendSV Handler,
	xPortSysTickHandler,          // SysTick Handler,

	// External Interrupts
	WDT_IRQHandler,            // 16: Watchdog Timer
	TIMER0_IRQHandler,         // 17: Timer0
	TIMER1_IRQHandler,         // 18: Timer1
	TIMER2_IRQHandler,         // 19: Timer2
	TIMER3_IRQHandler,         // 20: Timer3
	UART0_IRQHandler,          // 21: UART0
	UART1_IRQHandler,          // 22: UART1
	UART2_IRQHandler,          // 23: UART2
	UART3_IRQHandler,          // 24: UART3
	PWM1_IRQHandler,           // 25: PWM1
	I2C0_IRQHandler,           // 26: I2C0
	I2C1_IRQHandler,           // 27: I2C1
	I2C2_IRQHandler,           // 28: I2C2
	SPI_IRQHandler,            // 29: SPI
	SSP0_IRQHandler,           // 30: SSP0
	SSP1_IRQHandler,           // 31: SSP1
	PLL0_IRQHandler,           // 32: PLL0 Lock (Main PLL)
	RTC_IRQHandler,            // 33: Real Time Clock
	EINT0_IRQHandler,          // 34: External Interrupt 0
	EINT1_IRQHandler,          // 35: External Interrupt 1
	EINT2_IRQHandler,          // 36: External Interrupt 2
	EINT3_IRQHandler,          // 37: External Interrupt 3
	ADC_IRQHandler,            // 38: A/D Converter
	BOD_IRQHandler,            // 39: Brown-Out Detect
	USB_IRQHandler,            // 40: USB
	CAN_IRQHandler,            // 41: CAN
	DMA_IRQHandler,            // 42: General Purpose DMA
	I2S_IRQHandler,            // 43: I2S
	ENET_IRQHandler,           // 44: Ethernet
	RIT_IRQHandler,            // 45: Repetitive Interrupt Timer
	MCPWM_IRQHandler,          // 46: Motor Control PWM
	QEI_IRQHandler,            // 47: Quadrature Encoder Interface
	PLL1_IRQHandler,           // 48: PLL1 Lock (USB PLL)

};

/**
 * @brief  This is the code that gets called when the processor first
 *          starts execution following a reset event. Only the absolutely
 *          necessary set is performed, after which the application
 *          supplied main() routine is called.
 * @param  None
 * @retval : None
*/




/*******************************************************************************
*
* Provide weak aliases for each Exception handler to the Default_Handler.
* As they are weak aliases, any function with the same name will override
* this definition.
*
*******************************************************************************/
#pragma weak NMI_Handler = Default_Handler               // NMI Handler = Default_Handler
#pragma weak HardFault_Handler = Default_Handler         // Hard Fault Handler = Default_Handler
#pragma weak MemManage_Handler = Default_Handler         // MPU Fault Handler = Default_Handler
#pragma weak BusFault_Handler = Default_Handler          // Bus Fault Handler = Default_Handler
#pragma weak UsageFault_Handler = Default_Handler        // Usage Fault Handler = Default_Handler
//#pragma weak SVC_Handler = Default_Handler               // SVCall Handler = Default_Handler
#pragma weak DebugMon_Handler = Default_Handler          // Debug Monitor Handler = Default_Handler
//#pragma weak PendSV_Handler = Default_Handler            // PendSV Handler = Default_Handler
//#pragma weak SysTick_Handler = Default_Handler           // SysTick Handler = Default_Handler

                // External Interrupts
#pragma weak WDT_IRQHandler = Default_Handler            // 16: Watchdog Timer
#pragma weak TIMER0_IRQHandler = Default_Handler         // 17: Timer0
#pragma weak TIMER1_IRQHandler = Default_Handler         // 18: Timer1
#pragma weak TIMER2_IRQHandler = Default_Handler         // 19: Timer2
#pragma weak TIMER3_IRQHandler = Default_Handler         // 20: Timer3
#pragma weak UART0_IRQHandler = Default_Handler          // 21: UART0
#pragma weak UART1_IRQHandler = Default_Handler          // 22: UART1
#pragma weak UART2_IRQHandler = Default_Handler          // 23: UART2
#pragma weak UART3_IRQHandler = Default_Handler          // 24: UART3
#pragma weak PWM1_IRQHandler = Default_Handler           // 25: PWM1
#pragma weak I2C0_IRQHandler = Default_Handler           // 26: I2C0
#pragma weak I2C1_IRQHandler = Default_Handler           // 27: I2C1
#pragma weak I2C2_IRQHandler = Default_Handler           // 28: I2C2
#pragma weak SPI_IRQHandler = Default_Handler            // 29: SPI
#pragma weak SSP0_IRQHandler = Default_Handler           // 30: SSP0
#pragma weak SSP1_IRQHandler = Default_Handler           // 31: SSP1
#pragma weak PLL0_IRQHandler = Default_Handler           // 32: PLL0 Lock (Main PLL)
#pragma weak RTC_IRQHandler = Default_Handler            // 33: Real Time Clock
#pragma weak EINT0_IRQHandler = Default_Handler          // 34: External Interrupt 0
#pragma weak EINT1_IRQHandler = Default_Handler          // 35: External Interrupt 1
#pragma weak EINT2_IRQHandler = Default_Handler          // 36: External Interrupt 2
#pragma weak EINT3_IRQHandler = Default_Handler          // 37: External Interrupt 3
#pragma weak ADC_IRQHandler = Default_Handler            // 38: A/D Converter
#pragma weak BOD_IRQHandler = Default_Handler            // 39: Brown-Out Detect
#pragma weak USB_IRQHandler = Default_Handler            // 40: USB
#pragma weak CAN_IRQHandler = Default_Handler            // 41: CAN
#pragma weak DMA_IRQHandler = Default_Handler            // 42: General Purpose DMA
#pragma weak I2S_IRQHandler = Default_Handler            // 43: I2S
#pragma weak ENET_IRQHandler = Default_Handler           // 44: Ethernet
#pragma weak RIT_IRQHandler = Default_Handler            // 45: Repetitive Interrupt Timer
#pragma weak MCPWM_IRQHandler = Default_Handler          // 46: Motor Control PWM
#pragma weak QEI_IRQHandler = Default_Handler            // 47: Quadrature Encoder Interface
#pragma weak PLL1_IRQHandler = Default_Handler           // 48: PLL1 Lock (USB PLL)

/**
 * @brief  This is the code that gets called when the processor receives an
 *         unexpected interrupt.  This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.
 *
 * @param  None
 * @retval : None
*/

__attribute__ ((section(".text.preCRP")))
void Default_Handler(void)
{	
  /* Go into an infinite loop. */
  volatile  int interrupt_nr = __get_xPSR() & 0xFF;
	
  while (interrupt_nr);
}

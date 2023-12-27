/****************************************************
 *********************************************************************************************************
AUTHOR  : Seifeldin Ahmed
MICRO   : STM32F401CCU6
LAYER   : MCAL
DRIVER  : SysTick_Driver
File    : source File
Version :1.0
********************************************************************************************************
*********************************************************�
*/

#include "../../LIB/BIT_MATHS.h"
#include "../../LIB/STD_TYPES.h"
#include "SysTick_interface.h"
#include "SysTick_private.h"
#include "SysTick_config.h"



/*************************************************************************************** Global Section ********************************************************************************************************
***************************************************************************************************************************************************************************************************************/

 static volatile func_ptr Global_pfuncCBF = NULL;

/*************************************************************************************  APIS Definitions *******************************************************************************************************
***************************************************************************************************************************************************************************************************************/

void SysTick_vInit(void)
{
	/******************************************************************* Clk Source ***************************************************************************************************/
	#if SYSTIC_CLKSOURCE_OPTION == SYSTICK_CLKSOURCE_PROCESSOR_CLK_AHB
         SET_BIT(SYSTIC->STK_CTRL,SYSTICK_CLKSOURCE_BIT);
	#elif SYSTIC_CLKSOURCE_OPTION == SYSTICK_CLKSOURCE_AHB_DIV_8
         CLEAR_BIT(SYSTIC->STK_CTRL,SYSTICK_CLKSOURCE_BIT);
    #else
         #error "WRONG INPUT"
    #endif


    /******************************************************************* Interrupt Option ***************************************************************************************************/
    #if SYSTIC_INTURRPT_OPTION == SYSTICK_INTURRPT_ENABLE
         SET_BIT(SYSTIC->STK_CTRL,SYSTICK_INTURRPT_BIT);
	#elif SYSTIC_INTURRPT_OPTION == SYSTICK_INTURRPT_DISABLE
         CLEAR_BIT(SYSTIC->STK_CTRL,SYSTICK_INTURRPT_BIT);
    #else
         #error "WRONG INPUT"
    #endif

}



/******************************************************************************************************************************************************************************************************************
 *****************************************************************************************************************************************************************************************************************/

void SysTick_vSetValue(u32 Copy_u32PreloadValue)
{
	CLEAR_BIT(SYSTIC->STK_CTRL,SYSTICK_EN_BIT);
	SYSTIC->STK_LOAD = Copy_u32PreloadValue - 1;
	SYSTIC->STK_VAL  = 0;
    SET_BIT(SYSTIC->STK_CTRL,SYSTICK_EN_BIT);

}


void SysTick_start_time()
{
	CLEAR_BIT(SYSTIC->STK_CTRL,SYSTICK_EN_BIT);
	SYSTIC->STK_LOAD = 0xffffff - 1;
	SYSTIC->STK_VAL  = 0;
    SET_BIT(SYSTIC->STK_CTRL,SYSTICK_EN_BIT);
}

u32 SysTick_stop_and_return_time()
{
	CLEAR_BIT(SYSTIC->STK_CTRL,SYSTICK_EN_BIT);
	return (SYSTIC->STK_VAL);
}

/******************************************************************************************************************************************************************************************************************
 *****************************************************************************************************************************************************************************************************************/

u32 SysTick_u32GetRemainingTime(void)
{
   return (SYSTIC->STK_VAL);
}

/******************************************************************************************************************************************************************************************************************
 *****************************************************************************************************************************************************************************************************************/

u32 SysTick_u32GetElapsedTime(void)
{
	return ((SYSTIC->STK_LOAD) - (SYSTIC->STK_VAL));
}

/******************************************************************************************************************************************************************************************************************
 *****************************************************************************************************************************************************************************************************************/

u8 SysTick_u8GetFlag(void)
{
  return GET_BIT(SYSTIC->STK_CTRL , 16);
}

/******************************************************************************************************************************************************************************************************************
 *****************************************************************************************************************************************************************************************************************/

void Systic_vDelay_ms(u32 Copy_u32_Time_in_ms)
{
	 CLEAR_BIT(SYSTIC->STK_CTRL,SYSTICK_EN_BIT);
	 u32 number_of_ticks = (Copy_u32_Time_in_ms/1000.0)*Freq_cpu;
	 if(number_of_ticks > 0xffffff)
	 {
		 number_of_ticks = number_of_ticks - 0xffffff;
		 SysTick_vSetValue(0xffffff);
	     while(SysTick_u8GetFlag() == 0);
	 }
	 SysTick_vSetValue(number_of_ticks);
	 while(SysTick_u8GetFlag() == 0);
}

/******************************************************************************************************************************************************************************************************************
 *****************************************************************************************************************************************************************************************************************/

void Systic_vDelay_us(u32 Copy_u32_Time_in_us)
{
	 CLEAR_BIT(SYSTIC->STK_CTRL,SYSTICK_EN_BIT);
	 u32 number_of_ticks = (Copy_u32_Time_in_us/1000000.0)*Freq_cpu;
	 if(number_of_ticks > 0xffffff)
	 {
		 number_of_ticks = number_of_ticks - 0xffffff;
		 SysTick_vSetValue(0xffffff);
	     while(SysTick_u8GetFlag() == 0);
	 }
	 SysTick_vSetValue(number_of_ticks);
	 while(SysTick_u8GetFlag() == 0);
}


/******************************************************************************************************************************************************************************************************************
 *****************************************************************************************************************************************************************************************************************/

void SysTick_Handler(void) {
    if (Global_pfuncCBF != NULL) {
    	Global_pfuncCBF();
    }
}
//***************************************************//

void Systickk_vPerodic(u32 Copy_u32Ticktime,func_ptr Add_pfuncCBF)
{
	Global_pfuncCBF = Add_pfuncCBF;
	SysTick_vSetValue(Copy_u32Ticktime);
}

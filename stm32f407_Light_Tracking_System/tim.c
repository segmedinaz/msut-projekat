#include "tim.h"
#include "rcc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* ---------------------- TIM registers bit mask ------------------------ */
#define SMCR_ETR_MASK      ((uint16_t)0x00FF) 
#define CCMR_OFFSET        ((uint16_t)0x0018)
#define CCER_CCE_SET       ((uint16_t)0x0001)  
#define	CCER_CCNE_SET      ((uint16_t)0x0004) 
#define CCMR_OC13M_MASK    ((uint16_t)0xFF8F)
#define CCMR_OC24M_MASK    ((uint16_t)0x8FFF) 

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

static void TI4_Config(TIM_TypeDef* TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter);



/**
  * @brief  Initializes the TIMx Time Base Unit peripheral according to 
  *         the specified parameters in the TIM_TimeBaseInitStruct.
  * @param  TIMx: where x can be  1 to 14 to select the TIM peripheral.
  * @param  TIM_TimeBaseInitStruct: pointer to a TIM_TimeBaseInitTypeDef structure
  *         that contains the configuration information for the specified TIM peripheral.
  * @retval None
  */
void TIM_TimeBaseInit(TIM_TypeDef* TIMx, TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct)
{
  uint16_t tmpcr1 = 0;


  tmpcr1 = TIMx->CR1;  

  if((TIMx == TIM1) || (TIMx == TIM8)||
     (TIMx == TIM2) || (TIMx == TIM3)||
     (TIMx == TIM4) || (TIMx == TIM5)) 
  {
    /* Select the Counter Mode */
    tmpcr1 &= (uint16_t)(~(TIM_CR1_DIR | TIM_CR1_CMS));
    tmpcr1 |= (uint32_t)TIM_TimeBaseInitStruct->TIM_CounterMode;
  }
 
  if((TIMx != TIM6) && (TIMx != TIM7))
  {
    /* Set the clock division */
    tmpcr1 &=  (uint16_t)(~TIM_CR1_CKD);
    tmpcr1 |= (uint32_t)TIM_TimeBaseInitStruct->TIM_ClockDivision;
  }

  TIMx->CR1 = tmpcr1;

  /* Set the Autoreload value */
  TIMx->ARR = TIM_TimeBaseInitStruct->TIM_Period ;
 
  /* Set the Prescaler value */
  TIMx->PSC = TIM_TimeBaseInitStruct->TIM_Prescaler;
    
  if ((TIMx == TIM1) || (TIMx == TIM8))  
  {
    /* Set the Repetition Counter value */
    TIMx->RCR = TIM_TimeBaseInitStruct->TIM_RepetitionCounter;
  }

  /* Generate an update event to reload the Prescaler 
     and the repetition counter(only for TIM1 and TIM8) value immediately */
  TIMx->EGR = TIM_PSCReloadMode_Immediate;          
}






/**
  * @brief  Enables or disables TIMx peripheral Preload register on ARR.
  * @param  TIMx: where x can be 1 to 14 to select the TIM peripheral.
  * @param  NewState: new state of the TIMx peripheral Preload register
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void TIM_ARRPreloadConfig(TIM_TypeDef* TIMx, FunctionalState NewState)
{


  if (NewState != DISABLE)
  {
    /* Set the ARR Preload Bit */
    TIMx->CR1 |= TIM_CR1_ARPE;
  }
  else
  {
    /* Reset the ARR Preload Bit */
    TIMx->CR1 &= (uint16_t)~TIM_CR1_ARPE;
  }
}





/**
  * @brief  Enables or disables the specified TIM peripheral.
  * @param  TIMx: where x can be 1 to 14 to select the TIMx peripheral.
  * @param  NewState: new state of the TIMx peripheral.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void TIM_Cmd(TIM_TypeDef* TIMx, FunctionalState NewState)
{

  
  if (NewState != DISABLE)
  {
    /* Enable the TIM Counter */
    TIMx->CR1 |= TIM_CR1_CEN;
  }
  else
  {
    /* Disable the TIM Counter */
    TIMx->CR1 &= (uint16_t)~TIM_CR1_CEN;
  }
}


/**
  * @brief  Initializes the TIMx Channel1 according to the specified parameters in
  *         the TIM_OCInitStruct.
  * @param  TIMx: where x can be 1 to 14 except 6 and 7, to select the TIM peripheral.
  * @param  TIM_OCInitStruct: pointer to a TIM_OCInitTypeDef structure that contains
  *         the configuration information for the specified TIM peripheral.
  * @retval None
  */
void TIM_OC1Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct)
{
  uint16_t tmpccmrx = 0, tmpccer = 0, tmpcr2 = 0;  

  /* Disable the Channel 1: Reset the CC1E Bit */
  TIMx->CCER &= (uint16_t)~TIM_CCER_CC1E;
  
  /* Get the TIMx CCER register value */
  tmpccer = TIMx->CCER;
  /* Get the TIMx CR2 register value */
  tmpcr2 =  TIMx->CR2;
  
  /* Get the TIMx CCMR1 register value */
  tmpccmrx = TIMx->CCMR1;
    
  /* Reset the Output Compare Mode Bits */
  tmpccmrx &= (uint16_t)~TIM_CCMR1_OC1M;
  tmpccmrx &= (uint16_t)~TIM_CCMR1_CC1S;
  /* Select the Output Compare Mode */
  tmpccmrx |= TIM_OCInitStruct->TIM_OCMode;
  
  /* Reset the Output Polarity level */
  tmpccer &= (uint16_t)~TIM_CCER_CC1P;
  /* Set the Output Compare Polarity */
  tmpccer |= TIM_OCInitStruct->TIM_OCPolarity;
  
  /* Set the Output State */
  tmpccer |= TIM_OCInitStruct->TIM_OutputState;
    
  if((TIMx == TIM1) || (TIMx == TIM8))
  {
    
    /* Reset the Output N Polarity level */
    tmpccer &= (uint16_t)~TIM_CCER_CC1NP;
    /* Set the Output N Polarity */
    tmpccer |= TIM_OCInitStruct->TIM_OCNPolarity;
    /* Reset the Output N State */
    tmpccer &= (uint16_t)~TIM_CCER_CC1NE;
    
    /* Set the Output N State */
    tmpccer |= TIM_OCInitStruct->TIM_OutputNState;
    /* Reset the Output Compare and Output Compare N IDLE State */
    tmpcr2 &= (uint16_t)~TIM_CR2_OIS1;
    tmpcr2 &= (uint16_t)~TIM_CR2_OIS1N;
    /* Set the Output Idle state */
    tmpcr2 |= TIM_OCInitStruct->TIM_OCIdleState;
    /* Set the Output N Idle state */
    tmpcr2 |= TIM_OCInitStruct->TIM_OCNIdleState;
  }
  /* Write to TIMx CR2 */
  TIMx->CR2 = tmpcr2;
  
  /* Write to TIMx CCMR1 */
  TIMx->CCMR1 = tmpccmrx;
  
  /* Set the Capture Compare Register value */
  TIMx->CCR1 = TIM_OCInitStruct->TIM_Pulse;
  
  /* Write to TIMx CCER */
  TIMx->CCER = tmpccer;
}






/**
  * @brief  Enables or disables the TIMx peripheral Preload register on CCR1.
  * @param  TIMx: where x can be 1 to 14 except 6 and 7, to select the TIM peripheral.
  * @param  TIM_OCPreload: new state of the TIMx peripheral Preload register
  *          This parameter can be one of the following values:
  *            @arg TIM_OCPreload_Enable
  *            @arg TIM_OCPreload_Disable
  * @retval None
  */
void TIM_OC1PreloadConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPreload)
{
  uint16_t tmpccmr1 = 0;

  tmpccmr1 = TIMx->CCMR1;

  /* Reset the OC1PE Bit */
  tmpccmr1 &= (uint16_t)(~TIM_CCMR1_OC1PE);

  /* Enable or Disable the Output Compare Preload feature */
  tmpccmr1 |= TIM_OCPreload;

  /* Write to TIMx CCMR1 register */
  TIMx->CCMR1 = tmpccmr1;
}





/**
  * @brief  Initializes the TIM peripheral according to the specified parameters
  *         in the TIM_ICInitStruct.
  * @param  TIMx: where x can be 1 to 14 except 6 and 7, to select the TIM peripheral.
  * @param  TIM_ICInitStruct: pointer to a TIM_ICInitTypeDef structure that contains
  *         the configuration information for the specified TIM peripheral.
  * @retval None
  */
void TIM_ICInit(TIM_TypeDef* TIMx, TIM_ICInitTypeDef* TIM_ICInitStruct)
{

  
  
  TI4_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity,
             TIM_ICInitStruct->TIM_ICSelection,
             TIM_ICInitStruct->TIM_ICFilter);
             
             
  /* Set the Input Capture Prescaler value */
  TIM_SetIC4Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  
  
  
}




/**
  * @brief  Sets the TIMx Input Capture 4 prescaler.
  * @param  TIMx: where x can be 1, 2, 3, 4, 5 or 8 to select the TIM peripheral.
  * @param  TIM_ICPSC: specifies the Input Capture4 prescaler new value.
  *          This parameter can be one of the following values:
  *            @arg TIM_ICPSC_DIV1: no prescaler
  *            @arg TIM_ICPSC_DIV2: capture is done once every 2 events
  *            @arg TIM_ICPSC_DIV4: capture is done once every 4 events
  *            @arg TIM_ICPSC_DIV8: capture is done once every 8 events
  * @retval None
  */
void TIM_SetIC4Prescaler(TIM_TypeDef* TIMx, uint16_t TIM_ICPSC)
{  

  /* Reset the IC4PSC Bits */
  TIMx->CCMR2 &= (uint16_t)~TIM_CCMR2_IC4PSC;

  /* Set the IC4PSC value */
  TIMx->CCMR2 |= (uint16_t)(TIM_ICPSC << 8);
}



/**
  * @brief  Configure the TI4 as Input.
  * @param  TIMx: where x can be 1, 2, 3, 4, 5 or 8 to select the TIM peripheral.
  * @param  TIM_ICPolarity : The Input Polarity.
  *          This parameter can be one of the following values:
  *            @arg TIM_ICPolarity_Rising
  *            @arg TIM_ICPolarity_Falling
  *            @arg TIM_ICPolarity_BothEdge     
  * @param  TIM_ICSelection: specifies the input to be used.
  *          This parameter can be one of the following values:
  *            @arg TIM_ICSelection_DirectTI: TIM Input 4 is selected to be connected to IC4.
  *            @arg TIM_ICSelection_IndirectTI: TIM Input 4 is selected to be connected to IC3.
  *            @arg TIM_ICSelection_TRC: TIM Input 4 is selected to be connected to TRC.
  * @param  TIM_ICFilter: Specifies the Input Capture Filter.
  *          This parameter must be a value between 0x00 and 0x0F.
  * @retval None
  */
static void TI4_Config(TIM_TypeDef* TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter)
{
  uint16_t tmpccmr2 = 0, tmpccer = 0, tmp = 0;

  /* Disable the Channel 4: Reset the CC4E Bit */
  TIMx->CCER &= (uint16_t)~TIM_CCER_CC4E;
  tmpccmr2 = TIMx->CCMR2;
  tmpccer = TIMx->CCER;
  tmp = (uint16_t)(TIM_ICPolarity << 12);

  /* Select the Input and set the filter */
  tmpccmr2 &= ((uint16_t)~TIM_CCMR1_CC2S) & ((uint16_t)~TIM_CCMR1_IC2F);
  tmpccmr2 |= (uint16_t)(TIM_ICSelection << 8);
  tmpccmr2 |= (uint16_t)(TIM_ICFilter << 12);

  /* Select the Polarity and set the CC4E Bit */
  tmpccer &= (uint16_t)~(TIM_CCER_CC4P | TIM_CCER_CC4NP);
  tmpccer |= (uint16_t)(tmp | (uint16_t)TIM_CCER_CC4E);

  /* Write to TIMx CCMR2 and CCER registers */
  TIMx->CCMR2 = tmpccmr2;
  TIMx->CCER = tmpccer ;
}



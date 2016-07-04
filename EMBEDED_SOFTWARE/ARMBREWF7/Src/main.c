/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "cmsis_os.h"
#include "fatfs.h"

/* USER CODE BEGIN Includes */
#include "main.h"
#include "MyGui.h"
#include "WM.h"
#include "GlobalVar.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

SD_HandleTypeDef hsd1;
HAL_SD_CardInfoTypedef SDCardInfo1;

osThreadId defaultTaskHandle;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
osThreadId GuiTaskHandle;
osThreadId ControlTaskHandle;
SemaphoreHandle_t xSemaphoreGraphData;

uint32_t G_mSCounter=0;
uint32_t G_SecondCounter=0;

uint32_t G_PWMCounter=0;

uint16_t G_pwm_output_status=0;
uint16_t G_pwm_output=0;
uint16_t G_pwm_pct_value=0;

double G_temperature=0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SDMMC1_SD_Init(void);
void StartDefaultTask(void const * argument);


/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void StartGuiTask(void const * argument);
void StartControlTask(void const * argument);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SDMMC1_SD_Init();


  /* USER CODE BEGIN 2 */
  //SCB_DisableDCache() ;
  BSP_SDRAM_Init(); /* Initializes the SDRAM device */
  __HAL_RCC_CRC_CLK_ENABLE();
  GUI_Init();
  GUI_DispStringAt("Starting...", 0, 0);
  WM_SetCreateFlags(WM_CF_MEMDEV);

  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  xSemaphoreGraphData= xSemaphoreCreateBinary();//xSemaphoreCreateMutex();
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 1, 1024);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  osThreadDef(GuiTask, StartGuiTask, osPriorityLow, 1, 1024);
  GuiTaskHandle = osThreadCreate(osThread(GuiTask), NULL);

  osThreadDef(ControlTask, StartControlTask, osPriorityHigh, 1, 1024);
  ControlTaskHandle = osThreadCreate(osThread(ControlTask), NULL);
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
 

  /* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

	  RCC_OscInitTypeDef RCC_OscInitStruct;
	  RCC_ClkInitTypeDef RCC_ClkInitStruct;
	  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

	  __HAL_RCC_PWR_CLK_ENABLE();

	  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	  RCC_OscInitStruct.PLL.PLLM = 25;
	  RCC_OscInitStruct.PLL.PLLN = 432;
	  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	  RCC_OscInitStruct.PLL.PLLQ = 9;
	  HAL_RCC_OscConfig(&RCC_OscInitStruct);

	  HAL_PWREx_EnableOverDrive();
	  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SDMMC1|RCC_PERIPHCLK_CLK48;
    PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
    PeriphClkInitStruct.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_CLK48;
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

/* SDMMC1 init function */
void MX_SDMMC1_SD_Init(void)
{

  hsd1.Instance = SDMMC1;
  hsd1.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
  hsd1.Init.ClockBypass = SDMMC_CLOCK_BYPASS_DISABLE;
  hsd1.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  hsd1.Init.BusWide = SDMMC_BUS_WIDE_1B;
  hsd1.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd1.Init.ClockDiv = 0;

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */
void StartGuiTask(void const * argument)
{
	TickType_t xLastWakeTime;
	//xSemaphore = xSemaphoreCreateMutex();

	InitGui();
	xLastWakeTime = xTaskGetTickCount();
  /* Infinite loop */
  for(;;)
  {
    vTaskDelayUntil(&xLastWakeTime,25);
    xSemaphoreTake(xSemaphoreGraphData,100);
	GuiLoop();
	xSemaphoreGive(xSemaphoreGraphData);

	//GUI_Delay(25);
  }

}


void StartControlTask(void const * argument){
	TickType_t xLastWakeTime;
	int GraphSampleCounter=0;
	int temp=0;
	int add=1;

	//InitGui();
	xLastWakeTime = xTaskGetTickCount();
  /* Infinite loop */
  for(;;)
  {

    vTaskDelayUntil(&xLastWakeTime,1);

    //mS counter
    G_mSCounter++;
    //if(G_mSCounter>=4294967295){ // 4294967295 mS = 4294967.295 S = 71582.78825 min = 1193.0465 h = 49.71 jours
    if(G_mSCounter>=4000000000){
    	G_mSCounter=0;
    }

    //S counter
    if(G_mSCounter%1000==0){
		G_SecondCounter++;
		if(G_SecondCounter>=4294967295){
			G_SecondCounter=0;
		}
    }

    //INPUTS
    GraphSampleCounter++;
        if(GraphSampleCounter>=100){
        	GraphSampleCounter=0;

        		    if(add){
        		    	temp+=3;
        		    }else{
        		    	temp-=3;
        		    }

        		    if(temp>=50){
        		    	add=0;
        		    }
        		    if(temp<=0){
        		    	add=1;
        		    }
        		    xSemaphoreTake(xSemaphoreGraphData,100);
        		    UpdateGraph(temp);
        		    //G_temperature=temp;
        		    xSemaphoreGive(xSemaphoreGraphData);
        }


    // OUTPUTS
    //PWM emulation
    if(G_pwm_output_status){
        G_PWMCounter++;
        //Period limit
        if(G_PWMCounter>=PWM_PERIOD){
        	G_PWMCounter=0;
        }

        //Duty Cycle
        if(G_PWMCounter>=G_pwm_pct_value*PWM_PERIOD/100){
        	G_pwm_output=0;
        }else{
        	G_pwm_output=1;
        }
    }else{
    	//When PWM is Turned off reset the counter
    	G_pwm_output=0;
    	G_PWMCounter=0;
    }





  }
}


/* USER CODE END 4 */

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{
	uint8_t wtext[] = "This is STM32 working with FatFs";
	 FIL myfile;
	 int byteswritten;
  /* init code for FATFS */
  MX_FATFS_Init();

  /* USER CODE BEGIN 5 */


  f_open(&myfile,"TEST.TXT",FA_CREATE_ALWAYS | FA_WRITE);

  f_write(&myfile,  wtext, sizeof(wtext), (void *)&byteswritten);

  f_close(&myfile);


  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

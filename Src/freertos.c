/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for Bno085Task */
osThreadId_t Bno085TaskHandle;
const osThreadAttr_t Bno085Task_attributes = {
  .name = "Bno085Task",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for NmeaTask */
osThreadId_t NmeaTaskHandle;
const osThreadAttr_t NmeaTask_attributes = {
  .name = "NmeaTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for UartTxTask */
osThreadId_t UartTxTaskHandle;
const osThreadAttr_t UartTxTask_attributes = {
  .name = "UartTxTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartBno085Task(void *argument);
void StartNmeaTask(void *argument);
void StartUartTxTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of Bno085Task */
  Bno085TaskHandle = osThreadNew(StartBno085Task, NULL, &Bno085Task_attributes);

  /* creation of NmeaTask */
  NmeaTaskHandle = osThreadNew(StartNmeaTask, NULL, &NmeaTask_attributes);

  /* creation of UartTxTask */
  UartTxTaskHandle = osThreadNew(StartUartTxTask, NULL, &UartTxTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartBno085Task */
/**
  * @brief  Function implementing the Bno085Task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartBno085Task */
void StartBno085Task(void *argument)
{
  /* USER CODE BEGIN StartBno085Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartBno085Task */
}

/* USER CODE BEGIN Header_StartNmeaTask */
/**
* @brief Function implementing the NmeaTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartNmeaTask */
void StartNmeaTask(void *argument)
{
  /* USER CODE BEGIN StartNmeaTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartNmeaTask */
}

/* USER CODE BEGIN Header_StartUartTxTask */
/**
* @brief Function implementing the UartTxTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUartTxTask */
void StartUartTxTask(void *argument)
{
  /* USER CODE BEGIN StartUartTxTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartUartTxTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */


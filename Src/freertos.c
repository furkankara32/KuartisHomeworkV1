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
#include "app_types.h"
#include "queue.h"
#include "timestamp.h"
#include "bno085.h"
#include "orientation_math.h"
#include "nmea.h"
#include "telemetry_protocol.h"
#include "debug_port.h"
#include "performance_monitor.h"
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
QueueHandle_t orientationQueueHandle = NULL;
QueueHandle_t txQueueHandle = NULL;
static volatile uint8_t bno085TaskNotificationEnabled = 0U;

static TxRingBuffer_t uartTxRingBuffer = {0};

static uint32_t uartTxRingFullCount = 0U;
static uint32_t uartTxDmaErrorCount = 0U;

static uint32_t txQueueDropCount = 0U;

static uint32_t nmeaFormatErrorCount = 0U;
static uint32_t quaternionFormatErrorCount = 0U;

static uint32_t staleSampleCount = 0U;

/* USER CODE END Variables */
/* Definitions for Bno085Task */
osThreadId_t Bno085TaskHandle;
const osThreadAttr_t Bno085Task_attributes = {
  .name = "Bno085Task",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for TelemetryTask */
osThreadId_t TelemetryTaskHandle;
const osThreadAttr_t TelemetryTask_attributes = {
  .name = "TelemetryTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for UartTxTask */
osThreadId_t UartTxTaskHandle;
const osThreadAttr_t UartTxTask_attributes = {
  .name = "UartTxTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartBno085Task(void *argument);
void StartTelemetryTask(void *argument);
void StartUartTxTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);
void vApplicationMallocFailedHook(void);

/* USER CODE BEGIN 4 */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{
   /* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   called if a stack overflow is detected. */
}
/* USER CODE END 4 */

/* USER CODE BEGIN 5 */
void vApplicationMallocFailedHook(void)
{
   /* vApplicationMallocFailedHook() will only be called if
   configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h. It is a hook
   function that will get called if a call to pvPortMalloc() fails.
   pvPortMalloc() is called internally by the kernel whenever a task, queue,
   timer or semaphore is created. It is also called by various parts of the
   demo application. If heap_1.c or heap_2.c are used, then the size of the
   heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
   FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
   to query the size of free heap space that remains (although it does not
   provide information on how the remaining heap might be fragmented). */
}
/* USER CODE END 5 */

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
	orientationQueueHandle = xQueueCreate(1U,sizeof(OrientationSample_t));
	txQueueHandle = xQueueCreate(8U,sizeof(UartMessage_t));

	if((orientationQueueHandle == NULL) || (txQueueHandle == NULL) )
	{
		Error_Handler();
	}
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of Bno085Task */
  Bno085TaskHandle = osThreadNew(StartBno085Task, NULL, &Bno085Task_attributes);

  /* creation of TelemetryTask */
  TelemetryTaskHandle = osThreadNew(StartTelemetryTask, NULL, &TelemetryTask_attributes);

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

  BNO085_Status_t status;

  BNO085_Quaternion_t quaternion;

  OrientationSample_t sample = {0};

  uint32_t sequence = 0U;
  float heading_deg;


  /*
   * Do not send task notifications while the sensor
   * is being initialized.
   */
  bno085TaskNotificationEnabled = 0U;


  /*
   * Sensor initialization.
   *
   * If the sensor is not available, retry every second.
   */
  for (;;)
  {
    status = BN085_Init();

    if (status == BNO085_STATUS_OK)
    {
      break;
    }

    vTaskDelay(pdMS_TO_TICKS(1000U));
  }


  /*
   * Request Rotation Vector reports at 100 Hz.
   *
   * This only queues the SHTP command inside the driver.
   * BNO085_Process() will actually start the SPI transfer.
   */
  status = BNO085_EnableRotationVector(
      BNO085_ROTATION_VECTOR_INTERVAL_US
  );

  if (status != BNO085_STATUS_OK)
  {
    /* Error handling will be improved later. */
    Error_Handler();
  }


  /*
   * Clear any notifications that may have accumulated
   * during initialization.
   */
  (void)ulTaskNotifyTake(pdTRUE, 0U);


  /*
   * ISR -> task notification mechanism is now active.
   */
  bno085TaskNotificationEnabled = 1U;


  /*
   * Kick the state machine once.
   *
   * EnableRotationVector() only sets tx_pending.
   * This call starts the actual command transmission
   * or starts the WAKE/ready procedure.
   */
  BNO085_Process();


  for (;;)
  {
    /*
     * Normally the task wakes up because:
     *
     * - BNO085 INT falling edge
     * - SPI RX DMA complete
     * - SPI TX DMA complete
     *
     * 20 ms timeout is only a recovery/safety mechanism.
     *
     * Sensor period = 10 ms (100 Hz)
     */
    (void)ulTaskNotifyTake(
        pdTRUE,
        pdMS_TO_TICKS(20U)
    );


    /*
     * Service the state machine.
     *
     * Two calls are intentional.
     *
     * Example:
     *
     * Call #1:
     *   DMA header completed
     *   -> parse header
     *   -> state = WAIT_PACKET
     *
     * Call #2:
     *   -> start packet DMA immediately
     *
     * This avoids waiting another 10/20 ms for a state
     * transition that can already be performed.
     */
    BNO085_Process();
    BNO085_Process();


    /*
     * If a complete Rotation Vector report has been parsed,
     * obtain the latest quaternion.
     */
    status = BNO085_GetQuaternion(&quaternion);

    if (status == BNO085_STATUS_OK)
    {

    if (Orientation_QuaternionToHeadingDeg(
   	        quaternion.x,
   	        quaternion.y,
   	        quaternion.z,
   	        quaternion.w,
   	        &heading_deg) != ORIENTATION_STATUS_OK)
   	{
    	continue;
    }


      /*
       * All fields belong to the same BNO085 sample.
       */
      sample.qx = quaternion.x;
      sample.qy = quaternion.y;
      sample.qz = quaternion.z;
      sample.qw = quaternion.w;

      sample.heading_deg = heading_deg;

      sample.timestamp_us = Timestamp_GetUs();
      sample.sequence = sequence++;
      sample.accuracy = (uint8_t)quaternion.accuracy;


      /*
       * Queue length = 1.
       *
       * We always want the most recent orientation.
       */
      (void)xQueueOverwrite(
          orientationQueueHandle,
          &sample
      );
    }
  }

  /* USER CODE END StartBno085Task */
}
/* USER CODE BEGIN Header_StartTelemetryTask */
/**
* @brief Function implementing the TelemetryTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTelemetryTask */
void StartTelemetryTask(void *argument)
{
  /* USER CODE BEGIN StartTelemetryTask */

  OrientationSample_t sample;
  UartMessage_t nmea_message;
  UartMessage_t quaternion_message;

  TickType_t last_wake_time;
  const TickType_t period_ticks = pdMS_TO_TICKS(100U);

  uint32_t last_sequence = 0U;
  uint8_t first_sample_received = 0U;

  PerformanceStats_t performance_stats;
  uint32_t telemetry_timestamp_us;
  uint32_t sample_age_us;

  UBaseType_t bno_stack_min;
  UBaseType_t telemetry_stack_min;
  UBaseType_t uart_stack_min;

  size_t free_heap;
  size_t min_ever_free_heap;

  last_wake_time = xTaskGetTickCount();

  PerformanceMonitor_Init();
  for (;;)
  {
    /*
     * Run at a fixed 10 Hz rate.
     *
     * 100 ms period = 10 Hz
     */
    vTaskDelayUntil(
        &last_wake_time,
        period_ticks
    );


    /*
     * orientationQueue length = 1.
     *
     * Peek instead of Receive because we want to keep
     * the latest sample available in the queue.
     */
    if (xQueuePeek(
            orientationQueueHandle,
            &sample,
            0U) != pdPASS)
    {
      /*
       * No orientation sample has been received yet.
       */
      continue;
    }

    telemetry_timestamp_us = Timestamp_GetUs();

    PerformanceMonitor_Update(telemetry_timestamp_us);

    sample_age_us =
        telemetry_timestamp_us - sample.timestamp_us;

    PerformanceMonitor_UpdateSampleAge(sample_age_us);


    if (NMEA_FormatHCHDM(
            sample.heading_deg,
            nmea_message.data,
            UART_MESSAGE_MAX_LENGTH,
            &nmea_message.length) == NMEA_STATUS_OK)
    {
        if (xQueueSend(
                txQueueHandle,
                &nmea_message,
                0U) != pdPASS)
        {
            txQueueDropCount++;
        }
    }
    else
    {
        nmeaFormatErrorCount++;
    }


    if (Telemetry_FormatQuaternion(
            &sample,
            quaternion_message.data,
            UART_MESSAGE_MAX_LENGTH,
            &quaternion_message.length) == TELEMETRY_STATUS_OK)
    {
        if (xQueueSend(
                txQueueHandle,
                &quaternion_message,
                0U) != pdPASS)
        {
            txQueueDropCount++;
        }
    }
    else
    {
        quaternionFormatErrorCount++;
    }
    /*
     * Detect whether the BNO085 produced a new sample
     * since the previous telemetry cycle.
     */
    if (first_sample_received == 0U)
    {
      first_sample_received = 1U;
      last_sequence = sample.sequence;
    }
    else
    {
    	if (sample.sequence == last_sequence)
    	{
    	    staleSampleCount++;
    	}
    	else
    	{
    	    last_sequence = sample.sequence;
    	}
    }


    /*
     * Next step:
     *
     * 1) Generate $HCHDM NMEA sentence
     * 2) Generate quaternion telemetry packet
     * 3) Send both to txQueue
     */

    PerformanceMonitor_GetStats(
        &performance_stats
    );

    bno_stack_min =
        uxTaskGetStackHighWaterMark(
            (TaskHandle_t)Bno085TaskHandle
        );

    telemetry_stack_min =
        uxTaskGetStackHighWaterMark(
            (TaskHandle_t)TelemetryTaskHandle
        );

    uart_stack_min =
        uxTaskGetStackHighWaterMark(
            (TaskHandle_t)UartTxTaskHandle
        );

    free_heap = xPortGetFreeHeapSize();

    min_ever_free_heap =
        xPortGetMinimumEverFreeHeapSize();
  }

  /* USER CODE END StartTelemetryTask */
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

  UartMessage_t tx_message;

  BufferStatus_t buffer_status;
  DebugStatus_t debug_status;


  for (;;)
  {
    /*
     * No polling.
     *
     * Sleep until TelemetryTask puts a message
     * into txQueue.
     */
    if (xQueueReceive(
            txQueueHandle,
            &tx_message,
            portMAX_DELAY) != pdPASS)
    {
      continue;
    }


    /*
     * Basic message validation.
     */
    if ((tx_message.length == 0U) ||
        (tx_message.length > UART_MESSAGE_MAX_LENGTH))
    {
      continue;
    }


    /*
     * Only UartTxTask writes to the TX ring buffer.
     *
     * This avoids multiple-producer race conditions
     * and removes the need for a mutex.
     */
    buffer_status = RingBufferWrite(
        &uartTxRingBuffer,
        tx_message.data,
        tx_message.length
    );


    if (buffer_status != BUFFER_STATUS_OK)
    {
      /*
       * Ring buffer did not have enough free space.
       *
       * For now drop the complete message.
       * Diagnostics will expose this counter later.
       */
      uartTxRingFullCount++;

      continue;
    }


    /*
     * Start DMA if it is currently idle.
     *
     * If DMA is already BUSY, this is not an error:
     * HAL_UART_TxCpltCallback() will automatically
     * continue transmitting the data that we just
     * appended to the ring buffer.
     */
    debug_status = DebugSend_DMA(
        &uartTxRingBuffer
    );


    if (debug_status == DEBUG_STATUS_ERROR)
    {
      uartTxDmaErrorCount++;
    }

    /*
     * DEBUG_STATUS_BUSY is normal here.
     *
     * It means an existing DMA transfer is already
     * consuming the ring buffer.
     */
  }

  /* USER CODE END StartUartTxTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void BNO085_EventCallbackFromISR(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if ((bno085TaskNotificationEnabled != 0U) &&
        (Bno085TaskHandle != NULL))
    {
        vTaskNotifyGiveFromISR((TaskHandle_t)Bno085TaskHandle, &xHigherPriorityTaskWoken);

        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}
/* USER CODE END Application */


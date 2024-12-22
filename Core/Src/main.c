/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdio.h>
#include <stdlib.h>
// #include <SysprogsProfiler.h>

#include "../../midi.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticQueue_t osStaticMessageQDef_t;
/* USER CODE BEGIN PTD */

typedef struct {
	// object data type
	uint8_t Buf[_UART_RX_MSSG_MAX_LENGTH];
	uint16_t data_size;
	uint8_t uartid;
} UART_RX_MSGQUEUE_OBJ_t;

typedef struct {
	// object data type
	uint8_t Buf[_UART_TX_MSSG_MAX_LENGTH];
	uint16_t data_size;
	uint8_t uartid;
} UART_TX_MSGQUEUE_OBJ_t;

typedef struct { 
	uint8_t current_input_val;
	uint8_t prev_input_val;
	uint8_t state;
	uint8_t prev_state;
} PUSHBUTTON_OBJ_t;

typedef struct { 
	uint8_t current_input_a_val;
	uint8_t current_input_b_val;
	uint8_t transient_state;
	uint8_t prev_transient_state;
	uint8_t count;
	uint8_t prev_count;
	PUSHBUTTON_OBJ_t pushbutton;
} ROTARY_ENCODER_OBJ_t;

typedef struct {
	uint16_t led_on_ms;
	uint16_t led_off_ms;
} LED_BLINK_PARAMS_OBJ_t;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* Definitions for mainTask */
osThreadId_t mainTaskHandle;
uint32_t mainTaskBuffer[ 128 ];
osStaticThreadDef_t mainTaskControlBlock;
const osThreadAttr_t mainTask_attributes = {
  .name = "mainTask",
  .cb_mem = &mainTaskControlBlock,
  .cb_size = sizeof(mainTaskControlBlock),
  .stack_mem = &mainTaskBuffer[0],
  .stack_size = sizeof(mainTaskBuffer),
  .priority = (osPriority_t) osPriorityRealtime5,
};
/* Definitions for midiUartRxTask */
osThreadId_t midiUartRxTaskHandle;
uint32_t midiUartRxTaskBuffer[ 128 ];
osStaticThreadDef_t midiUartRxTaskControlBlock;
const osThreadAttr_t midiUartRxTask_attributes = {
  .name = "midiUartRxTask",
  .cb_mem = &midiUartRxTaskControlBlock,
  .cb_size = sizeof(midiUartRxTaskControlBlock),
  .stack_mem = &midiUartRxTaskBuffer[0],
  .stack_size = sizeof(midiUartRxTaskBuffer),
  .priority = (osPriority_t) osPriorityRealtime5,
};
/* Definitions for midiUartTxTask */
osThreadId_t midiUartTxTaskHandle;
uint32_t midiUartTxTaskBuffer[ 128 ];
osStaticThreadDef_t midiUartTxTaskControlBlock;
const osThreadAttr_t midiUartTxTask_attributes = {
  .name = "midiUartTxTask",
  .cb_mem = &midiUartTxTaskControlBlock,
  .cb_size = sizeof(midiUartTxTaskControlBlock),
  .stack_mem = &midiUartTxTaskBuffer[0],
  .stack_size = sizeof(midiUartTxTaskBuffer),
  .priority = (osPriority_t) osPriorityRealtime5,
};
/* Definitions for controlUartRxTa */
osThreadId_t controlUartRxTaHandle;
uint32_t controlUartRxTaBuffer[ 128 ];
osStaticThreadDef_t controlUartRxTaControlBlock;
const osThreadAttr_t controlUartRxTa_attributes = {
  .name = "controlUartRxTa",
  .cb_mem = &controlUartRxTaControlBlock,
  .cb_size = sizeof(controlUartRxTaControlBlock),
  .stack_mem = &controlUartRxTaBuffer[0],
  .stack_size = sizeof(controlUartRxTaBuffer),
  .priority = (osPriority_t) osPriorityRealtime5,
};
/* Definitions for controlUartTxTa */
osThreadId_t controlUartTxTaHandle;
uint32_t controlUartTxTaBuffer[ 128 ];
osStaticThreadDef_t controlUartTxTaControlBlock;
const osThreadAttr_t controlUartTxTa_attributes = {
  .name = "controlUartTxTa",
  .cb_mem = &controlUartTxTaControlBlock,
  .cb_size = sizeof(controlUartTxTaControlBlock),
  .stack_mem = &controlUartTxTaBuffer[0],
  .stack_size = sizeof(controlUartTxTaBuffer),
  .priority = (osPriority_t) osPriorityRealtime5,
};
/* Definitions for updateRotaryEnc */
osThreadId_t updateRotaryEncHandle;
uint32_t updatenRotaryEncodersStateBuffer[ 128 ];
osStaticThreadDef_t updateRotaryEncodersStateControlBlock;
const osThreadAttr_t updateRotaryEnc_attributes = {
  .name = "updateRotaryEnc",
  .cb_mem = &updateRotaryEncodersStateControlBlock,
  .cb_size = sizeof(updateRotaryEncodersStateControlBlock),
  .stack_mem = &updatenRotaryEncodersStateBuffer[0],
  .stack_size = sizeof(updatenRotaryEncodersStateBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for updateSlidersTa */
osThreadId_t updateSlidersTaHandle;
uint32_t updateSlidersTaskBuffer[ 128 ];
osStaticThreadDef_t updateSlidersTaskControlBlock;
const osThreadAttr_t updateSlidersTa_attributes = {
  .name = "updateSlidersTa",
  .cb_mem = &updateSlidersTaskControlBlock,
  .cb_size = sizeof(updateSlidersTaskControlBlock),
  .stack_mem = &updateSlidersTaskBuffer[0],
  .stack_size = sizeof(updateSlidersTaskBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for updatePushbutto */
osThreadId_t updatePushbuttoHandle;
uint32_t updatePushbuttonsBuffer[ 128 ];
osStaticThreadDef_t updatePushbuttonsControlBlock;
const osThreadAttr_t updatePushbutto_attributes = {
  .name = "updatePushbutto",
  .cb_mem = &updatePushbuttonsControlBlock,
  .cb_size = sizeof(updatePushbuttonsControlBlock),
  .stack_mem = &updatePushbuttonsBuffer[0],
  .stack_size = sizeof(updatePushbuttonsBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for updateYellowLed */
osThreadId_t updateYellowLedHandle;
uint32_t updateYellowLedTaskBuffer[ 128 ];
osStaticThreadDef_t updateYellowLedTaskControlBlock;
const osThreadAttr_t updateYellowLed_attributes = {
  .name = "updateYellowLed",
  .cb_mem = &updateYellowLedTaskControlBlock,
  .cb_size = sizeof(updateYellowLedTaskControlBlock),
  .stack_mem = &updateYellowLedTaskBuffer[0],
  .stack_size = sizeof(updateYellowLedTaskBuffer),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for updateGreenLed */
osThreadId_t updateGreenLedHandle;
uint32_t updateGreenLedBuffer[ 128 ];
osStaticThreadDef_t updateGreenLedControlBlock;
const osThreadAttr_t updateGreenLed_attributes = {
  .name = "updateGreenLed",
  .cb_mem = &updateGreenLedControlBlock,
  .cb_size = sizeof(updateGreenLedControlBlock),
  .stack_mem = &updateGreenLedBuffer[0],
  .stack_size = sizeof(updateGreenLedBuffer),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for midiUartTxQueue */
osMessageQueueId_t midiUartTxQueueHandle;
uint8_t midiUartTxQueueBuffer[ 32 * sizeof( UART_TX_MSGQUEUE_OBJ_t ) ];
osStaticMessageQDef_t midiUartTxQueueControlBlock;
const osMessageQueueAttr_t midiUartTxQueue_attributes = {
  .name = "midiUartTxQueue",
  .cb_mem = &midiUartTxQueueControlBlock,
  .cb_size = sizeof(midiUartTxQueueControlBlock),
  .mq_mem = &midiUartTxQueueBuffer,
  .mq_size = sizeof(midiUartTxQueueBuffer)
};
/* Definitions for controlUartRxQueue */
osMessageQueueId_t controlUartRxQueueHandle;
uint8_t controlUartRxQueueBuffer[ 32 * sizeof( UART_RX_MSGQUEUE_OBJ_t ) ];
osStaticMessageQDef_t controlUartRxQueueControlBlock;
const osMessageQueueAttr_t controlUartRxQueue_attributes = {
  .name = "controlUartRxQueue",
  .cb_mem = &controlUartRxQueueControlBlock,
  .cb_size = sizeof(controlUartRxQueueControlBlock),
  .mq_mem = &controlUartRxQueueBuffer,
  .mq_size = sizeof(controlUartRxQueueBuffer)
};
/* Definitions for controlUartTxQueue */
osMessageQueueId_t controlUartTxQueueHandle;
uint8_t controlUartTxQueueBuffer[ 32 * sizeof( UART_TX_MSGQUEUE_OBJ_t ) ];
osStaticMessageQDef_t controlUartTxQueueControlBlock;
const osMessageQueueAttr_t controlUartTxQueue_attributes = {
  .name = "controlUartTxQueue",
  .cb_mem = &controlUartTxQueueControlBlock,
  .cb_size = sizeof(controlUartTxQueueControlBlock),
  .mq_mem = &controlUartTxQueueBuffer,
  .mq_size = sizeof(controlUartTxQueueBuffer)
};
/* Definitions for yellowLedInputQueue */
osMessageQueueId_t yellowLedInputQueueHandle;
uint8_t yellowLedInputQueueBuffer[ 2 * sizeof( LED_BLINK_PARAMS_OBJ_t ) ];
osStaticMessageQDef_t yellowLedInputQueueControlBlock;
const osMessageQueueAttr_t yellowLedInputQueue_attributes = {
  .name = "yellowLedInputQueue",
  .cb_mem = &yellowLedInputQueueControlBlock,
  .cb_size = sizeof(yellowLedInputQueueControlBlock),
  .mq_mem = &yellowLedInputQueueBuffer,
  .mq_size = sizeof(yellowLedInputQueueBuffer)
};
/* Definitions for greenLedInputQueue */
osMessageQueueId_t greenLedInputQueueHandle;
uint8_t greenLedInputQueueBuffer[ 2 * sizeof( LED_BLINK_PARAMS_OBJ_t ) ];
osStaticMessageQDef_t greenLedInputQueueControlBlock;
const osMessageQueueAttr_t greenLedInputQueue_attributes = {
  .name = "greenLedInputQueue",
  .cb_mem = &greenLedInputQueueControlBlock,
  .cb_size = sizeof(greenLedInputQueueControlBlock),
  .mq_mem = &greenLedInputQueueBuffer,
  .mq_size = sizeof(greenLedInputQueueBuffer)
};
/* Definitions for midiUartRxQueue */
osMessageQueueId_t midiUartRxQueueHandle;
uint8_t midiUartRxQueueBuffer[ 32 * sizeof( UART_RX_MSGQUEUE_OBJ_t ) ];
osStaticMessageQDef_t midiUartRxQueueControlBlock;
const osMessageQueueAttr_t midiUartRxQueue_attributes = {
  .name = "midiUartRxQueue",
  .cb_mem = &midiUartRxQueueControlBlock,
  .cb_size = sizeof(midiUartRxQueueControlBlock),
  .mq_mem = &midiUartRxQueueBuffer,
  .mq_size = sizeof(midiUartRxQueueBuffer)
};
/* Definitions for onBoardLedTimer */
osTimerId_t onBoardLedTimerHandle;
const osTimerAttr_t onBoardLedTimer_attributes = {
  .name = "onBoardLedTimer"
};
/* Definitions for updateSlidersTimer */
osTimerId_t updateSlidersTimerHandle;
const osTimerAttr_t updateSlidersTimer_attributes = {
  .name = "updateSlidersTimer"
};
/* Definitions for updatePushbuttonsTimer */
osTimerId_t updatePushbuttonsTimerHandle;
const osTimerAttr_t updatePushbuttonsTimer_attributes = {
  .name = "updatePushbuttonsTimer"
};
/* Definitions for updateRotaryEncodersTimer */
osTimerId_t updateRotaryEncodersTimerHandle;
const osTimerAttr_t updateRotaryEncodersTimer_attributes = {
  .name = "updateRotaryEncodersTimer"
};
/* Definitions for updateLedsTimer */
osTimerId_t updateLedsTimerHandle;
const osTimerAttr_t updateLedsTimer_attributes = {
  .name = "updateLedsTimer"
};
/* Definitions for updateEncodersEvent */
osEventFlagsId_t updateEncodersEventHandle;
const osEventFlagsAttr_t updateEncodersEvent_attributes = {
  .name = "updateEncodersEvent"
};
/* Definitions for updatePushbuttonsEvent */
osEventFlagsId_t updatePushbuttonsEventHandle;
const osEventFlagsAttr_t updatePushbuttonsEvent_attributes = {
  .name = "updatePushbuttonsEvent"
};
/* Definitions for updateSlidersEvent */
osEventFlagsId_t updateSlidersEventHandle;
const osEventFlagsAttr_t updateSlidersEvent_attributes = {
  .name = "updateSlidersEvent"
};
/* USER CODE BEGIN PV */

uint32_t adc_channels_buffer[_NUM_OF_SLIDERS];
uint32_t adc_channels_compenstaed_buffer[_NUM_OF_SLIDERS];
uint32_t adc_filtered_channels_compenstaed_buffer[_NUM_OF_SLIDERS] = { 0 };
uint32_t prev_adc_filtered_channels_compenstaed_buffer[_NUM_OF_SLIDERS] = { 0 };

// Sliders requires linearization (x+b) x slope + offset.
// Linearization is divided into 3 segments

int adc_channels_libearization_segment_1_end[_NUM_OF_SLIDERS] =  // start from 0
	{ 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300 };

int adc_channels_libearization_segment_2_end[_NUM_OF_SLIDERS] =  
	{ 3400, 3400, 3400, 3400, 3400, 3400, 3400, 3400, 3400, 3400, 3400, 3400 };


float adc_channels_linearization_slope_segment_1[_NUM_OF_SLIDERS] =
	{ 2.4, 2.4, 2.4, 2.4, 2.4, 2.4, 2.4, 2.4, 2.4, 2.4, 2.4, 2.4 };
uint32_t adc_channels_linearization_b_segment_1[_NUM_OF_SLIDERS] = 
	{ 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60 };
uint32_t adc_channels_linearization_offset_segment_1[_NUM_OF_SLIDERS] = 
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };


float adc_channels_linearization_slope_segment_2[_NUM_OF_SLIDERS] =
	{ 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8 };
uint32_t adc_channels_linearization_b_segment_2[_NUM_OF_SLIDERS] = 
	{ 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700 };
uint32_t adc_channels_linearization_offset_segment_2[_NUM_OF_SLIDERS] = 
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };


float adc_channels_linearization_slope_segment_3[_NUM_OF_SLIDERS] =
	{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 };
uint32_t adc_channels_linearization_b_segment_3[_NUM_OF_SLIDERS] = 
	{ -2340, -2340, -2340, -2340, -2340, -2340, -2340, -2340, -2340, -2340, -2340, -2340 };
uint32_t adc_channels_linearization_offset_segment_3[_NUM_OF_SLIDERS] = 
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

uint8_t midi_uart_rx_bytes[2];
uint8_t uart2_rx_bytes[2];


/* DEBUG */
uint16_t rotary_test_vector[64] = { 0 };
uint16_t rotary_test_counter = 0;

const uint8_t ROTARY_ENCODER_state_transition_table[7][4] = { 
		
	{ _ROTARY_STATE_CW_START, _ROTARY_STATE_START, _ROTARY_STATE_START, _ROTARY_STATE_CCW_START },
	{ _ROTARY_STATE_CW_START, _ROTARY_STATE_CW_MID, _ROTARY_STATE_CW_MID, _ROTARY_STATE_START },
	{ _ROTARY_STATE_START, _ROTARY_STATE_CW_MID, _ROTARY_STATE_CW_MID, _ROTARY_STATE_CW_END },
	{ _ROTARY_STATE_CW_START, _ROTARY_STATE_START, _ROTARY_STATE_START, _ROTARY_STATE_CW_END },
	{ _ROTARY_STATE_START, _ROTARY_STATE_CCW_MID, _ROTARY_STATE_START, _ROTARY_STATE_CCW_START },
	{ _ROTARY_STATE_CCW_END, _ROTARY_STATE_CCW_MID, _ROTARY_STATE_START, _ROTARY_STATE_START },
	{ _ROTARY_STATE_CCW_END, _ROTARY_STATE_START, _ROTARY_STATE_START, _ROTARY_STATE_CCW_START }	
};

const uint8_t ROTARY_ENCODERS_IDS[12] = { 
	_CONTROL_ENCODER_3,
	_CONTROL_ENCODER_2,
	_CONTROL_ENCODER_1,
	_CONTROL_ENCODER_6,
	_CONTROL_ENCODER_5,
	_CONTROL_ENCODER_4,
	_CONTROL_ENCODER_9,
	_CONTROL_ENCODER_8,
	_CONTROL_ENCODER_7,
	_CONTROL_ENCODER_12,
	_CONTROL_ENCODER_11,
	_CONTROL_ENCODER_10
};

const uint8_t ROTARY_ENCODERS_PUSHBUTTONS_IDS[12] = { 
	_CONTROL_ENCODER_PUSHBUTTON_3,
	_CONTROL_ENCODER_PUSHBUTTON_2,
	_CONTROL_ENCODER_PUSHBUTTON_1,
	_CONTROL_ENCODER_PUSHBUTTON_6,
	_CONTROL_ENCODER_PUSHBUTTON_5,
	_CONTROL_ENCODER_PUSHBUTTON_4,
	_CONTROL_ENCODER_PUSHBUTTON_9,
	_CONTROL_ENCODER_PUSHBUTTON_8,
	_CONTROL_ENCODER_PUSHBUTTON_7,
	_CONTROL_ENCODER_PUSHBUTTON_12,
	_CONTROL_ENCODER_PUSHBUTTON_11,
	_CONTROL_ENCODER_PUSHBUTTON_10
};



PUSHBUTTON_OBJ_t pushbutton_1 = { 
		.current_input_val = _INPUT_PIN_HIGH, 
		.prev_input_val = _INPUT_PIN_HIGH, 
		.state = _STATE_RELEASED,
		.prev_state = _STATE_RELEASED
};

PUSHBUTTON_OBJ_t pushbutton_2 = { 
	.current_input_val = _INPUT_PIN_HIGH, 
	.prev_input_val = _INPUT_PIN_HIGH, 
	.state = _STATE_RELEASED,
	.prev_state = _STATE_RELEASED
};

PUSHBUTTON_OBJ_t pushbutton_3 = { 
	.current_input_val = _INPUT_PIN_HIGH, 
	.prev_input_val = _INPUT_PIN_HIGH, 
	.state = _STATE_RELEASED,
	.prev_state = _STATE_RELEASED
};

PUSHBUTTON_OBJ_t pushbutton_4 = { 
	.current_input_val = _INPUT_PIN_HIGH, 
	.prev_input_val = _INPUT_PIN_HIGH, 
	.state = _STATE_RELEASED,
	.prev_state = _STATE_RELEASED
};

PUSHBUTTON_OBJ_t pushbutton_5 = { 
	.current_input_val = _INPUT_PIN_HIGH, 
	.prev_input_val = _INPUT_PIN_HIGH, 
	.state = _STATE_RELEASED,
	.prev_state = _STATE_RELEASED
};

PUSHBUTTON_OBJ_t pushbutton_6 = { 
	.current_input_val = _INPUT_PIN_HIGH, 
	.prev_input_val = _INPUT_PIN_HIGH, 
	.state = _STATE_RELEASED,
	.prev_state = _STATE_RELEASED
};

PUSHBUTTON_OBJ_t pushbutton_7 = { 
	.current_input_val = _INPUT_PIN_HIGH, 
	.prev_input_val = _INPUT_PIN_HIGH, 
	.state = _STATE_RELEASED,
	.prev_state = _STATE_RELEASED
};

PUSHBUTTON_OBJ_t pushbutton_8 = { 
	.current_input_val = _INPUT_PIN_HIGH, 
	.prev_input_val = _INPUT_PIN_HIGH, 
	.state = _STATE_RELEASED,
	.prev_state = _STATE_RELEASED
};

PUSHBUTTON_OBJ_t pushbutton_9 = { 
	.current_input_val = _INPUT_PIN_HIGH, 
	.prev_input_val = _INPUT_PIN_HIGH, 
	.state = _STATE_RELEASED,
	.prev_state = _STATE_RELEASED
};

PUSHBUTTON_OBJ_t pushbutton_10 = { 
	.current_input_val = _INPUT_PIN_HIGH, 
	.prev_input_val = _INPUT_PIN_HIGH, 
	.state = _STATE_RELEASED,
	.prev_state = _STATE_RELEASED
};

PUSHBUTTON_OBJ_t pushbutton_11 = { 
	.current_input_val = _INPUT_PIN_HIGH, 
	.prev_input_val = _INPUT_PIN_HIGH, 
	.state = _STATE_RELEASED,
	.prev_state = _STATE_RELEASED
};

PUSHBUTTON_OBJ_t pushbutton_12 = { 
	.current_input_val = _INPUT_PIN_HIGH, 
	.prev_input_val = _INPUT_PIN_HIGH, 
	.state = _STATE_RELEASED,
	.prev_state = _STATE_RELEASED
};

PUSHBUTTON_OBJ_t pushbutton_function_1 = { 
	.current_input_val = _INPUT_PIN_HIGH, 
	.prev_input_val = _INPUT_PIN_HIGH, 
	.state = _STATE_RELEASED,
	.prev_state = _STATE_RELEASED
};

PUSHBUTTON_OBJ_t pushbutton_function_2 = { 
	.current_input_val = _INPUT_PIN_HIGH, 
	.prev_input_val = _INPUT_PIN_HIGH, 
	.state = _STATE_RELEASED,
	.prev_state = _STATE_RELEASED
};

PUSHBUTTON_OBJ_t pushbutton_function_3 = { 
	.current_input_val = _INPUT_PIN_HIGH, 
	.prev_input_val = _INPUT_PIN_HIGH, 
	.state = _STATE_RELEASED,
	.prev_state = _STATE_RELEASED
};

PUSHBUTTON_OBJ_t *pushbuttons[_NUM_OF_PUSHBUTTONS] = { 
	&pushbutton_1,
	&pushbutton_2,
	&pushbutton_3,
	&pushbutton_4,
	&pushbutton_5,
	&pushbutton_6,
	&pushbutton_7,
	&pushbutton_8,
	&pushbutton_9,
	&pushbutton_10,
	&pushbutton_11,
	&pushbutton_12
};

PUSHBUTTON_OBJ_t *function_pushbuttons[_NUM_OF_FUNCTION_PUSHBUTTONS] = { 
	&pushbutton_function_1,
	&pushbutton_function_2,
	&pushbutton_function_3
};

ROTARY_ENCODER_OBJ_t rotary_encoder_1 = { 
	.current_input_a_val = _INPUT_PIN_HIGH,
	.current_input_b_val = _INPUT_PIN_HIGH,
	.transient_state = _ROTARY_STATE_START,
	.prev_transient_state = _ROTARY_STATE_START,
	.count = 0,
	.prev_count = 0,
	.pushbutton.current_input_val = _INPUT_PIN_HIGH,
	.pushbutton.prev_input_val = _INPUT_PIN_HIGH, 
	.pushbutton.state = _STATE_RELEASED
};

ROTARY_ENCODER_OBJ_t rotary_encoder_2 = { 
	.current_input_a_val = _INPUT_PIN_HIGH,
	.current_input_b_val = _INPUT_PIN_HIGH,
	.transient_state = _ROTARY_STATE_START,
	.prev_transient_state = _ROTARY_STATE_START,
	.count = 0,
	.prev_count = 0,
	.pushbutton.current_input_val = _INPUT_PIN_HIGH,
	.pushbutton.prev_input_val = _INPUT_PIN_HIGH, 
	.pushbutton.state = _STATE_RELEASED
};

ROTARY_ENCODER_OBJ_t rotary_encoder_3 = { 
	.current_input_a_val = _INPUT_PIN_HIGH,
	.current_input_b_val = _INPUT_PIN_HIGH,
	.transient_state = _ROTARY_STATE_START,
	.prev_transient_state = _ROTARY_STATE_START,
	.count = 0,
	.prev_count = 0,
	.pushbutton.current_input_val = _INPUT_PIN_HIGH,
	.pushbutton.prev_input_val = _INPUT_PIN_HIGH, 
	.pushbutton.state = _STATE_RELEASED
};

ROTARY_ENCODER_OBJ_t rotary_encoder_4 = { 
	.current_input_a_val = _INPUT_PIN_HIGH,
	.current_input_b_val = _INPUT_PIN_HIGH,
	.transient_state = _ROTARY_STATE_START,
	.prev_transient_state = _ROTARY_STATE_START,
	.count = 0,
	.prev_count = 0,
	.pushbutton.current_input_val = _INPUT_PIN_HIGH,
	.pushbutton.prev_input_val = _INPUT_PIN_HIGH, 
	.pushbutton.state = _STATE_RELEASED
};

ROTARY_ENCODER_OBJ_t rotary_encoder_5 = { 
	.current_input_a_val = _INPUT_PIN_HIGH,
	.current_input_b_val = _INPUT_PIN_HIGH,
	.transient_state = _ROTARY_STATE_START,
	.prev_transient_state = _ROTARY_STATE_START,
	.count = 0,
	.prev_count = 0,
	.pushbutton.current_input_val = _INPUT_PIN_HIGH,
	.pushbutton.prev_input_val = _INPUT_PIN_HIGH, 
	.pushbutton.state = _STATE_RELEASED
};

ROTARY_ENCODER_OBJ_t rotary_encoder_6 = { 
	.current_input_a_val = _INPUT_PIN_HIGH,
	.current_input_b_val = _INPUT_PIN_HIGH,
	.transient_state = _ROTARY_STATE_START,
	.prev_transient_state = _ROTARY_STATE_START,
	.count = 0,
	.prev_count = 0,
	.pushbutton.current_input_val = _INPUT_PIN_HIGH,
	.pushbutton.prev_input_val = _INPUT_PIN_HIGH, 
	.pushbutton.state = _STATE_RELEASED
};

ROTARY_ENCODER_OBJ_t rotary_encoder_7 = { 
	.current_input_a_val = _INPUT_PIN_HIGH,
	.current_input_b_val = _INPUT_PIN_HIGH,
	.transient_state = _ROTARY_STATE_START,
	.prev_transient_state = _ROTARY_STATE_START,
	.count = 0,
	.prev_count = 0,
	.pushbutton.current_input_val = _INPUT_PIN_HIGH,
	.pushbutton.prev_input_val = _INPUT_PIN_HIGH, 
	.pushbutton.state = _STATE_RELEASED
};

ROTARY_ENCODER_OBJ_t rotary_encoder_8 = { 
	.current_input_a_val = _INPUT_PIN_HIGH,
	.current_input_b_val = _INPUT_PIN_HIGH,
	.transient_state = _ROTARY_STATE_START,
	.prev_transient_state = _ROTARY_STATE_START,
	.count = 0,
	.prev_count = 0,
	.pushbutton.current_input_val = _INPUT_PIN_HIGH,
	.pushbutton.prev_input_val = _INPUT_PIN_HIGH, 
	.pushbutton.state = _STATE_RELEASED
};

ROTARY_ENCODER_OBJ_t rotary_encoder_9 = { 
	.current_input_a_val = _INPUT_PIN_HIGH,
	.current_input_b_val = _INPUT_PIN_HIGH,
	.transient_state = _ROTARY_STATE_START,
	.prev_transient_state = _ROTARY_STATE_START,
	.count = 0,
	.prev_count = 0,
	.pushbutton.current_input_val = _INPUT_PIN_HIGH,
	.pushbutton.prev_input_val = _INPUT_PIN_HIGH, 
	.pushbutton.state = _STATE_RELEASED
};

ROTARY_ENCODER_OBJ_t rotary_encoder_10 = { 
	.current_input_a_val = _INPUT_PIN_HIGH,
	.current_input_b_val = _INPUT_PIN_HIGH,
	.transient_state = _ROTARY_STATE_START,
	.prev_transient_state = _ROTARY_STATE_START,
	.count = 0,
	.prev_count = 0,
	.pushbutton.current_input_val = _INPUT_PIN_HIGH,
	.pushbutton.prev_input_val = _INPUT_PIN_HIGH, 
	.pushbutton.state = _STATE_RELEASED
};

ROTARY_ENCODER_OBJ_t rotary_encoder_11 = { 
	.current_input_a_val = _INPUT_PIN_HIGH,
	.current_input_b_val = _INPUT_PIN_HIGH,
	.transient_state = _ROTARY_STATE_START,
	.prev_transient_state = _ROTARY_STATE_START,
	.count = 0,
	.prev_count = 0,
	.pushbutton.current_input_val = _INPUT_PIN_HIGH,
	.pushbutton.prev_input_val = _INPUT_PIN_HIGH, 
	.pushbutton.state = _STATE_RELEASED
};

ROTARY_ENCODER_OBJ_t rotary_encoder_12 = { 
	.current_input_a_val = _INPUT_PIN_HIGH,
	.current_input_b_val = _INPUT_PIN_HIGH,
	.transient_state = _ROTARY_STATE_START,
	.prev_transient_state = _ROTARY_STATE_START,
	.count = 0,
	.prev_count = 0,
	.pushbutton.current_input_val = _INPUT_PIN_HIGH,
	.pushbutton.prev_input_val = _INPUT_PIN_HIGH, 
	.pushbutton.state = _STATE_RELEASED
};

ROTARY_ENCODER_OBJ_t *encoders[_NUM_OF_ENCODERS] = { 
	&rotary_encoder_1,
	&rotary_encoder_2,
	&rotary_encoder_3,
	&rotary_encoder_4,
	&rotary_encoder_5,
	&rotary_encoder_6,
	&rotary_encoder_7,
	&rotary_encoder_8,
	&rotary_encoder_9,
	&rotary_encoder_10,
	&rotary_encoder_11,
	&rotary_encoder_12
};


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART2_UART_Init(void);
void StartMainTask(void *argument);
void StartMidiUartRxTask(void *argument);
void StartMidiUartTxTask(void *argument);
void StartControlUartRxTask(void *argument);
void StartControlUartTxTask(void *argument);
void StartUpdateRotaryEncodersTask(void *argument);
void StartUpdateSlidersTask(void *argument);
void StartUpdatePushbuttonsTask(void *argument);
void StartUpdateYellowLedTask(void *argument);
void StartUpdateGreenLedTask(void *argument);
void onBoardLedCallback(void *argument);
void updateSlidersCallback(void *argument);
void updatePushbuttonsTimerCallback(void *argument);
void updateRotaryEncodersTimerCallback(void *argument);
void updateLedsTimerCallback(void *argument);

/* USER CODE BEGIN PFP */

//void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
void SLIDERS_update_state();
void ROTARY_ENCODER_update_state(ROTARY_ENCODER_OBJ_t *encoder,
	GPIO_TypeDef *GPIOx_A,
	uint16_t GPIO_A_Pin,
	GPIO_TypeDef *GPIOx_B,
	uint16_t GPIO_B_Pin);
void PUSHBUTTON_update_state(PUSHBUTTON_OBJ_t *pushbutton,
	GPIO_TypeDef *GPIOx,
	uint16_t GPIO_Pin);
void DEVICE_update_state();
void DEVICE_blink_led(uint16_t led_id, uint16_t led_on_ms, uint16_t led_off_ms);
void DEVICE_control_change(int control_id, uint16_t value);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	
	_CONTROL_BOARD_LED_GREEN_OFF;
	_CONTROL_BOARD_LED_YELLOW_OFF;

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of onBoardLedTimer */
  onBoardLedTimerHandle = osTimerNew(onBoardLedCallback, osTimerPeriodic, NULL, &onBoardLedTimer_attributes);

  /* creation of updateSlidersTimer */
  updateSlidersTimerHandle = osTimerNew(updateSlidersCallback, osTimerPeriodic, NULL, &updateSlidersTimer_attributes);

  /* creation of updatePushbuttonsTimer */
  updatePushbuttonsTimerHandle = osTimerNew(updatePushbuttonsTimerCallback, osTimerPeriodic, NULL, &updatePushbuttonsTimer_attributes);

  /* creation of updateRotaryEncodersTimer */
  updateRotaryEncodersTimerHandle = osTimerNew(updateRotaryEncodersTimerCallback, osTimerPeriodic, NULL, &updateRotaryEncodersTimer_attributes);

  /* creation of updateLedsTimer */
  updateLedsTimerHandle = osTimerNew(updateLedsTimerCallback, osTimerPeriodic, NULL, &updateLedsTimer_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
	
	osTimerStart(onBoardLedTimerHandle, 1000 / configTICK_RATE_HZ * _ON_BOARD_LED_BLINK_PERIOD_MS);
	
	osTimerStart(updateSlidersTimerHandle, 1000 / configTICK_RATE_HZ * _SLIDERS_SCAN_PERIOD_MS);
				 
	osTimerStart(updatePushbuttonsTimerHandle, 1000 / configTICK_RATE_HZ * _PUSHBUTTONS_SCAN_PERIOD_MS);
				 
	osTimerStart(updateRotaryEncodersTimerHandle, 1000 / configTICK_RATE_HZ * _ROTARY_ENCODERS_SCAN_PERIOD_MS);
	
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of midiUartTxQueue */
  midiUartTxQueueHandle = osMessageQueueNew (32, sizeof(UART_TX_MSGQUEUE_OBJ_t), &midiUartTxQueue_attributes);

  /* creation of controlUartRxQueue */
  controlUartRxQueueHandle = osMessageQueueNew (32, sizeof(UART_RX_MSGQUEUE_OBJ_t), &controlUartRxQueue_attributes);

  /* creation of controlUartTxQueue */
  controlUartTxQueueHandle = osMessageQueueNew (32, sizeof(UART_TX_MSGQUEUE_OBJ_t), &controlUartTxQueue_attributes);

  /* creation of yellowLedInputQueue */
  yellowLedInputQueueHandle = osMessageQueueNew (2, sizeof(LED_BLINK_PARAMS_OBJ_t), &yellowLedInputQueue_attributes);

  /* creation of greenLedInputQueue */
  greenLedInputQueueHandle = osMessageQueueNew (2, sizeof(LED_BLINK_PARAMS_OBJ_t), &greenLedInputQueue_attributes);

  /* creation of midiUartRxQueue */
  midiUartRxQueueHandle = osMessageQueueNew (32, sizeof(UART_RX_MSGQUEUE_OBJ_t), &midiUartRxQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of mainTask */
  mainTaskHandle = osThreadNew(StartMainTask, NULL, &mainTask_attributes);

  /* creation of midiUartRxTask */
  midiUartRxTaskHandle = osThreadNew(StartMidiUartRxTask, NULL, &midiUartRxTask_attributes);

  /* creation of midiUartTxTask */
  midiUartTxTaskHandle = osThreadNew(StartMidiUartTxTask, NULL, &midiUartTxTask_attributes);

  /* creation of controlUartRxTa */
  controlUartRxTaHandle = osThreadNew(StartControlUartRxTask, NULL, &controlUartRxTa_attributes);

  /* creation of controlUartTxTa */
  controlUartTxTaHandle = osThreadNew(StartControlUartTxTask, NULL, &controlUartTxTa_attributes);

  /* creation of updateRotaryEnc */
  updateRotaryEncHandle = osThreadNew(StartUpdateRotaryEncodersTask, NULL, &updateRotaryEnc_attributes);

  /* creation of updateSlidersTa */
  updateSlidersTaHandle = osThreadNew(StartUpdateSlidersTask, NULL, &updateSlidersTa_attributes);

  /* creation of updatePushbutto */
  updatePushbuttoHandle = osThreadNew(StartUpdatePushbuttonsTask, NULL, &updatePushbutto_attributes);

  /* creation of updateYellowLed */
  updateYellowLedHandle = osThreadNew(StartUpdateYellowLedTask, NULL, &updateYellowLed_attributes);

  /* creation of updateGreenLed */
  updateGreenLedHandle = osThreadNew(StartUpdateGreenLedTask, NULL, &updateGreenLed_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the event(s) */
  /* creation of updateEncodersEvent */
  updateEncodersEventHandle = osEventFlagsNew(&updateEncodersEvent_attributes);

  /* creation of updatePushbuttonsEvent */
  updatePushbuttonsEventHandle = osEventFlagsNew(&updatePushbuttonsEvent_attributes);

  /* creation of updateSlidersEvent */
  updateSlidersEventHandle = osEventFlagsNew(&updateSlidersEvent_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

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

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 12;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_12;
  sConfig.Rank = ADC_REGULAR_RANK_3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_REGULAR_RANK_4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = ADC_REGULAR_RANK_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_6;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = ADC_REGULAR_RANK_7;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_7;
  sConfig.Rank = ADC_REGULAR_RANK_8;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = ADC_REGULAR_RANK_9;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_10;
  sConfig.Rank = ADC_REGULAR_RANK_10;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_13;
  sConfig.Rank = ADC_REGULAR_RANK_11;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_11;
  sConfig.Rank = ADC_REGULAR_RANK_12;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 32500;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, ON_BOARD_LED_Pin|LED_4_OUT_Pin|LED_3_OUT_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED_1_OUT_Pin|LED_2_OUT_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PB_10_IN_Pin PB_9_IN_Pin PB_12_IN_Pin PB_11_IN_Pin
                           IO_1_Pin ROT_3_S_IN_Pin ROT_2_S_IN_Pin ROT_3_A_IN_Pin
                           ROT_2_B_IN_Pin ROT_2_A_IN_Pin ROT_1_S_IN_Pin ROT_1_B_IN_Pin
                           ROT_1_A_IN_Pin PB_FUNC_3_IN_Pin PB_8_IN_Pin PB_7_IN_Pin */
  GPIO_InitStruct.Pin = PB_10_IN_Pin|PB_9_IN_Pin|PB_12_IN_Pin|PB_11_IN_Pin
                          |IO_1_Pin|ROT_3_S_IN_Pin|ROT_2_S_IN_Pin|ROT_3_A_IN_Pin
                          |ROT_2_B_IN_Pin|ROT_2_A_IN_Pin|ROT_1_S_IN_Pin|ROT_1_B_IN_Pin
                          |ROT_1_A_IN_Pin|PB_FUNC_3_IN_Pin|PB_8_IN_Pin|PB_7_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : ON_BOARD_LED_Pin LED_4_OUT_Pin LED_3_OUT_Pin */
  GPIO_InitStruct.Pin = ON_BOARD_LED_Pin|LED_4_OUT_Pin|LED_3_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_1_OUT_Pin LED_2_OUT_Pin */
  GPIO_InitStruct.Pin = LED_1_OUT_Pin|LED_2_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : ROT_3_B_IN_Pin PB_FUNC_2_IN_Pin PB_FUNC_1_IN_Pin ROT_4_A_IN_Pin
                           ROT_4_B_IN_Pin ROT_4_S_IN_Pin ROT_5_A_IN_Pin PB_4_IN_Pin
                           PB_3_IN_Pin PB_2_IN_Pin PB_1_IN_Pin IO_4_Pin
                           PB8 */
  GPIO_InitStruct.Pin = ROT_3_B_IN_Pin|PB_FUNC_2_IN_Pin|PB_FUNC_1_IN_Pin|ROT_4_A_IN_Pin
                          |ROT_4_B_IN_Pin|ROT_4_S_IN_Pin|ROT_5_A_IN_Pin|PB_4_IN_Pin
                          |PB_3_IN_Pin|PB_2_IN_Pin|PB_1_IN_Pin|IO_4_Pin
                          |GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : ROT_5_B_IN_Pin ROT_5_S_IN_Pin ROT_6_A_IN_Pin ROT_6_B_IN_Pin
                           ROT_6_S_IN_Pin ROT_7_A_IN_Pin ROT_7_S_IN_Pin ROT_7_B_IN_Pin
                           ROT_11_S_IN_Pin ROT_12_B_IN_Pin ROT_12_S_IN_Pin PB_6_IN_Pin
                           PB_5_IN_Pin */
  GPIO_InitStruct.Pin = ROT_5_B_IN_Pin|ROT_5_S_IN_Pin|ROT_6_A_IN_Pin|ROT_6_B_IN_Pin
                          |ROT_6_S_IN_Pin|ROT_7_A_IN_Pin|ROT_7_S_IN_Pin|ROT_7_B_IN_Pin
                          |ROT_11_S_IN_Pin|ROT_12_B_IN_Pin|ROT_12_S_IN_Pin|PB_6_IN_Pin
                          |PB_5_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : ROT_8_B_IN_Pin ROT_8_A_IN_Pin ROT_9_A_IN_Pin ROT_8_S_IN_Pin
                           ROT_10_S_IN_Pin ROT_11_A_IN_Pin ROT_11_B_IN_Pin */
  GPIO_InitStruct.Pin = ROT_8_B_IN_Pin|ROT_8_A_IN_Pin|ROT_9_A_IN_Pin|ROT_8_S_IN_Pin
                          |ROT_10_S_IN_Pin|ROT_11_A_IN_Pin|ROT_11_B_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : ROT_9_B_IN_Pin ROT_9_S_IN_Pin ROT_10_A_IN_Pin ROT_10_B_IN_Pin */
  GPIO_InitStruct.Pin = ROT_9_B_IN_Pin|ROT_9_S_IN_Pin|ROT_10_A_IN_Pin|ROT_10_B_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : ROT_12_A_IN_Pin */
  GPIO_InitStruct.Pin = ROT_12_A_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ROT_12_A_IN_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	// Conversion Complete & DMA Transfer Complete As Well
	// So The AD_RES_BUFFER Is Now Updated
	
	osEventFlagsSet(updateSlidersEventHandle, _EVENT_FLAGS_MSK_UPDATE_SLIDERS);	
}


void SLIDERS_update_state()
{
	// Linearization
	for (int i = 0; i < _NUM_OF_SLIDERS; i++)
	{
		if (adc_channels_buffer[i] == 0)
		{
			adc_channels_compenstaed_buffer[i] = 0;
		}
		else if ((adc_channels_buffer[i] > 0) && (adc_channels_buffer[i] <= adc_channels_libearization_segment_1_end[i]))
		{
			adc_channels_compenstaed_buffer[i] = 
				(adc_channels_buffer[i] + adc_channels_linearization_b_segment_1[i]) * 
				adc_channels_linearization_slope_segment_1[i] + 
				adc_channels_linearization_offset_segment_1[i];
		}
		else if ((adc_channels_buffer[i] > adc_channels_libearization_segment_1_end[i]) && 
			(adc_channels_buffer[i] <= adc_channels_libearization_segment_2_end[i]))
		{
			adc_channels_compenstaed_buffer[i] = (adc_channels_buffer[i] + adc_channels_linearization_b_segment_2[i]) * 
				adc_channels_linearization_slope_segment_2[i] + 
				adc_channels_linearization_offset_segment_2[i];
			
		}
		else
		{
			adc_channels_compenstaed_buffer[i] = (adc_channels_buffer[i] + adc_channels_linearization_b_segment_3[i]) * 
				adc_channels_linearization_slope_segment_3[i] + 
				adc_channels_linearization_offset_segment_3[i];
		}
		
		if (adc_channels_compenstaed_buffer[i] > _MAX_SLIDER_VAL)
		{
			adc_channels_compenstaed_buffer[i] = _MAX_SLIDER_VAL;
		}
		
		adc_filtered_channels_compenstaed_buffer[i] = 
			adc_channels_compenstaed_buffer[i] * _SLIDERS_AVR_FACTOR +
			adc_filtered_channels_compenstaed_buffer[i] * (1 - _SLIDERS_AVR_FACTOR);
	}
	
}

void ROTARY_ENCODER_update_state(ROTARY_ENCODER_OBJ_t *encoder,
								GPIO_TypeDef *GPIOx_A, uint16_t GPIO_A_Pin,
								GPIO_TypeDef *GPIOx_B, uint16_t GPIO_B_Pin)
{
	encoder->current_input_a_val = HAL_GPIO_ReadPin(GPIOx_A, GPIO_A_Pin);
	encoder->current_input_b_val = HAL_GPIO_ReadPin(GPIOx_B, GPIO_B_Pin);
	
	encoder->transient_state = 
		ROTARY_ENCODER_state_transition_table[encoder->transient_state]
			[encoder->current_input_a_val + encoder->current_input_b_val * 2];
	
	/* NOTE: in used decoders there are some encoders with only 3 states 0, 1 and 3 */
	if (((encoder->transient_state == _ROTARY_STATE_CW_MID) &&
		(encoder->prev_transient_state == _ROTARY_STATE_CW_START)) ||
		((encoder->transient_state == _ROTARY_STATE_CW_MID) &&
		(encoder->prev_transient_state == _ROTARY_STATE_CW_END)) ||
		((encoder->transient_state == _ROTARY_STATE_CW_START) &&
		(encoder->prev_transient_state == _ROTARY_STATE_CW_END)))
	{
		encoder->count++;
	}
	else if (((encoder->transient_state == _ROTARY_STATE_CCW_MID) &&
		(encoder->prev_transient_state == _ROTARY_STATE_CCW_START)) ||
		((encoder->transient_state == _ROTARY_STATE_CCW_MID) &&
		(encoder->prev_transient_state == _ROTARY_STATE_CCW_END)) ||
		((encoder->transient_state == _ROTARY_STATE_CCW_START) &&
		(encoder->prev_transient_state == _ROTARY_STATE_CCW_END)))
	{
		encoder->count--;
	}
	
	/* DEBUG
	if (encoder->transient_state != encoder->prev_transient_state)
	{
		rotary_test_vector[rotary_test_counter & 0x7] = 
			encoder->transient_state;
		
		rotary_test_counter++;
		
	}
	*/
	
	encoder->prev_transient_state = encoder->transient_state;
}

void PUSHBUTTON_update_state(PUSHBUTTON_OBJ_t *pushbutton,
	GPIO_TypeDef *GPIOx,
	uint16_t GPIO_Pin)
{
	pushbutton->current_input_val = HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
	if (pushbutton->current_input_val == pushbutton->prev_input_val)
	{
		pushbutton->state = pushbutton->current_input_val;
	}
	
	pushbutton->prev_input_val = pushbutton->current_input_val;
}

void DEVICE_blink_led(uint16_t led_id, uint16_t led_on_ms, uint16_t led_off_ms)
{
	LED_BLINK_PARAMS_OBJ_t times;
	
	times.led_on_ms = led_on_ms;
	times.led_off_ms = led_off_ms;
	
	if (led_id == _CONTROL_YELLOW_LED)
	{
		osMessageQueuePut(yellowLedInputQueueHandle, &times, 0, 100);
	}
	else if (led_id == _CONTROL_GREEN_LED)
	{
		osMessageQueuePut(greenLedInputQueueHandle, &times, 0, 100);
	}
}

void DEVICE_update_state()
{
	/* Scan Sliders Values */
	for (int ch = 0; ch < _NUM_OF_SLIDERS; ch++)
	{
		if (abs((int)adc_channels_compenstaed_buffer[ch] - (int)prev_adc_filtered_channels_compenstaed_buffer[ch]) > _SLIDER_MIN_CHANGE)
		{
			/* Slider value changed */
			DEVICE_control_change(_CONTROL_SLIDER_1 + ch,
				adc_filtered_channels_compenstaed_buffer[ch]);
			
			prev_adc_filtered_channels_compenstaed_buffer[ch] = adc_filtered_channels_compenstaed_buffer[ch];
		}
	}
	
	/* Scan Pushbuttons */
	for (int pb = 0; pb < _NUM_OF_PUSHBUTTONS; pb++)
	{
		if (pushbuttons[pb]->state != pushbuttons[pb]->prev_state)
		{
			/* Pushbutton state changed */
			DEVICE_control_change(_CONTROL_PUSHBUTTON_1 + pb,
				pushbuttons[pb]->state);
			
			pushbuttons[pb]->prev_state = pushbuttons[pb]->state;
		}
	}
	
	/* Scan Function Pushbuttons */
	for (int fpb = 0; fpb < _NUM_OF_FUNCTION_PUSHBUTTONS; fpb++)
	{
		if (function_pushbuttons[fpb]->state != function_pushbuttons[fpb]->prev_state)
		{
			/* Function Pushbutton state changed */
			DEVICE_control_change(_CONTROL_FUNCTION_PUSHBUTTON_1 + fpb,
				function_pushbuttons[fpb]->state);
			
			function_pushbuttons[fpb]->prev_state = function_pushbuttons[fpb]->state;
		}
	}
	
	/* Scan Encoders */
	for (int enc = 0; enc < _NUM_OF_ENCODERS; enc++)
	{
		if (encoders[enc]->count != encoders[enc]->prev_count)			
		{
			/* Encoder value changed */
			DEVICE_control_change(ROTARY_ENCODERS_IDS[enc],
				encoders[enc]->count);
			
			encoders[enc]->prev_count = encoders[enc]->count;
		}
		
		if (encoders[enc]->pushbutton.state != encoders[enc]->pushbutton.prev_state)
		{
			DEVICE_control_change(ROTARY_ENCODERS_PUSHBUTTONS_IDS[enc],
				encoders[enc]->pushbutton.state);
			
			encoders[enc]->pushbutton.prev_state = encoders[enc]->pushbutton.state;
		}
	}	
}

void DEVICE_control_change(int control_id, uint16_t value)
{
	
	uint8_t *sysex_command_ptr;
	UART_TX_MSGQUEUE_OBJ_t control_mssg;
	
	sysex_command_ptr = MIDI_COMMAND_build_sysex_control_mssg(control_id, value);
	
	for (int i = 0; i < 8; i++)
	{
		control_mssg.Buf[i] = *(sysex_command_ptr + i);
	}
	
	control_mssg.data_size = 8;
	control_mssg.uartid = _CONTROL_UART;
	
	osMessageQueuePut(controlUartTxQueueHandle, &control_mssg, 0, 10);
	
	// Blink the led
	DEVICE_blink_led(_CONTROL_YELLOW_LED, 100, 100);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	UART_RX_MSGQUEUE_OBJ_t mssg;
	osStatus res;
	
	if (huart == &huart1)
	{
		/* Handle MIDI interface input stream */
		mssg.uartid = _MIDI_UART;
		mssg.Buf[0] = midi_uart_rx_bytes[0];
		mssg.data_size = 1;
		res = osMessageQueuePut(midiUartRxQueueHandle, &mssg, 0, 0); // Interupt: TO = 0
		
		HAL_UART_Receive_IT(&huart1, &midi_uart_rx_bytes[0], 1);
	}
	else if (huart == &huart2)
	{
		/* Handle messages received from the Raspberry Pi */
		mssg.uartid = _CONTROL_UART;
		mssg.Buf[0] = uart2_rx_bytes[0];
		mssg.data_size = 1;
		res = osMessageQueuePut(controlUartRxQueueHandle, &mssg, 0, 0);
		
		HAL_UART_Receive_IT(&huart2, uart2_rx_bytes, 1);
	}
}



/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartMainTask */
/**
  * @brief  Function implementing the mainTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartMainTask */
void StartMainTask(void *argument)
{
  /* USER CODE BEGIN 5 */
	/* Initialize */
	/* Scan Sliders Values */
	HAL_ADC_Start_DMA(&hadc1, adc_channels_buffer, _NUM_OF_SLIDERS);
	osDelay(500);
	
	for (int ch = 0; ch < _NUM_OF_SLIDERS; ch++)
	{		
		prev_adc_filtered_channels_compenstaed_buffer[ch] = adc_filtered_channels_compenstaed_buffer[ch];
	}
	
  /* Infinite loop */
	
  for(;;)
  {
	  DEVICE_update_state();
	  osDelay(_MAIN_TASK_PERIOD_MS);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartMidiUartRxTask */
/**
* @brief Function implementing the midiUartRxTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMidiUartRxTask */
void StartMidiUartRxTask(void *argument)
{
  /* USER CODE BEGIN StartMidiUartRxTask */
	
	HAL_UART_Receive_IT(&huart1, &midi_uart_rx_bytes[0], 1);
	
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartMidiUartRxTask */
}

/* USER CODE BEGIN Header_StartMidiUartTxTask */
/**
* @brief Function implementing the midiUartTxTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMidiUartTxTask */
void StartMidiUartTxTask(void *argument)
{
  /* USER CODE BEGIN StartMidiUartTxTask */
	
	UART_TX_MSGQUEUE_OBJ_t mssg;
	osStatus_t status;

	//mssg.Idx = 0;
	//sprintf(&mssg.Buf, "STM MIDI UART TX TEST MSSG");
	
	/* Infinite loop */
	for (;;)
	{
    
		//HAL_UART_Transmit(&huart1, &mssg.Buf[0], sizeof(mssg.Buf), HAL_MAX_DELAY);
		//mssg.Idx++;
	  
		osDelay(1000); // TODO: 1!!!
	}
  /* USER CODE END StartMidiUartTxTask */
}

/* USER CODE BEGIN Header_StartControlUartRxTask */
/**
* @brief Function implementing the controlUartRxTa thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartControlUartRxTask */
void StartControlUartRxTask(void *argument)
{
  /* USER CODE BEGIN StartControlUartRxTask */
	
	HAL_UART_Receive_IT(&huart2, uart2_rx_bytes, 1);
	
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartControlUartRxTask */
}

/* USER CODE BEGIN Header_StartControlUartTxTask */
/**
* @brief Function implementing the controlUartTxTa thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartControlUartTxTask */
void StartControlUartTxTask(void *argument)
{
  /* USER CODE BEGIN StartControlUartTxTask */
	
	UART_RX_MSGQUEUE_OBJ_t midi_mssg;
	UART_TX_MSGQUEUE_OBJ_t control_mssg;
	osStatus_t res;
	int state;
	uint8_t *midi_command_ptr;
	uint8_t midi_command[_MIDI_PARSER_COMMAND_BUFFER_LEN];
	uint8_t midi_command_length;	
	
  /* Infinite loop */
  for(;;)
  {
	  /* Handle external MIDI input interface messages - get next byte */
	  res = osMessageQueueGet(midiUartRxQueueHandle, &midi_mssg, NULL, 1);
	  
	  if (res == osOK)
	  {
		  state = MIDI_PARSER_add_byte(midi_mssg.Buf[0]);
		  if (state == _MIDI_PARSER_STATE_NEW_COMMAND_AVAILABLE) 
		  {
			  /* Get and add the data bytes. */
			  midi_command_ptr = MIDI_PARSER_get_last_command();
			  midi_command_length = *midi_command_ptr;
			  for (int i = 1; i <= midi_command_length; i++)
			  {
				  midi_command[i+1] = *(midi_command_ptr + i);
			  }
			  
			  /* Add the source and message length */
			  midi_command[0] = _MIDI_UART;
			  midi_command[1] = midi_command_length;
			  
			  HAL_UART_Transmit(&huart2, &midi_command[0], midi_command_length + 2, HAL_MAX_DELAY);
			  
			  /* Dummy call to intialize the parser. */
			  MIDI_PARSER_add_byte(0);
		  }		  		  
	  }
	  
	  /* Handle internal control messages */
	  res = osMessageQueueGet(controlUartTxQueueHandle, &control_mssg, NULL, 10);
	  
	  if (res == osOK)
	  {
		  for (int i = 1; i <= control_mssg.data_size; i++)
		  {
			  midi_command[i + 1] = control_mssg.Buf[i-1];
		  }
			  
		  /* Add the source and message length */
		  midi_command[0] = _CONTROL_UART;
		  midi_command[1] = control_mssg.data_size;
		 
		  HAL_UART_Transmit(&huart2, &midi_command[0], control_mssg.data_size + 2, HAL_MAX_DELAY);
	  }	 
	  
	  osDelay(1);   // TODO: 1!!!
  }
  /* USER CODE END StartControlUartTxTask */
}

/* USER CODE BEGIN Header_StartUpdateRotaryEncodersTask */
/**
* @brief Function implementing the updateRotaryEnc thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUpdateRotaryEncodersTask */
void StartUpdateRotaryEncodersTask(void *argument)
{
  /* USER CODE BEGIN StartUpdateRotaryEncodersTask */
	
	uint32_t flags;
	
  /* Infinite loop */
  for(;;)
  {
    
	  flags = osEventFlagsWait(updateEncodersEventHandle, 
		  _EVENT_FLAGS_MSK_UPDATE_ENCODERS, 
		  osFlagsWaitAny, osWaitForever);
	  
	  ROTARY_ENCODER_update_state(&rotary_encoder_1,
		  ROT_1_A_IN_GPIO_Port, 
		  ROT_1_A_IN_Pin, 
		  ROT_1_B_IN_GPIO_Port, 
		  ROT_1_B_IN_Pin);
	
	  ROTARY_ENCODER_update_state(&rotary_encoder_2,
		  ROT_2_A_IN_GPIO_Port,
		  ROT_2_A_IN_Pin, 
		  ROT_2_B_IN_GPIO_Port,
		  ROT_2_B_IN_Pin);
	
	  ROTARY_ENCODER_update_state(&rotary_encoder_3,
		  ROT_3_A_IN_GPIO_Port, 
		  ROT_3_A_IN_Pin,
		  ROT_3_B_IN_GPIO_Port, 
		  ROT_3_B_IN_Pin);
	
	  ROTARY_ENCODER_update_state(&rotary_encoder_4,
		  ROT_4_A_IN_GPIO_Port,
		  ROT_4_A_IN_Pin, 
		  ROT_4_B_IN_GPIO_Port,
		  ROT_4_B_IN_Pin);
	
	  ROTARY_ENCODER_update_state(&rotary_encoder_5,
		  ROT_5_A_IN_GPIO_Port,
		  ROT_5_A_IN_Pin, 
		  ROT_5_B_IN_GPIO_Port,
		  ROT_5_B_IN_Pin);
	
	  ROTARY_ENCODER_update_state(&rotary_encoder_6,
		  ROT_6_A_IN_GPIO_Port,
		  ROT_6_A_IN_Pin, 
		  ROT_6_B_IN_GPIO_Port,
		  ROT_6_B_IN_Pin);
	
	  ROTARY_ENCODER_update_state(&rotary_encoder_7,
		  ROT_7_A_IN_GPIO_Port,
		  ROT_7_A_IN_Pin, 
		  ROT_7_B_IN_GPIO_Port,
		  ROT_7_B_IN_Pin);
	
	  ROTARY_ENCODER_update_state(&rotary_encoder_8,
		  ROT_8_A_IN_GPIO_Port,
		  ROT_8_A_IN_Pin, 
		  ROT_8_B_IN_GPIO_Port,
		  ROT_8_B_IN_Pin);
	
	  ROTARY_ENCODER_update_state(&rotary_encoder_9,
		  ROT_9_A_IN_GPIO_Port,
		  ROT_9_A_IN_Pin, 
		  ROT_9_B_IN_GPIO_Port,
		  ROT_9_B_IN_Pin);
	
	  ROTARY_ENCODER_update_state(&rotary_encoder_10,
		  ROT_10_A_IN_GPIO_Port,
		  ROT_10_A_IN_Pin, 
		  ROT_10_B_IN_GPIO_Port,
		  ROT_10_B_IN_Pin);
	
	  ROTARY_ENCODER_update_state(&rotary_encoder_11,
		  ROT_11_A_IN_GPIO_Port,
		  ROT_11_A_IN_Pin, 
		  ROT_11_B_IN_GPIO_Port,
		  ROT_11_B_IN_Pin);
	
	  ROTARY_ENCODER_update_state(&rotary_encoder_12,
		  ROT_12_A_IN_GPIO_Port,
		  ROT_12_A_IN_Pin, 
		  ROT_12_B_IN_GPIO_Port,
		  ROT_12_B_IN_Pin);
  }
  /* USER CODE END StartUpdateRotaryEncodersTask */
}

/* USER CODE BEGIN Header_StartUpdateSlidersTask */
/**
* @brief Function implementing the updateSlidersTa thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUpdateSlidersTask */
void StartUpdateSlidersTask(void *argument)
{
  /* USER CODE BEGIN StartUpdateSlidersTask */
	
	uint32_t flags;
	
  /* Infinite loop */
  for(;;)
  {
	  flags = osEventFlagsWait(updateSlidersEventHandle, 
		  _EVENT_FLAGS_MSK_UPDATE_SLIDERS, 
		  osFlagsWaitAny,
		  osWaitForever);
	  
	  SLIDERS_update_state();
  }
  /* USER CODE END StartUpdateSlidersTask */
}

/* USER CODE BEGIN Header_StartUpdatePushbuttonsTask */
/**
* @brief Function implementing the updatePushbutto thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUpdatePushbuttonsTask */
void StartUpdatePushbuttonsTask(void *argument)
{
  /* USER CODE BEGIN StartUpdatePushbuttonsTask */
	
	uint32_t flags;
	
  /* Infinite loop */
  for(;;)
  {
	  flags = osEventFlagsWait(updatePushbuttonsEventHandle, 
		  _EVENT_FLAGS_MSK_UPDATE_PUSHBUTTONS, 
		  osFlagsWaitAny,
		  osWaitForever);
	  
	  PUSHBUTTON_update_state(&pushbutton_1, PB_1_IN_GPIO_Port, PB_1_IN_Pin);
	  PUSHBUTTON_update_state(&pushbutton_2, PB_2_IN_GPIO_Port, PB_2_IN_Pin);
	  PUSHBUTTON_update_state(&pushbutton_3, PB_3_IN_GPIO_Port, PB_3_IN_Pin);
	  PUSHBUTTON_update_state(&pushbutton_4, PB_4_IN_GPIO_Port, PB_4_IN_Pin);
	  PUSHBUTTON_update_state(&pushbutton_5, PB_5_IN_GPIO_Port, PB_5_IN_Pin);
	  PUSHBUTTON_update_state(&pushbutton_6, PB_6_IN_GPIO_Port, PB_6_IN_Pin);
	  PUSHBUTTON_update_state(&pushbutton_7, PB_7_IN_GPIO_Port, PB_7_IN_Pin);
	  PUSHBUTTON_update_state(&pushbutton_8, PB_8_IN_GPIO_Port, PB_8_IN_Pin);
	  PUSHBUTTON_update_state(&pushbutton_9, PB_9_IN_GPIO_Port, PB_9_IN_Pin);
	  PUSHBUTTON_update_state(&pushbutton_10, PB_10_IN_GPIO_Port, PB_10_IN_Pin);
	  PUSHBUTTON_update_state(&pushbutton_11, PB_11_IN_GPIO_Port, PB_11_IN_Pin);
	  PUSHBUTTON_update_state(&pushbutton_12, PB_12_IN_GPIO_Port, PB_12_IN_Pin);
	  
	  PUSHBUTTON_update_state(&pushbutton_function_1, PB_FUNC_1_IN_GPIO_Port, PB_FUNC_1_IN_Pin);
	  PUSHBUTTON_update_state(&pushbutton_function_2, PB_FUNC_2_IN_GPIO_Port, PB_FUNC_2_IN_Pin);
	  PUSHBUTTON_update_state(&pushbutton_function_3, PB_FUNC_3_IN_GPIO_Port, PB_FUNC_3_IN_Pin);
	  
		
	  // Rotary encoder push switch.
	  PUSHBUTTON_update_state(&rotary_encoder_1.pushbutton, ROT_1_S_IN_GPIO_Port, ROT_1_S_IN_Pin);
	  PUSHBUTTON_update_state(&rotary_encoder_2.pushbutton, ROT_2_S_IN_GPIO_Port, ROT_2_S_IN_Pin);
	  PUSHBUTTON_update_state(&rotary_encoder_3.pushbutton, ROT_3_S_IN_GPIO_Port, ROT_3_S_IN_Pin);
	  PUSHBUTTON_update_state(&rotary_encoder_4.pushbutton, ROT_4_S_IN_GPIO_Port, ROT_4_S_IN_Pin);
	  PUSHBUTTON_update_state(&rotary_encoder_5.pushbutton, ROT_5_S_IN_GPIO_Port, ROT_5_S_IN_Pin);
	  PUSHBUTTON_update_state(&rotary_encoder_6.pushbutton, ROT_6_S_IN_GPIO_Port, ROT_6_S_IN_Pin);
	  PUSHBUTTON_update_state(&rotary_encoder_7.pushbutton, ROT_7_S_IN_GPIO_Port, ROT_7_S_IN_Pin);
	  PUSHBUTTON_update_state(&rotary_encoder_8.pushbutton, ROT_8_S_IN_GPIO_Port, ROT_8_S_IN_Pin);
	  PUSHBUTTON_update_state(&rotary_encoder_9.pushbutton, ROT_9_S_IN_GPIO_Port, ROT_9_S_IN_Pin);
	  PUSHBUTTON_update_state(&rotary_encoder_10.pushbutton, ROT_10_S_IN_GPIO_Port, ROT_10_S_IN_Pin);
	  PUSHBUTTON_update_state(&rotary_encoder_11.pushbutton, ROT_11_S_IN_GPIO_Port, ROT_11_S_IN_Pin);
	  PUSHBUTTON_update_state(&rotary_encoder_12.pushbutton, ROT_12_S_IN_GPIO_Port, ROT_12_S_IN_Pin);
  }
  /* USER CODE END StartUpdatePushbuttonsTask */
}

/* USER CODE BEGIN Header_StartUpdateYellowLedTask */
/**
* @brief Function implementing the updateYellowLed thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUpdateYellowLedTask */
void StartUpdateYellowLedTask(void *argument)
{
  /* USER CODE BEGIN StartUpdateYellowLedTask */
	
	LED_BLINK_PARAMS_OBJ_t times;
	osStatus res;
	
  /* Infinite loop */
  for(;;)
  {
	  res = osMessageQueueGet(yellowLedInputQueueHandle, &times, NULL, 10);
	  
	  if (res == osOK)
	  {
		  _CONTROL_BOARD_LED_YELLOW_ON;
		  osDelay(times.led_on_ms);
		  _CONTROL_BOARD_LED_YELLOW_OFF;
		  osDelay(times.led_off_ms);
	  }
	  
	  //osDelay(10);
  }
  /* USER CODE END StartUpdateYellowLedTask */
}

/* USER CODE BEGIN Header_StartUpdateGreenLedTask */
/**
* @brief Function implementing the updateGreenLed thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUpdateGreenLedTask */
void StartUpdateGreenLedTask(void *argument)
{
  /* USER CODE BEGIN StartUpdateGreenLedTask */
	
	LED_BLINK_PARAMS_OBJ_t times;
	osStatus res;
	
  /* Infinite loop */
  for(;;)
  {
	  res = osMessageQueueGet(greenLedInputQueueHandle, &times, NULL, 10);
	  
	  if (res == osOK)
	  {
		  _CONTROL_BOARD_LED_GREEN_ON;
		  osDelay(times.led_on_ms);
		  _CONTROL_BOARD_LED_GREEN_OFF;
		  osDelay(times.led_off_ms);
	  }
	  
	  //osDelay(1);
  }
  /* USER CODE END StartUpdateGreenLedTask */
}

/* onBoardLedCallback function */
void onBoardLedCallback(void *argument)
{
  /* USER CODE BEGIN onBoardLedCallback */
	
	static int led_state = 0;
	
	// Blink the LED
	if (led_state == 0) 
	{
		_ON_BOARD_LED_SWITCH_OFF;
		led_state = 1;
	}
	else
	{
		_ON_BOARD_LED_SWITCH_ON;
		led_state = 0;
	}

  /* USER CODE END onBoardLedCallback */
}

/* updateSlidersCallback function */
void updateSlidersCallback(void *argument)
{
  /* USER CODE BEGIN updateSlidersCallback */

	HAL_ADC_Start_DMA(&hadc1, adc_channels_buffer, _NUM_OF_SLIDERS);
	
  /* USER CODE END updateSlidersCallback */
}

/* updatePushbuttonsTimerCallback function */
void updatePushbuttonsTimerCallback(void *argument)
{
  /* USER CODE BEGIN updatePushbuttonsTimerCallback */
	
	osEventFlagsSet(updatePushbuttonsEventHandle, _EVENT_FLAGS_MSK_UPDATE_PUSHBUTTONS);

  /* USER CODE END updatePushbuttonsTimerCallback */
}

/* updateRotaryEncodersTimerCallback function */
void updateRotaryEncodersTimerCallback(void *argument)
{
  /* USER CODE BEGIN updateRotaryEncodersTimerCallback */

	osEventFlagsSet(updateEncodersEventHandle, _EVENT_FLAGS_MSK_UPDATE_ENCODERS);
	
  /* USER CODE END updateRotaryEncodersTimerCallback */
}

/* updateLedsTimerCallback function */
void updateLedsTimerCallback(void *argument)
{
  /* USER CODE BEGIN updateLedsTimerCallback */

  /* USER CODE END updateLedsTimerCallback */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

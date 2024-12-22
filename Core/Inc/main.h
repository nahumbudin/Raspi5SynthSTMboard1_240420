/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MSGQUEUE_OBJECTS 32
#define PB_10_IN_Pin GPIO_PIN_2
#define PB_10_IN_GPIO_Port GPIOE
#define PB_9_IN_Pin GPIO_PIN_3
#define PB_9_IN_GPIO_Port GPIOE
#define PB_12_IN_Pin GPIO_PIN_4
#define PB_12_IN_GPIO_Port GPIOE
#define PB_11_IN_Pin GPIO_PIN_5
#define PB_11_IN_GPIO_Port GPIOE
#define IO_1_Pin GPIO_PIN_6
#define IO_1_GPIO_Port GPIOE
#define ON_BOARD_LED_Pin GPIO_PIN_13
#define ON_BOARD_LED_GPIO_Port GPIOC
#define SLIDER_10_IN_Pin GPIO_PIN_0
#define SLIDER_10_IN_GPIO_Port GPIOC
#define SLIDER_12_IN_Pin GPIO_PIN_1
#define SLIDER_12_IN_GPIO_Port GPIOC
#define SLIDER_3_IN_Pin GPIO_PIN_2
#define SLIDER_3_IN_GPIO_Port GPIOC
#define SLIDER_11_IN_Pin GPIO_PIN_3
#define SLIDER_11_IN_GPIO_Port GPIOC
#define SLIDER_2_IN_Pin GPIO_PIN_0
#define SLIDER_2_IN_GPIO_Port GPIOA
#define SLIDER_1_IN_Pin GPIO_PIN_1
#define SLIDER_1_IN_GPIO_Port GPIOA
#define SLIDER_5_IN_Pin GPIO_PIN_2
#define SLIDER_5_IN_GPIO_Port GPIOA
#define SLIDER_6_IN_Pin GPIO_PIN_3
#define SLIDER_6_IN_GPIO_Port GPIOA
#define SLIDER_4_IN_Pin GPIO_PIN_4
#define SLIDER_4_IN_GPIO_Port GPIOA
#define SLIDER_9_IN_Pin GPIO_PIN_5
#define SLIDER_9_IN_GPIO_Port GPIOA
#define SLIDER_7_IN_Pin GPIO_PIN_6
#define SLIDER_7_IN_GPIO_Port GPIOA
#define SLIDER_8_IN_Pin GPIO_PIN_7
#define SLIDER_8_IN_GPIO_Port GPIOA
#define LED_4_OUT_Pin GPIO_PIN_4
#define LED_4_OUT_GPIO_Port GPIOC
#define LED_3_OUT_Pin GPIO_PIN_5
#define LED_3_OUT_GPIO_Port GPIOC
#define LED_1_OUT_Pin GPIO_PIN_0
#define LED_1_OUT_GPIO_Port GPIOB
#define LED_2_OUT_Pin GPIO_PIN_1
#define LED_2_OUT_GPIO_Port GPIOB
#define ROT_3_B_IN_Pin GPIO_PIN_2
#define ROT_3_B_IN_GPIO_Port GPIOB
#define ROT_3_S_IN_Pin GPIO_PIN_7
#define ROT_3_S_IN_GPIO_Port GPIOE
#define ROT_2_S_IN_Pin GPIO_PIN_8
#define ROT_2_S_IN_GPIO_Port GPIOE
#define ROT_3_A_IN_Pin GPIO_PIN_9
#define ROT_3_A_IN_GPIO_Port GPIOE
#define ROT_2_B_IN_Pin GPIO_PIN_10
#define ROT_2_B_IN_GPIO_Port GPIOE
#define ROT_2_A_IN_Pin GPIO_PIN_11
#define ROT_2_A_IN_GPIO_Port GPIOE
#define ROT_1_S_IN_Pin GPIO_PIN_12
#define ROT_1_S_IN_GPIO_Port GPIOE
#define ROT_1_B_IN_Pin GPIO_PIN_13
#define ROT_1_B_IN_GPIO_Port GPIOE
#define ROT_1_A_IN_Pin GPIO_PIN_14
#define ROT_1_A_IN_GPIO_Port GPIOE
#define PB_FUNC_3_IN_Pin GPIO_PIN_15
#define PB_FUNC_3_IN_GPIO_Port GPIOE
#define PB_FUNC_2_IN_Pin GPIO_PIN_10
#define PB_FUNC_2_IN_GPIO_Port GPIOB
#define PB_FUNC_1_IN_Pin GPIO_PIN_11
#define PB_FUNC_1_IN_GPIO_Port GPIOB
#define ROT_4_A_IN_Pin GPIO_PIN_12
#define ROT_4_A_IN_GPIO_Port GPIOB
#define ROT_4_B_IN_Pin GPIO_PIN_13
#define ROT_4_B_IN_GPIO_Port GPIOB
#define ROT_4_S_IN_Pin GPIO_PIN_14
#define ROT_4_S_IN_GPIO_Port GPIOB
#define ROT_5_A_IN_Pin GPIO_PIN_15
#define ROT_5_A_IN_GPIO_Port GPIOB
#define ROT_5_B_IN_Pin GPIO_PIN_8
#define ROT_5_B_IN_GPIO_Port GPIOD
#define ROT_5_S_IN_Pin GPIO_PIN_9
#define ROT_5_S_IN_GPIO_Port GPIOD
#define ROT_6_A_IN_Pin GPIO_PIN_10
#define ROT_6_A_IN_GPIO_Port GPIOD
#define ROT_6_B_IN_Pin GPIO_PIN_11
#define ROT_6_B_IN_GPIO_Port GPIOD
#define ROT_6_S_IN_Pin GPIO_PIN_12
#define ROT_6_S_IN_GPIO_Port GPIOD
#define ROT_7_A_IN_Pin GPIO_PIN_13
#define ROT_7_A_IN_GPIO_Port GPIOD
#define ROT_7_S_IN_Pin GPIO_PIN_14
#define ROT_7_S_IN_GPIO_Port GPIOD
#define ROT_7_B_IN_Pin GPIO_PIN_15
#define ROT_7_B_IN_GPIO_Port GPIOD
#define ROT_8_B_IN_Pin GPIO_PIN_6
#define ROT_8_B_IN_GPIO_Port GPIOC
#define ROT_8_A_IN_Pin GPIO_PIN_7
#define ROT_8_A_IN_GPIO_Port GPIOC
#define ROT_9_A_IN_Pin GPIO_PIN_8
#define ROT_9_A_IN_GPIO_Port GPIOC
#define ROT_8_S_IN_Pin GPIO_PIN_9
#define ROT_8_S_IN_GPIO_Port GPIOC
#define ROT_9_B_IN_Pin GPIO_PIN_8
#define ROT_9_B_IN_GPIO_Port GPIOA
#define MIDI_OUT_Pin GPIO_PIN_9
#define MIDI_OUT_GPIO_Port GPIOA
#define MIDI_IN_Pin GPIO_PIN_10
#define MIDI_IN_GPIO_Port GPIOA
#define ROT_9_S_IN_Pin GPIO_PIN_11
#define ROT_9_S_IN_GPIO_Port GPIOA
#define ROT_10_A_IN_Pin GPIO_PIN_12
#define ROT_10_A_IN_GPIO_Port GPIOA
#define ROT_10_B_IN_Pin GPIO_PIN_15
#define ROT_10_B_IN_GPIO_Port GPIOA
#define ROT_10_S_IN_Pin GPIO_PIN_10
#define ROT_10_S_IN_GPIO_Port GPIOC
#define ROT_11_A_IN_Pin GPIO_PIN_11
#define ROT_11_A_IN_GPIO_Port GPIOC
#define ROT_11_B_IN_Pin GPIO_PIN_12
#define ROT_11_B_IN_GPIO_Port GPIOC
#define ROT_11_S_IN_Pin GPIO_PIN_0
#define ROT_11_S_IN_GPIO_Port GPIOD
#define ROT_12_A_IN_Pin GPIO_PIN_1
#define ROT_12_A_IN_GPIO_Port GPIOD
#define ROT_12_B_IN_Pin GPIO_PIN_2
#define ROT_12_B_IN_GPIO_Port GPIOD
#define ROT_12_S_IN_Pin GPIO_PIN_3
#define ROT_12_S_IN_GPIO_Port GPIOD
#define PB_6_IN_Pin GPIO_PIN_4
#define PB_6_IN_GPIO_Port GPIOD
#define CONTROL_OUT_Pin GPIO_PIN_5
#define CONTROL_OUT_GPIO_Port GPIOD
#define CONTROL_IN_Pin GPIO_PIN_6
#define CONTROL_IN_GPIO_Port GPIOD
#define PB_5_IN_Pin GPIO_PIN_7
#define PB_5_IN_GPIO_Port GPIOD
#define PB_4_IN_Pin GPIO_PIN_3
#define PB_4_IN_GPIO_Port GPIOB
#define PB_3_IN_Pin GPIO_PIN_4
#define PB_3_IN_GPIO_Port GPIOB
#define PB_2_IN_Pin GPIO_PIN_5
#define PB_2_IN_GPIO_Port GPIOB
#define PB_1_IN_Pin GPIO_PIN_6
#define PB_1_IN_GPIO_Port GPIOB
#define IO_4_Pin GPIO_PIN_7
#define IO_4_GPIO_Port GPIOB
#define PB_8_IN_Pin GPIO_PIN_0
#define PB_8_IN_GPIO_Port GPIOE
#define PB_7_IN_Pin GPIO_PIN_1
#define PB_7_IN_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */
	
#define _EVENT_FLAGS_MSK_UPDATE_ENCODERS 0x00000001U
#define _EVENT_FLAGS_MSK_UPDATE_PUSHBUTTONS 0x00000010U
#define _EVENT_FLAGS_MSK_UPDATE_SLIDERS 0x00000100U
	
#define _ON_BOARD_LED_BLINK_PERIOD_MS 500U	
#define _ON_BOARD_LED_SWITCH_ON HAL_GPIO_WritePin(ON_BOARD_LED_GPIO_Port, ON_BOARD_LED_Pin, 0)
#define _ON_BOARD_LED_SWITCH_OFF HAL_GPIO_WritePin(ON_BOARD_LED_GPIO_Port, ON_BOARD_LED_Pin, 1)
	
#define _CONTROL_BOARD_LED_YELLOW_ON HAL_GPIO_WritePin(LED_1_OUT_GPIO_Port, LED_1_OUT_Pin, 1)
#define _CONTROL_BOARD_LED_YELLOW_OFF HAL_GPIO_WritePin(LED_1_OUT_GPIO_Port, LED_1_OUT_Pin, 0)
	
#define _CONTROL_BOARD_LED_GREEN_ON HAL_GPIO_WritePin(LED_2_OUT_GPIO_Port, LED_2_OUT_Pin, 1)
#define _CONTROL_BOARD_LED_GREEN_OFF HAL_GPIO_WritePin(LED_2_OUT_GPIO_Port, LED_2_OUT_Pin, 0)
	
// Scanning periods - try to avoid sync.
#define _MAIN_TASK_PERIOD_MS 10U
#define _SLIDERS_SCAN_PERIOD_MS	97U	
#define _PUSHBUTTONS_SCAN_PERIOD_MS	19U
#define _ROTARY_ENCODERS_SCAN_PERIOD_MS	11U
	
#define _NUM_OF_SLIDERS 12
#define _MAX_SLIDER_VAL 4000
#define _SLIDERS_AVR_FACTOR 0.5
#define _SLIDER_MIN_CHANGE 50
	
#define _NUM_OF_PUSHBUTTONS 12
#define _NUM_OF_FUNCTION_PUSHBUTTONS 3
#define _NUM_OF_ENCODERS 12
	
#define _INPUT_PIN_LOW 0
#define _INPUT_PIN_HIGH 1
#define _STATE_PRESSED 0
#define _STATE_RELEASED 1

/* USED ROTARY ENCODERS PROVIDES ONLY 3 STATES: 0, 1 and 3 !!!!!? */
#define _ROTARY_STATE_START 0
#define _ROTARY_STATE_CW_START 1
#define _ROTARY_STATE_CW_MID 2
#define _ROTARY_STATE_CW_END 3
#define _ROTARY_STATE_CCW_START 4
#define _ROTARY_STATE_CCW_MID 5
#define _ROTARY_STATE_CCW_END 6	
	
#define _MIDI_UART 0
#define _CONTROL_UART 1
	
#define _UART_RX_MSSG_MAX_LENGTH 1
#define _UART_TX_MSSG_MAX_LENGTH 64
	
	/* HW disorder! */
	
#define _CONTROL_PUSHBUTTON_1  0x10
#define _CONTROL_PUSHBUTTON_2  0x11
#define _CONTROL_PUSHBUTTON_3  0x12
#define _CONTROL_PUSHBUTTON_4  0x13
#define _CONTROL_PUSHBUTTON_5  0x14
#define _CONTROL_PUSHBUTTON_6  0x15
#define _CONTROL_PUSHBUTTON_7  0x16
#define _CONTROL_PUSHBUTTON_8  0x17
#define _CONTROL_PUSHBUTTON_9  0x18
#define _CONTROL_PUSHBUTTON_10 0x19
#define _CONTROL_PUSHBUTTON_11 0x20
#define _CONTROL_PUSHBUTTON_12 0x21
	
#define _CONTROL_FUNCTION_PUSHBUTTON_1  0x30
#define _CONTROL_FUNCTION_PUSHBUTTON_2  0x31
#define _CONTROL_FUNCTION_PUSHBUTTON_3  0x32

	
	
#define _CONTROL_ENCODER_1  0x40
#define _CONTROL_ENCODER_2  0x41
#define _CONTROL_ENCODER_3  0x42
#define _CONTROL_ENCODER_4  0x43
#define _CONTROL_ENCODER_5  0x44
#define _CONTROL_ENCODER_6  0x45
#define _CONTROL_ENCODER_7  0x46
#define _CONTROL_ENCODER_8  0x47
#define _CONTROL_ENCODER_9  0x48
#define _CONTROL_ENCODER_10 0x49
#define _CONTROL_ENCODER_11 0x4a
#define _CONTROL_ENCODER_12 0x4b
	
#define _CONTROL_ENCODER_PUSHBUTTON_1  0x50
#define _CONTROL_ENCODER_PUSHBUTTON_2  0x51
#define _CONTROL_ENCODER_PUSHBUTTON_3  0x52
#define _CONTROL_ENCODER_PUSHBUTTON_4  0x53
#define _CONTROL_ENCODER_PUSHBUTTON_5  0x54
#define _CONTROL_ENCODER_PUSHBUTTON_6  0x55
#define _CONTROL_ENCODER_PUSHBUTTON_7  0x56
#define _CONTROL_ENCODER_PUSHBUTTON_8  0x57
#define _CONTROL_ENCODER_PUSHBUTTON_9  0x58
#define _CONTROL_ENCODER_PUSHBUTTON_10 0x59
#define _CONTROL_ENCODER_PUSHBUTTON_11 0x5a
#define _CONTROL_ENCODER_PUSHBUTTON_12 0x5b
	
#define _CONTROL_SLIDER_1  0x60
#define _CONTROL_SLIDER_2  0x61
#define _CONTROL_SLIDER_3  0x62
#define _CONTROL_SLIDER_4  0x63
#define _CONTROL_SLIDER_5  0x64
#define _CONTROL_SLIDER_6  0x65
#define _CONTROL_SLIDER_7  0x66
#define _CONTROL_SLIDER_8  0x67
#define _CONTROL_SLIDER_9  0x68
#define _CONTROL_SLIDER_10 0x69
#define _CONTROL_SLIDER_11 0x6a
#define _CONTROL_SLIDER_12 0x6b
	
#define _CONTROL_YELLOW_LED 0x70
#define _CONTROL_GREEN_LED  0x71
	


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

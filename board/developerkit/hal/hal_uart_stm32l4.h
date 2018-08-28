/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#ifndef __HAL_UART_STM32L4_H
#define __HAL_UART_STM32L4_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32l4xx_hal.h"  
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "aos/kernel.h"

#define PORT_UART1 1           /* USART1 */
#define PORT_UART2 2           /* the port number of stand uart must be 0 */
#define PORT_UART3 3           /* USART3 */
#define PORT_UART4 4           /* UART4 */
#define PORT_UART5 5           /* UART5 */
#define PORT_UART6 0           /* LPUART1 */

#define PORT_UART_MAX_NUM 6

/***************************** uart1 configure *******************************/
#define UART1                          USART1
#define UART1_OVER_SAMPLING            UART_OVERSAMPLING_16
#define UART1_ONE_BIT_SAMPLING         UART_ONE_BIT_SAMPLE_DISABLE
#define UART1_ADV_FEATURE_INIT         UART_ADVFEATURE_NO_INIT
#define UART1_IRQn                     USART1_IRQn
#define UART1_CLK_ENABLE()             __HAL_RCC_USART1_CLK_ENABLE()
#define UART1_TX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define UART1_RX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()


/***************************** uart2 configure *******************************/
#define UART2                          USART2
#define UART2_OVER_SAMPLING            UART_OVERSAMPLING_16
#define UART2_ONE_BIT_SAMPLING         UART_ONE_BIT_SAMPLE_DISABLE
#define UART2_ADV_FEATURE_INIT         UART_ADVFEATURE_NO_INIT
#define UART2_IRQn                     USART2_IRQn
#define UART2_CLK_ENABLE()             __HAL_RCC_USART2_CLK_ENABLE()
#define UART2_TX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define UART2_RX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()

/***************************** uart3 configure *******************************/
#define UART3                          USART3
#define UART3_OVER_SAMPLING            UART_OVERSAMPLING_16
#define UART3_ONE_BIT_SAMPLING         UART_ONE_BIT_SAMPLE_DISABLE
#define UART3_ADV_FEATURE_INIT         UART_ADVFEATURE_NO_INIT
#define UART3_IRQn                     USART3_IRQn
#define UART3_CLK_ENABLE()             __HAL_RCC_USART3_CLK_ENABLE()
#define UART3_TX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOC_CLK_ENABLE()
#define UART3_RX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOC_CLK_ENABLE()

/***************************** uart4 configure *******************************/
//#define UART4                          UART4
#define UART4_OVER_SAMPLING            UART_OVERSAMPLING_16
#define UART4_ONE_BIT_SAMPLING         UART_ONE_BIT_SAMPLE_DISABLE
#define UART4_ADV_FEATURE_INIT         UART_ADVFEATURE_NO_INIT
#define UART4_IRQn                     UART4_IRQn
#define UART4_CLK_ENABLE()             __HAL_RCC_UART4_CLK_ENABLE()
#define UART4_TX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOC_CLK_ENABLE()
#define UART4_RX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOC_CLK_ENABLE()

/***************************** uart5 configure *******************************/
//#define UART5                          UART5
#define UART5_OVER_SAMPLING            UART_OVERSAMPLING_16
#define UART5_ONE_BIT_SAMPLING         UART_ONE_BIT_SAMPLE_DISABLE
#define UART5_ADV_FEATURE_INIT         UART_ADVFEATURE_NO_INIT
#define UART5_IRQn                     UART5_IRQn
#define UART5_CLK_ENABLE()             __HAL_RCC_UART5_CLK_ENABLE()
#define UART5_TX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOC_CLK_ENABLE()
#define UART5_RX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOC_CLK_ENABLE()

/***************************** uart6 configure *******************************/
#define UART6                          LPUART1
#define UART6_ONE_BIT_SAMPLING         UART_ONE_BIT_SAMPLE_DISABLE
#define UART6_ADV_FEATURE_INIT         UART_ADVFEATURE_NO_INIT
#define UART6_IRQn                     LPUART1_IRQn
#define UART6_CLK_ENABLE()             __HAL_RCC_LPUART1_CLK_ENABLE()
#define UART6_TX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define UART6_RX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()

#define MAX_BUF_UART_BYTES  4096

#ifdef __cplusplus
}
#endif

#endif /* __HAL_UART_STM32L4_H */

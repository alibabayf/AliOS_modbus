/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#include <stdint.h>
#include "hal/hal.h"
#include "k_config.h"
#include "soc_init.h"

#define main st_main
#include "Src/main.c"

#if defined (__CC_ARM) && defined(__MICROLIB)
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
size_t g_iram1_start = 0x20000000;
size_t g_iram1_total_size = 0x00040000;
#elif defined(__ICCARM__)
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#else
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define GETCHAR_PROTOTYPE int __io_getchar(void)
#endif /* defined (__CC_ARM) && defined(__MICROLIB) */

uart_dev_t uart_0;

static void stduart_init(void);
static void brd_peri_init(void);
static void MX_SPI1_Init(void);
static void MX_SAI1_Init(void);
static void MX_CRC_Init(void);
static void MX_DMA_Init(void);

void stm32_soc_init(void)
{
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();

    /**Configure the Systick interrupt time 
    */
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/RHINO_CONFIG_TICKS_PER_SECOND);
    /* PendSV_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(PendSV_IRQn, 0x0f, 0);
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /*default uart init*/
    stduart_init();
    brd_peri_init();
    MX_DMA_Init();
    MX_ADC3_Init();
    MX_DCMI_Init();
    MX_SAI2_Init();
    MX_SDMMC1_Init();
    MX_SPI1_Init();
    MX_USB_OTG_FS_USB_Init();
    MX_CRC_Init();
    MX_TIM1_Init();
    MX_TIM17_Init();
    MX_TIM16_Init();
    MX_IRTIM_Init();

#ifdef DEVELOPERKIT_IRDA
    irda_init();
#endif
}

static void stduart_init(void)
{
    uart_0.port = 0;
    uart_0.config.baud_rate = 115200;
    uart_0.config.data_width = DATA_WIDTH_8BIT;
    uart_0.config.flow_control = FLOW_CONTROL_DISABLED;
    uart_0.config.mode = MODE_TX_RX;
    uart_0.config.parity = NO_PARITY;
    uart_0.config.stop_bits = STOP_BITS_1;

    hal_uart_init(&uart_0);
}

static gpio_irq_trigger_t mode_rising = IRQ_TRIGGER_RISING_EDGE;
static gpio_irq_trigger_t mode_falling = IRQ_TRIGGER_FALLING_EDGE;
static gpio_irq_trigger_t mode_both = IRQ_TRIGGER_BOTH_EDGES;
static uint8_t gpio_set = 1;
static uint8_t gpio_reset = 0;

gpio_dev_t brd_gpio_table[] = {
	{ALS_INT, IRQ_MODE, &mode_rising},
	{ALS_LED, OUTPUT_PUSH_PULL, &gpio_set},
	{AUDIO_CTL, OUTPUT_PUSH_PULL, &gpio_reset},
	{AUDIO_RST, OUTPUT_PUSH_PULL, &gpio_set},
	{AUDIO_WU, OUTPUT_PUSH_PULL, &gpio_set},
	{CAM_PD, OUTPUT_PUSH_PULL, &gpio_set},
	{CAM_RST, OUTPUT_PUSH_PULL, &gpio_set},
	{COMPASS_LED, OUTPUT_PUSH_PULL, &gpio_set},
	{GS_LED, OUTPUT_PUSH_PULL, &gpio_set},
	{HTS_LED, OUTPUT_PUSH_PULL, &gpio_set},
	{KEY_1, IRQ_MODE, &mode_rising},
	{KEY_2, IRQ_MODE, &mode_rising},
	{KEY_3, IRQ_MODE, &mode_rising},
	{LCD_DCX, OUTPUT_PUSH_PULL, &gpio_set},
	{LCD_PWR, OUTPUT_PUSH_PULL, &gpio_reset},
	{LCD_RST, OUTPUT_PUSH_PULL, &gpio_set},
	{PCIE_RST, OUTPUT_PUSH_PULL, &gpio_set},
	{PS_LED, OUTPUT_PUSH_PULL, &gpio_set},
	{SECURE_CLK, OUTPUT_PUSH_PULL, &gpio_set},
	{SECURE_IO, OUTPUT_PUSH_PULL, &gpio_set},
	{SECURE_RST, OUTPUT_PUSH_PULL, &gpio_set},
	{SIM_DET, INPUT_HIGH_IMPEDANCE, NULL},
	{USB_PCIE_SW, OUTPUT_PUSH_PULL, &gpio_set},
	{WIFI_RST, OUTPUT_PUSH_PULL, &gpio_set},
	{WIFI_WU, OUTPUT_PUSH_PULL, &gpio_set},
	{ZIGBEE_INT, IRQ_MODE, &mode_rising},
	{ZIGBEE_RST, OUTPUT_PUSH_PULL, &gpio_set},
};

i2c_dev_t brd_i2c2_dev = {AOS_PORT_I2C2, {0}, NULL};
i2c_dev_t brd_i2c3_dev = {AOS_PORT_I2C3, {0}, NULL};
i2c_dev_t brd_i2c4_dev = {AOS_PORT_I2C4, {0}, NULL};

static void brd_peri_init(void)
{
	int i;
	int gpcfg_num = sizeof(brd_gpio_table) / sizeof(brd_gpio_table[0]);

	for (i = 0; i < gpcfg_num; ++i) {
		hal_gpio_init(&brd_gpio_table[i]);
	}
	hal_i2c_init(&brd_i2c2_dev);
	hal_i2c_init(&brd_i2c3_dev);
	hal_i2c_init(&brd_i2c4_dev);
}
/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  HAL_IncTick();
  krhino_intrpt_enter();
  krhino_tick_proc();
  krhino_intrpt_exit();

#ifdef LITTLEVGL_DEVELOPERKIT
  lv_tick_inc(1);
#endif
}

void HardFault_Handler(void)
{
  while (1)
  {
	//#ifdef exc_print
   // cpu_hardfault_handle();
   // #endif
  }
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  if (ch == '\n') {
    //hal_uart_send(&console_uart, (void *)"\r", 1, 30000);
    hal_uart_send(&uart_0, (void *)"\r", 1, 30000);
  }
  hal_uart_send(&uart_0, &ch, 1, 30000);
  return ch;
}

/**
  * @brief  Retargets the C library scanf function to the USART.
  * @param  None
  * @retval None
  */
GETCHAR_PROTOTYPE
{
  /* Place your implementation of fgetc here */
  /* e.g. readwrite a character to the USART2 and Loop until the end of transmission */
  uint8_t ch = EOF;
  int32_t ret = -1;
  
  uint32_t recv_size;
  ret = hal_uart_recv_II(&uart_0, &ch, 1, &recv_size, HAL_WAIT_FOREVER);

  if (ret == 0) {
      return ch;
  } else {
      return -1;
  }
}



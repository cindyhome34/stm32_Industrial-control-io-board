/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "string.h"
#include "control.h"

#include "stdlib.h"


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
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM4_Init(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#ifdef _GNUC_
/* With  GCC/RAISONANCE,small printf(option LD Linker ->Libraries -> Small printf
	set to 'Yes')calls _io putchar() */
#define PUTCHAR_PROTOTYPE int _io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* _GNUC_ */

/* implementation of fputc  */
PUTCHAR_PROTOTYPE {
    HAL_UART_Transmit(&huart1, (uint8_t *) &ch, 1, 0xffff);
    return ch;
}


//定义Uart2的变量

uint8_t UartRxData;				  //接受一个字节数据
char UartRxBuf[128];			  //接收一个128字节的数组
uint8_t UartRxCount =0;			//接收计数位
_Bool UartRxFlag;				    //接收标志位

uint8_t UartTxNum =1;				//485发送值为1，默认处于接收状态
char *g_cNum =NULL;
int res;                    //返回指令信号
uint8_t stares =1;					//获得的一个状态值用485发送
int num, wnum, onum;
uint8_t port;								//DI端口号
int flag;

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
    int value =0;
    int p_value =0;

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_USART1_UART_Init();
    MX_USART2_UART_Init();
    MX_TIM4_Init();
    /* USER CODE BEGIN 2 */

    //使能Uart2的中断使能，并在中断使能后产生中断
    HAL_UART_Receive_IT(&huart2, &UartRxData, 1);
    HAL_Delay(50);
    //接收模式
    HAL_UART_Transmit(&huart2, &UartTxNum, 1, 10);
    printf("hello\r\n");

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        value = getGpioFun();									//查询检测io口

        if((value & 0xffff0000) != 0x7fff)
        {
            int i =0;

            value = ~(value & 0x7fff);					//取出IO口的值
            if(p_value != value)								//防止重复执行发送
            {
                p_value = value;
                for(i=0; i<15; i++)
                {
                    if(!(value & (0x01 << i)))
                    {
                        port++;
                    }
                    else
                    {
                        port = ((port+1) << 4) | 0x0d;
                        printf("0x%x\r\n", port);
                        HAL_UART_Transmit(&huart2, &port, 1, 0xffff);				//用485发送io口的返回值
                        port =0;
                        value =0;
//						HAL_UART_Transmit(&huart2, &UartTxNum, 1, 0xffff);
                        break;
                    }
                }
            }
        }
        port  =0;
        value =0;

        if(UartRxFlag == 1 && *UartRxBuf)						//打印485接收到的信息内容
        {
            MX_GPIO_Init();							//重置端口
            HAL_Delay(100);

            float revalue =0.0;
            onum =atoi(UartRxBuf);
            num  =((onum >> 4) & 0x1f);
            wnum =(onum & 0xf);
            res =ControlAgreement(UartRxBuf, g_cNum);			//控制协议，判断是否命令存在
            printf("%d\r\n", res);
            if(((res < 14) && (res > 0)) || (res == 0))   //调用命令对应的端口使能或动作
            {
                stares =SectionFun(res);
                HAL_UART_Transmit(&huart2, &stares, 1, 0xffff);				//用485发送返回值
//				HAL_UART_Transmit(&huart2, &UartTxNum, 1, 0xffff);
            }
            else if((res > 13) && (res < 22))																				//其他端口的控制
            {
                DoFun(res);
            }
            //判断进入pwm的模式
            else if(((onum >> 9) & 0x1) && (((onum >> 10) & 0x3) == 0x1 || ((onum >> 10) & 0x3) == 0x2) && \
                    (((num < 20) || ((num == 20) && (wnum == 0))) && ((num > 4) || ((num == 4) && (wnum == 0)))))
            {
                if((wnum > -1) && (wnum < 10))												//小数点的判断  1位小数位
                {
                    revalue = num + ((float)wnum / 10.0);								//构造数值
                    if(((onum >> 10) & 0x3) == 0x1)											//判断是哪一路的PWM   第一路PWM
                    {
                        num =500.0- revalue * 20.0;								//计算PWM占空比，输出使能
                        ControlPwm1(num);
                    }
                    else																								//第二路PWM
                    {
                        num =500.0- revalue * 20.0;								//计算PWM占空比，输出使能
                        ControlPwm2(num);
                    }
                }
            }
            else
            {
                printf("no command\r\n");
            }

            memset(UartRxBuf, 0, 128);         		//清除buffer内的内容
            UartRxCount =0;							//清除计数值和标志位
            UartRxFlag  =0;
            stares =1;
        }
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

    /** Initializes the CPU, AHB and APB busses clocks
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
    /** Initializes the CPU, AHB and APB busses clocks
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
}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

    /* USER CODE BEGIN TIM4_Init 0 */

    /* USER CODE END TIM4_Init 0 */

    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};

    /* USER CODE BEGIN TIM4_Init 1 */

    /* USER CODE END TIM4_Init 1 */
    htim4.Instance = TIM4;
    htim4.Init.Prescaler = 720-1;
    htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim4.Init.Period = 500-1;
    htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
    {
        Error_Handler();
    }
    __HAL_TIM_DISABLE_OCxPRELOAD(&htim4, TIM_CHANNEL_3);
    if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
    {
        Error_Handler();
    }
    __HAL_TIM_DISABLE_OCxPRELOAD(&htim4, TIM_CHANNEL_4);
    /* USER CODE BEGIN TIM4_Init 2 */

    /* USER CODE END TIM4_Init 2 */
    HAL_TIM_MspPostInit(&htim4);

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
    huart1.Init.BaudRate = 115200;
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
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOE, MAEN1_Pin|MAEN2_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, ACMD1_Pin|ACMD2_Pin|SW_Pin|SW10_Pin
                      |VALVE5_EN_Pin|VALVE8_EN_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, DO01_Pin|DO02_Pin|DO03_Pin|VALVE14_EN_Pin
                      |VALVE13_EN_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOD, DO04_Pin|VALVE2_EN_Pin|VALVE1_EN_Pin|VALVE4_EN_Pin
                      |VALVE10_EN_Pin|VALVE12_EN_Pin|VALVE11_EN_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOC, VALVE3_EN_Pin|VALVE6_EN_Pin|VALVE7_EN_Pin|VALVE9_EN_Pin, GPIO_PIN_SET);

    /*Configure GPIO pins : MAEN1_Pin MAEN2_Pin */
    GPIO_InitStruct.Pin = MAEN1_Pin|MAEN2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /*Configure GPIO pins : MAF1_Pin MAF2_Pin DI06_Pin DI05_Pin
                             DI08_Pin DI07_Pin DI10_Pin DI09_Pin
                             DI12_Pin DI11_Pin DI14_Pin */
    GPIO_InitStruct.Pin = MAF1_Pin|MAF2_Pin|DI06_Pin|DI05_Pin
                          |DI08_Pin|DI07_Pin|DI10_Pin|DI09_Pin
                          |DI12_Pin|DI11_Pin|DI14_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /*Configure GPIO pins : ACMD1_Pin ACMD2_Pin SW_Pin SW10_Pin
                             VALVE5_EN_Pin VALVE8_EN_Pin */
    GPIO_InitStruct.Pin = ACMD1_Pin|ACMD2_Pin|SW_Pin|SW10_Pin
                          |VALVE5_EN_Pin|VALVE8_EN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : DI01_Pin DI02_Pin VELVE5_STATUS_Pin VELVE6_STATUS_Pin
                             VELVE8_STATUS_Pin */
    GPIO_InitStruct.Pin = DI01_Pin|DI02_Pin|VELVE5_STATUS_Pin|VELVE6_STATUS_Pin
                          |VELVE8_STATUS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : DI04_Pin DI03_Pin DI13_Pin DI15_Pin
                             VELVE14_STATUS_Pin */
    GPIO_InitStruct.Pin = DI04_Pin|DI03_Pin|DI13_Pin|DI15_Pin
                          |VELVE14_STATUS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : DO01_Pin DO02_Pin DO03_Pin VALVE14_EN_Pin
                             VALVE13_EN_Pin */
    GPIO_InitStruct.Pin = DO01_Pin|DO02_Pin|DO03_Pin|VALVE14_EN_Pin
                          |VALVE13_EN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : DO04_Pin VALVE2_EN_Pin VALVE1_EN_Pin VALVE4_EN_Pin
                             VALVE10_EN_Pin VALVE12_EN_Pin VALVE11_EN_Pin */
    GPIO_InitStruct.Pin = DO04_Pin|VALVE2_EN_Pin|VALVE1_EN_Pin|VALVE4_EN_Pin
                          |VALVE10_EN_Pin|VALVE12_EN_Pin|VALVE11_EN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pins : VELVE1_STATUS_Pin VELVE2_STATUS_Pin VELVE3_STATUS_Pin VELVE4_STATUS_Pin
                             VELVE9_STATUS_Pin VELVE10_STATUS_Pin VELVE11_STATUS_Pin VELVE12_STATUS_Pin
                             VELVE13_STATUS_Pin */
    GPIO_InitStruct.Pin = VELVE1_STATUS_Pin|VELVE2_STATUS_Pin|VELVE3_STATUS_Pin|VELVE4_STATUS_Pin
                          |VELVE9_STATUS_Pin|VELVE10_STATUS_Pin|VELVE11_STATUS_Pin|VELVE12_STATUS_Pin
                          |VELVE13_STATUS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pins : VALVE3_EN_Pin VALVE6_EN_Pin VALVE7_EN_Pin VALVE9_EN_Pin */
    GPIO_InitStruct.Pin = VALVE3_EN_Pin|VALVE6_EN_Pin|VALVE7_EN_Pin|VALVE9_EN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pin : VELVE7_STATUS_Pin */
    GPIO_InitStruct.Pin = VELVE7_STATUS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(VELVE7_STATUS_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

//Uart2的接受回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart == &huart2)								//判断是否为Uart2接收中断
    {
        UartRxBuf[UartRxCount++] = UartRxData;			//接收到的一个字节的数据存放在buf中
        if(UartRxBuf[UartRxCount-1] == 0)		//存到结束符
        {
            UartRxFlag = 1;								//置位结束标志位
        }
        HAL_UART_Receive_IT(&huart2, &UartRxData, 1);	//重新开启接收中断
    }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

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
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

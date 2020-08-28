#include <stdio.h>
#include "string.h"
#include "control.h"

extern TIM_HandleTypeDef htim4;

static char *CommendPage[22] ={
	//14路0/24v高低电平检测输出
	"007", "008", "009", "010", "011", "012", "013",
	"018", "019", "020", "021", "022", "023", "024",
	//4路24/1A控制输出
	"441", "442", "443", "444",
	//1路24v/10v机械开关输出
	"501",
	//2路DO接口蠕动泵
	"201", "202", 
	//1路DO接口外接水泵
	"301"
};

//输入传输指令进行对比控制端口，返回命令序号
int ControlAgreement(char *pGetNum, char *pCommand)    
{	
	int iNum =0;
	
	for(iNum=0; iNum<22; iNum++)			//逐个寻找命令
	{
		if(!strncmp(pGetNum, CommendPage[iNum], strlen(pGetNum)))			//对比指令, 相等跳出循环
		{
			pCommand = pGetNum;																						//确定命令
			return iNum;
		}
	}
	return -1;
}	

//查询模式检测io口
int getGpioFun(void)
{
		_Bool reval =0;
	  int res = 1;
	
		reval = HAL_GPIO_ReadPin(DI15_GPIO_Port, DI15_Pin);
		res = reval;
		reval = HAL_GPIO_ReadPin(DI14_GPIO_Port, DI14_Pin);
		res = (res << 1) | reval;
		reval = HAL_GPIO_ReadPin(DI13_GPIO_Port, DI13_Pin);
		res = (res << 1) | reval;
		reval = HAL_GPIO_ReadPin(DI12_GPIO_Port, DI12_Pin);
		res = (res << 1) | reval;
		reval = HAL_GPIO_ReadPin(DI11_GPIO_Port, DI11_Pin);
		res = (res << 1) | reval;
		reval = HAL_GPIO_ReadPin(DI10_GPIO_Port, DI10_Pin);
		res = (res << 1) | reval;
		reval = HAL_GPIO_ReadPin(DI09_GPIO_Port, DI09_Pin);
		res = (res << 1) | reval;
		reval = HAL_GPIO_ReadPin(DI08_GPIO_Port, DI08_Pin);
		res = (res << 1) | reval;
	
		reval = HAL_GPIO_ReadPin(DI07_GPIO_Port, DI07_Pin);
		res = (res << 1) | reval;
		reval = HAL_GPIO_ReadPin(DI06_GPIO_Port, DI06_Pin);
		res = (res << 1) | reval;
		reval = HAL_GPIO_ReadPin(DI05_GPIO_Port, DI05_Pin);
		res = (res << 1) | reval;
		reval = HAL_GPIO_ReadPin(DI04_GPIO_Port, DI04_Pin);
		res = (res << 1) | reval;
		reval = HAL_GPIO_ReadPin(DI03_GPIO_Port, DI03_Pin);
		res = (res << 1) | reval;
		reval = HAL_GPIO_ReadPin(DI02_GPIO_Port, DI02_Pin);
		res = (res << 1) | reval;
		reval = HAL_GPIO_ReadPin(DI01_GPIO_Port, DI01_Pin);
		res = (res << 1) | reval;

		res |= 0xffff0000;
		return res;
}

//uint16_t
void ControlPwm1(uint16_t value)
{
//		printf("%f\r\n", value);
    TIM_OC_InitTypeDef sConfigOC;

    sConfigOC.OCMode = TIM_OCMODE_PWM1;

    sConfigOC.Pulse = value;

    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;

    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

    HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_3);

    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
	
		HAL_GPIO_WritePin(MAEN1_GPIO_Port, MAEN1_Pin, GPIO_PIN_RESET);  
  
}

void ControlPwm2(uint16_t value)
{
    TIM_OC_InitTypeDef sConfigOC;

    sConfigOC.OCMode = TIM_OCMODE_PWM1;

    sConfigOC.Pulse = value;

    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;

    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

    HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_4);

    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4); 
	
		HAL_GPIO_WritePin(MAEN2_GPIO_Port, MAEN2_Pin, GPIO_PIN_RESET);  
}

//选取对应的引脚定义函数
uint8_t SectionFun(int Num)
{
	int status =1;
	
	switch(Num)
	{
		case 0:
			HAL_GPIO_WritePin(VALVE1_EN_GPIO_Port, VALVE1_EN_Pin, GPIO_PIN_RESET);
			HAL_Delay(100);
			status = HAL_GPIO_ReadPin(VELVE1_STATUS_GPIO_Port, VELVE1_STATUS_Pin);
			status = (Num << 4) | status | 0x8;
			break;
		
		case 1:
			HAL_GPIO_WritePin(VALVE2_EN_GPIO_Port, VALVE2_EN_Pin, GPIO_PIN_RESET);
			HAL_Delay(100);
			status = HAL_GPIO_ReadPin(VELVE2_STATUS_GPIO_Port, VELVE2_STATUS_Pin);
			status = (Num << 4) | status | 0x8;
			break;
		
		case 2:
			HAL_GPIO_WritePin(VALVE3_EN_GPIO_Port, VALVE3_EN_Pin, GPIO_PIN_RESET);
			HAL_Delay(100);
			status = HAL_GPIO_ReadPin(VELVE3_STATUS_GPIO_Port, VELVE3_STATUS_Pin);
			status = (Num << 4) | status | 0x8;
			break;
		
		case 3:
			HAL_GPIO_WritePin(VALVE4_EN_GPIO_Port, VALVE4_EN_Pin, GPIO_PIN_RESET);
			HAL_Delay(100);
			status = HAL_GPIO_ReadPin(VELVE4_STATUS_GPIO_Port, VELVE4_STATUS_Pin);
			status = (Num << 4) | status | 0x8;
			break;

		case 4:
			HAL_GPIO_WritePin(VALVE5_EN_GPIO_Port, VALVE5_EN_Pin, GPIO_PIN_RESET);
			HAL_Delay(100);
			status = HAL_GPIO_ReadPin(VELVE5_STATUS_GPIO_Port, VELVE5_STATUS_Pin);
			status = (Num << 4) | status | 0x8;
			break;

		case 5:
			HAL_GPIO_WritePin(VALVE6_EN_GPIO_Port, VALVE6_EN_Pin, GPIO_PIN_RESET);
			HAL_Delay(100);
			status = HAL_GPIO_ReadPin(VELVE6_STATUS_GPIO_Port, VELVE6_STATUS_Pin);
			status = (Num << 4) | status | 0x8;
			break;

		case 6:
			HAL_GPIO_WritePin(VALVE7_EN_GPIO_Port, VALVE7_EN_Pin, GPIO_PIN_RESET);
			HAL_Delay(100);
			status = HAL_GPIO_ReadPin(VELVE7_STATUS_GPIO_Port, VELVE7_STATUS_Pin);
			status = (Num << 4) | status | 0x8;
			break;
		
		case 7:
			HAL_GPIO_WritePin(VALVE8_EN_GPIO_Port, VALVE8_EN_Pin, GPIO_PIN_RESET);
			HAL_Delay(100);
			status = HAL_GPIO_ReadPin(VELVE8_STATUS_GPIO_Port, VELVE8_STATUS_Pin);
			status = (Num << 4) | status | 0x8;
			break;
		
		case 8:
			HAL_GPIO_WritePin(VALVE9_EN_GPIO_Port, VALVE9_EN_Pin, GPIO_PIN_RESET);
			HAL_Delay(100);
			status = HAL_GPIO_ReadPin(VELVE9_STATUS_GPIO_Port, VELVE9_STATUS_Pin);
			status = (Num << 4) | status | 0x8;
			break;
		
		case 9:
			HAL_GPIO_WritePin(VALVE10_EN_GPIO_Port, VALVE10_EN_Pin, GPIO_PIN_RESET);
			HAL_Delay(100);
			status = HAL_GPIO_ReadPin(VELVE10_STATUS_GPIO_Port, VELVE10_STATUS_Pin);
			status = (Num << 4) | status | 0x8;
			break;

		case 10:
			HAL_GPIO_WritePin(VALVE11_EN_GPIO_Port, VALVE11_EN_Pin, GPIO_PIN_RESET);
			HAL_Delay(100);
			status = HAL_GPIO_ReadPin(VELVE11_STATUS_GPIO_Port, VELVE11_STATUS_Pin);
			status = (Num << 4) | status | 0x8;
			break;

		case 11:
			HAL_GPIO_WritePin(VALVE12_EN_GPIO_Port, VALVE12_EN_Pin, GPIO_PIN_RESET);
			HAL_Delay(100);
			status = HAL_GPIO_ReadPin(VELVE12_STATUS_GPIO_Port, VELVE12_STATUS_Pin);
			status = (Num << 4) | status | 0x8;
			break;
		
		case 12:
			HAL_GPIO_WritePin(VALVE13_EN_GPIO_Port, VALVE13_EN_Pin, GPIO_PIN_RESET);
			HAL_Delay(100);
			status = HAL_GPIO_ReadPin(VELVE13_STATUS_GPIO_Port, VELVE13_STATUS_Pin);
			status = (Num << 4) | status | 0x8;
			break;

		case 13:
			HAL_GPIO_WritePin(VALVE14_EN_GPIO_Port, VALVE14_EN_Pin, GPIO_PIN_RESET);
			HAL_Delay(100);
			status = HAL_GPIO_ReadPin(VELVE14_STATUS_GPIO_Port, VELVE14_STATUS_Pin);
			status = (Num << 4) | status | 0x8;
			break;
			
		default:
			break;
	}
	return status;
}

void DoFun(int Num)
{
	switch(Num)
	{
		case 14:
			HAL_GPIO_WritePin(DO01_GPIO_Port, DO01_Pin, GPIO_PIN_RESET);
			break;

		case 15:
			HAL_GPIO_WritePin(DO02_GPIO_Port, DO02_Pin, GPIO_PIN_RESET);
			break;

		case 16:
			HAL_GPIO_WritePin(DO03_GPIO_Port, DO03_Pin, GPIO_PIN_RESET);
			break;

		case 17:
			HAL_GPIO_WritePin(DO04_GPIO_Port, DO04_Pin, GPIO_PIN_RESET);
			break;
		
		case 18:
			HAL_GPIO_WritePin(SW10_GPIO_Port, SW10_Pin, GPIO_PIN_RESET);
			break;
		
		case 19:
			HAL_GPIO_WritePin(ACMD1_GPIO_Port, ACMD1_Pin, GPIO_PIN_RESET);
			break;

		case 20:
			HAL_GPIO_WritePin(ACMD2_GPIO_Port, ACMD2_Pin, GPIO_PIN_RESET);
			break;

		case 21:
			HAL_GPIO_WritePin(SW_GPIO_Port, SW_Pin, GPIO_PIN_RESET);
			break;
				
		default:
			break;
	}
}

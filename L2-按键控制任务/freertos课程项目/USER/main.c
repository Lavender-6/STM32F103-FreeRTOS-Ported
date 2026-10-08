#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "KEY.h"

/* 任务优先级 严格按照题目：key(3) > task2(2) > start(1) = task1(1) */
#define Start_TASK_PRIO  1
#define Key_TASK_PRIO    3
#define TASK1_PRIO       1
#define TASK2_PRIO       2

/* 任务堆栈大小（字） */
#define TASK_STACK_SIZE  128

/* 任务句柄 */
static TaskHandle_t Task1_Handler = NULL;
static TaskHandle_t Task2_Handler = NULL;
static TaskHandle_t Start_Task_Handler = NULL;
static TaskHandle_t Key_Task_Handler = NULL;

uint8_t num = 0;

/* 函数声明 */
void Key(void *pvParameters);
void task1(void *pvParameters);
void task2(void *pvParameters);
void StartTask(void *pvParameters);

void StartTask(void *pvParameters)
{
	xTaskCreate((TaskFunction_t )task1,
								(const char*    )"task1",
                (uint16_t       )TASK_STACK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK1_PRIO,
                (TaskHandle_t*  )&Task1_Handler);
	xTaskCreate((TaskFunction_t )Key,
								(const char*    )"Key",
                (uint16_t       )TASK_STACK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )Key_TASK_PRIO,
                (TaskHandle_t*  )&Key_Task_Handler);
	xTaskCreate((TaskFunction_t )task2,
								(const char*    )"task2",
                (uint16_t       )TASK_STACK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK2_PRIO,
                (TaskHandle_t*  )&Task2_Handler);

	vTaskDelete(NULL); 
}

void Key(void *pvParameters)
{
	u8 key_val;
	while(1)
	{
		key_val=KEY_Scan(0);
		if(key_val != 0)
		{
			printf("KEY:%d pressed\r\n",key_val);
			if(key_val == 2)  //按键2，挂起task1
			{
				vTaskSuspend(Task1_Handler);
				printf("suspend task1\r\n");
			}
			if(key_val == 3)  //按键3，恢复task1
			{
				vTaskResume(Task1_Handler);
				printf("resume task1\r\n");
			}
		}
		vTaskDelay(pdMS_TO_TICKS(10));
	}
}

void task1(void *pvParameters)
{
	while(1)
	{
		LED1 = !LED1;
		vTaskDelay(pdMS_TO_TICKS(500));
		LED1 = !LED1;
		vTaskDelay(pdMS_TO_TICKS(500));
		num++;
		printf("led1num:%d\r\n",num);
		if(num == 5)
		{
			printf("delete task2!\r\n");
			vTaskDelete(Task2_Handler); //直接销毁task2任务
		}
	}
}

void task2(void *pvParameters)
{
	while(1)
	{
		printf("led2running\r\n");
		LED2 = !LED2;
		vTaskDelay(pdMS_TO_TICKS(500));
		LED2 = !LED2;
		vTaskDelay(pdMS_TO_TICKS(500));
	}
}

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    delay_init();
    uart_init(115200);
	  LED_Init();
		KEY_Init();
    printf("\r\nFreeRTOS ported successfully!\r\n");
		xTaskCreate((TaskFunction_t ) StartTask,
                (const char*    )"StartTask",
                (uint16_t       )TASK_STACK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )Start_TASK_PRIO,
                (TaskHandle_t*  )&Start_Task_Handler);
		
    vTaskStartScheduler();
}

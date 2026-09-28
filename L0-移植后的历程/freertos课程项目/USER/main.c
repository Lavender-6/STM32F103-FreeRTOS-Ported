#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"

/*
 * FreeRTOS 移植验证例程：
 * 创建两个任务分别控制 LED1(PE2) 和 LED2(PE3) 以不同周期翻转，
 * 并通过串口1(115200)打印任务运行信息。
 */

/* 任务优先级 */
#define LED1_TASK_PRIO       2
#define LED2_TASK_PRIO       3

/* 任务堆栈大小（字） */
#define LED_TASK_STACK_SIZE  128

/* 任务句柄 */
static TaskHandle_t LED1Task_Handler = NULL;
static TaskHandle_t LED2Task_Handler = NULL;

/* 任务1：控制LED1，500ms翻转一次 */
void led1_task(void *pvParameters)
{
    while(1)
    {
        LED1 = !LED1;
        printf("LED1 task running, tick = %d\r\n", (int)xTaskGetTickCount());
        vTaskDelay(500);
    }
}

/* 任务2：控制LED2，1000ms翻转一次 */
void led2_task(void *pvParameters)
{
    while(1)
    {
        LED2 = !LED2;
        printf("LED2 task running\r\n");
        vTaskDelay(1000);
    }
}

int main(void)
{
    /* FreeRTOS要求：中断优先级分组设置为组4，全部为抢占优先级 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    delay_init();          /* SysTick初始化（FreeRTOS节拍来源） */
    LED_Init();            /* LED初始化 */
    uart_init(115200);     /* 串口1初始化 */

    printf("\r\nFreeRTOS ported successfully!\r\n");

    /* 创建任务1 */
    xTaskCreate((TaskFunction_t )led1_task,
                (const char*    )"led1_task",
                (uint16_t       )LED_TASK_STACK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED1_TASK_PRIO,
                (TaskHandle_t*  )&LED1Task_Handler);

    /* 创建任务2 */
    xTaskCreate((TaskFunction_t )led2_task,
                (const char*    )"led2_task",
                (uint16_t       )LED_TASK_STACK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED2_TASK_PRIO,
                (TaskHandle_t*  )&LED2Task_Handler);

    /* 开启任务调度器，程序不会返回 */
    vTaskStartScheduler();
}

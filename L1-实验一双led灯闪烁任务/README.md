# L1 - 实验一：双 LED 灯闪烁任务

> 对应仓库第二次提交（`272a211`，2026-09-28）：在移植工程基础上实现 LED 双任务，加入任务间联动控制。
> 本阶段完整工程代码位于 `L1-实验一双led灯闪烁任务/freertos课程项目/` 目录下（Keil 工程）。

## 项目实现的功能

本阶段在 FreeRTOS 移植基础上，实现 **两个 LED 灯闪烁任务 + 任务间状态联动**。核心逻辑如下：

1. **任务结构重构（StartTask 起始任务）**
   - `main()` 只创建 1 个 **StartTask（优先级 3）**
   - StartTask 内部通过 `xTaskCreate` 创建两个 LED 任务后调用 `vTaskDelete(NULL)` 删除自身
   - 典型 FreeRTOS 应用写法：起始任务负责初始化并派生实际任务

2. **LED1 闪烁 + 计数任务（led1_task，优先级 2）**
   - LED1 每 500ms 翻转一次（一个完整亮灭周期约 1s）
   - 每周期计数变量 `num` 自增，并通过串口打印 `led1num:N`
   - 当 **num 计数到 5** 时，置全局标志 `turn_off_led2 = pdTRUE`，并打印 `led2close`

3. **LED2 闪烁 / 关闭任务（led2_task，优先级 2）**
   - 当 `turn_off_led2 == pdFALSE`：LED2 每 500ms 翻转一次，打印 `led2running`
   - 当 `turn_off_led2 == pdTRUE`：**LED2 置 1（熄灭）**，每 100ms 检测一次

4. **整体演示效果**
   - LED1、LED2 同时各自闪烁 → LED1 闪烁约 5 次后 → 触发标志 → **LED2 停止闪烁并熄灭**
   - 展示 FreeRTOS 任务之间通过**共享全局变量/标志**实现通信与控制

## 关键代码逻辑
```c
/* led1_task：LED1 翻转 + 计数，第 5 次后通知关闭 LED2 */
if(num == 5){
    turn_off_led2 = pdTRUE;
    printf("led2close\r\n");
}

/* led2_task：根据标志位决定闪烁还是熄灭 */
if( turn_off_led2 == pdFALSE){
    LED2 = !LED2;  vTaskDelay(500);
    LED2 = !LED2;  vTaskDelay(500);
}else{
    LED2 = 1;      vTaskDelay(100);   /* 关闭 */
}
```

## 开发环境
- 芯片：STM32F103ZET6
- 工具链：Keil MDK V5.06（ARM Compiler 5）
- 串口：115200 baud，NVIC 分组 4

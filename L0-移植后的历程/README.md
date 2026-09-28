# L0 - 移植后的历程（FreeRTOS 移植工程）

> 对应仓库第一次提交（`f31db5f`，2026-09-14）：将 FreeRTOS v202212.00 成功移植到 STM32F103。
> 本阶段完整工程代码位于 `L0-移植后的历程/freertos课程项目/` 目录下（Keil 工程）。

## 项目实现的功能

本阶段完成 **FreeRTOS 内核到 STM32F103 的移植**，跑通最小系统并验证内核能正常调度任务。具体包含：

1. **内核移植**
   - 移植 **FreeRTOS v202212.00** 到 STM32F103ZET6（正点原子 ALIENTEK 标准库 Keil MDK 模板）
   - 内核代码：`FREERTOS/include`（头文件 + FreeRTOSConfig.h）、`FREERTOS/src`（7 个内核 .c + heap_4.c）、`FREERTOS/portable`（RVDS/ARM_CM3 端口）
   - 内存管理采用 **heap_4.c**（首次适应 + 空闲块合并）

2. **工程环境适配**
   - `FreeRTOSConfig.h`：内核裁剪配置
   - `USER/FreeRTOS.uvprojx`：增加 FreeRTOS 文件组与头文件路径
   - `SYSTEM/sys/sys.h`：`SYSTEM_SUPPORT_OS` 置 1，启用 FreeRTOS 版 delay/usart
   - `USER/stm32f10x_it.c`：注释 `SVC_Handler` / `PendSV_Handler` / `SysTick_Handler`，避免与 port.c、delay.c 重复定义

3. **功能验证（双任务跑通）**
   - Task1：控制 **LED1(PE2)** 每 500ms 翻转一次，串口打印运行信息
   - Task2：控制 **LED2(PE3)** 每 1000ms 翻转一次，串口打印运行信息
   - 烧录后两个 LED 以不同频率交替闪烁，串口(115200)能看到两个任务的交替打印，证明 FreeRTOS 调度器已正常工作

## 开发环境
- 芯片：STM32F103ZET6
- 工具链：Keil MDK + ARM Compiler 5
- 串口：115200 baud，NVIC 分组 4（FreeRTOS 推荐）

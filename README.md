# STM32F103-FreeRTOS-Ported

FreeRTOS 内核移植到 STM32F103 的实验工程，基于正点原子（ALIENTEK）的标准库 Keil MDK 模板。

## 仓库结构

```
freertos课程项目/
├── freertos课程项目/                   # Keil 工程本体
│   ├── CORE/                           # 内核启动文件
│   ├── STM32F10x_FWLib/                # 官方标准库
│   ├── SYSTEM/                         # delay / sys / usart
│   ├── HARDWARE/                       # LED / KEY / LCD / EXTI / TIMER
│   ├── FREERTOS/                       # 本次移植的 FreeRTOS 内核
│   │   ├── include/                    #   - 内核头 + FreeRTOSConfig.h
│   │   ├── portable/                   #   - RVDS/ARM_CM3 端口代码
│   │   └── src/                        #   - 7 个内核 .c + heap_4.c
│   ├── USER/                           # main.c / stm32f10x_it.c / 工程文件
│   ├── OBJ/                            # （被 .gitignore 排除）
│   └── keilkilll.bat
├── L0-移植后的历程/                      # 阶段说明：FreeRTOS 移植工程（见文件夹内 README.md）
├── L1-实验一双led灯闪烁任务/             # 阶段说明：双 LED 闪烁任务实验（见文件夹内 README.md）
├── .gitignore
└── README.md
```

## 移植要点

- **源码版本**：FreeRTOSv202212.00
- **目标硬件**：STM32F103ZET6（正点原子开发板）
- **开发环境**：Keil MDK + ARM Compiler 5
- **内存管理**：heap_4.c（首次适应 + 合并空闲块）

**改动列表**：
1. `FREERTOS/include/FreeRTOSConfig.h` — 配置文件（来自桌面）
2. `USER/FreeRTOS.uvprojx` — 增加 FreeRTOS 文件组与头文件路径
3. `SYSTEM/sys/sys.h` — `SYSTEM_SUPPORT_OS` 置 1（delay.c / usart.c 已是 FreeRTOS 版本）
4. `USER/stm32f10x_it.c` — 注释掉 `SVC_Handler` / `PendSV_Handler` / `SysTick_Handler`（避免与 port.c、delay.c 重复定义）
5. `USER/main.c` — 创建两个任务验证移植：
   - Task1：LED1(PE2) 500ms 翻转 + 串口打印
   - Task2：LED2(PE3) 1000ms 翻转 + 串口打印

## 验证方式

- 默认堆 `configTOTAL_HEAP_SIZE = 20KB`
- 串口 115200 baud，NVIC 分组 4（FreeRTOS 推荐）
- 烧录后串口助手能看到两个任务的交替打印，LED1/LED2 不同频率闪烁

## 参考

- 正点原子 STM32F103 标准库模板（USER/README.TXT）
- FreeRTOS 官方文档 https://www.freertos.org/Documentation/RTOS_book.html
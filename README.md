# 简易电子琴 - 开发文档

>  2025 寒假 山东大学电子设计 STM32校园比赛作品
>
> #### 这个文档会显示在 Github 仓库的主页上，我会在这里更新一些通知，比如当前项目的开发进度以及待开发的功能，你们每次开发之前可以来看看这个文档
>
> 我会开发 底层的框架、部分硬件层和逻辑层、大部分交互层 以及 上位机，剩下的模块你们根据自己的能力认领，有问题可以群里问





## 功能模块清单

> #### 涉及到硬件上的 GPIO 口时，建议你们使用宏定义
>
> ```c
> #define REDLED_GPIO GPIOA
> #define REDLED_PIN GPIO_Pin_10
> 
> void RedLED_On(void)
> {
>     GPIO_WriteBit(REDLED_GPIO, REDLED_PIN, Bit_SET);
> }
> ```
>
> #### 不仅增强了代码的可读性，也方便后期更改

- [x] ### OLED 显示模块

  这个显示模块是我自己写的，主要是我自己用，不过还是介绍一下用法，和常规显示的稍有不同。简单来说我在内存里有一个缓冲区数组（更高级一点可以叫显存），里面存放着屏幕的显示数据，所有的绘制函数都只会修改这块内存里的数据，并不会影响到屏幕实际的显示。另外提供一系列刷新函数，可以将内存中的数据刷新到屏幕上。由于主机和屏幕之间采用 I2C 通信，数据传输速度不如直接操作内存快，采用这套渲染逻辑使得无论绘制多少次内容，都只需要传输一次数据，提升效率。

  另外说明一下绘制时使用的坐标系：以左上角为原点，向右为 x 轴正半轴，向下为 y 轴正半轴，以像素为单位，从 0 开始计数（如果你写过图形程序，你就会发现大部分图形程序都是采用的这套坐标系，符合人眼的阅读习惯）

  ```c
  #include "MyOLED.h"    // 提供基础的屏幕控制函数，我习惯在自己写的模块前面加上 My
  #include "MyOLED_Render.h"    // 提供通用的内容绘制函数
  
  int main(void)
  {
      MyOLED_Init();
      // 显示屏本身初始化后可能存在杂乱的显示数据，最好先清空一下
      MyOLED_Clear();    // 清除屏幕内容
      MyOLED_Flip();    // 更新整个屏幕
      
      MyOLED_Blit_String_ASCII_8x16(0, 0, "HelloWorld");    // 从 (0, 0) 像素开始，绘制 HelloWorld 字样，每个字符大小为 8x16
      MyOLED_Blit_Num_ASCII_8x16(0, 16, 2025, 4)    // 从 (0, 16) 像素开始，绘制 2025 数字，长度为 4，每个字符大小为 8x16
          
      MyOLED_Flip();    // 更新整个屏幕
      
      while (1)
      {
          
      }
  }
  
  ```

  上面是一个基础示例的演示，更多的函数可以去看源码，有详细的头部注释

  另外，我写的时候没有开发边界检测功能，屏幕的大小是 128×64，绘制的区域超出了这个范围是不安全的，轻则显示错位，重则程序崩溃，在调用的时候需要注意一下

  

- [ ] ### 无源蜂鸣器控制模块（基础功能）

  控制一个蜂鸣器需要两根线，一根输出 PWM 驱动信号，另一根输出高 / 低电平控制信号

  - 初始化函数（无参数）
  - 蜂鸣器启动 / 关闭函数（传入一个参数指定是哪一个蜂鸣器）
  - 设置 / 获取频率函数（使用 PWM 的频率控制）（两个蜂鸣器的频率独立控制）
  - 设置 / 获取音量函数（使用 PWM 的占空比控制）（两个蜂鸣器的音量独立控制）
  - 统一设置 / 获取音量函数（控制音量使用这个函数，上面两个是给进阶功能中的淡入淡出声音用的）



- [ ] ### 无源蜂鸣器控制模块（进阶功能）

  - 蜂鸣器启动 / 关闭函数（与基础功能中不同的是要求声音淡入淡出）

    实现这一功能需要对蜂鸣器进行连续的控制，如何做到这一点的同时不影响其他代码的运行

  - 控制蜂鸣器发出指定频率的声音函数（淡入）（只传入一个频率参数）

    要求优先使用主蜂鸣器发声；如果主蜂鸣器正在发声，则使用副蜂鸣器；如果两个蜂鸣器都在发声，则直接切换更早发声的蜂鸣器的频率（主 → 副 → 主 → 副 ···）

  - 关闭发出指定频率声音的蜂鸣器函数（淡出）

    如果没有发出此频率声音的蜂鸣器，则忽略

    （按键抬起的时候会调用这个函数）

    

- [ ] ### 矩阵键盘读取模块

  - 初始化函数（无参数）

  - 读取键盘输入函数（无参数）

    矩阵键盘的读取方式在项目框架图里有一定介绍，这个函数要求返回一个长度为十六的二进制数，每一位的 0 或 1 对应了每一个按键的按下状态

  



## 当前项目进度

#### 搭建底层框架（不是硬件层）

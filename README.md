# 简易电子琴 - 开发文档

>  2025 寒假 山东大学电子设计 STM32校园比赛作品
>
> #### 这个文档会显示在 Github 仓库的主页上，我会在这里更新一些通知，比如当前项目的开发进度以及待开发的功能，你们每次开发之前可以来看看这个文档
>
> 我会开发 底层的框架、部分硬件层和逻辑层、大部分交互层 以及 上位机，剩下的模块你们根据自己的能力认领，有问题可以群里问





## 硬件资源使用情况

- PA9、PA10 ------ 串口通信预留
- TIM1 ------ 用作全局计时时钟
- TIM2、TIM3 ------ 两路独立 PWM 波输出
- PB8、PB9 ------ OLED 屏幕通信
- PB3、PB4、PB5、PB6、PB7、PA11、PA12、PA15 ------ LED 灯列
- PA0、PA6 ------ PWM 输出口





## 功能模块清单

> #### 涉及到硬件上的 GPIO 口时，建议你们使用宏定义
>
> ```c
> #define REDLED_GPIO GPIOA
> #define REDLED_PIN GPIO_Pin_10
> 
> void RedLED_On(void)
> {
>        GPIO_SetBits(REDLED_GPIO, REDLED_PIN);
> }
> ```
>
> #### 不仅增强了代码的可读性，也方便后期更改



### 无源蜂鸣器控制模块（基础功能）

- [ ] 待开发

- 初始化函数（无参数）
- 蜂鸣器启动 / 关闭函数（传入一个参数指定是哪一个蜂鸣器）
- 设置 / 获取频率函数（使用 PWM 的频率控制）（两个蜂鸣器的频率独立控制）
- 设置 / 获取音量函数（使用 PWM 的占空比控制）（两个蜂鸣器的音量独立控制）
- 统一设置 / 获取音量函数（控制音量使用这个函数，上面两个是给进阶功能中的淡入淡出声音用的）



### 无源蜂鸣器控制模块（进阶功能）

- [ ] 待开发

- 蜂鸣器启动 / 关闭函数（与基础功能中不同的是要求声音淡入淡出）

  实现这一功能需要对蜂鸣器进行连续的控制，如何在做到这一点的同时不影响其他代码的运行会在程序底层框架搭建完毕之后讲解

- 控制蜂鸣器发出指定频率的声音函数（淡入）（只传入一个频率参数）

  要求优先使用主蜂鸣器发声；如果主蜂鸣器正在发声，则使用副蜂鸣器；如果两个蜂鸣器都在发声，则直接切换更早发声的蜂鸣器的频率（主 → 副 → 主 → 副 ···）

- 关闭发出指定频率声音的蜂鸣器函数（淡出）（只传入一个频率参数）

  如果没有发出此频率声音的蜂鸣器，则忽略

  （按键抬起的时候会调用这个函数）


后面两个函数不会指定要控制的蜂鸣器，它们是其他模块控制蜂鸣器的顶层接口，一般不需要使用其他函数（除了初始化）



### 矩阵键盘读取模块

- [ ] 待开发

- 初始化函数（无参数）

- 读取键盘输入函数（无参数）

  矩阵键盘的读取方式在项目框架图里有一定介绍，这个函数要求返回一个长度为十六的二进制数，每一位的 0 或 1 对应了每一个按键的按下状态

介绍一下实现读取可能要用到的两种位运算：<<（左移）和 >>（右移），可以很方便的实现对二进制数的操作（而且位运算的速度非常快，包括其他的比如与运算、异或运算什么的，因为这就是逻辑电路的原生运算，加减乘除都是模拟出来的）

```c
int a = 1;    // a 的值写成 4 位二进制就是 0001
int b = a << 2;    // 左移顾名思义，就是在二进制层面上将数字向左移动指定位数，b 的值是 0100
```

需要注意的是，溢出数据类型长度的位将被丢弃，低位空出来的数据位补 0，高位空出来的数据位一般补符号位（正 0 负 1），它的效果和对二进制数乘 2 除 2 一样，但是运算速度更快



### 音频存储播放模块

- [ ] 待开发

此模块需要使用蜂鸣器的控制函数，不过由于蜂鸣器控制模块的结构和函数接口已经给出，两者可以同时开发，只是在蜂鸣器控制模块完成之前此模块无法得到验证（可以暂时使用蜂鸣器的基础控制函数，那个开发的会比较快）

- 定义音频结构体数据类型

  音频名称

  音频数据，音频数据存储 8 个音符的开关时刻（会有同时存在两个音符的情况，因此不能采用顺序存储的方式）

- 播放音乐（接收一个音频结构体参数）

  根据音频结构体的音频数据信息 **开始** 播放音乐，这个函数不能阻塞整个程序的运行，同样需要连续控制声音播放

- 暂停音乐

- 继续播放

- 设置 / 获取播放速度

- 设置播放模式：顺序播放、随机播放、单曲循环

- 获取音频列表，返回一个音频结构体数组

- 计算音频长度（接收一个音频结构体参数）

  统计每一个音符持续长度之和，并转换成秒（单位转换会有单独的函数）



### 时钟模块

- [x] 已完成

- 获取时刻

  获取当前时刻

- 时间转换

  将程序时刻转换成秒



### 杂项

这个模块用来放一些需要但是不知道放哪里的函数

- 获取数字指定位

  这是为分析矩阵键盘的输入设计的函数，可以方便的获取单个按键的输入状态

  - [x] 已完成



### 灯效模块

- [ ] 待开发

利用剩余的 GPIO 口搭建了 8 个 LED 灯列，可以实现一些灯效

- 实时模式：单独控制每颗灯珠的亮灭（用于同步手动演奏）
- 预设模式：按照时序播放一些灯效，比如流水灯等，这里自由发挥



## 当前项目进度

- [x] 硬件初步测试完毕
- [x] 底层框架（不是硬件层）搭建完毕，可根据后期需求微调
- [x] 电路基本设计完毕
- [ ] 正在开发硬件层
- [ ] 上位机初步框架建立

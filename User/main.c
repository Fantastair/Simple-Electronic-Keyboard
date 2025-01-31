#include "stm32f10x.h"
#include "MyTime.h"
#include "MyOLED.h"
#include "MyOLED_Render.h"
#include "frame.h"
#include "Serial.h"
#include "Keyboard.h"
#include "Page.h"
#include "Buzzer.h"

int main(void)
{
    // 模块初始化
    FrameInit();
    MyOLED_Init(); MyOLED_Clear_GRAM(); MyOLED_Flip();
    MyTime_Init();
    Serial_Init();
    Keyboard_Init();
    Buzzer_Init();
    Page_Init();

    while (1)
    {
        CheckHardware(current_page);    // 检查硬件输入
        FrameTick();    // 执行帧函数
        current_page->PageRender();    // 渲染屏幕
    }
}

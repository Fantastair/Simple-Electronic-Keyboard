#include "stm32f10x.h"
#include "MyGPIO.h"
#include "MyADC.h"

MyADC MyADC1 = {RCC_APB2Periph_ADC1, ADC1};
MyADC MyADC2 = {RCC_APB2Periph_ADC2, ADC2};

/**
 * @brief 初始化 ADC 外设，单 ADC 模式，软件触发
 *     需要自行配置相应的 GPIO 口为模拟输入模式
 * @param MyADCx 指定初始化的ADC，x 可以为1、2
 * @param ADC_ContinuousConvMode 指定是否使用连续转换模式（否则使用单次转换模式），可以为 ENABLE 或 DISABLE
 * @param ADC_ScanConvMode 指定是否使用扫描模式（否则使用非扫描模式），可以为 ENABLE 或 DISABLE
 * @param ADC_NbrOfChannel 指定通道数量
 * @retval 无
 */
void MyADC_Init(MyADC* MyADCx, FunctionalState ADC_ContinuousConvMode, FunctionalState ADC_ScanConvMode, uint8_t ADC_NbrOfChannel)
{
    RCC_APB2PeriphClockCmd(MyADCx->RCC_APB2Periph_ADCx, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_ContinuousConvMode = ADC_ContinuousConvMode;
    ADC_InitStructure.ADC_ScanConvMode = ADC_ScanConvMode;
    ADC_InitStructure.ADC_NbrOfChannel = ADC_NbrOfChannel;
    ADC_Init(MyADCx->ADCx, &ADC_InitStructure);

    ADC_Cmd(MyADCx->ADCx, ENABLE);

    ADC_ResetCalibration(MyADCx->ADCx);
    while (ADC_GetResetCalibrationStatus(MyADCx->ADCx) == SET);
    ADC_StartCalibration(MyADCx->ADCx);
    while (ADC_GetCalibrationStatus(MyADCx->ADCx) == SET);
}

/**
 * @brief 配置规则组转换通道
 * @param MyADCx 指定配置的ADC，x可以为1、2
 * @param ADC_Channel_x 指定配置的 ADC 通道，x 可以为 0 ~ 17
 * @param Rank 指定在规则组列表的位次，可以为 1 ~ 16
 * @param ADC_SampleTime 指定采样时间
 * @retval 无
 */
void MyADC_RegularChannelConFig(MyADC* MyADCx, uint8_t ADC_Channel_x, uint8_t Rank, uint8_t ADC_SampleTime)
{
    ADC_RegularChannelConfig(MyADCx->ADCx, ADC_Channel_x, Rank, ADC_SampleTime);
}

/**
 * @brief 触发 ADC 转换
 * @param MyADCx 指定ADC，x可以为1、2
 * @retval 无
 */
void MyADC_Start(MyADC* MyADCx)
{
    ADC_SoftwareStartConvCmd(MyADCx->ADCx, ENABLE);
}

/**
 * @brief 等待 ADC 转换完成
 * @param MyADCx 指定配置的ADC，x可以为1、2
 * @retval 无
 */
void MyADC_Wait(MyADC* MyADCx)
{
    while (ADC_GetFlagStatus(MyADCx->ADCx, ADC_FLAG_EOC) == RESET);
}

/**
 * @brief 获取 ADC 转换结果
 * @param MyADCx 指定ADC，x可以为1、2
 * @retval 转换结果值
 */
uint16_t MyADC_GetValue(MyADC* MyADCx)
{
    return ADC_GetConversionValue(MyADCx->ADCx);
}

/**
 * @brief 获取指定 ADC 的数据寄存器，提供给 DMA 转运使用
 * @param MyADCx 指定ADC，x可以为1、2
 * @retval DR 寄存器地址
 */
uint32_t MyADC_GetAddr(MyADC * MyADCx)
{
    return (uint32_t)(&(MyADCx->ADCx)->DR);
}

/**
 * @brief 使用 ADC 触发 DMA 转运
 * @param MyADCx 指定ADC，x可以为1、2
 * @retval 无
 */
void MyADC_StartDMA(MyADC * MyADCx)
{
    ADC_DMACmd(MyADCx->ADCx, ENABLE);
}

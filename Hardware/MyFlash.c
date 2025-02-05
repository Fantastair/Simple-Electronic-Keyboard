#include "stm32f10x.h"
#include "MyFlash.h"

/**
 * @brief 读取指定地址的 32 位数据
 * @param Address 地址
 * @return 32 位数据
 */
uint32_t MyFlash_ReadData_32(uint32_t Address)
{
    return *((__IO uint32_t *) (Address));
}

/**
 * @brief 读取指定地址的 16 位数据
 * @param Address 地址
 * @return 16 位数据
 */
uint16_t MyFlash_ReadData_16(uint32_t Address)
{
    return *((__IO uint16_t *) (Address));
}

/**
 * @brief 读取指定地址的 8 位数据
 * @param Address 地址
 * @return 8 位数据
 */
uint8_t MyFlash_ReadData_8(uint32_t Address)
{
    return *((__IO uint8_t *) (Address));
}

/**
 * @brief 擦除所有页
 */
void MyFlash_EraseAllPages(void)
{
    FLASH_Unlock();
    FLASH_EraseAllPages();
    FLASH_Lock();
}

/**
 * @brief 擦除指定页
 * @param page 页号
 */
void MyFLASH_ErasePage(uint8_t page)
{
    FLASH_Unlock();
    FLASH_ErasePage(MyFlash_GetPageAddress(page));
    FLASH_Lock();
}

/**
 * @brief 写入 32 位数据
 * @param Address 地址
 * @param Data 数据
 */
void MyFlash_WriteData_32(uint32_t Address, uint32_t Data)
{
    FLASH_Unlock();
    FLASH_ProgramWord(Address, Data);
    FLASH_Lock();
}

/**
 * @brief 写入 16 位数据
 * @param Address 地址
 * @param Data 数据
 */
void MyFlash_WriteData_16(uint32_t Address, uint16_t Data)
{
    FLASH_Unlock();
    FLASH_ProgramHalfWord(Address, Data);
    FLASH_Lock();
}

/**
 * @brief 复制 16 字节数据数组到闪存中
 * @param Address 地址
 * @param Data 数据数组
 * @param Length 数据长度
 */
void MyFlash_WriteData_16x(uint32_t Address, uint16_t *Data, uint32_t Length)
{
    uint32_t i;
    FLASH_Unlock();
    for (i = 0; i < Length; i ++)
    {
        FLASH_ProgramHalfWord(Address + 2 * i, Data[i]);
    }
    FLASH_Lock();
}

/**
 * @brief 复制 32 字节数据数组到闪存中
 * @param Address 地址
 * @param Data 数据数组
 * @param Length 数据长度
 */
void MyFlash_WriteData_32x(uint32_t Address, uint32_t *Data, uint32_t Length)
{
    uint32_t i;
    FLASH_Unlock();
    for (i = 0; i < Length; i ++)
    {
        FLASH_ProgramWord(Address + 4 * i, Data[i]);
    }
    FLASH_Lock();
}

/**
 * @brief 获取页地址
 * @param page 页号
 * @return 页地址
 */
uint32_t MyFlash_GetPageAddress(uint8_t page)
{
    return 0x08000000 + 0x00000400 * page;
}

/**
 * @brief 判断指定页是否为空
 * @param page 页号
 * @return 1 表示为空，0 表示不为空
 */
uint8_t MyFlash_IsEmptyPage(uint8_t page)

{
    uint32_t page_address = MyFlash_GetPageAddress(page);
    uint32_t i;

    for (i = 0; i < 0x400; i += 4)
    {
        if (MyFlash_ReadData_32(page_address + i) != 0xFFFFFFFF)
        {
            return 0;
        }
    }

    return 1;
}

#include "stm32f10x.h"                  // Device header
#include "MyOLED.h"
#include "MyOLED_Render.h"

const uint8_t Font_ASCII_8x16[][8] =
{
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},/*" ",0*/
{0x00,0x00,0x00,0xFC,0xFC,0x00,0x00,0x00,},
{0x00,0x00,0x00,0x33,0x33,0x00,0x00,0x00,},/*"!",1*/
{0x00,0x12,0x0E,0x00,0x12,0x0E,0x00,0x00,},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},/*""",2*/
{0x00,0x40,0xC0,0x78,0x40,0xC0,0x78,0x40,},
{0x04,0x3C,0x07,0x04,0x3C,0x07,0x04,0x00,},/*"#",3*/
{0x00,0x70,0x88,0x88,0xFC,0x08,0x30,0x00,},
{0x00,0x0C,0x10,0x3F,0x11,0x11,0x0E,0x00,},/*"$",4*/
{0x00,0x38,0x44,0x38,0xC0,0x30,0x0C,0x00,},
{0x00,0x30,0x0C,0x03,0x1C,0x22,0x1C,0x00,},/*"%",5*/
{0x00,0x78,0x84,0x44,0x44,0x38,0x00,0x80,},
{0x0E,0x11,0x20,0x21,0x22,0x14,0x0C,0x33,},/*"&",6*/
{0x00,0x12,0x0E,0x00,0x00,0x00,0x00,0x00,},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},/*"'",7*/
{0x00,0x00,0x00,0xE0,0x18,0x04,0x02,0x00,},
{0x00,0x00,0x00,0x07,0x18,0x20,0x40,0x00,},/*"(",8*/
{0x00,0x02,0x04,0x18,0xE0,0x00,0x00,0x00,},
{0x00,0x40,0x20,0x18,0x07,0x00,0x00,0x00,},/*")",9*/
{0x40,0x80,0x80,0xF0,0x80,0x80,0x40,0x00,},
{0x00,0x0C,0x02,0x01,0x02,0x0C,0x00,0x00,},/*"*",10*/
{0x00,0x00,0x00,0x00,0xE0,0x00,0x00,0x00,},
{0x00,0x01,0x01,0x01,0x0F,0x01,0x01,0x01,},/*"+",11*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
{0x00,0x90,0x70,0x00,0x00,0x00,0x00,0x00,},/*",",12*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
{0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x00,},/*"-",13*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
{0x00,0x30,0x30,0x00,0x00,0x00,0x00,0x00,},/*".",14*/
{0x00,0x00,0x00,0x00,0xC0,0x30,0x0C,0x00,},
{0x00,0x30,0x0C,0x03,0x00,0x00,0x00,0x00,},/*"/",15*/
{0x00,0xF0,0x08,0x04,0x84,0x48,0xF0,0x00,},
{0x00,0x0F,0x12,0x21,0x20,0x10,0x0F,0x00,},/*"0",16*/
{0x00,0x00,0x08,0x08,0xFC,0x00,0x00,0x00,},
{0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,},/*"1",17*/
{0x00,0x38,0x04,0x04,0x04,0x84,0x78,0x00,},
{0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00,},/*"2",18*/
{0x00,0x18,0x04,0x84,0x84,0x44,0x38,0x00,},
{0x00,0x18,0x20,0x20,0x20,0x21,0x1E,0x00,},/*"3",19*/
{0x00,0x80,0x60,0x10,0x08,0xFC,0x00,0x00,},
{0x00,0x07,0x04,0x24,0x24,0x3F,0x24,0x00,},/*"4",20*/
{0x00,0xFC,0x44,0x44,0x44,0x84,0x0C,0x00,},
{0x00,0x18,0x20,0x20,0x20,0x10,0x0F,0x00,},/*"5",21*/
{0x00,0xF0,0x88,0x44,0x44,0x44,0x88,0x00,},
{0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00,},/*"6",22*/
{0x00,0x0C,0x04,0x04,0x04,0xF4,0x0C,0x00,},
{0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,},/*"7",23*/
{0x00,0x78,0x84,0x84,0x84,0x84,0x78,0x00,},
{0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,},/*"8",24*/
{0x00,0xF8,0x04,0x04,0x04,0x04,0xF8,0x00,},
{0x00,0x11,0x22,0x22,0x22,0x11,0x0F,0x00,},/*"9",25*/
{0x00,0x00,0x30,0x30,0x00,0x00,0x00,0x00,},
{0x00,0x00,0x0C,0x0C,0x00,0x00,0x00,0x00,},/*":",26*/
{0x00,0x00,0x30,0x30,0x00,0x00,0x00,0x00,},
{0x00,0x00,0x12,0x0E,0x00,0x00,0x00,0x00,},/*";",27*/
{0x00,0x00,0x80,0x40,0x40,0x20,0x20,0x00,},
{0x00,0x01,0x02,0x04,0x04,0x08,0x08,0x00,},/*"<",28*/
{0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x00,},
{0x00,0x02,0x02,0x02,0x02,0x02,0x02,0x00,},/*"=",29*/
{0x00,0x20,0x20,0x40,0x40,0x80,0x00,0x00,},
{0x00,0x08,0x08,0x04,0x04,0x02,0x01,0x00,},/*">",30*/
{0x00,0x38,0x04,0x04,0x04,0x84,0x78,0x00,},
{0x00,0x00,0x00,0x36,0x31,0x00,0x00,0x00,},/*"?",31*/
{0x00,0xF0,0x08,0xC4,0x24,0xE4,0x08,0xF0,},
{0x00,0x0F,0x10,0x23,0x24,0x27,0x24,0x13,},/*"@",32*/
{0x00,0xE0,0x18,0x04,0x04,0x18,0xE0,0x00,},
{0x20,0x3F,0x21,0x01,0x01,0x21,0x3F,0x20,},/*"A",33*/
{0x04,0xFC,0x84,0x84,0x84,0x78,0x00,0x00,},
{0x20,0x3F,0x20,0x20,0x20,0x21,0x1E,0x00,},/*"B",34*/
{0x00,0xF0,0x08,0x04,0x04,0x04,0x18,0x00,},
{0x00,0x0F,0x10,0x20,0x20,0x20,0x18,0x00,},/*"C",35*/
{0x04,0xFC,0x04,0x04,0x04,0x08,0xF0,0x00,},
{0x20,0x3F,0x20,0x20,0x20,0x10,0x0F,0x00,},/*"D",36*/
{0x04,0xFC,0x84,0x84,0x84,0xC4,0x18,0x00,},
{0x20,0x3F,0x20,0x20,0x20,0x21,0x18,0x00,},/*"E",37*/
{0x04,0xFC,0x84,0x84,0x84,0xC4,0x08,0x00,},
{0x20,0x3F,0x20,0x00,0x00,0x01,0x00,0x00,},/*"F",38*/
{0xE0,0x18,0x04,0x04,0x04,0x3C,0x00,0x00,},
{0x07,0x18,0x20,0x20,0x22,0x1E,0x02,0x00,},/*"G",39*/
{0x04,0xFC,0x84,0x80,0x80,0x84,0xFC,0x04,},
{0x20,0x3F,0x20,0x00,0x00,0x20,0x3F,0x20,},/*"H",40*/
{0x00,0x00,0x04,0x04,0xFC,0x04,0x04,0x00,},
{0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,},/*"I",41*/
{0x00,0x0C,0x04,0x04,0x04,0xFC,0x04,0x00,},
{0x00,0x18,0x20,0x20,0x10,0x0F,0x00,0x00,},/*"J",42*/
{0x04,0xFC,0x84,0x40,0x34,0x0C,0x04,0x00,},
{0x20,0x3F,0x20,0x01,0x22,0x3C,0x20,0x00,},/*"K",43*/
{0x04,0xFC,0x04,0x00,0x00,0x00,0x00,0x00,},
{0x20,0x3F,0x20,0x20,0x20,0x20,0x18,0x00,},/*"L",44*/
{0x04,0xFC,0x7C,0x80,0x7C,0xFC,0x04,0x00,},
{0x20,0x3F,0x00,0x3F,0x00,0x3F,0x20,0x00,},/*"M",45*/
{0x04,0xFC,0x18,0xE0,0x00,0x04,0xFC,0x04,},
{0x20,0x3F,0x20,0x00,0x07,0x18,0x3F,0x20,},/*"N",46*/
{0xE0,0x18,0x04,0x04,0x04,0x04,0x18,0xE0,},
{0x07,0x18,0x20,0x20,0x20,0x20,0x18,0x07,},/*"O",47*/
{0x04,0xFC,0x04,0x04,0x04,0x84,0x78,0x00,},
{0x20,0x3F,0x21,0x01,0x01,0x00,0x00,0x00,},/*"P",48*/
{0xF0,0x08,0x04,0x04,0x04,0x08,0xF0,0x00,},
{0x0F,0x10,0x28,0x28,0x70,0x50,0x4F,0x00,},/*"Q",49*/
{0x04,0xFC,0x84,0x84,0x84,0x84,0x78,0x00,},
{0x20,0x3F,0x20,0x00,0x03,0x0C,0x30,0x20,},/*"R",50*/
{0x00,0x38,0x44,0x84,0x84,0x04,0x1C,0x00,},
{0x00,0x38,0x20,0x20,0x20,0x21,0x1E,0x00,},/*"S",51*/
{0x1C,0x04,0x04,0xFC,0x04,0x04,0x1C,0x00,},
{0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00,},/*"T",52*/
{0x04,0xFC,0x04,0x00,0x00,0x04,0xFC,0x04,},
{0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,},/*"U",53*/
{0x04,0x7C,0x84,0x00,0x00,0x84,0x7C,0x04,},
{0x00,0x00,0x0F,0x30,0x30,0x0F,0x00,0x00,},/*"V",54*/
{0x04,0xFC,0x00,0xF8,0x00,0xFC,0x04,0x00,},
{0x00,0x3F,0x3C,0x03,0x3C,0x3F,0x00,0x00,},/*"W",55*/
{0x04,0x0C,0x34,0xC0,0xC0,0x34,0x0C,0x04,},
{0x20,0x38,0x26,0x01,0x01,0x26,0x38,0x20,},/*"X",56*/
{0x04,0x3C,0xC4,0x00,0xC4,0x3C,0x04,0x00,},
{0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00,},/*"Y",57*/
{0x18,0x04,0x04,0x84,0x64,0x1C,0x04,0x00,},
{0x20,0x38,0x26,0x21,0x20,0x20,0x18,0x00,},/*"Z",58*/
{0x00,0x00,0x00,0xFE,0x02,0x02,0x02,0x00,},
{0x00,0x00,0x00,0x7F,0x40,0x40,0x40,0x00,},/*"[",59*/
{0x00,0x0C,0x30,0xC0,0x00,0x00,0x00,0x00,},
{0x00,0x00,0x00,0x00,0x03,0x0C,0x30,0x00,},/*"\",60*/
{0x00,0x02,0x02,0x02,0xFE,0x00,0x00,0x00,},
{0x00,0x40,0x40,0x40,0x7F,0x00,0x00,0x00,},/*"]",61*/
{0x00,0x20,0x10,0x08,0x08,0x10,0x20,0x00,},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},/*"^",62*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
{0x00,0x40,0x40,0x40,0x40,0x40,0x40,0x00,},/*"_",63*/
{0x00,0x00,0x08,0x30,0x20,0x00,0x00,0x00,},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},/*"`",64*/
{0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,},
{0x00,0x19,0x24,0x24,0x12,0x3F,0x20,0x00,},/*"a",65*/
{0x10,0xF0,0x00,0x80,0x80,0x00,0x00,0x00,},
{0x00,0x3F,0x11,0x20,0x20,0x11,0x0E,0x00,},/*"b",66*/
{0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x00,},
{0x00,0x0E,0x11,0x20,0x20,0x20,0x11,0x00,},/*"c",67*/
{0x00,0x00,0x80,0x80,0x80,0x90,0xF0,0x00,},
{0x00,0x1F,0x20,0x20,0x20,0x10,0x3F,0x20,},/*"d",68*/
{0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,},
{0x00,0x1F,0x24,0x24,0x24,0x24,0x17,0x00,},/*"e",69*/
{0x00,0x80,0x80,0xE0,0x90,0x90,0x20,0x00,},
{0x00,0x80,0x80,0x7F,0x00,0x00,0x00,0x00,},/*"f",70*/
{0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,},
{0x00,0x6B,0x94,0x94,0x94,0x93,0x60,0x00,},/*"g",71*/
{0x10,0xF0,0x00,0x80,0x80,0x00,0x00,0x00,},
{0x20,0x3F,0x21,0x00,0x20,0x3F,0x20,0x00,},/*"h",72*/
{0x00,0x80,0x98,0x98,0x00,0x00,0x00,0x00,},
{0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,},/*"i",73*/
{0x00,0x00,0x40,0x4C,0xCC,0x00,0x00,0x00,},
{0x00,0x80,0x80,0x40,0x3F,0x00,0x00,0x00,},/*"j",74*/
{0x10,0xF0,0x00,0x00,0x80,0x80,0x80,0x00,},
{0x20,0x3F,0x24,0x06,0x29,0x30,0x20,0x00,},/*"k",75*/
{0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,},
{0x00,0x00,0x00,0x1F,0x20,0x20,0x00,0x00,},/*"l",76*/
{0x80,0x80,0x80,0x00,0x80,0x80,0x00,0x00,},
{0x3F,0x00,0x00,0x3F,0x00,0x00,0x3F,0x00,},/*"m",77*/
{0x80,0x80,0x00,0x80,0x80,0x00,0x00,0x00,},
{0x20,0x3F,0x21,0x00,0x20,0x3F,0x20,0x00,},/*"n",78*/
{0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,},
{0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,},/*"o",79*/
{0x80,0x80,0x00,0x80,0x80,0x00,0x00,0x00,},
{0x80,0xFF,0x91,0x20,0x20,0x11,0x0E,0x00,},/*"p",80*/
{0x00,0x00,0x00,0x80,0x80,0x00,0x80,0x00,},
{0x00,0x0E,0x11,0x20,0x20,0x91,0xFF,0x80,},/*"q",81*/
{0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00,},
{0x20,0x20,0x3F,0x21,0x20,0x00,0x01,0x00,},/*"r",82*/
{0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,},
{0x00,0x33,0x24,0x24,0x24,0x24,0x19,0x00,},/*"s",83*/
{0x00,0x80,0x80,0xE0,0x80,0x80,0x00,0x00,},
{0x00,0x00,0x00,0x1F,0x20,0x20,0x10,0x00,},/*"t",84*/
{0x80,0x80,0x00,0x00,0x00,0x80,0x80,0x00,},
{0x00,0x1F,0x20,0x20,0x20,0x10,0x3F,0x20,},/*"u",85*/
{0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00,},
{0x00,0x03,0x0C,0x30,0x0C,0x03,0x00,0x00,},/*"v",86*/
{0x80,0x80,0x00,0x80,0x80,0x00,0x80,0x80,},
{0x01,0x0E,0x30,0x0C,0x07,0x38,0x06,0x01,},/*"w",87*/
{0x00,0x80,0x80,0x80,0x00,0x80,0x80,0x00,},
{0x00,0x20,0x31,0x0E,0x2E,0x31,0x20,0x00,},/*"x",88*/
{0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,},
{0x00,0x81,0x86,0x78,0x18,0x06,0x01,0x00,},/*"y",89*/
{0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x00,},
{0x00,0x21,0x30,0x2C,0x22,0x21,0x30,0x00,},/*"z",90*/
{0x00,0x00,0x80,0x7C,0x02,0x02,0x00,0x00,},
{0x00,0x01,0x02,0x3C,0x40,0x40,0x00,0x00,},/*"{",91*/
{0x00,0x00,0x00,0xFE,0x00,0x00,0x00,0x00,},
{0x00,0x00,0x00,0x7F,0x00,0x00,0x00,0x00,},/*"|",92*/
{0x00,0x00,0x02,0x02,0x7C,0x80,0x00,0x00,},
{0x00,0x00,0x40,0x40,0x3C,0x02,0x01,0x00,},/*"}",93*/
{0x80,0xC0,0x40,0x80,0x00,0x00,0x00,0x80,},
{0x01,0x00,0x00,0x00,0x01,0x02,0x03,0x01,},/*"~",94*/
};

const uint8_t Icon_64x64[8][64] =
{
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0xC0,0xC0,0xE0,0xE0,0xE0,0xF0,0x70,0x70,0x78,0x78,0xFF,0xFF,0xFF,0xFF,0x78,0x78,0x70,0x70,0xF0,0xE0,0xE0,0xE0,0xC0,0xC0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xE0,0xF0,0xF8,0x7C,0x3C,0x1E,0x1F,0x0F,0x07,0x07,0x03,0xF9,0xFD,0xFC,0xFE,0x8E,0x8E,0x8E,0x8E,0x8E,0x8F,0x8F,0x8F,0x8F,0x8E,0x8E,0x8E,0x8E,0x8E,0xFC,0xFD,0xF9,0xF3,0x03,0x07,0x07,0x0F,0x1F,0x1E,0x3C,0x7C,0xF8,0xF0,0xE0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
{0x00,0x00,0x00,0x00,0x80,0xE0,0xF8,0xFE,0x7F,0x1F,0x07,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x87,0x87,0x87,0x87,0x87,0x87,0x87,0x87,0x87,0x87,0x87,0x87,0x87,0x87,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x07,0x1F,0x7F,0xFE,0xF8,0xE0,0x80,0x00,0x00,0x00,0x00,},
{0x00,0x00,0x00,0xFC,0xFF,0xFF,0xFF,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0xFF,0xFF,0xFF,0xFF,0x73,0x73,0x73,0x73,0x73,0x73,0x73,0x73,0x73,0x73,0xFF,0xFF,0xFF,0xFF,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0xFF,0xFF,0xFC,0x00,0x00,0x00,},
{0x00,0x00,0x00,0x3F,0xFF,0xFF,0xFF,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xFF,0xFF,0xFF,0xFF,0xCE,0xCE,0xCE,0xCE,0xCE,0xCE,0xCE,0xCE,0xCE,0xCE,0xFF,0xFF,0xFF,0xFF,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xFF,0xFF,0xFF,0x3F,0x00,0x00,0x00,},
{0x00,0x00,0x00,0x00,0x01,0x0F,0x3F,0x7F,0xFE,0xF8,0xE0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xE1,0xE1,0xE1,0xE1,0xE1,0xE1,0xE1,0xE1,0xE1,0xE1,0xE1,0xE1,0xE1,0xE1,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xE0,0xF8,0xFE,0x7F,0x3F,0x0F,0x01,0x00,0x00,0x00,0x00,},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x07,0x0F,0x1F,0x3E,0x7C,0x78,0xF0,0xF0,0xE0,0xC0,0x80,0xBF,0x7F,0x7F,0xFF,0xF1,0xF1,0xF1,0xF1,0xF1,0xF1,0xF1,0xF1,0xF1,0xF1,0xF1,0xF1,0xF1,0xF1,0xFF,0x7F,0x7F,0xBF,0x80,0xC0,0xE0,0xF0,0xF0,0x78,0x7C,0x3E,0x1F,0x0F,0x07,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x03,0x03,0x07,0x07,0x07,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0xFF,0xFF,0xFF,0xFF,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x07,0x07,0x07,0x03,0x03,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,}
};


/**
 * @brief 将一个字符图像数据覆盖在显存缓存的指定位置上
 * @param Left 字符左上角的横坐标像素位置，0 ~ 127
 * @param Top 字符左上角的纵坐标像素位置，0 ~ 63
 * @param Char 字符
 * @retval 无
 */
void MyOLED_Blit_Char_ASCII_8x16(uint8_t Left, uint8_t Top, char Char)
{
    MyOLED_Blit_GRAM(Font_ASCII_8x16[(Char-' ')*2], Left, Top, 8);
    MyOLED_Blit_GRAM(Font_ASCII_8x16[(Char-' ')*2+1], Left, Top+8, 8);
}

/**
 * @brief 将一个字符串图像数据覆盖在显存缓存的指定位置上
 * @param Left 字符左上角的横坐标像素位置，0 ~ 127
 * @param Top 字符左上角的纵坐标像素位置，0 ~ 63
 * @param String 字符串
 * @retval 无
 */
void MyOLED_Blit_String_ASCII_8x16(uint8_t Left, uint8_t Top, char *String)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++)
    {
        MyOLED_Blit_Char_ASCII_8x16(Left + i * 8, Top, String[i]);
    }
}

/**
 * @brief 将正十进制数形式数字的图像数据覆盖在显存缓存指定区域
 * @param Left 0 ~ 127
 * @param Top 0 ~ 64
 * @param Number 要显示的数字，0 ~ 0xFFFFFFFF
 * @param Length 要显示数字的长度，1 ~ 10
 * @retval 无
 */
void MyOLED_Blit_Num_ASCII_8x16(uint8_t Left, uint8_t Top, uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = Length; i > 0; i--)                            
    {
        MyOLED_Blit_Char_ASCII_8x16(Left + 8 * (i-1), Top, Number % 10 + '0');
        Number /= 10;
    }
}

/**
 * @brief 将带符号十进制数形式数字的图像数据覆盖在显存缓存指定区域
 * @param Left 0 ~ 127
 * @param Top 0 ~ 64
 * @param Number 要显示的数字，-0xFFFFFFF ~ +0xFFFFFFF
 * @param Length 要显示数字的长度，1 ~ 10
 * @retval 无
 */
void MyOLED_Blit_SignedNum_ASCII_8x16(uint8_t Left, uint8_t Top, int32_t Number, uint8_t Length)
{
    if (Number < 0)
    {
        MyOLED_Blit_Char_ASCII_8x16(Left, Top, '-');
        MyOLED_Blit_Num_ASCII_8x16(Left + 8, Top, -Number, Length);
    } else
    {
        MyOLED_Blit_Char_ASCII_8x16(Left, Top, '+');
        MyOLED_Blit_Num_ASCII_8x16(Left + 8, Top, Number, Length);
    }
}

/**
 * @brief 将十六进制数形式数字的图像数据覆盖在显存缓存指定区域
 * @param Left 0 ~ 127
 * @param Top 0 ~ 64
 * @param Number 要显示的数字，0xFFFFFFFF ~ 0xFFFFFFFF
 * @param Length 要显示数字的长度，1 ~ 8，不包括前面的“0x”
 * @retval 无
 */
void MyOLED_Blit_HexNum_ASCII_8x16(uint8_t Left, uint8_t Top, uint32_t Number, uint8_t Length)
{
    uint8_t i, SingleNumber;
    MyOLED_Blit_Char_ASCII_8x16(Left, Top, '0');
    MyOLED_Blit_Char_ASCII_8x16(Left + 8, Top, 'x');
    for (i = Length; i > 0; i--)
    {
        SingleNumber = Number % 16;
        Number /= 16;
        if (SingleNumber < 10)
            MyOLED_Blit_Char_ASCII_8x16(Left + 8 + 8*i, Top, SingleNumber + '0');
        else
            MyOLED_Blit_Char_ASCII_8x16(Left + 8 + 8*i, Top, SingleNumber - 10 + 'A');
    }
}

/**
 * @brief 将二进制数形式数字的图像数据覆盖在显存缓存指定区域
 * @param Left 0 ~ 127
 * @param Top 0 ~ 64
 * @param Number 要显示的数字，0xFFFF ~ 0xFFFF
 * @param Length 要显示数字的长度，1 ~ 14，不包括前面的“0b”
 * @retval 无
 */
void MyOLED_Blit_BinNum_ASCII_8x16(uint8_t Left, uint8_t Top, uint16_t Number, uint8_t Length)
{
    uint8_t i;

    MyOLED_Blit_Char_ASCII_8x16(Left, Top, '0');
    MyOLED_Blit_Char_ASCII_8x16(Left + 8, Top, 'b');
    for (i = Length; i > 0; i--)                            
    {
        MyOLED_Blit_Char_ASCII_8x16(Left + 8 + 8*i, Top, Number % 2 + '0');
        Number /= 2;
    }
}

/**
 * @brief 将小数的图像数据覆盖在显存缓存指定区域
 * @param Left 0 ~ 127
 * @param Top 0 ~ 64
 * @param Number 要显示的小数
 * @param int_Length 整数部分的长度
 * @param float_Length 小数部分的长度
 * @retval 无
 */
void MyOLED_Blit_FloatNum_ASCII_8x16(uint8_t Left, uint8_t Top, float Number, uint8_t int_Length, uint8_t float_Length)
{
    int i, j = 1;
    for (i = 0; i < float_Length; i ++) j *= 10;
    MyOLED_Blit_Num_ASCII_8x16(Left, Top, Number, int_Length);
    MyOLED_Blit_Char_ASCII_8x16(Left + 8 * int_Length, Top, '.');
    MyOLED_Blit_Num_ASCII_8x16(Left + 8 * (int_Length + 1), Top, (uint32_t)(Number * j) % j, float_Length);
}

/**
 * @brief 绘制圆
 * @param centerX 圆心横坐标
 * @param centerY 圆心纵坐标
 * @param radius 半径
 * @param bd 边框粗细
 */
void MyOLED_DrawCircle(uint8_t centerX, uint8_t centerY, uint8_t radius, uint8_t bd)
{
    int x = radius;
    int y = 0;
    int err = 0;

    while (x >= y) {
        for (int i = 0; i < bd; i++) {
            MyOLED_Blit_Point(centerX + x - i, centerY + y, 1);
            MyOLED_Blit_Point(centerX + y - i, centerY + x, 1);
            MyOLED_Blit_Point(centerX - y - i, centerY + x, 1);
            MyOLED_Blit_Point(centerX - x - i, centerY + y, 1);
            MyOLED_Blit_Point(centerX - x - i, centerY - y, 1);
            MyOLED_Blit_Point(centerX - y - i, centerY - x, 1);
            MyOLED_Blit_Point(centerX + y - i, centerY - x, 1);
            MyOLED_Blit_Point(centerX + x - i, centerY - y, 1);
        }

        if (err <= 0) {
            y += 1;
            err += 2 * y + 1;
        }

        if (err > 0) {
            x -= 1;
            err -= 2 * x + 1;
        }
    }
}

/**
 * @brief 在指定位置上渲染fantas图标
 * @param Left 图标左端的横坐标像素位置，0 ~ 64
 * @retval 无
 */
void MyOLED_Blit_Icon_64x64(uint8_t Left)
{
    uint8_t j;
    for (j = 0; j < 8; j++)
    {
        MyOLED_Blit_GRAM(Icon_64x64[j], Left, j*8, 64);
    }
}

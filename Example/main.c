/**
 * @file main.c
 * @author DT9025A (dt9025a@163.com)
 * @brief Rotary Encoder Drv Lib Testbench
 * @version 0.1
 * @date 2024-12-12
 *
 * Encoder A: 1C1P, id = 1
 *  A --- P33, INT1, PULLED UP
 *  B --- P34, PULLED UP
 * Encoder B: 2C1P, id = 2
 *  A --- P32, INT0, PULLED UP
 *  B --- P35, PULLED UP
 *
 * UART: 115200bps @ 12MHz
 * Chip: STC8H8H64U
 */

#include "STC8H.h"
#include "encoder.h"
#include <STDIO.H>

Encoder ecA, ecB;

void UART_Init(void)
{
    // 115200bps@12.000MHz
    SCON = 0x50;  // 8λ����,�ɱ䲨����
    AUXR |= 0x40; // ��ʱ��ʱ��1Tģʽ
    AUXR &= 0xFE; // ����1ѡ��ʱ��1Ϊ�����ʷ�����
    TMOD &= 0x0F; // ���ö�ʱ��ģʽ
    TL1 = 0xE6;   // ���ö�ʱ��ʼֵ
    TH1 = 0xFF;   // ���ö�ʱ��ʼֵ
    ET1 = 0;      // ��ֹ��ʱ���ж�
    TR1 = 1;      // ��ʱ��1��ʼ��ʱ
}

void UART_Send(unsigned char dat)
{
    SBUF = dat;
    while (TI == 0);
    TI = 0;
}

void UART_SendString(const char *s)
{
    while (*s) {
        UART_Send(*s++);
    }
}

void INT1_Interrupt() interrupt 2
{
    Encoder_A_Dual_Edge_Interrupt(&ecA);
}

void INT0_Interrupt() interrupt 0
{
    Encoder_A_Dual_Edge_Interrupt(&ecB);
}

unsigned char ENCA_Read_A()
{
    return P33;
}

unsigned char ENCA_Read_B()
{
    return P34;
}

unsigned char ENCB_Read_A()
{
    return P32;
}

unsigned char ENCB_Read_B()
{
    return P35;
}

void ENC_Rotate_Callback(unsigned char id, unsigned char direction, unsigned char num)
{
    UART_SendString("Encoder ");
    UART_Send(id + '0');

    if (direction == ENCODER_DIR_CW) {
        UART_SendString(" CW ");
    } else {
        UART_SendString(" CCW ");
    }

    UART_Send(num + '0');
    UART_SendString("\r\n");
}

void main()
{
    P0M0 = 0x00;
    P0M1 = 0x00;
    P1M0 = 0x00;
    P1M1 = 0x00;
    P2M0 = 0x00;
    P2M1 = 0x00;
    P3M0 = 0x00;
    P3M1 = 0x00;
    P4M0 = 0x00;
    P4M1 = 0x00;
    P5M0 = 0x00;
    P5M1 = 0x00;

    UART_Init();

    P32 = 1;
    P33 = 1;
    P34 = 1;
    P35 = 1;

    IT0 = 0; // ʹ��INT1�����غ��½����ж�
    EX0 = 1; // ʹ��INT1�ж�
    IT1 = 0; // ʹ��INT1�����غ��½����ж�
    EX1 = 1; // ʹ��INT1�ж�
    EA  = 1;

    UART_SendString("START\r\n");
    Encoder_Init(&ecA, ENCODER_MODE_1C1P, 1, ENCA_Read_A, ENCA_Read_B, ENC_Rotate_Callback);
    Encoder_Init(&ecB, ENCODER_MODE_2C1P, 2, ENCB_Read_A, ENCB_Read_B, ENC_Rotate_Callback);

    while (1) {
        Encoder_Poll();
    }
}
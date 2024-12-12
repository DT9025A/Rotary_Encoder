/**
 * @file encoder.h
 * @author DT9025A (dt9025a@163.com)
 * @brief Rotary Encoder Drv Lib Header file
 * @version 1.1
 * @date 2024-12-10
 *
 *
 */
#ifndef _ENCODER_H_
#define _ENCODER_H_

#include <string.h>

/**
 * @brief 编码器模式
 *
 */
enum ENCODER_MODES {
    /**
     * @brief 编码器模式：n 定位 n 脉冲
     *
     */
    ENCODER_MODE_1C1P = 0,
    /**
     * @brief 编码器模式：2n 定位 n 脉冲
     *
     */
    ENCODER_MODE_2C1P = 1
};

/**
 * @brief 编码器事件
 *
 */
enum ENCODER_EVENT {
    /**
     * @brief 顺时针旋转一格
     *
     */
    ENCODER_DIR_CW = 0,
    /**
     * @brief 逆时针旋转一格
     *
     */
    ENCODER_DIR_CCW = 1
};

/**
 * @brief 编码器信号读取函数指针类型
 *
 */
typedef unsigned char (*Encoder_Signal_Read)(void);

/**
 * @brief 编码器事件回调函数指针类型
 *
 * @param id 编码器ID
 * @param direction 编码器方向，ENCODER_DIR_x
 * @param num 事件个数，最大事件暂存个数为 7
 */
typedef void (*Encoder_Rotate_Callback)(unsigned char id, unsigned char direction, unsigned char num);

/**
 * @brief 编码器对象
 *
 */
typedef struct Encoder_Struct {
    Encoder_Signal_Read a_signal_read;
    Encoder_Signal_Read b_signal_read;
    Encoder_Rotate_Callback user_callback;
    unsigned char id;
    void *next;
    unsigned char encoder_mode : 1;
    unsigned char cw_event : 3;
    unsigned char ccw_event : 3;
} Encoder;

/**
 * @brief 双边沿 A 信号中断函数
 *
 * @param instance Encoder 实例指针
 */
void Encoder_A_Dual_Edge_Interrupt(Encoder *instance);

/**
 * @brief 初始化编码器
 *
 * @param instance Encoder 实例指针
 * @param mode 编码器模式，ENCODER_MODE_*
 * @param id 编码器预设 id
 * @param a_read 编码器 A 信号读取函数指针
 * @param b_read 编码器 B 信号读取函数指针
 * @param callback 编码器事件回调函数
 */
void Encoder_Init(Encoder *instance, unsigned char mode, unsigned char id,
                  Encoder_Signal_Read a_read, Encoder_Signal_Read b_read,
                  Encoder_Rotate_Callback callback);

/**
 * @brief 编码器事件处理，请放置于主循环中，保证编码器事件能被及时处理
 *
 */
void Encoder_Poll(void);

#endif // _ENCODER_H_

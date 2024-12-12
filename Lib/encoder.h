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
 * @brief ������ģʽ
 *
 */
enum ENCODER_MODES {
    /**
     * @brief ������ģʽ��n ��λ n ����
     *
     */
    ENCODER_MODE_1C1P = 0,
    /**
     * @brief ������ģʽ��2n ��λ n ����
     *
     */
    ENCODER_MODE_2C1P = 1
};

/**
 * @brief �������¼�
 *
 */
enum ENCODER_EVENT {
    /**
     * @brief ˳ʱ����תһ��
     *
     */
    ENCODER_DIR_CW = 0,
    /**
     * @brief ��ʱ����תһ��
     *
     */
    ENCODER_DIR_CCW = 1
};

/**
 * @brief �������źŶ�ȡ����ָ������
 *
 */
typedef unsigned char (*Encoder_Signal_Read)(void);

/**
 * @brief �������¼��ص�����ָ������
 *
 * @param id ������ID
 * @param direction ����������ENCODER_DIR_x
 * @param num �¼�����������¼��ݴ����Ϊ 7
 */
typedef void (*Encoder_Rotate_Callback)(unsigned char id, unsigned char direction, unsigned char num);

/**
 * @brief ����������
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
 * @brief ˫���� A �ź��жϺ���
 *
 * @param instance Encoder ʵ��ָ��
 */
void Encoder_A_Dual_Edge_Interrupt(Encoder *instance);

/**
 * @brief ��ʼ��������
 *
 * @param instance Encoder ʵ��ָ��
 * @param mode ������ģʽ��ENCODER_MODE_*
 * @param id ������Ԥ�� id
 * @param a_read ������ A �źŶ�ȡ����ָ��
 * @param b_read ������ B �źŶ�ȡ����ָ��
 * @param callback �������¼��ص�����
 */
void Encoder_Init(Encoder *instance, unsigned char mode, unsigned char id,
                  Encoder_Signal_Read a_read, Encoder_Signal_Read b_read,
                  Encoder_Rotate_Callback callback);

/**
 * @brief �������¼����������������ѭ���У���֤�������¼��ܱ���ʱ����
 *
 */
void Encoder_Poll(void);

#endif // _ENCODER_H_
/**
 * @file encoder.c
 * @author DT9025A (dt9025a@163.com)
 * @brief Rotary Encoder Drv Lib file
 * @version 1.1
 * @date 2024-12-10
 *
 */

#include "encoder.h"

Encoder *encoder_list = NULL;

void Encoder_Init(Encoder *instance, unsigned char mode, unsigned char id,
                  Encoder_Signal_Read a_read, Encoder_Signal_Read b_read,
                  Encoder_Rotate_Callback callback)
{
    Encoder *tmp;

    instance->a_signal_read = a_read;
    instance->b_signal_read = b_read;
    instance->encoder_mode  = mode;
    instance->id            = id;
    instance->user_callback = callback;
    instance->next          = NULL;

    if (encoder_list == NULL) {
        encoder_list = instance;
    } else {
        tmp = encoder_list;
        while (tmp->next != NULL) {
            tmp = tmp->next;
        }
        tmp->next = instance;
    }
}

void Encoder_A_Dual_Edge_Interrupt(Encoder *instance)
{
    if (instance == NULL) {
        return;
    }

    if (instance->encoder_mode == ENCODER_MODE_2C1P) {
        // 2C1P 处理双边沿
        if (instance->a_signal_read() == instance->b_signal_read()) {
            // CCW
            instance->ccw_event += 1;
        } else {
            instance->cw_event += 1;
        }

    } else {
        // 1C1P 仅处理上升沿
        if (instance->a_signal_read()) {
            if (instance->b_signal_read()) {
                // CCW
                instance->ccw_event += 1;
            } else {
                instance->cw_event += 1;
            }
        }
    }
}

void Encoder_Event_Handler(Encoder *instance)
{
    unsigned char evnum;

    if (instance->cw_event) {
        evnum              = instance->cw_event;
        instance->cw_event = 0;
        instance->user_callback(instance->id, ENCODER_DIR_CW, evnum);
    } else if (instance->ccw_event) {
        evnum               = instance->ccw_event;
        instance->ccw_event = 0;
        instance->user_callback(instance->id, ENCODER_DIR_CCW, evnum);
    }
}

void Encoder_Poll(void)
{
    static Encoder *tmp;

    tmp = encoder_list;
    while (tmp != NULL) {
        Encoder_Event_Handler(tmp);
        tmp = tmp->next;
    }
}

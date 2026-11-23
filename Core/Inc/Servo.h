#ifndef __SERVO_H
#define __SERVO_H

#include "stm32f1xx_hal.h"

/* ------------------------------------------------------------
 * SG90（180° 型）舵机驱动
 *
 * 使用方法：
 *   1. CubeMX 里把 PA0 配成 TIM2_CH1，PWM Generation CH1，频率 50Hz
 *   2. 把 Servo.c 放进 Core/Src，Servo.h 放进 Core/Inc
 *   3. main() 里先 HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
 *   4. 然后就可以 Servo_SetAngle(90.0f);
 *
 * 注意：下面的 SERVO_MIN_US / SERVO_MAX_US 是"留了 2° 余量"的建议值，
 *       做完标定之后请改成你自己舵机的实测端点（见文档 9.5 节）。
 * ------------------------------------------------------------ */
#define SERVO_MIN_US    550     /* 对应 0° 左右 */
#define SERVO_MAX_US    2450    /* 对应 180° 左右 */

/* 把角度（0~180）换算成 CCR 值 */
uint16_t Servo_AngleToCCR(float angle);

/* 设置舵机角度，范围会被限制在 0~180 */
void Servo_SetAngle(float angle);

/* 在 OLED 上显示三行信息：目标角度 / CCR / 脉宽 */
void Servo_ShowOnOLED(float angle, uint16_t ccr);

#endif

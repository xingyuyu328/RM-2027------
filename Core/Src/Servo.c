/* ============================================================
 * Servo.c  —  SG90（180° 型）舵机驱动
 * 芯片：STM32F103C8T6      库：HAL
 * ============================================================ */
#include "Servo.h"
#include "OLED.h"

#define SERVO_MIN_US 550.0f
#define SERVO_MAX_US 2450.0f
/* CubeMX 生成的定时器句柄，名字要和 main.c 里的一致 */
extern TIM_HandleTypeDef htim2;

/**
 * @brief  角度 -> CCR
 * @param  angle  0~180
 * @retval 写进比较寄存器的值
 *
 * 前提：PSC 和 ARR 已经配成"1 个计数 = 1us"，
 *       所以 CCR 的数值就等于脉宽的微秒数。
 */
uint16_t Servo_AngleToCCR(float angle)
{
    if (angle < 0.0f) angle = 0.0f;
    if (angle > 180.f) angle = 180.f;

    /* --------------------------------------------------------
     * TODO 1
     * 现在用的是名义映射：0° = 500us，180° = 2500us。
     * 请把两端各留 2° 的余量，也就是改成 2° = 550us、178° = 2450us，
     * 让映射落在一个"不会顶到机械限位"的区间里。
     *
     * 提示：可以用 SERVO_MIN_US 和 SERVO_MAX_US 这两个宏，
     *       也可以直接把 500 和 2000 换成你的实测值。
     * -------------------------------------------------------- */
    return (uint16_t)(SERVO_MIN_US+(angle / 180.0f) * (SERVO_MAX_US - SERVO_MIN_US));
}

/**
 * @brief  设置舵机角度
 */
void Servo_SetAngle(float angle)
{
    uint16_t ccr = Servo_AngleToCCR(angle);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, ccr);
}

/**
 * @brief  在 OLED 上显示三行
 *
 * 目标格式（和文档 9.6 节一致，方便验收）：
 *   第 1 行   Target:090deg
 *   第 2 行   CCR   :1500
 *   第 3 行   Pulse :1.500ms
 */
void Servo_ShowOnOLED(float angle, uint16_t ccr)
{


    OLED_ShowString(1, 1,  "Target:");
    OLED_ShowNum(1, 9, (uint32_t)angle, 3);
    OLED_ShowString(1, 12, "deg");

    OLED_ShowString(2, 1,  "CCR   :");
    OLED_ShowNum(2, 9, ccr, 4);

    OLED_ShowString(3, 1,  "Pulse :");
    OLED_ShowNum(3, 9, ccr / 1000, 1);
    OLED_ShowString(3, 10, ".");
    OLED_ShowNum(3, 11, ccr % 1000, 3);
    OLED_ShowString(3, 14, "ms");

}
/* --------------------------------------------------------
     * TODO 2
     * 用 OLED_ShowString / OLED_ShowNum 把上面三行显示出来。
     *
     * 可用的函数（见 OLED.h）：
     *   OLED_ShowString(行, 列, "文字");     行 1~4，列 1~16
     *   OLED_ShowNum(行, 列, 数字, 位数);     数字超过位数会从低位截断
     *
     * 提示：
     *   - 第 3 行的 1.500ms，是把 ccr 拆成 ccr/1000 和 ccr%1000 分别显示，
     *     中间补一个 "."，这样就不需要处理浮点打印。
     *   - 每个数字都用固定位数，屏幕才不会残留上一位的字符。
     * -------------------------------------------------------- */
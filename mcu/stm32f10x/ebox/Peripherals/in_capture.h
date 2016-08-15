/**
  ******************************************************************************
  * @file    *.h
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief   
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form 
  * or means, without the prior written consent of shentq. This specification is 
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/


#ifndef __IN_CAPTURE_H
#define __IN_CAPTURE_H
#include "common.h"

#define TIM_ICPOLARITY_FALLING TIM_ICPolarity_Falling
#define TIM_ICPOLARITY_RISING  TIM_ICPolarity_Rising
/*
1.֧��TIM2��3��4��ch1,2,3,4.����12��ͨ��
2.֧�ֲ������ڡ�Ƶ�ʡ��߼��÷�֧�ֲ���ռ�ձ�
3.Ĭ�ϲ����½��أ���ͨ���������ò�������ģʽ
4.��ʱ�����������ֵΪ0xffff,Ϊ��ͻ��������ƣ�
    �ڱ������У�ʹ����update����жϵ���tx_overflow_times���Խ�������
    ��չ��2^32�������߲�����Χ������ʵ�����Ƶ�ʣ�1��Ƶ���������ڵ���120s���źš�
    ���ʹ��2��Ƶ���ɲ������ڵ���240s���źš��Դ����ơ�
5.���ڷ�Ƶϵ����������Ȳ����ļ����ϵ��Ҫ��ѭһ��ԭ���ڲ����������¾���ʹ�õͷ�Ƶϵ������TIMʱ�ӣ�ȥ������
6.����get_capture()�Ͳ���ʱ����ת���Ĺ�ϵ��
    ʱ��(us)=get_capture()/(72/prescaler);
    ʱ��(ms)=get_capture()/(72000/prescaler);
    ʱ��(s)=get_capture()/(72000000/prescaler);
    ���ֱ��ʹ��get_zone_time_us()�������ֱ�ӵõ�һ������õ�ֵ������ʡȥ�ֹ�����Ĺ��̡�
    �˴��ṩ�����ֻ�ȡ���ؿ��ȵķ�����һ���ǰ���ʱ����������һ���ǰ�ʱ�䵥λע��������


7.���ʹ��ĳ����ʱ��ͨ���������벶����ö�ʱ������ͨ�������������벶��ģʽ������������Ϊ��������ģʽ
�ص㣺
    �ڲ��õͷ�Ƶϵ����ʱ�򣬿��Ա�֤�������ȣ����ǻ�����ʱ�����Ƶ�ʣ���������cpu������
    �ڲ��ø߷�Ƶϵ����ʱ�򣬲������Ƚϵͣ����ǻή�Ͷ�ʱ�����Ƶ�ʣ���������cpu������
    stm32��72M��Ƶ�£���߿ɲ�160Khz���źš�����ٴ󣬽��޷�������
    

*/

class InCapture
{
public:
    InCapture(Gpio *capture_pin);
    void        begin();//ʹ��Ĭ�ϲ�������Ƶϵ��Ϊ1���������Ϊ120s
    void        begin(uint16_t prescaler = 1);
    void        set_count(uint16_t count);
    void        set_polarity_falling();
    void        set_polarity_rising();


    //��Ҫ�û����ж��д�������ϸ�����񣬴���״̬�������񣬱��������룬���������
    uint32_t    get_capture();//
    float       get_zone_time_us();//

    //���εĻ����Ĳ�������
    void        complex_event();//������Ҫ�����ռ�ձȵ��������������������ܵ���4us****
    void        simple_event();//���������������ִ��Ч�ʸߣ���߿ɲ�180K,���ǲ��ܲ���ռ�ձ�
    float       get_wave_frq();///<���ε�Ƶ��
    float       get_wave_peroid();///<���ε�����
    float       get_wave_high_duty();///<���εĸߵ�ƽռ�ձ�
    float       get_wave_low_duty();///<���εĵ͵�ƽռ�ձ�
    float       get_wave_high_time();///<���εĸߵ�ƽʱ��
    float       get_wave_low_time();///<���εĵ͵�ƽʱ��
    bool        available();///<���εĲ������
    
    //���ж�
    void        attch_ic_interrupt(void(*callback)(void));

    uint8_t     polarity;
private:
    Gpio        *capture_pin;
    TIM_TypeDef *TIMx;
    uint8_t     ch;
    uint16_t    period;//�������ֵ�����ڼ���ռ�ձ�
    uint16_t    prescaler;//�������ֵ�����ڼ���ռ�ձ�
    uint32_t    _capture;
    uint32_t    high_capture;
    uint32_t    low_capture;

    uint16_t    *overflow_times;
    uint32_t    last_value;
    bool        _available;

    void        init_info(Gpio *capture_pin);
    void        base_init(uint16_t Period, uint16_t Prescaler);

    uint16_t    (*_get_capture)(TIM_TypeDef *TIMx);
    void        (*_set_polarity)(TIM_TypeDef *TIMx, uint16_t TIM_OCPolarity); //����Ϊ�½��ػ��������ز���
};
#endif
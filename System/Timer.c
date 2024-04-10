#include "stm32f10x.h"                  // Device header  

extern uint16_t Timmer_NumCount;

void Timer_Init(void)
{
	//������ʱ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	//ѡ���ڲ�ʱ��ģʽ
	TIM_InternalClockConfig(TIM2);
	//ʱ����Ԫ ����Ƶ�� 72MHz /��ARR+1��/��PSC+1�� 72MHz/10000/7200 =1s
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitSturcture;
	TIM_TimeBaseInitSturcture.TIM_ClockDivision = TIM_CKD_DIV1; //1��Ƶ
	TIM_TimeBaseInitSturcture.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���
	TIM_TimeBaseInitSturcture.TIM_Period = 2000-1;//ARR��װ�Ĵ���
	TIM_TimeBaseInitSturcture.TIM_Prescaler = 7200-1;//PSCԤ��Ƶ�Ĵ���
	TIM_TimeBaseInitSturcture.TIM_RepetitionCounter = 0; //�ظ���������
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitSturcture);
	
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update); //������ʱ����Ԫ��ʼ��֮�������������¼��ж�λ
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//�ж��������,ֻ��ָ���¼����Դ����ж�
	
	//����NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2,ENABLE);  //ʹ�ܶ�ʱ��
}

//void TIM2_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)  //�鿴TIM2�ĸ����жϱ�־λ
//	{
//		Timmer_NumCount++;
//		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //�ֶ������־λ
//	}
//}




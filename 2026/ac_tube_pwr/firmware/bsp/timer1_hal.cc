#include "misc.h"
#include "platform.h"

// 48 MHz / 960 = 50 kHz. 1000 points, step 8 -> 400 Hz.
static constexpr uint16_t SPWM_ARR = 959;
static constexpr int SPWM_POINTS = 1000;
static constexpr int SPWM_STEP = 8;
static constexpr uint8_t SPWM_DEADTIME = 12;     // 12 / 48 MHz = 250 ns
static constexpr int SPWM_TABLE_PERCENT = 80;    // �������Ұ� 80% ��������

static int SPWM_MOD_PERCENT = 40;       // �������� 5%��ȷ�ϵ������ټӴ�

// ��ֵ��Ӧ 80% ���ơ�ʵ�� CCR �� SPWM_MOD_PERCENT ����ƫ�
static const uint16_t spwm_ccr[SPWM_POINTS] = {
  //
  479, 477, 474, 472, 469, 467, 465, 462, 460, 457, 455, 453, 450, 448, 445, 443, 441,
  438, 436, 433, 431, 429, 426, 424, 421, 419, 417, 414, 412, 410, 407, 405, 403, 400,
  398, 395, 393, 391, 388, 386, 384, 381, 379, 377, 374, 372, 370, 368, 365, 363, 361,
  358, 356, 354, 352, 349, 347, 345, 342, 340, 338, 336, 334, 331, 329, 327, 325, 322,
  320, 318, 316, 314, 312, 309, 307, 305, 303, 301, 299, 297, 294, 292, 290, 288, 286,
  284, 282, 280, 278, 276, 274, 272, 270, 268, 266, 264, 262, 260, 258, 256, 254, 252,
  250, 248, 246, 244, 242, 240, 239, 237, 235, 233, 231, 229, 228, 226, 224, 222, 220,
  219, 217, 215, 213, 212, 210, 208, 206, 205, 203, 201, 200, 198, 197, 195, 193, 192,
  190, 189, 187, 185, 184, 182, 181, 179, 178, 176, 175, 173, 172, 171, 169, 168, 166,
  165, 164, 162, 161, 160, 158, 157, 156, 154, 153, 152, 151, 149, 148, 147, 146, 145,
  143, 142, 141, 140, 139, 138, 137, 136, 135, 133, 132, 131, 130, 129, 128, 127, 127,
  126, 125, 124, 123, 122, 121, 120, 119, 119, 118, 117, 116, 115, 115, 114, 113, 113,
  112, 111, 111, 110, 109, 109, 108, 107, 107, 106, 106, 105, 105, 104, 104, 103, 103,
  102, 102, 101, 101, 101, 100, 100, 100, 99, 99, 99, 98, 98, 98, 98, 97, 97, 97, 97,
  97, 97, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 97, 97,
  97, 97, 97, 97, 98, 98, 98, 98, 99, 99, 99, 100, 100, 100, 101, 101, 101, 102, 102,
  103, 103, 104, 104, 105, 105, 106, 106, 107, 107, 108, 109, 109, 110, 111, 111, 112,
  113, 113, 114, 115, 115, 116, 117, 118, 119, 119, 120, 121, 122, 123, 124, 125, 126,
  127, 127, 128, 129, 130, 131, 132, 133, 135, 136, 137, 138, 139, 140, 141, 142, 143,
  145, 146, 147, 148, 149, 151, 152, 153, 154, 156, 157, 158, 160, 161, 162, 164, 165,
  166, 168, 169, 171, 172, 173, 175, 176, 178, 179, 181, 182, 184, 185, 187, 189, 190,
  192, 193, 195, 197, 198, 200, 201, 203, 205, 206, 208, 210, 212, 213, 215, 217, 219,
  220, 222, 224, 226, 228, 229, 231, 233, 235, 237, 239, 240, 242, 244, 246, 248, 250,
  252, 254, 256, 258, 260, 262, 264, 266, 268, 270, 272, 274, 276, 278, 280, 282, 284,
  286, 288, 290, 292, 294, 297, 299, 301, 303, 305, 307, 309, 312, 314, 316, 318, 320,
  322, 325, 327, 329, 331, 334, 336, 338, 340, 342, 345, 347, 349, 352, 354, 356, 358,
  361, 363, 365, 368, 370, 372, 374, 377, 379, 381, 384, 386, 388, 391, 393, 395, 398,
  400, 403, 405, 407, 410, 412, 414, 417, 419, 421, 424, 426, 429, 431, 433, 436, 438,
  441, 443, 445, 448, 450, 453, 455, 457, 460, 462, 465, 467, 469, 472, 474, 477, 479,
  481, 484, 486, 489, 491, 493, 496, 498, 501, 503, 505, 508, 510, 513, 515, 517, 520,
  522, 525, 527, 529, 532, 534, 537, 539, 541, 544, 546, 548, 551, 553, 555, 558, 560,
  563, 565, 567, 570, 572, 574, 577, 579, 581, 584, 586, 588, 590, 593, 595, 597, 600,
  602, 604, 606, 609, 611, 613, 616, 618, 620, 622, 624, 627, 629, 631, 633, 636, 638,
  640, 642, 644, 646, 649, 651, 653, 655, 657, 659, 661, 664, 666, 668, 670, 672, 674,
  676, 678, 680, 682, 684, 686, 688, 690, 692, 694, 696, 698, 700, 702, 704, 706, 708,
  710, 712, 714, 716, 718, 719, 721, 723, 725, 727, 729, 730, 732, 734, 736, 738, 739,
  741, 743, 745, 746, 748, 750, 752, 753, 755, 757, 758, 760, 761, 763, 765, 766, 768,
  769, 771, 773, 774, 776, 777, 779, 780, 782, 783, 785, 786, 787, 789, 790, 792, 793,
  794, 796, 797, 798, 800, 801, 802, 804, 805, 806, 807, 809, 810, 811, 812, 813, 815,
  816, 817, 818, 819, 820, 821, 822, 823, 825, 826, 827, 828, 829, 830, 831, 831, 832,
  833, 834, 835, 836, 837, 838, 839, 839, 840, 841, 842, 843, 843, 844, 845, 845, 846,
  847, 847, 848, 849, 849, 850, 851, 851, 852, 852, 853, 853, 854, 854, 855, 855, 856,
  856, 857, 857, 857, 858, 858, 858, 859, 859, 859, 860, 860, 860, 860, 861, 861, 861,
  861, 861, 861, 862, 862, 862, 862, 862, 862, 862, 862, 862, 862, 862, 862, 862, 862,
  862, 862, 862, 861, 861, 861, 861, 861, 861, 860, 860, 860, 860, 859, 859, 859, 858,
  858, 858, 857, 857, 857, 856, 856, 855, 855, 854, 854, 853, 853, 852, 852, 851, 851,
  850, 849, 849, 848, 847, 847, 846, 845, 845, 844, 843, 843, 842, 841, 840, 839, 839,
  838, 837, 836, 835, 834, 833, 832, 831, 831, 830, 829, 828, 827, 826, 825, 823, 822,
  821, 820, 819, 818, 817, 816, 815, 813, 812, 811, 810, 809, 807, 806, 805, 804, 802,
  801, 800, 798, 797, 796, 794, 793, 792, 790, 789, 787, 786, 785, 783, 782, 780, 779,
  777, 776, 774, 773, 771, 769, 768, 766, 765, 763, 761, 760, 758, 757, 755, 753, 752,
  750, 748, 746, 745, 743, 741, 739, 738, 736, 734, 732, 730, 729, 727, 725, 723, 721,
  719, 718, 716, 714, 712, 710, 708, 706, 704, 702, 700, 698, 696, 694, 692, 690, 688,
  686, 684, 682, 680, 678, 676, 674, 672, 670, 668, 666, 664, 661, 659, 657, 655, 653,
  651, 649, 646, 644, 642, 640, 638, 636, 633, 631, 629, 627, 624, 622, 620, 618, 616,
  613, 611, 609, 606, 604, 602, 600, 597, 595, 593, 590, 588, 586, 584, 581, 579, 577,
  574, 572, 570, 567, 565, 563, 560, 558, 555, 553, 551, 548, 546, 544, 541, 539, 537,
  534, 532, 529, 527, 525, 522, 520, 517, 515, 513, 510, 508, 505, 503, 501, 498, 496,
  493, 491, 489, 486, 484, 481, };

static TIM_HandleTypeDef htim1;
static uint16_t spwm_i;

static uint16_t spwm_ccr1_at(int i)
{
  int center = SPWM_ARR / 2;
  int delta = (int)spwm_ccr[i] - center;
  delta = delta * SPWM_MOD_PERCENT / SPWM_TABLE_PERCENT;
  return (uint16_t)(center + delta);
}

void TIMER1_Zero(void)
{
  SPWM_MOD_PERCENT = 0;
}

void TIMER1_AdjustMod(void)
{
  const int mod[] = {40, 50, 60, 70, 80, 90};
  static int index = 0;
  index++;
  index %= sizeof(mod) / sizeof(mod[0]);
  SPWM_MOD_PERCENT = mod[index];
}

extern "C" void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
  if(__HAL_TIM_GET_FLAG(&htim1, TIM_FLAG_UPDATE) == RESET)
    return;
  __HAL_TIM_CLEAR_IT(&htim1, TIM_IT_UPDATE);

  uint16_t ccr1 = spwm_ccr1_at(spwm_i);
  TIM1->CCR1 = ccr1;
  TIM1->CCR2 = SPWM_ARR - ccr1;
  spwm_i += SPWM_STEP;
  if(spwm_i >= SPWM_POINTS)
    spwm_i = 0;
}

static void spwm_prime(void)
{
  uint16_t ccr1 = spwm_ccr1_at(0);
  TIM1->CCR1 = ccr1;
  TIM1->CCR2 = SPWM_ARR - ccr1;
  TIM1->EGR = TIM_EGR_UG;
  __HAL_TIM_CLEAR_FLAG(&htim1, TIM_FLAG_UPDATE);
  spwm_i = SPWM_STEP;
}

void TIMER1_Config(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_TIM1_CLK_ENABLE();

  GPIO_InitTypeDef gis = {0};
  gis.Mode = GPIO_MODE_AF_PP;
  gis.Pull = GPIO_PULLDOWN;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
  gis.Alternate = GPIO_AF2_TIM1;
  gis.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_7;    // LO1 CH1, LO2 CH2, HO1 CH1N
  HAL_GPIO_Init(GPIOA, &gis);
  gis.Pin = GPIO_PIN_0;    // HO2 CH2N
  HAL_GPIO_Init(GPIOB, &gis);

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.Period = SPWM_ARR;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if(HAL_TIM_Base_Init(&htim1) != HAL_OK)
    Error_Handler();

  TIM_OC_InitTypeDef tois = {0};
  tois.OCMode = TIM_OCMODE_PWM1;
  tois.OCPolarity = TIM_OCPOLARITY_HIGH;
  tois.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  tois.OCFastMode = TIM_OCFAST_DISABLE;
  tois.OCIdleState = TIM_OCIDLESTATE_RESET;
  tois.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  tois.Pulse = spwm_ccr1_at(0);
  if(HAL_TIM_PWM_ConfigChannel(&htim1, &tois, TIM_CHANNEL_1) != HAL_OK)
    Error_Handler();
  if(HAL_TIM_PWM_ConfigChannel(&htim1, &tois, TIM_CHANNEL_2) != HAL_OK)
    Error_Handler();

  TIM_BreakDeadTimeConfigTypeDef bdt = {0};
  bdt.OffStateRunMode = TIM_OSSR_DISABLE;
  bdt.OffStateIDLEMode = TIM_OSSI_DISABLE;
  bdt.LockLevel = TIM_LOCKLEVEL_OFF;
  bdt.DeadTime = SPWM_DEADTIME;
  bdt.BreakState = TIM_BREAK_DISABLE;
  bdt.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  bdt.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if(HAL_TIMEx_ConfigBreakDeadTime(&htim1, &bdt) != HAL_OK)
    Error_Handler();

  spwm_prime();

  HAL_NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ (TIM1_BRK_UP_TRG_COM_IRQn);
  __HAL_TIM_ENABLE_IT(&htim1, TIM_IT_UPDATE);

  if(HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1) != HAL_OK)
    Error_Handler();
  if(HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1) != HAL_OK)
    Error_Handler();
  if(HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2) != HAL_OK)
    Error_Handler();
  if(HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2) != HAL_OK)
    Error_Handler();

  print_log();
}

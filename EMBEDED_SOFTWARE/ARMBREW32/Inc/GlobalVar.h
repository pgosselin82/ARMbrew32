#ifndef GlobalVars_H
#define GlobalVars_H 100

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif



extern uint32_t G_mSCounter;
extern uint32_t G_SecondCounter;

extern uint32_t G_PWMCounter;
extern uint32_t G_PWMfreqCounter;

extern uint16_t G_pwm_output_status;
extern uint16_t G_pwm_output;

extern uint16_t G_pwm_pct_value;

extern float G_temperature;

#define PWM_PERIOD 2000


/* C++ detection */
#ifdef __cplusplus
}
#endif


#endif

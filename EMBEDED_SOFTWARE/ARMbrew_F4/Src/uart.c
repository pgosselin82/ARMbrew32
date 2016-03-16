
#include "uart.h"
#include <stdint.h>
#include "stm32f4xx_hal.h"

/*HANDLE hSerial;
DCB    dcb;
char   *words, *buffRead, *buffWrite;
DWORD  dwBytesWritten, dwBytesRead;*/

UART_HandleTypeDef huart5;

int uart_init(void) {
  return UART_SUCCESS;
}

void uart_finit(void) {
  //if(hSerial == INVALID_HANDLE_VALUE) return;

}

void uart_setb(uint32_t baud) {
  //if(hSerial == INVALID_HANDLE_VALUE) return;

  HAL_UART_DeInit(&huart5);

  huart5.Instance = UART5;
  huart5.Init.BaudRate = baud;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart5);



 /* dcb.DCBlength       = sizeof(dcb);
  dcb.BaudRate        = dcb_buad;
  dcb.ByteSize        = 8;
  dcb.StopBits        = ONESTOPBIT;
  dcb.Parity          = NOPARITY;
  dcb.fBinary         = TRUE;
  dcb.fDtrControl     = DTR_CONTROL_DISABLE;
  dcb.fRtsControl     = RTS_CONTROL_DISABLE;
  dcb.fOutxCtsFlow    = FALSE;
  dcb.fOutxDsrFlow    = FALSE;
  dcb.fDsrSensitivity = FALSE;
  dcb.fAbortOnError   = TRUE;

  SetCommState(hSerial, &dcb);*/
}

void uart_putc(uint8_t c){

	HAL_UART_Transmit(&huart5,&c,1,1000);

}

uint8_t uart_getc(void){
	uint8_t c;
	HAL_UART_Receive(&huart5,&c,1,1000);
	return c;
}

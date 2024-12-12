/*
 * command_parser_fsm.c
 *
 *  Created on: Dec 11, 2024
 *      Author: kanza
 */


#include "command_parser_fsm.h"
uint32_t command_flag = 0;

void command_parser_fsm()
{

				if(strncmp((char *)buffer, "!RST#", 5) == 0)
				{
					memset(buffer, 0, MAX_BUFFER_SIZE);
					command_flag = 1;
					index_buffer = 0;
				}
				else if(strncmp((char *)buffer, "!OK#", 4) == 0)
				{
					memset(buffer, 0, MAX_BUFFER_SIZE);
					command_flag = 2;
					index_buffer = 0;
				}

}




uint32_t last_time = 0;
uint8_t waiting = 0;
char str[50];

void uart_communication_fsm()
{
	switch(command_flag)
	{

		case 1:

			last_time = HAL_GetTick();
			HAL_UART_Transmit(&huart2, (uint8_t *)str,sprintf(str, "\r\nADC0 = %lu \r\n", ADC_value), 1000);
			command_flag = 3;
			waiting = 1;
			break;

		case 2:

			HAL_UART_Transmit(&huart2, (uint8_t *)"\r\nEND COMMUNICATION", 20, 1000);
			command_flag = 0;
			waiting = 0;
			break;

		case 3:

				if(waiting && (HAL_GetTick() - last_time >= three_Sec))
				{
					last_time = HAL_GetTick();

					HAL_UART_Transmit(&huart2, (uint8_t *)str,sprintf(str, "\r\nADC0 = %lu \r\n", ADC_value), 1000);
				}
				break;

		default:
			waiting = 0;
			break;
	}

}

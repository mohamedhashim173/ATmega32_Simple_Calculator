/*
 * main.c
 *
 *  Created on: Oct 7, 2022
 *      Author: MSAEED99
 */


// AVR libraries
#include <util/delay.h>

// Libraries
#include "STD_TYPES.h"
#include "BIT_MATH.h"

// MCAL
#include "DIO_interface.h"

// HAL
#include "LCD_interface.h"
#include "KEYPAD_interface.h"


// Prototypes
void operand_value(void);
void clear_memories(void);



// Global Variables
u8 local_u8PressedKey;

s8 input[10] = {0};

s32 operand1;
s32 operand2;
s32 operand_temp;
u8 operation;
s32 answer;

u8 operand1_flag = 0;
u8 operand2_flag = 0;
u8 operation_flag = 0;
u8 answer_flag = 0;

u8 _index = 0;
u8 num_of_digits = 0;
u32 digit_counter;



// Calculator app
void main(void)
{
	// Calculator Initialization (LCD and Keypad)
	LCD_voidInit();		// Clear LCD
	KEYPAD_voidPinInit();

	// LCD_voidWriteNum(x);

	while(1)
	{
		local_u8PressedKey = KEYPAD_u8GetPressedKey();

		// Print numbers on LCD
		if(local_u8PressedKey >= '0'  && local_u8PressedKey <= '9')
		{
			// Print on LCD
			LCD_voidSendCharacter(local_u8PressedKey);

			// Store in input array
			input[_index] = local_u8PressedKey - '0';
			_index++;
			num_of_digits++;
		}


		// Operation
		if('+' == local_u8PressedKey || '-' == local_u8PressedKey || 'x' == local_u8PressedKey || 253 == local_u8PressedKey)
		{
			operation = local_u8PressedKey;
			operation_flag = 1;

			// Send operation
			LCD_voidSendCharacter(local_u8PressedKey);

			// Assign to operand1
			if(operand1_flag == 0 && operand2_flag == 0)
			{
				operand_value();
				operand1 = operand_temp;
				operand1_flag = 1;

				// Reset input array
				for(s8 i = 0; i < 10; i++)
				{
					input[i] = 0;
				}
				// Reset number of digits
				num_of_digits = 0;
				_index = 0;
			}
		}

		// Print answer
		if('=' == local_u8PressedKey)
		{
			operand_value();
			operand2 = operand_temp;

			// Calculate function
			switch(operation)
			{
			case '+': answer = operand1 + operand2; break;
			case '-': answer = operand1 - operand2; break;
			case 'x': answer = operand1 * operand2; break;
			case 253: answer = operand1 / operand2; break;
			}

			LCD_voidPositionSet(1, 0);
			LCD_voidSendCharacter('=');
			LCD_voidWriteNum(answer);

			clear_memories();
		}

		// Clear LCD
		if('c' == local_u8PressedKey)
		{
			// Clear memories
			clear_memories();

			// Clear LCD
			LCD_voidInit();
		}
	}
}



void operand_value(void)
{
	// Save input to temporary operand
	digit_counter = 1;
	operand_temp = 0;
	for(s8 i = num_of_digits - 1; i >= 0; i--)
	{
		operand_temp += input[i] * digit_counter;
		digit_counter *= 10;
	}

}



void clear_memories(void)
{
	operand1_flag = 0;
	operand2_flag = 0;
	operation_flag = 0;
	answer_flag = 0;
	_index = 0;
	num_of_digits = 0;

	for(s8 i = 0; i < 40; i++)
	{
		input[i] = 0;
	}
}


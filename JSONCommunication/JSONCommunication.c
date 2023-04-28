#include "JSONCommunication.h"
#include "main.h"

//extern void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

char sizeOfNextJSON[4]; //количество байт, которые будут получены в следующем сообщении, записанные в виде массива символов

//Функция отправки строки JSON COM-порту
//вход: указатель на строку
void JSONTransmit(char *JSONstr) {
	uint16_t length = strlen(JSONstr);
	uint16_t size = sizeof(char) * length; // Размер данных
	HAL_UART_Transmit(&huart2, (uint8_t*) JSONstr, size, HAL_MAX_DELAY); // Передача данных через USART или UART модуль huart1 с максимальным временем ожидания
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) { //функция, вызывающаяся каждый раз после приема данных (после заполнения буфера приема данных)
	static int intSizeOfNextJSON; //количество байт, которые будут переданы в JSON сообщении (длина строки JSON)
	static char* JSONString; //указатель на строку JSON
	static int readyGetJSON = 0; // флаг - следующее сообщение - это JSON, размер сообщения получили

	//обработка получения JSON
	if (readyGetJSON) {
		readyGetJSON=0;
		HAL_UART_AbortReceive(&huart2);	 //очистка буфера
		HAL_UART_Receive_IT(&huart2, sizeOfNextJSON, 4); //инициализируем прием следующего сообщения
		json_value* result = json_parse(JSONString, intSizeOfNextJSON); //преобразуем полученную строку в JSON

		//пример получения значений из JSON
		json_object_entry key1 = result->u.object.values[0];
		json_object_entry key2 = result->u.object.values[1];
		json_object_entry key3 = result->u.object.values[2];
		char* string = result->u.object.values[0].value->u.string.ptr;
		int temp = 5;

		//todo: будет вызов задаче для обработки полученного сообщения
	}
	//обработка получения размера следующего JSON
	else {
		intSizeOfNextJSON = atoi(sizeOfNextJSON);		//Преобразуем размер следующего сообщения в число
		JSONString = (char*) malloc(sizeof(char)*intSizeOfNextJSON); // выделение памяти для строки JSON
		HAL_UART_AbortReceive(&huart2);	  				//очистка буфера
		HAL_UART_Receive_IT(&huart2, JSONString, intSizeOfNextJSON); //инициализируем прием следующего сообщения todo данные не приходят в JSONString

		readyGetJSON =1;
	}
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}

void JSONCommunication_init() {
	HAL_UART_Receive_IT(&huart2, sizeOfNextJSON, 4); // Приготовиться принять информацию о размере следующего JSON
}

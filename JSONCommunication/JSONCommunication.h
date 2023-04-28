//обяъвление функций, использующихся для общения при помощи JSON по COM порту
#ifndef JSON_COMMUNICATION
#define JSON_COMMUNICATION

#include "json.h" //библиотека для работы с JSON
void JSONCommunication_init();

void JSONTransmit(char *JSONstr);

#endif

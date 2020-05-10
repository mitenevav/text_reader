#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED

#include <windows.h>


/*
параметры окна
*/
typedef struct WindParam_t
{
    int cxChar;         //ширина символа
    int cyChar;         //высота символа
    int cxClient;       //ширина окна
    int cyClient;       //высота окна
} WindParam_t;

/*
режим
*/
typedef enum Mode_t
{
    NOWRAP,         //верстка
    WRAP            //без верстки
} Mode_t;

/*
структура храниеня
*/
typedef struct Data_t
{
    char* str;              //строка с данными
    long size;              //размер строки
} Data_t;

/*
структура представления
*/
typedef struct WorkWind_t
{
    char* str;              //строка с данными
    int* lengthLine;        //смещения строк
    long numOfLines;        //количество строк
    int wrapWidth;          //ширина рабочей области(только в режме WRAP)
    long iFirstLine;        //номер ведущей строки
    long firstCh;           //смещение ведущего элемента
    Mode_t wrap;            //режим
} WorkWind_t;


/*
Функция заполняет поля дефолтными значениями и создает модель без верстки
Arguments:      (in)data - стркутура хранения
                (out)workWind - структура представления
OUT:код ошибки
*/
int initModelOfData(Data_t* data, WorkWind_t* workWind);

/*
Функция создает модель без верстки
Arguments:      (in)data - стркутура хранения
                (out)workWind - структура представления
OUT:код ошибки
*/
int makeModelOfData(Data_t* data, WorkWind_t* workWind);

/*
Функция создает модель с версткой
Arguments:      (in)data - стркутура хранения
                (out)workWind - структура представления
                (in)window - параметры окна
OUT:код ошибки
*/
int makeWrap(Data_t* data, WorkWind_t* workWind, WindParam_t* window);

/*
Функция очищает память структуры хранения
Arguments:      (in/out)data - стркутура хранения
*/
void FreeData(Data_t* data);

/*
Функция очищает память структуры представления
Arguments:      (in/out)workWind - структура представления
*/
void FreeWorkModel(WorkWind_t* workWind);

/*
Функция вычисляет максимальную длину строки
Arguments:      (in)workWind - структура представления
OUT:            максимальная длина строки
*/
int maxLineLength(WorkWind_t* workWind);

/*
Функция чтения данных из файла
IN:             (in)file - имя файла
                (out)data - структура хранения
OUT:код ошибки
*/
int processFile(char* file, Data_t* data);

/*
Функция поиска ведущей строки
Arguments:      (in)workWind - структура представления
OUT:номер ведущей строки
*/
long FindFirstLine(WorkWind_t* workWind);

#endif // DATA_H_INCLUDED

#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED

#include <windows.h>


/*
��������� ����
*/
typedef struct WindParam_t
{
    int cxChar;         //������ �������
    int cyChar;         //������ �������
    int cxClient;       //������ ����
    int cyClient;       //������ ����
} WindParam_t;

/*
�����
*/
typedef enum Mode_t
{
    NOWRAP,         //�������
    WRAP            //��� �������
} Mode_t;

/*
��������� ��������
*/
typedef struct Data_t
{
    char* str;              //������ � �������
    long size;              //������ ������
} Data_t;

/*
��������� �������������
*/
typedef struct WorkWind_t
{
    char* str;              //������ � �������
    int* lengthLine;        //�������� �����
    long numOfLines;        //���������� �����
    int wrapWidth;          //������ ������� �������(������ � ����� WRAP)
    long iFirstLine;        //����� ������� ������
    long firstCh;           //�������� �������� ��������
    Mode_t wrap;            //�����
} WorkWind_t;


/*
������� ��������� ���� ���������� ���������� � ������� ������ ��� �������
Arguments:      (in)data - ��������� ��������
                (out)workWind - ��������� �������������
OUT:��� ������
*/
int initModelOfData(Data_t* data, WorkWind_t* workWind);

/*
������� ������� ������ ��� �������
Arguments:      (in)data - ��������� ��������
                (out)workWind - ��������� �������������
OUT:��� ������
*/
int makeModelOfData(Data_t* data, WorkWind_t* workWind);

/*
������� ������� ������ � ��������
Arguments:      (in)data - ��������� ��������
                (out)workWind - ��������� �������������
                (in)window - ��������� ����
OUT:��� ������
*/
int makeWrap(Data_t* data, WorkWind_t* workWind, WindParam_t* window);

/*
������� ������� ������ ��������� ��������
Arguments:      (in/out)data - ��������� ��������
*/
void FreeData(Data_t* data);

/*
������� ������� ������ ��������� �������������
Arguments:      (in/out)workWind - ��������� �������������
*/
void FreeWorkModel(WorkWind_t* workWind);

/*
������� ��������� ������������ ����� ������
Arguments:      (in)workWind - ��������� �������������
OUT:            ������������ ����� ������
*/
int maxLineLength(WorkWind_t* workWind);

/*
������� ������ ������ �� �����
IN:             (in)file - ��� �����
                (out)data - ��������� ��������
OUT:��� ������
*/
int processFile(char* file, Data_t* data);

/*
������� ������ ������� ������
Arguments:      (in)workWind - ��������� �������������
OUT:����� ������� ������
*/
long FindFirstLine(WorkWind_t* workWind);

#endif // DATA_H_INCLUDED

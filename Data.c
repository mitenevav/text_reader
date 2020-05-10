#include "Data.h"
#include <stdlib.h>
#include <stdio.h>
#include "Errors.h"

int makeModelOfData(Data_t* data, WorkWind_t* workWind)
{
    workWind->str = data->str;
    workWind->wrap = NOWRAP;
    workWind->numOfLines = 0;

    long i, numOfStr;

    numOfStr=0;

    for(i=0; i<data->size; i++)
    {
        if(workWind->str[i]=='\n' || workWind->str[i]=='\0')
        {
            numOfStr++;
        }
    }

    if(workWind->lengthLine!=NULL)
        free(workWind->lengthLine);
    workWind->lengthLine=(int*)malloc((numOfStr+2) * sizeof(int));
    if(workWind->lengthLine==NULL)
        return MEMORY_SHORTAGE_e;

    numOfStr=0;

    workWind->lengthLine[0] = 0;
    numOfStr++;


    for(i=0; i<data->size; i++)
    {
        if(workWind->str[i]=='\n' || workWind->str[i]=='\0')
        {
            workWind->lengthLine[numOfStr]=i+1;

            numOfStr++;
        }
    }
    workWind->lengthLine[numOfStr]=i;

    workWind->numOfLines = numOfStr;

    workWind->iFirstLine=FindFirstLine(workWind);

    return 0;
}

int makeWrap(Data_t* data, WorkWind_t* workWind, WindParam_t* window)
{
    workWind->str = data->str;
    workWind->wrap = WRAP;
    workWind->numOfLines = 0;
    workWind->wrapWidth = window->cxClient / window->cxChar - 1;

    long int i, numOfCh, numOfStr;

    numOfCh=0;
    numOfStr=0;

    for(i=0; i<data->size; i++, numOfCh++)
    {
        if(workWind->str[i]=='\n' || workWind->str[i]=='\0' || numOfCh==workWind->wrapWidth)
        {
            numOfStr++;
            numOfCh=-1;
        }
    }

    if(workWind->lengthLine!=NULL)
        free(workWind->lengthLine);
    workWind->lengthLine=(int*)malloc((numOfStr + 2) * sizeof(int));
    if(workWind->lengthLine==NULL)
        return MEMORY_SHORTAGE_e;

    numOfCh=0;
    numOfStr=0;

    workWind->lengthLine[0] = 0;
    numOfStr++;


    for(i=0; i<data->size; i++, numOfCh++)
    {
        if(workWind->str[i]=='\n' || workWind->str[i]=='\0' || numOfCh==workWind->wrapWidth)
        {
            workWind->lengthLine[numOfStr]=i + 1;

            numOfCh=-1;
            numOfStr++;
        }
    }

    workWind->lengthLine[numOfStr]=i + 1;

    workWind->numOfLines = numOfStr;

    workWind->iFirstLine=FindFirstLine(workWind);



    return 0;

}

void FreeData(Data_t* data)
{
    free(data->str);
    data->str=NULL;
}

void FreeWorkModel(WorkWind_t* workWind)
{
    workWind->str=NULL;
    free(workWind->lengthLine);
    workWind->lengthLine=NULL;
}

int maxLineLength(WorkWind_t* workWind)
{
    int i;
    int maxLength = 0;
    for(i=0; i<workWind->numOfLines; i++)
    {
        maxLength=max(maxLength, workWind->lengthLine[i+1]-workWind->lengthLine[i]);
    }
    return maxLength;
}

int processFile(char* file, Data_t* data)
{
    long size;
    FILE*fp=fopen(file,"rb");
    if(fp!=NULL)
    {
        fseek(fp,0,SEEK_END);
        data->size = ftell(fp);
        fseek(fp,0,SEEK_SET);
        data->str=(char*)calloc(data->size,sizeof(char)+1);
        if(data->str!=NULL)
        {
            size=fread(data->str,sizeof(char),data->size,fp);
            if(size!=data->size)
            {
                free(data->str);
                return NO_READ_FILE_e;
            }

        }
        else
        {
            return MEMORY_SHORTAGE_e;
        }
        fclose(fp);
    }
    /*
    else
    {
        return MEMORY_SHORTAGE_e;
    }*/
    return 0;
}

long FindFirstLine(WorkWind_t* workWind)
{
    long i;
    for(i=0; workWind->lengthLine[i] <= workWind->firstCh; i++);
    i--;
    return i;
}

int initModelOfData(Data_t* data, WorkWind_t* workWind)
{
    int err;
    workWind->str = NULL;
    workWind->lengthLine = NULL;
    workWind->wrapWidth = NOWRAP;
    workWind->iFirstLine=0;
    workWind->firstCh=0;

    err=makeModelOfData(data, workWind);
    if(err)
    {
        return err;
    }
    return 0;
}

int processNewFile(HWND hwnd, WorkWind_t* workWind, Data_t* data)
{


    return 0;
}

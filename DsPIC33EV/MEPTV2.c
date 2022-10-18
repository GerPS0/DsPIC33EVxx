#include <stdint.h>
#include "Eta_val_uint16.c"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float lineal_interpolation(float *arrY, uint16_t *arrX, float x0);
float FuncionObjetivo(float w, float V, float I, float Eta);
uint16_t MinimumSize(float Uin);
uint16_t SizeOfColumn(float Vin);
float TableValue(uint16_t TableNumber,uint16_t offset_table, uint16_t row, uint16_t offset_row, uint16_t column, uint16_t offset_column);

uint16_t *Tablas[5] = {(uint16_t*)Eta1,(uint16_t*)Eta2,(uint16_t*)Eta3,(uint16_t*)Eta4,(uint16_t*)Eta5};
float EtaV_Lv[2] = {0.0,0.0}, EtaU[2] = {0.0,0.0};
uint16_t arrayX[2] = {0,0};
uint16_t SizeOfTable = 86, NEntradas = 3, PosF = 0;
uint16_t Minimo, TablaMinima, NTabla, Columna = 0;

float lineal_interpolation(float *arrY, uint16_t *arrX, float x0)
{
    float Ys = 0;
	if (x0 == *(arrX+0) || *(arrX+0) == *(arrX+1))    //when x0 is equal to the first value of the arrX (Vertice) is not necessary the interpolation)
		Ys = *(arrY+0);
	else{
		Ys = ((*(arrY + 1)-*(arrY + 0))/(*(arrX + 1)-*(arrX + 0))) * (x0 - *(arrX + 0)) + *(arrY + 0); 
	}return Ys;
}
float FuncionObjetivo(float w, float V, float I, float Eta){
    return (w*(pow((V*I*Eta),2)));}

uint16_t MinimumSize(float Uin)
{
    uint16_t i = 0;
    uint16_t Minimo = 0;
    Minimo =  ((uint16_t)Uin)/10;
    Minimo = 86 - Minimo * 10;
    if (Minimo < SizeOfTable)
        SizeOfTable = Minimo;
    return SizeOfTable;
}
uint16_t SizeOfColumn(float Vin){
    return ((uint16_t)Vin - 15.0);}
uint16_t NumberOfTable(float Uin){
    return (((uint16_t)(Uin - 10.0))/10);
}
float TableValue(uint16_t TableNumber,uint16_t offset_table, uint16_t row, uint16_t offset_row, uint16_t column, uint16_t offset_column)
{
    uint16_t aux = *(*(Tablas +TableNumber+offset_table)+(row + offset_row)*11+(column+offset_column));
    return  (float)(aux)/10000;
}

void main(void)
{
    float Vin[3] = {18.49,15.391,15.391};
	float Iin[3] = {4.038,4.038,4.038};
	float Uin[3] = {34.44,30.15,30.15};
	float w[3] = {100.0/300.0,100.0/300.0,100.0/300.0};
    float F, Maximo = 0.0, EtaY =0.0;
    uint8_t i = 0;
    uint16_t Size = 0, Fila =0, Offset = 0;

    for (i=0; i< NEntradas; i++)
        Size = MinimumSize(Uin[i]);
    for (Fila = 0; Fila < Size; Fila ++)
	{	
        F=0.0;
		for(i = 0; i < NEntradas; i++)
		{	
            NTabla = NumberOfTable(Uin[i]);
			Offset = 86 -  NTabla * 10 - Size;  
			Columna = SizeOfColumn(Vin[i]); 
			
            EtaV_Lv[0] = TableValue(NTabla,0,Fila,Offset,Columna,0);  
    		EtaV_Lv[1] = TableValue(NTabla,0,Fila,Offset,Columna,1);
    		arrayX[0] = 15 + Columna;
            arrayX[1] = (15 + Columna)+1;
            EtaU[0] = lineal_interpolation(EtaV_Lv,arrayX,Vin[i]);
			
			Offset = Offset - 10;
			
            EtaV_Lv[0] = TableValue(NTabla,1,Fila,Offset,Columna,0);  
    		EtaV_Lv[1] = TableValue(NTabla,1,Fila,Offset,Columna,1);
			EtaU[1] = lineal_interpolation(EtaV_Lv,arrayX,Vin[i]);
    		arrayX[0] = (NTabla+1)*10;
            arrayX[1] = ((NTabla+1)*10)+10;
			
            EtaY = lineal_interpolation(EtaU,arrayX,Uin[i]);
			
            F = F + FuncionObjetivo(w[i],Vin[i],Iin[i],EtaY);
		}
		printf("%.4f\n",F);
		if (F > Maximo)
		{	Maximo = F;
			PosF = Fila;
		}
	}
    printf("%i,%d",86-Size+PosF,PosF);
}
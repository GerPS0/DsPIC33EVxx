#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Eta_val_fixed.c"

float FuncionObjetivo(float *w, float *V, float *I, float *Eta, uint8_t size);
uint8_t MetodoDirecto(float *w, float *V, float *I,float *Eta[], uint8_t size);

uint8_t *limitPos(float valor, int8_t *arr_in, uint8_t size);
float Lineal_Interpolation(float *arrY, int8_t *arrX, float x0);
uint8_t Duty_Lv();
float *Eta_lv_low(uint8_t *Vin, uint8_t *PosC, uint8_t u_lv, uint8_t size, float V0);
float *Eta_lv_up(uint8_t *Vin, uint8_t *PosC, uint8_t u_lv, uint8_t size, float V0);
float *Eta_Lv_duty(uint8_t *uin, float *Lv_low, float *Lv_up, float u0, uint8_t size);
float *Eta_Ux(float Vin, float uin);

int8_t Vs[11] = {15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25}; 	//Vertices of Voltage
int8_t u[5] = {10, 20, 30, 40, 50};
float w[3] = {(float)100/300,(float)100/300,(float)100/300};

float FuncionObjetivo(float *w, float *V, float *I, float *Eta, uint8_t size)
{
    int8_t i =0; 
    float F = 0.0;
    for (i=0;i<size;i++)
        F = F + (float)pow(((*V++)*(*I++)*(*Eta++)),2)*(*w++);
    printf("%0.4f\n", F); 
    return F;
    
}

uint8_t MetodoDirecto(float *w, float *V, float *I,float *Eta[], uint8_t size)
{
    uint8_t i = 0;
    uint8_t PosF = 0;
    float F = 0.0;
    float F0 = 0.0;
    float *Eaux;

    Eaux = &Eta[0][0];
    //sacar la longitud de eaux
    F0 = FuncionObjetivo(w,V,I,Eaux,3);

    for (i = 1;i<size;i++)
    {
        Eaux = &Eta[i][0];
        F = FuncionObjetivo(w,V,I,Eaux,3);
            if (F>F0){
                F0 = F;
                PosF = i;
            }

    }
    
    return PosF;
}

uint8_t *limitPos(float valor, int8_t *arr_in, uint8_t size)
{
	/*  This function returns in a one-dimensional array the position values found in: "arr in" array_in"
		array_in represent the vertices of a certain input
	*/
	static uint8_t arr[2]={0,0};
	uint8_t i = 0;
	
	while(valor >= *(arr_in + i) && i < size){
		arr[0] = i;
		i += 1;
	}
	arr[1] = i;	
	return arr;
}

float Lineal_Interpolation(float *arrY, int8_t *arrX, float x0)
{
	/*  This function returns the interpolate value of the Eta(Ys) respect to arrX 
		The Ys represent the maximum value found in the interpolate function 
	*/
	float Ys = 0;
	if (x0 == *(arrX+0) || *(arrX+0) == *(arrX+1))    //when x0 is equal to the first value of the arrX (Vertice) is not necessary the interpolation)
		Ys = *(arrY+0);
	else{
		Ys = ((*(arrY + 1)-*(arrY + 0))/(*(arrX + 1)-*(arrX + 0))) * (x0 - *(arrX + 0)) + *(arrY + 0); 
	}return Ys;
			
}

uint8_t Duty_Lv()
{
    uint8_t size;
    size = sizeof(Eta1)/sizeof(Eta1[0]);
    return size;
}

float *Eta_lv_low(uint8_t *Vin, uint8_t *PosC, uint8_t u_lv, uint8_t size, float V0)
{
    /* PosC corresponding to the voltage level and u_lv the duty level.
        The duty level allows select the correct EtaX table where X represent the number of the table
        -----------------------------------------------------------------------------
        This function return a interpolate values of Eta respect to the voltage values 
        in the minimum EtaX 
    */
    uint8_t i =0;
    float aux[2] = {0.0};
    static float Eta_to_VL[46]={0.0};

    if (u_lv == 0)
    {
        for(i=0;i<size;i++)
        {
            aux[0] = Eta1[i][*(PosC+0)];
            aux[1] = Eta1[i][*(PosC+1)];
            Eta_to_VL[i] = Lineal_Interpolation(aux,Vin,V0);
        }        
    }
    if (u_lv == 1)
    {
        for(i=0;i<size;i++)
        {
            aux[0] = Eta2[i][*(PosC+0)];
            aux[1] = Eta2[i][*(PosC+1)];
            Eta_to_VL[i] = Lineal_Interpolation(aux,Vin,V0);
        }  
    }
    if (u_lv == 2)
    {
        for(i=0;i<size;i++)
        {
            aux[0] = Eta3[i][*(PosC+0)];
            aux[1] = Eta3[i][*(PosC+1)];
            Eta_to_VL[i] = Lineal_Interpolation(aux,Vin,V0);
        }  
    }
    if (u_lv == 3)
    {
        for(i=0;i<size;i++)
        {
            aux[0] = Eta4[i][*(PosC+0)];
            aux[1] = Eta4[i][*(PosC+1)];
            Eta_to_VL[i] = Lineal_Interpolation(aux,Vin,V0);
        }  
    }
    if (u_lv == 4)
    {
        for(i=0;i<size;i++)
        {
            aux[0] = Eta5[i][*(PosC+0)];
            aux[1] = Eta5[i][*(PosC+1)];
            Eta_to_VL[i] = Lineal_Interpolation(aux,Vin,V0);
        }  
    }
    return Eta_to_VL;
}

float *Eta_lv_up(uint8_t *Vin, uint8_t *PosC, uint8_t u_lv, uint8_t size, float V0)
{
    /* PosC corresponding to the voltage level and u_lv the duty level.
        The duty level allows select the correct EtaX table where X represent the number of the table
        -----------------------------------------------------------------------------
        This function return a interpolate values of Eta respect to the voltage values 
        in the minimum EtaX 
    */

    uint8_t i =0;
    float aux[2] = {0.0};
    static float Eta_to_V[100]={0.0};

    if (u_lv == 0)
    {
        for(i=0;i<size;i++)
        {
            aux[0] = Eta1[i][*(PosC+0)];
            aux[1] = Eta1[i][*(PosC+1)];
            Eta_to_V[i] = Lineal_Interpolation(aux,Vin,V0);
        }        
    }
    if (u_lv == 1)
    {
        for(i=0;i<size;i++)
        {
            aux[0] = Eta2[i][*(PosC+0)];
            aux[1] = Eta2[i][*(PosC+1)];
            Eta_to_V[i] = Lineal_Interpolation(aux,Vin,V0);
        }  
    }
    if (u_lv == 2)
    {
        for(i=0;i<size;i++)
        {
            aux[0] = Eta3[i][*(PosC+0)];
            aux[1] = Eta3[i][*(PosC+1)];
            Eta_to_V[i] = Lineal_Interpolation(aux,Vin,V0);
        }  
    }
    if (u_lv == 3)
    {
        for(i=0;i<size;i++)
        {
            aux[0] = Eta4[i][*(PosC+0)];
            aux[1] = Eta4[i][*(PosC+1)];
            Eta_to_V[i] = Lineal_Interpolation(aux,Vin,V0);
        }  
    }
    if (u_lv == 4)
    {
        for(i=0;i<size;i++)
        {
            aux[0] = Eta5[i][*(PosC+0)];
            aux[1] = Eta5[i][*(PosC+1)];
            Eta_to_V[i] = Lineal_Interpolation(aux,Vin,V0);
        }  
    }
    return Eta_to_V;
}

float *Eta_Lv_duty(uint8_t *uin, float *Lv_low, float *Lv_up, float u0, uint8_t size)
{
    static float Eta[46] = {0.0};
    int8_t i = 0;
    float aux[2] = {0.0};

    for ( i = 0; i < size; i++)
    {
        aux[0] = *(Lv_low + i);
        aux[1] = *(Lv_up + i);
        Eta[i] = Lineal_Interpolation(aux,uin,u0);
    }
    return Eta;
}
float *Eta_Ux(float Vin, float uin)
{
    
    uint8_t *temp1;
    uint8_t size = 0, i=0;
    float *Eta_V1, *Eta_V2, *aux;
    static float Eta_U[46] = {0.0};
    
    if (Vin > Vs[0] && Vin < Vs[10]) 
    {
        size = sizeof(Vs)/sizeof(Vs[0]);
        temp1 = limitPos(Vin,Vs, size);
        //inferior and superior limits of V (ld,lu) 
        uint8_t arrPosV[2] = {temp1[0], temp1[1]};
        uint8_t VinValues[2] = {Vs[arrPosV[0]],Vs[arrPosV[1]]};

        //printf("%d,%d\n",*(temp1+0),*(temp1+1));
        //Positions of u
        size = sizeof(u)/sizeof(u[0]);
        temp1 = limitPos(uin,u, size);
        //inferior and superior limits of u (ld,lu) 
        uint8_t arrPosU[2] = {temp1[0], temp1[1]};
        uint8_t uinValues[2] = {u[arrPosU[0]],u[arrPosU[1]]};

        //printf("%d,%d\n",*(temp1+0),*(temp1+1));

        Eta_V1 = Eta_lv_low(VinValues,arrPosV,arrPosU[0],Duty_Lv(),Vin);
        Eta_V2 = Eta_lv_up(VinValues,arrPosV,arrPosU[1],Duty_Lv(),Vin);
        aux = Eta_Lv_duty(uinValues,Eta_V1,Eta_V2,uin,Duty_Lv());
        size = Duty_Lv();
        /*printf("%0.4f,%0.4f,%0.4f,%0.4f,%0.4f",*(Eta_V1+0),*(Eta_V1+1),*(Eta_V1+2),*(Eta_V1+3),*(Eta_V1+4));
        printf("\n%0.4f,%0.4f,%0.4f,%0.4f,%0.4f",*(Eta_V2+0),*(Eta_V2+1),*(Eta_V2+2),*(Eta_V2+3),*(Eta_V2+4));
        printf("\n%0.4f,%0.4f,%0.4f,%0.4f,%0.4f",*(aux+0),*(aux+1),*(aux+2),*(aux+3),*(aux+4));*/
        
        for (i =0;i<size;i++)
            Eta_U[i] = *aux++;

    }
    return Eta_U;

    
}

void main(void){
//float V = 15.6, uin = 18.3;
    float *Eta1_U;
    float *aux[46];
    float Eta_U[46][3];
    float V[3] = {18.49,15.391,15.391};
    float I[3] = {4.038,4.038,4.038};
    float uin[3] = {34.44,30.15,30.15};
    
    uint8_t i=0, j=0;
    
   
    for (i=0;i<3;i++)
    {
        Eta1_U = Eta_Ux(V[i],uin[i]);
        for(j = 0;j<46;j++)
        Eta_U[j][i] = *Eta1_U++;
    }
    for(i=0;i<46;i++)
        aux[i] = &Eta_U[i][0];
    
    
    
           
    /*printf("\n%0.4f,%0.4f,%0.4f,%0.4f",Eta1_U[0],Eta1_U[1],Eta1_U[2],Eta1_U[3]);
    printf("\n%0.4f,%0.4f,%0.4f,%0.4f",Eta2_U[0],Eta2_U[1],Eta2_U[2],Eta2_U[3]);
    printf("\n%0.4f,%0.4f,%0.4f,%0.4f",Eta3_U[0],Eta3_U[1],Eta3_U[2],Eta3_U[3]);*/

    
    uint8_t PosF = MetodoDirecto(w,V,I,aux,Duty_Lv());
    uint32_t Fs = 55000+((PosF-1)*1000);
    printf("\n%d, %d", PosF,Fs);
}
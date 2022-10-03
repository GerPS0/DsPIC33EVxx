#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

uint8_t *limitPos(float valor, int8_t *arr_int, uint8_t size);
uint8_t Duty_Lv(uint8_t u_dim);

int16_t Lineal_Interpolation(uint16_t *arrY, int8_t *arrX, float x0);
uint16_t *Eta_lv_low(uint8_t *Vin, uint8_t *PosC, uint8_t u_lv, uint8_t size, float V0);
uint16_t *Eta_lv_up(uint8_t *Vin, uint8_t *PosC, uint8_t u_lv, uint8_t size, float V0);
uint16_t *Eta_Lv_duty(uint8_t *uin, uint16_t *Lv_low, uint16_t *Lv_up, float u0, uint8_t size_low,uint8_t size_up);
uint16_t *Eta_Ux(float Vin, float uin);

uint16_t Eta1[10][5] = {{9124, 9360, 9050, 9206, 9184},
                    {9060, 9438, 9142, 9301, 9213},
                    {9163, 9321, 9180, 9350, 9309},
                    {9316, 9377, 9240, 9359, 9303},
                    {9223, 9450, 9259, 9364, 9352},
                    {9124, 9360, 9050, 9206, 9184},
                    {9060, 9438, 9142, 9301, 9213},
                    {9163, 9321, 9180, 9350, 9309},
                    {9316, 9377, 9240, 9359, 9303},
                    {9223, 9450, 9259, 9364, 9352}};

uint16_t Eta2[4][5] = {{9124, 9360, 9050, 9206, 9184},
                    {9060, 9438, 9142, 9301, 9213},
                    {9163, 9321, 9180, 9350, 9309},
                    {9316, 9377, 9240, 9359, 9303}};

uint16_t Eta3[3][5] = {{9124, 9360, 9050, 9206, 9184},
                    {9060, 9438, 9142, 9301, 9213},
                    {9163, 9321, 9180, 9350, 9309}};

uint8_t *limitPos(float valor, int8_t *arr_in, uint8_t size)
{
	/*  This function returns in a one-dimensional array the position values found in: "arr in" array_in"
		array_in represent the vertices of a certain input
	*/
	static uint8_t arr[2];
	uint8_t i = 0;
	
	while(valor >= *(arr_in + i) && i < size){
		arr[0] = i;
		i += 1;
	}
	arr[1] = i;	
	return arr;
}

uint8_t Duty_Lv(uint8_t u_dim)
{
    uint8_t size;
    if (u_dim == 0){size = sizeof(Eta1)/sizeof(Eta1[0]);}
    else if (u_dim == 1){size = sizeof(Eta2)/sizeof(Eta2[0]);}
    else if (u_dim == 2){size = sizeof(Eta3)/sizeof(Eta3[0]);}
    else if (u_dim == 3){size = sizeof(Eta1)/sizeof(Eta1[0]);}
    else if (u_dim == 4){size = sizeof(Eta1)/sizeof(Eta1[0]);}

    return size;
}

int16_t Lineal_Interpolation(uint16_t *arrY, int8_t *arrX, float x0)
{
	/*  This function returns the interpolate value of the Eta(Ys) respect to arrX 
		The Ys represent the maximum value found in the interpolate function 
	*/
	float Ys = 0;
	if (x0 == *(arrX+0))    //when x0 is equal to the first value of the arrX (Vertice) is not necessary the interpolation)
		Ys = *(arrY+0);
	else{
		Ys = ((*(arrY + 1)-*(arrY + 0))/(*(arrX + 1)-*(arrX + 0))) * (x0 - *(arrX + 0)) + *(arrY + 0); 
	}return Ys;
			
}

uint16_t *Eta_lv_low(uint8_t *Vin, uint8_t *PosC, uint8_t u_lv, uint8_t size, float V0)
{
    /* PosC corresponding to the voltage level and u_lv the duty level.
        The duty level allows select the correct EtaX table where X represent the number of the table
        -----------------------------------------------------------------------------
        This function return a interpolate values of Eta respect to the voltage values 
        in the minimum EtaX 
    */

    uint8_t i =0;
    uint16_t aux[2] = {0.0};
    static uint16_t Eta_to_V[100]={0.0};

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
            aux[0] = Eta1[i][*(PosC+0)];
            aux[1] = Eta1[i][*(PosC+1)];
            Eta_to_V[i] = Lineal_Interpolation(aux,Vin,V0);
        }  
    }
    if (u_lv == 4)
    {
        for(i=0;i<size;i++)
        {
            aux[0] = Eta1[i][*(PosC+0)];
            aux[1] = Eta1[i][*(PosC+1)];
            Eta_to_V[i] = Lineal_Interpolation(aux,Vin,V0);
        }  
    }

    return Eta_to_V;
}
uint16_t *Eta_lv_up(uint8_t *Vin, uint8_t *PosC, uint8_t u_lv, uint8_t size, float V0)
{
    /* PosC corresponding to the voltage level and u_lv the duty level.
        The duty level allows select the correct EtaX table where X represent the number of the table
        -----------------------------------------------------------------------------
        This function return a interpolate values of Eta respect to the voltage values 
        in the minimum EtaX 
    */

    uint8_t i =0;
    uint16_t aux[2] = {0.0};
    static uint16_t Eta_to_V[100]={0.0};

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
            aux[0] = Eta1[i][*(PosC+0)];
            aux[1] = Eta1[i][*(PosC+1)];
            Eta_to_V[i] = Lineal_Interpolation(aux,Vin,V0);
        }  
    }
    if (u_lv == 4)
    {
        for(i=0;i<size;i++)
        {
            aux[0] = Eta1[i][*(PosC+0)];
            aux[1] = Eta1[i][*(PosC+1)];
            Eta_to_V[i] = Lineal_Interpolation(aux,Vin,V0);
        }  
    }
    return Eta_to_V;
}
uint16_t *Eta_Lv_duty(uint8_t *uin, uint16_t *Lv_low, uint16_t *Lv_up, float u0, uint8_t size_low,uint8_t size_up)
{
    static uint16_t Eta[100] = {0.0};
    int8_t i = 0;
    int8_t size = size_low - size_up;
    uint16_t aux[2] = {0.0};

    for ( i = 0; i < size_up; i++)
    {
        aux[0] = *(Lv_low + (i + size));
        aux[1] = *(Lv_up + i);
        Eta[i] = Lineal_Interpolation(aux,uin,u0);
    }
    return Eta;
}
uint16_t *Eta_Ux(float Vin, float uin)
{
    int8_t Vs[11] = {15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25}; 	//Vertices of Voltage
	int8_t u[5] = {10, 20, 30, 40, 50};
    uint8_t *temp1;
    uint8_t size = 0, i=0;
    uint16_t *Eta_V1, *Eta_V2, *aux;
    static uint16_t Eta_U[100] = {0.0};
    if (Vin > Vs[0] && Vin < Vs[10]) 
    {
        size = sizeof(Vs)/sizeof(Vs[0]);
        temp1 = limitPos(Vin,Vs, size);
        //inferior and superior limits of V (ld,lu) 
        uint8_t arrPosV[2] = {temp1[0], temp1[1]};
        uint8_t VinValues[2] = {Vs[arrPosV[0]],Vs[arrPosV[1]]};

        printf("%d,%d\n",*(temp1+0),*(temp1+1));
        //Positions of u
        size = sizeof(u)/sizeof(u[0]);
        temp1 = limitPos(uin,u, size);
        //inferior and superior limits of u (ld,lu) 
        uint8_t arrPosU[2] = {temp1[0], temp1[1]};
        uint8_t uinValues[2] = {u[arrPosU[0]],u[arrPosU[1]]};

        printf("%d,%d\n",*(temp1+0),*(temp1+1));

        Eta_V1 = Eta_lv_low(VinValues,arrPosV,arrPosU[0],Duty_Lv(arrPosU[0]),Vin); 
        Eta_V2 = Eta_lv_up(VinValues,arrPosV,arrPosU[1],Duty_Lv(arrPosU[1]),Vin);
        aux = Eta_Lv_duty(uinValues,Eta_V1,Eta_V2,uin,Duty_Lv(arrPosU[0]),Duty_Lv(arrPosU[1]));
        size = Duty_Lv(arrPosU[1]);
        printf("%d,%d,%d,%d,%d",*(Eta_V1+0),*(Eta_V1+1),*(Eta_V1+2),*(Eta_V1+3),*(Eta_V1+4)); 
        printf("\n%d,%d,%d,%d,%d",*(Eta_V2+0),*(Eta_V2+1),*(Eta_V2+2),*(Eta_V2+3),*(Eta_V2+4)); 
        printf("\n%d,%d,%d,%d,%d",*(aux+0),*(aux+1),*(aux+2),*(aux+3),*(aux+4)); 
        
        for (i =0;i<size;i++)
            Eta_U[i] = *aux++;

    }
    return Eta_U;

    
}
void main(void)
{
    float V = 15.6, uin = 18.3;
    uint16_t *Eta_U;
    
    Eta_U = Eta_Ux(V,uin);
    printf("\n%d,%d,%d",*(Eta_U+0),*(Eta_U+1),*(Eta_U+2));
}
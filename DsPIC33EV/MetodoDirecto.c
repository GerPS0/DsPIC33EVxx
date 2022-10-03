#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float FuncionObjetivo(float *w, float *V, float *I, float *Eta, uint8_t size);
uint8_t MetodoDirecto(float *w, float *V, float *I, float *Eta[], uint8_t size);
uint8_t MetodoDirectoReducido(float *w, float *V, float *I, float *Eta[], uint8_t size);


float FuncionObjetivo(float *w, float *V, float *I, float *Eta, uint8_t size)
{
    int8_t i =0; 
    float F = 0.0;
    for (i=0;i<size;i++)
        F = F + (float)pow(((*V++)*(*I++)*(*Eta++)),2)*(*w++);
    //printf("%0.4f\n", F); 
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
uint8_t MetodoDirectoReducido(float *w, float *V, float *I, float *Eta[], uint8_t size)
{
    int8_t k0[3]  = {1,1,1};
    int8_t k[3]  = {1,1,1};
    uint8_t i= 0, m=0;
    uint8_t PosF = 0;
    float F = 0.0;
    float F0 = 0.0;
    float *E0;
    float *E1;
    double Dx[3] = {0.0,0.0,0.0};
    
    E0 = &Eta[0][0];
    F0 = FuncionObjetivo(w,V,I,E0,3);
    for (i=0;i<size-1;i++)
    {
        E0 = &Eta[i][0];
        E1 = &Eta[i+1][0];
        for (m =0;m<3;m++)
        {
            Dx[m] = *(E1+m) - *(E0+m);
            if(k[m]*Dx[m]<0)
                k[m] = -1*k[m];
        }
    
        /*Dx[1] = *(E1+1) - *(E0+1);
        Dx[2] = *(E1+2) - *(E0+2);
        if(k[0]*Dx[0]<0)
            k[0] = -1*k[0];
        if(k[1]*Dx[1]<0)
            k[1] = -1*k[1];
        if(k[2]*Dx[1]<0)
            k[2] = -1*k[2];
        */
        if(k0[0]!=k[0] || k0[1]!=k[1] || k0[2]!=k[2])
        {
            F = FuncionObjetivo(w,V,I,E1,3);
            if (F>F0){F0 = F;PosF = i+1;}
            k0[0] = k[0];
            k0[1] = k[1];
            k0[2] = k[2];
        }
    }

    return PosF;
}


/*void main(void)
{
    float V[3] = {15.3,16.2,15.8};
    float I[3] = {3.1,2.8,2.8};
    float w[3] = {(float)50/150,(float)50/150,(float)50/150};
    float Eta[12][3] = {{0.9124, 0.9360, 0.9050},
                        {0.9060, 0.9438, 0.9142},
                        {0.9163, 0.9321, 0.9180},
                        {0.9316, 0.9377, 0.9240},
                        {0.9223, 0.9450, 0.9259},
                        {0.9267, 0.9498, 0.9346},
                        {0.9304, 0.9480, 0.9326},
                        {0.9355, 0.9559, 0.9360},
                        {0.9449, 0.9508, 0.9371},
                        {0.9416, 0.9602, 0.9389},
                        {0.9405, 0.9625, 0.9432},
                        {0.9416, 0.9600, 0.9389}};
    uint8_t size = sizeof(Eta)/sizeof(Eta[0]);
    uint8_t PosF = 0, PosF2 = 0;
    uint8_t i=0;
    float *aux[size];
    
    for(i=0;i<size;i++)
           aux[i] = &Eta[i][0];
    
    //F = FuncionObjetivo(w,V,I,Eta,size);
    PosF = MetodoDirecto(w,V,I,aux,size);
    PosF2 = MetodoDirectoReducido(w,V,I,aux,size);
    printf("%d \n %d", PosF,PosF2);

}*/
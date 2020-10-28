#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int escolhaExponencial(int, int*);

int main()
{
    int lista[] = {37, 72, 137};
    int tam = 3;        

    for (int i = 0; i < 20; i++)
    {
        printf("tentativa numero %d: %d\n\n", i, escolhaExponencial(tam, lista));
    }
    
} 

int escolhaExponencial(int quantCandidatos, int *candidatosValor)
{

    float soma = 0;
    float *representacaoReal = (float *)malloc(sizeof(float) * quantCandidatos);

    for (int i = 0; i < quantCandidatos; i++)
    {
        representacaoReal[i] = (float)candidatosValor[i] / (256.0*3);
        representacaoReal[i] = pow(M_E/10, representacaoReal[i]);
        soma += representacaoReal[i];
    }

    float aleatorio = rand();
    aleatorio = aleatorio / RAND_MAX * soma;

    printf("in function, representacao real: %f %f %f\n", representacaoReal[0], representacaoReal[1], representacaoReal[2]);
    printf("valor aleatorio escolido: %f\n", aleatorio);

    int indicePixagemEscolido = quantCandidatos;
    for (int i = 0; i < quantCandidatos; i++)
    {
        if(aleatorio < representacaoReal[i])
        {
            indicePixagemEscolido = i;
            break;
        }
        else
            aleatorio -= representacaoReal[i];
    }

    free(representacaoReal);

    return indicePixagemEscolido;

}
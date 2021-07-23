#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#define item3D(x, y, z, sx, sz) (y*(sz*sx)) + (x*sz+z)

typedef struct
{
    int top;
    int botton;
    int left;
    int right;
} coordenadas;

typedef struct
{
    unsigned char *imagem;
    int tamanho;
    int largura;
    int altura;
} imagem;

unsigned char *media(imagem *imagemBase, int alturaCortes, int larguraCortes);

int *imagemMaisProxima(unsigned char *medias, int tamMedias,
                       unsigned char *comparacoes, int tamComp);

int escolhaLinear(int quantCandidatos, int *candidatosValor);

int escolhaExponencial(int quantCandidatos, int *candidatosValor);

unsigned char mediaCanalRecorte(coordenadas *coordenadasCorte,
                                imagem *imagemBase, int canal);

int pegarItemLista3D(int x, int y, int z, int tamX, int tamZ);

// int main()
// {
//     FILE *fp;
//     fp = fopen("aaa.bmp", "r+");

//     fseek(fp, 0, SEEK_END); // seek to end of file
//     int size = ftell(fp); // get current file pointer
//     fseek(fp, 0x7a, SEEK_SET); // seek back to beginning of file

//     char* lista1 = (char*) malloc(sizeof(char) * size);

//     fread(lista1, sizeof(char), sizeof(char) * (size - 4), fp);

//     fclose(fp);

//     imagem imagemBase;
//     imagemBase.imagem = lista1;
//     imagemBase.altura = 32;
//     imagemBase.largura = 32;
//     imagemBase.tamanho = imagemBase.largura * imagemBase.altura;

//     unsigned char* joao = media(&imagemBase, 16, 16);

//     for(int i = 0; i < 4; i++)
//     {
//         int rodada = i*3;
//         printf("media RGB bloco %d: %d %d %d\n", i, joao[rodada], joao[rodada+1], joao[rodada+2]);
//     }

//     printf("rodada: %d\n", joao[16*3]);

//     free(joao);
//     free(lista1);

//     return 0;
// }

//vai pegar uma matrix de pixels e o largura e altura de cada Corte
unsigned char *media(imagem *imagemBase, int alturaCortes, int larguraCortes)
{
    //fica esperto que o pixels é uma mentira
    //ele não é um array simples
    //na verdade ele é um array com 3 dimensoes
    //sendo a primeira e a segunda altura e largura_imagemlargura
    //e a terceira as cores RGB

    //e o retorno vai ser um array
    //contendo a media de cada Corte

    //inicial umas variaveis auto explicativas

    int quantidadeCortesY = imagemBase->altura / alturaCortes;
    int quantidadeCortesX = imagemBase->largura / larguraCortes;
    int quantidadeCortes = quantidadeCortesX * quantidadeCortesY;

    //inicia a coordenadasCorte para ser usada depois
    coordenadas coordenadasCorte;

    //aloca o endereco que vai ser retornado
    unsigned char *pMediasCortes = (unsigned char *)malloc(sizeof(char) *
                                                           quantidadeCortes * 3);

    //roda pela altura da imagem
    for (int y = 0; y < quantidadeCortesY; y++)
    {
        //roda pela largura da imagem
        for (int x = 0; x < quantidadeCortesX; x++)
        {

            //define o quadrado da imagem passada que vai ser feito a media
            //o tamanho do quadrado é o tamanho das pixagens
            coordenadasCorte.top = alturaCortes * y;
            coordenadasCorte.botton = alturaCortes * (y + 1);

            coordenadasCorte.left = larguraCortes * x;
            coordenadasCorte.right = larguraCortes * (x + 1);

            //pega o index na lista que vai ser retornada para guardar a media
            int indexCanalR = item3D(x, y, 0, quantidadeCortesX, 3);
            //pegar a media do canal do bloco escolhido
            unsigned char mediaCanalR = mediaCanalRecorte(&coordenadasCorte, imagemBase, 0);
            pMediasCortes[indexCanalR] = mediaCanalR;

            //faz isso 3 vezes para cada canal RGB

            int indexCanalG = item3D(x, y, 1, quantidadeCortesX, 3);
            unsigned char mediaCanalG = mediaCanalRecorte(&coordenadasCorte, imagemBase, 1);
            pMediasCortes[indexCanalG] = mediaCanalG;

            int indexCanalB = item3D(x, y, 2, quantidadeCortesX, 3);
            unsigned char mediaCanalB = mediaCanalRecorte(&coordenadasCorte, imagemBase, 2);
            pMediasCortes[indexCanalB] = mediaCanalB;
        }
    }
    return pMediasCortes;
}

int *imagemMaisProxima(unsigned char *medias, int tamMedias,
                       unsigned char *comparacoes, int tamComp)
{
    //tam é o tamanho exato da lista
    //quant desconsidera a 2 dimensao da lista que é o RGB
    int quantMedias = tamMedias / 3;
    int quantComp = tamComp / 3;

    //aloca a memoria que vai ser retornada
    int *indexMaisProximos = (int *)malloc(sizeof(int) * quantMedias);

    for (int mediaAtual = 0; mediaAtual < quantMedias; mediaAtual++) {
      int pos = mediaAtual * 3;

      int x1 = medias[pos];//lixo
      int x2 = medias[pos + 1];
      int x3 = medias[pos + 2];

      long menorDistancia = LONG_MAX;
      int indexMenorDistancia;

      for(int comparacoesIndex = 0; comparacoesIndex < quantComp; comparacoesIndex++)
      {

        int y1 = comparacoes[comparacoesIndex * 3 ];
        int y2 = comparacoes[comparacoesIndex * 3 + 1];
        int y3 = comparacoes[comparacoesIndex * 3 + 2];

        long distancia = sqrt(
            pow(x1 - y1, 2) +
            pow(x2 - y2, 2) +
            pow(x3 - y3, 2) * 1.0);


        if(distancia < menorDistancia) {
          menorDistancia = distancia;
          indexMenorDistancia = comparacoesIndex;
        }

      }

    indexMaisProximos[mediaAtual] = indexMenorDistancia;

    }

    return indexMaisProximos;
}

int escolhaLinear(int quantCandidatos, int *candidatosValor)
{
    int soma = 0;

    for (int i = 0; i < quantCandidatos; i++)
    {
        candidatosValor[i] = 256 * 3 - candidatosValor[i];
        soma += candidatosValor[i];
    }

    int aleatorio = rand() % soma;

    int indicePixagemEscolido;
    for (int i = 0; i < quantCandidatos; i++)
    {
        if (aleatorio < candidatosValor[i])
        {
            indicePixagemEscolido = i;
            break;
        }
        else
            aleatorio -= candidatosValor[i];
    }

    return indicePixagemEscolido;
}

int escolhaExponencial(int quantCandidatos, int *candidatosValor)
{
    float soma = 0;
    float *representacaoReal = (float *)malloc(sizeof(float) * quantCandidatos);

    for (int i = 0; i < quantCandidatos; i++)
    {
        representacaoReal[i] = (float)candidatosValor[i] / (256.0 * 3);
        representacaoReal[i] = pow(0.01, representacaoReal[i]);
        soma += representacaoReal[i];
    }

    float aleatorio = rand();
    aleatorio = aleatorio / RAND_MAX * soma;

    int indicePixagemEscolido;
    for (int i = 0; i < quantCandidatos; i++)
    {
        if (aleatorio < representacaoReal[i])
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

//pega a media de cores de um canal de uma parte da imagem
unsigned char mediaCanalRecorte(coordenadas *coordenadasCorte,
                                imagem *imagemBase, int canal)
{
    //vai ser a soma de determinado canal de cor de todos os pixels
    long int somaTemporaria = 0;

    //vai rodas pela altura do bloco passado
    for (int y = coordenadasCorte->top; y < coordenadasCorte->botton; y++)
    {
        //vai rodar pela largura do bloco passado
        for (int x = coordenadasCorte->left; x < coordenadasCorte->right; x++)
        {
            int index = item3D(x, y, canal, imagemBase->largura, 3);
            somaTemporaria += imagemBase->imagem[index];
        }
    }
    //3 vem do tamanho da 3d dimensao da imagem
    //como a cor e rgb tem 3 char's no 3 dimensao

    //tamanho do X e Y do bloco passado
    //usado para calcular a media aritimetica depois
    int deltaX = coordenadasCorte->right - coordenadasCorte->left;
    int deltaY = coordenadasCorte->botton - coordenadasCorte->top;

    //media aritimetica depois
    unsigned char mediaAritimetica = somaTemporaria / (deltaX * deltaY);

    return mediaAritimetica;
}

//pega o x y z de uma lista
int pegarItemLista3D(int x, int y, int z, int tamX, int tamZ)
{
    //como todas a listas aqui sao de uma dimensao simples
    //imitando uma matrix tridimensional
    //eu preciso usar essa funcao toda fez que quero simular esse 3d
    return (y * (tamZ * tamX)) + (x * tamZ + z);
}

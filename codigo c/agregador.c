#include <stdlib.h>
#include <stdio.h>
#include <math.h>
//#define item3D(x, y, z, sx, sz) (y*(sz*sx)) + (x*sz+z)


typedef struct {
    int top;
    int botton;
    int left;
    int right;
} coordenadas;

typedef struct {
    unsigned char* imagem;
    int largura;
    int altura;
    int tamanho;
} imagem;


unsigned char* media(int alturaCortes, int larguraCortes,
    int alturaImagem, int larguraImagem, unsigned char* pPixelsImagem);

int* imagemMaisProxima(unsigned char* medias, int tamMedias, 
    unsigned char* comparacoes, int tamComp);

unsigned char mediaCanalRecorte(coordenadas coordenadasCorte,
    imagem imagemBase, int canal);

int pegarItemLista3D(int x, int y, int z, int tamX, int tamZ);

// int main() 
// {
//     FILE *fp;
//     fp = fopen("aaa.bmp", "r+");
    
//     fseek(fp, 0, SEEK_END); // seek to end of file
//     int size = ftell(fp); // get current file pointer
//     fseek(fp, 0, SEEK_SET); // seek back to beginning of file


//     char* lista1 = (char*) malloc(sizeof(char) * size);

//     fread(lista1, sizeof(char), sizeof(char) * size, fp);

//     fclose(fp);

//     unsigned char* joao = media(16, 16, 64, 64, lista1);

//     for(int i = 0; i < 16; i++)
//     {
//         int rodada = i*3;
//         printf("media RGB bloco %d: %d %d %d\n", i, joao[rodada], joao[rodada+1], joao[rodada+2]);
//     }

//     printf("rodada: %d\n", joao[16*3])

//     free(joao);
//     free(lista1);

//     return 0; 
// }


//vai pegar uma matrix de pixels e o largura e altura de cada Corte
unsigned char* media(int alturaCortes, int larguraCortes,
    int alturaImagem, int larguraImagem, unsigned char* pPixelsImagem)
{

    //fica esperto que o pixels é    uma mentira
    //na verdade ele é um array com 3 dimensoes
    //sendo a primeira e a segunda altura e largura
    //e a terceira 3 por ser as cores RGB

    //e o retorno vai ser um array normal
    //contendo a media de cada Corte

    //printf("%d %d %d %d\n", alturaCortes, larguraCortes, alturaImagem, larguraImagem);

    int tamanhoImagem = alturaImagem * larguraImagem;
    int quantidadeCortesY = alturaImagem / alturaCortes;
    int quantidadeCortesX = larguraImagem / larguraCortes;
    int quantidadeCortes = quantidadeCortesX * quantidadeCortesY;

    //printf("%d %d %d ", quantidadeCortesY, quantidadeCortesX, quantidadeCortes);

    imagem imagemBase;

    imagemBase.imagem = pPixelsImagem;
    imagemBase.largura = larguraImagem;
    imagemBase.altura = alturaImagem;
    imagemBase.tamanho = tamanhoImagem;

    coordenadas coordenadasCorte; 

    //printf("aaaaaaaa eu quero chorarrrrrrr\n");

    unsigned char* pMediasCortes = (unsigned char*) malloc(sizeof(char) * 
        quantidadeCortes * 3);

    for(int y = 0; y < quantidadeCortesY; y++)
    {
        for(int x = 0; x < quantidadeCortesX; x++)
        {

            coordenadasCorte.top = alturaCortes * y;
            coordenadasCorte.botton = alturaCortes * (y + 1);

            coordenadasCorte.left = larguraCortes * x;
            coordenadasCorte.right = larguraCortes * (x + 1);

            int indexCanalR = pegarItemLista3D(x, y, 0, quantidadeCortesX, 3);
            unsigned char mediaCanalR = mediaCanalRecorte(coordenadasCorte, imagemBase, 0);
            pMediasCortes[indexCanalR] = mediaCanalR;

            int indexCanalG = pegarItemLista3D(x, y, 1, quantidadeCortesX, 3);
            unsigned char mediaCanalG = mediaCanalRecorte(coordenadasCorte, imagemBase, 1);
            pMediasCortes[indexCanalG] = mediaCanalG;

            int indexCanalB = pegarItemLista3D(x, y, 2, quantidadeCortesX, 3);
            unsigned char mediaCanalB = mediaCanalRecorte(coordenadasCorte, imagemBase, 2);
            pMediasCortes[indexCanalB] = mediaCanalB;

            //printf("x: %d y: %d ", x, y);

        }

        //printf("\n");
    }



    // for(int y = 0; y < quantidadeCortesY; y++)
    // {
    //     for(int x = 0; x < quantidadeCortesX; x++)
    //     {
    //         coordenadasCorte.top = alturaCortes * y;
    //         coordenadasCorte.botton = alturaCortes * (y + 1);

    //         coordenadasCorte.left = larguraCortes * x;
    //         coordenadasCorte.right = larguraCortes * (x + 1);

    //         int indexCanalR = pegarItemLista3D(x, y, 0, quantidadeCortesX, 3);
    //         unsigned char mediaCanalR = mediaCanalRecorte(coordenadasCorte, imagemBase, 0);
    //         pMediasCortes[indexCanalR] = mediaCanalR;

    //         int indexCanalG = pegarItemLista3D(x, y, 1, quantidadeCortesX, 3);
    //         unsigned char mediaCanalG = mediaCanalRecorte(coordenadasCorte, imagemBase, 1);
    //         pMediasCortes[indexCanalG] = mediaCanalG;

    //         int indexCanalB = pegarItemLista3D(x, y, 2, quantidadeCortesX, 3);
    //         unsigned char mediaCanalB = mediaCanalRecorte(coordenadasCorte, imagemBase, 2);
    //         pMediasCortes[indexCanalB] = mediaCanalB;
    //     }
    // }

    return pMediasCortes;

}

int* imagemMaisProxima(unsigned char* medias, int tamMedias, 
    unsigned char* comparacoes, int tamComp)
{
    int quantMedias = tamMedias / 3;
    int quantComp = tamComp / 3;

    int* indexMaisProximos = (int*) malloc(
        sizeof(int) * quantMedias + 1);


    for(int media = 0; media < quantMedias; media++)
    {
        int indiceMenorDif = 0;
        int corMaisProx = 256*3;
        for(int comparacao = 0; comparacao < quantComp; comparacao++)
        {
            int diferenca = 0;
            //disasemble then unwrappe this
            for(int cores = 0; cores < 3; cores++)
            {
                diferenca += abs(medias[media*3+cores] - 
                    comparacoes[comparacao*3+cores]);
            }

            if(diferenca < corMaisProx)
            {
                indiceMenorDif = comparacao;
                corMaisProx = diferenca;
            }
        }
        indexMaisProximos[media] = indiceMenorDif;
    }

    indexMaisProximos[quantMedias] = 42069;

    return indexMaisProximos;
}

//pega a media de cores de um canal de uma parte da imagem
unsigned char mediaCanalRecorte(coordenadas coordenadasCorte,
    imagem imagemBase, int canal)
{
    long int somaTemporaria = 0;

    for(int y = coordenadasCorte.top; y < coordenadasCorte.botton; y++)
    {
        for(int x = coordenadasCorte.left; x < coordenadasCorte.right; x++)
        {
            somaTemporaria += imagemBase.imagem[pegarItemLista3D(
                x, y, canal, imagemBase.largura, 3)];
        }
    }
    //3 vem do tamanho da 3d dimensao da imagem
    //como a cor e rgb tem 3 char's no 3 dimensao

    int deltaX = coordenadasCorte.right - coordenadasCorte.left;
    int deltaY = coordenadasCorte.botton - coordenadasCorte.top;

    unsigned char teste = somaTemporaria / (deltaX * deltaY);
    return teste;
}

int pegarItemLista3D(int x, int y, int z, int tamX, int tamZ)
{
    return (y*(tamZ*tamX)) + (x*tamZ+z);
}

// void criarBloco(char* listaRetorno, char* listaBase, 
//     int TP, int BR)
// {

// }

// void mediaRGBBloco(char* bloco, int largura, int altura, char* resultadoMedia)

// {

//     int quantPixels = largura * altura;

//     //talves isso esteja funcionando mais seila testa dps
//     char mediaCanalR = mediaCanalLista3D(bloco, quantPixels, 0);
//     char mediaCanalG = mediaCanalLista3D(bloco, quantPixels, 1);
//     char mediaCanalB = mediaCanalLista3D(bloco, quantPixels, 2);

// }

// char mediaCanalLista3D(char* lista ,int tamanho, int descolcacaoCanal)
// {

//     unsigned long int somaCanalR = 0;

//     for(int x = 0; x < tamanho; x++)
//     {
//         somaCanalR += bloco[x* 3 + descolcacaoCanal];
//     }

//     return ceil(somaCanalR / tamanho);
// }


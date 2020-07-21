
/*

lista de mentirinha
{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18}
==
{
    [[1, 2, 3], [4, 5, 6]], 
    [[7, 8, 9], [10, 11, 12]], 
    [[13, 14, 15], [16, 17, 18]]
}
0, 0, 0, 2, 3, 3
x  y  z  sx sy sz
y*4+x
(y*(sz*sx)) + (x*sz+z)

*/
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

unsigned char* acesso(int alturaCortes, int larguraCortes,
    int alturaImagem, int larguraImagem, unsigned char* pPixelsImagem);

unsigned char mediaCanalRecorte(coordenadas coordenadasCorte,
    imagem imagemBase, int canal);

int pegarItemLista3D(int x, int y, int z, int tamX, int tamZ);

int main() 
{ 
    unsigned char lista[] = 
    {165, 11, 125, 45, 244, 31, 74, 195, 118, 208, 164, 119, 23, 230, 64, 84, 210, 60, 
    200, 9, 65, 13, 233, 196, 104, 39, 162, 174, 75, 235, 30, 251, 248, 106, 107, 
    224, 206, 116, 1, 92, 89, 215, 109, 49, 250, 192, 67, 93, 38, 129, 167, 66, 77, 
    247, 176, 178, 86, 147, 26, 148, 15, 98, 213, 73, 226, 253, 135, 231, 124, 137, 
    18, 29, 218, 189, 207, 236, 90, 42, 12, 56, 54, 199, 82, 110, 149, 241, 143, 
    246, 71, 120, 8, 223, 163, 216, 91, 252, 102, 190, 171, 201, 254, 188, 166, 34, 
    103, 59, 105, 134, 173, 181, 132, 55, 249, 211, 19, 43, 70, 108, 17, 136, 234, 
    193, 168, 194, 83, 79, 128, 96, 131, 203, 80, 212, 78, 209, 227, 3, 187, 95, 
    88, 40, 228, 0, 100, 101, 144, 48, 36, 232, 58, 182, 14, 10, 32, 94, 150, 16, 
    44, 51, 22, 217, 112, 25, 46, 126, 141, 185, 142, 53, 97, 221, 202, 47, 27, 20, 
    5, 186, 177, 139, 146, 204, 153, 198, 41, 114, 72, 87, 237, 160, 4, 21, 222, 6};

    /*
    {{165, 11, 125}, {45, 244, 31}, {74, 195, 118}, {208, 164, 119}, {23, 230, 64}, {84, 210, 60}, {200, 9, 65}, {13, 233, 196}},
    {{104, 39, 162}, {174, 75, 235}, {30, 251, 248}, {106, 107, 224}, {206, 116, 1}, {92, 89, 215}, {109, 49, 250}, {192, 67, 93}},
    {{38, 129, 167}, {66, 77, 247}, {176, 178, 86}, {147, 26, 148}, {15, 98, 213}, {73, 226, 253}, {135, 231, 124}, {137, 18, 29}},
    {{218, 189, 207}, {236, 90, 42}, {12, 56, 54}, {199, 82, 110}, {149, 241, 143}, {246, 71, 120}, {8, 223, 163}, {216, 91, 252}},
    {{102, 190, 171}, {201, 254, 188}, {166, 34, 103}, {59, 105, 134}, {173, 181, 132}, {55, 249, 211}, {19, 43, 70}, {108, 17, 136}},
    {{234, 193, 168}, {194, 83, 79}, {128, 96, 131}, {203, 80, 212}, {78, 209, 227}, {3, 187, 95}, {88, 40, 228}, {0, 100, 101}},
    {{144, 48, 36}, {232, 58, 182}, {14, 10, 32}, {94, 150, 16}, {44, 51, 22}, {217, 112, 25}, {46, 126, 141}, {185, 142, 53}},
    {{97, 221, 202}, {47, 27, 20}, {5, 186, 177}, {139, 146, 204}, {153, 198, 41}, {114, 72, 87}, {237, 160, 4}, {21, 222, 6}}
    */



    unsigned char* joao = acesso(2, 2, 8, 8, lista);

    for(int i = 0; i < 8 * 8; i++)
        printf("%i \n", joao[i]);

    free(joao);

    return 0; 
}


//vai pegar uma matrix de pixels e o largura e altura de cada Corte
unsigned char* acesso(int alturaCortes, int larguraCortes,
    int alturaImagem, int larguraImagem, unsigned char* pPixelsImagem)
{
    //fica esperto que o pixels é uma mentira
    //na verdade ele é um array com 3 dimensoes
    //sendo a primeira e a segunda altura e largura
    //e a terceira 3 por ser as cores RGB

    //e o retorno vai ser um array normal
    //contendo a media de cada Corte

    /*da um jeito de orderar essa poha*/

    int tamanhoImagem = alturaImagem * larguraImagem;
    int quantidadeCortesX = alturaImagem / alturaCortes;
    int quantidadeCortesY = larguraImagem / larguraCortes;
    int quantidadeCortes = quantidadeCortesX * quantidadeCortesY;

    imagem imagemBase;


    imagemBase.imagem = pPixelsImagem;
    imagemBase.largura = larguraImagem;
    imagemBase.altura = alturaImagem;
    imagemBase.tamanho = tamanhoImagem;

    coordenadas coordenadasCorte; 

    unsigned char* pMediasCortes = (unsigned char*) malloc(sizeof(char) * quantidadeCortes * 3);

    for(int y = 0; y <= quantidadeCortesY; y++)
        for(int x = 0; x <= quantidadeCortesX; x++)
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

            printf("na interacao x: %d y: %d o resultado foi\n", x, y);
            printf("media R: %u, media B: %u, media B: %u\n", mediaCanalR, mediaCanalG, mediaCanalB);

        }

    return pMediasCortes;

}

//pega a media de cores de um canal de uma parte da imagem
unsigned char mediaCanalRecorte(coordenadas coordenadasCorte,
    imagem imagemBase, int canal)
{
    int somaTemporaria = 0;

    for(int y = coordenadasCorte.top; y < coordenadasCorte.botton; y++)
        for(int x = coordenadasCorte.left; x < coordenadasCorte.right; x++)
            somaTemporaria += imagemBase.imagem[pegarItemLista3D(
                x, y, canal, imagemBase.largura, 3)];

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


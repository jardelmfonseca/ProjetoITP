#ifndef DOMINIO_H_INCLUDED
#define DOMINIO_H_INCLUDED

typedef struct color{
  int r,g,b;
}Color;

typedef struct pixel{
  Color color;
}Pixel;

typedef struct imagem{
  char identificador[2];
  int largura;
  int altura;
  int valorMaximo;
  Pixel **pixels;

}Imagem;

typedef struct coordenada{
 double x;
 double y;
}Coordenada;

#endif // DOMINIO_H_INCLUDED

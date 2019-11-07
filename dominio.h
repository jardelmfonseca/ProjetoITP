#ifndef DOMINIO_H_INCLUDED
#define DOMINIO_H_INCLUDED



typedef struct pixel{
  int r,g,b;
}Pixel;

typedef struct imagem{
  char identificador[2];
  Pixel pixels[3][2];
  int valorMaximo;

}Imagem;

#endif // DOMINIO_H_INCLUDED

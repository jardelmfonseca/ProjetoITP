#include <stdlib.h>
#include <gtk/gtk.h>
#include "dominio.h"

int largura=800,altura=600;
Color corPadrao={255,255,255};
Imagem imagem;
Imagem * pontImagem=&imagem;

void save(){

    FILE *file = fopen("imagem.ppm","w");
    fprintf(file,"%s\n%d %d\n%d\n",(*pontImagem).identificador,(*pontImagem).largura,(*pontImagem).altura,(*pontImagem).valorMaximo);
   int linha=0;
    int coluna=0;
    for(linha=0;linha<imagem.altura;linha++){
        for(coluna=0;coluna<imagem.largura;coluna++){
            fprintf(file,"%d %d %d\n",(*pontImagem).pixels[linha][coluna].color.r,(*pontImagem).pixels[linha][coluna].color.g,(*pontImagem).pixels[linha][coluna].color.b);
        }
    }
    fclose(file);

}



void newImagem(){

// alocando a matriz de pixel com tamanho dinamico
Pixel **matrixPixel;
   matrixPixel = malloc (altura * sizeof (Pixel *)); // alocando o numeto de linhas/altura
   int i;
   for (i=0; i < altura; ++i){// percorrendo as linhas e alocando a altura
     matrixPixel[i] = malloc (largura * sizeof (Pixel));
   }

    Pixel pixelPadrao= {corPadrao};

    int lg;
    for(i=0; i< altura;i++){
        for(lg=0;lg<largura;lg++){
            matrixPixel[i][lg]=pixelPadrao;
        }
    }


 Imagem im = {{'P','3'},largura,altura,255,matrixPixel};
 imagem=im;

}



int main (int argc, char *argv[]){

  GtkWidget *button = NULL;
  GtkWidget *win = NULL;
  GtkWidget *vbox = NULL;

  /* Initialize GTK+ */
  g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) gtk_false, NULL);
  gtk_init (&argc, &argv);
  g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, g_log_default_handler, NULL);

  /* Create the main window */
  win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_container_set_border_width (GTK_CONTAINER (win), 8);
  gtk_window_set_title (GTK_WINDOW (win), "Projeto ITP");
  gtk_window_set_position (GTK_WINDOW (win), GTK_WIN_POS_CENTER);
  gtk_window_unfullscreen (win);
  gtk_window_set_default_size(win,800,600);
  gtk_widget_realize (win);
  g_signal_connect (win, "destroy", gtk_main_quit, NULL);

  /* Create a vertical box with buttons */
  vbox = gtk_vbox_new (TRUE, 6);
  gtk_container_add (GTK_CONTAINER (win), vbox);

  button = gtk_button_new_from_stock ("Gerar Imagem");
  g_signal_connect (button,"clicked",newImagem,NULL);
  gtk_box_pack_start (GTK_BOX (vbox), button, TRUE, TRUE, 0);

  button = gtk_button_new_from_stock ("Salvar Imagem");
  g_signal_connect (button, "clicked",save,NULL);
  gtk_box_pack_start (GTK_BOX (vbox), button, TRUE, TRUE, 0);

  button = gtk_button_new_from_stock ("Sair");
  g_signal_connect (button, "clicked", gtk_main_quit, NULL);
  gtk_box_pack_start (GTK_BOX (vbox), button, TRUE, TRUE, 0);

  /* Enter the main loop */
  gtk_widget_show_all (win);
  gtk_main ();



  return 0;
}

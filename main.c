#include <stdlib.h>
#include <gtk/gtk.h>
#include "dominio.h"

int largura=3,altura=2;
Color corPadrao={255,255,255};
Imagem imagem;
Imagem * pontImagem=&imagem;

// Caixa de diálogo que apresenta a mensagem de imagem salva
static void dialogSalvar (GtkWidget *wid, GtkWidget *win){
  save();
  GtkWidget *dialog = NULL;

  dialog = gtk_message_dialog_new (GTK_WINDOW (win), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "Imegem salva com sucesso!");
  gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
}

// Caixa de dialogo que apresenta a mensagem imagem gerada
static void dialogGerar (GtkWidget *wid, GtkWidget *win){
  newImagem();
  GtkWidget *dialog = NULL;

  dialog = gtk_message_dialog_new (GTK_WINDOW (win), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "Imegem gerada com sucesso!");
  gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
}

// Função para salver o arquivo de imagem .PPM
void save(){

    FILE *file = fopen("imagem.ppm","w");
    fprintf(file,"%c%c\n%d %d\n%d\n",(*pontImagem).identificador[0],(*pontImagem).identificador[1],(*pontImagem).largura,(*pontImagem).altura,(*pontImagem).valorMaximo);
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
    Pixel cor1={255,0,0};
    Pixel cor2={0,255,0};
    Pixel cor3={0,0,255};
    Pixel cor4={255,255,0};
    Pixel cor5={255,255,255};
    Pixel cor6={0,0,0};
    Pixel cores[6]={cor1,cor2,cor3,cor4,cor5,cor6};

    int lg;
    int corCount=0;
    for(i=0; i< altura;i++){
        for(lg=0;lg<largura;lg++){
            matrixPixel[i][lg]=cores[corCount];
            corCount++;
        }
    }


 Imagem im = {"P3",largura,altura,255,matrixPixel};
 imagem=im;

}



int main (int argc, char *argv[]){

  GtkWidget *button = NULL;
  GtkWidget *win = NULL;
  GtkWidget *vbox = NULL;

  // Inicialização do GTK+
  g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) gtk_false, NULL);
  gtk_init (&argc, &argv);
  g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, g_log_default_handler, NULL);

  // Cria a tela principal no GTK+
  win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_container_set_border_width (GTK_CONTAINER (win), 8);
  gtk_window_set_title (GTK_WINDOW (win), "Projeto ITP");
  gtk_window_set_position (GTK_WINDOW (win), GTK_WIN_POS_CENTER);
  gtk_window_unfullscreen (win);
  gtk_window_set_default_size(win,800,600);
  gtk_widget_realize (win);
  g_signal_connect (win, "destroy", gtk_main_quit, NULL);

  // Cria uma caixa vertical com botões
  vbox = gtk_vbox_new (TRUE, 6);
  gtk_container_add (GTK_CONTAINER (win), vbox);

  // Botão 01 criar imagem
  button = gtk_button_new_from_stock ("Gerar Imagem");
  g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (dialogGerar), (gpointer) win);
  gtk_box_pack_start (GTK_BOX (vbox), button, TRUE, TRUE, 0);

  // Botaão 02 Salvar imagem
  button = gtk_button_new_from_stock ("Salvar Imagem");
  g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (dialogSalvar), (gpointer) win);
  gtk_box_pack_start (GTK_BOX (vbox), button, TRUE, TRUE, 0);

  // Botão 02 sair da aplicação
  button = gtk_button_new_from_stock ("Sair");
  g_signal_connect (button, "clicked", gtk_main_quit, NULL);
  gtk_box_pack_start (GTK_BOX (vbox), button, TRUE, TRUE, 0);

  // Mantem um loop para persistir a janela da aplicação aberta até o encerramento
  gtk_widget_show_all (win);
  gtk_main ();



  return 0;
}

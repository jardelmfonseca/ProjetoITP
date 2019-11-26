#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "dominio.h"

//int largura=3,altura=2;
int largura=800,altura=500,larguraLinha=3;
Color corPadrao={255,255,255};
Color corPincel={0,0,0};
Imagem imagem;
Imagem * pontImagem=&imagem;
int countCoordenadas=0;
Coordenada coordenadas[100];



int countCoordenadasPintar=0;
Coordenada coordenadasPintar[100];

int tipoUltipoDesenho=0;

double ultimaCoordenadaX,ultimaCoordenadaY,ultimaCoordenadaPintarX,ultimaCoordenadaPintarY;

GtkWidget *botaoPintar;
GtkWidget *botaoPoligono;
GtkWidget *botaoCirculo;
GtkWidget *botaoRetangulo;
GtkWidget *botaoCor;
GtkWidget *botaoAbrir;
GtkWidget *botaoNovo;
GtkWidget *botaoSalvar;
GtkWidget *botaoSair;
GtkWidget *areaImagem;

    GtkBuilder *builder;
    GtkWidget  *window;



/* Surface to store current scribbles */
static cairo_surface_t *surface = NULL;
 cairo_t *cr;





void save(GtkWidget *widget, gpointer window){




    GtkWidget *dialog;
    GtkFileFilter *filtro;

    dialog = gtk_file_chooser_dialog_new("Carregar imagem:", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_SAVE, GTK_STOCK_SAVE,GTK_RESPONSE_OK, GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL, NULL);
    //criando filtro de extenï¿½ï¿½o de arquivo
    filtro = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filtro, "*.ppm");
    //adicionando o filtro ao dialog
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filtro);
    // preenche a sugestï¿½o de nome para o arquivo
    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), "imagem.ppm");
    // exibe o dialog
    gtk_widget_show_all(dialog);

    // pegando a resposta
    gint resposta = gtk_dialog_run(GTK_DIALOG(dialog));
    if (resposta == GTK_RESPONSE_OK){
            // aqui vai carregar a imagem selecionada
    char *caminhoArquivo;
    caminhoArquivo = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

    //  printf("%s",caminhoArquivo);
        // salvando a imagem em arquivo
    FILE *file = fopen(caminhoArquivo,"w");
    fprintf(file,"%c%c\n%d %d\n%d\n",(*pontImagem).identificador[0],(*pontImagem).identificador[1],(*pontImagem).largura,(*pontImagem).altura,(*pontImagem).valorMaximo);
   int linha=0;
    int coluna=0;
    for(linha=0;linha<imagem.altura;linha++){
        for(coluna=0;coluna<imagem.largura;coluna++){
            fprintf(file,"%d %d %d\n",(*pontImagem).pixels[linha][coluna].color.r,(*pontImagem).pixels[linha][coluna].color.g,(*pontImagem).pixels[linha][coluna].color.b);
        }
    }
    fclose(file);


    }else{
      // aqui se foi cancelado
    }

    gtk_widget_destroy(dialog);




// coloca imagem no painel
//   painelImagem = gtk_image_new_from_file ("imagem.ppm");
<<<<<<< HEAD
    // exibe dialog de sucesso na operação
  GtkWidget *dialogConfirmacao;
=======
    // exibe dialog de sucesso na operaï¿½ï¿½o
    GtkWidget *dialogConfirmacao;
>>>>>>> refs/remotes/origin/master
  dialogConfirmacao = gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"Arquivo \"Imagem.ppm\" salvo com sucesso!");
  gtk_window_set_title(GTK_WINDOW(dialogConfirmacao), "Aviso");
  gtk_dialog_run(GTK_DIALOG(dialogConfirmacao));
  gtk_widget_destroy(dialogConfirmacao);


}



void image(GtkWidget *widget, gpointer window){

  cairo_t *cr;
  cr = cairo_create (surface);
  cairo_set_source_rgb (cr,corPadrao.r,corPadrao.g,corPadrao.b);
  cairo_paint (cr);
  cairo_destroy (cr);
  gtk_widget_set_size_request (areaImagem, largura,altura);
  gtk_widget_queue_draw_area (areaImagem,0,0,largura,altura);

  countCoordenadas=0;
  ultimaCoordenadaX=0;
  ultimaCoordenadaY=0;
  tipoUltipoDesenho=0;

  ultimaCoordenadaPintarX=0;
  ultimaCoordenadaPintarY=0;

}

<<<<<<< HEAD
// FUNÇÃO PARA ABRIR O DIALOG SELETOR DE CORES
void color () {
=======
/* FUNCAO PARA ABRIR O DIALOG SELETOR DE CORES */
void selecionarCor () {
>>>>>>> refs/remotes/origin/master

  GtkWidget *dialog;
  GtkResponseType *tipoResultadoSelecao;
  GtkColorSelection *corSelecionada;
  GdkColor corGdk;
  dialog = gtk_color_selection_dialog_new ("Cor da Linha");
<<<<<<< HEAD

  corSelecionada = GTK_COLOR_SELECTION (gtk_color_selection_dialog_get_color_selection (GTK_COLOR_SELECTION_DIALOG (dialog)));

  gtk_color_selection_set_previous_color (corSelecionada, &corGdk);
  gtk_color_selection_set_current_color (corSelecionada, &corGdk);
  gtk_color_selection_set_has_palette (corSelecionada, TRUE);




  tipoResultadoSelecao = gtk_dialog_run (GTK_DIALOG (dialog));
=======
>>>>>>> refs/remotes/origin/master

  corSelecionada = GTK_COLOR_SELECTION (gtk_color_selection_dialog_get_color_selection (GTK_COLOR_SELECTION_DIALOG (dialog)));

  gtk_color_selection_set_previous_color (corSelecionada, &corGdk);
  gtk_color_selection_set_current_color (corSelecionada, &corGdk);
  gtk_color_selection_set_has_palette (corSelecionada, TRUE);

  tipoResultadoSelecao = gtk_dialog_run(GTK_DIALOG (dialog));

  // pega o resultado da selecao da cor
  if(tipoResultadoSelecao == GTK_RESPONSE_OK){
<<<<<<< HEAD
=======

    gtk_color_selection_get_current_color(corSelecionada,&corGdk);
>>>>>>> refs/remotes/origin/master

     gtk_color_selection_get_current_color (corSelecionada,&corGdk);

    printf("Passou aqui");

    // alterarndo a cor da linha
<<<<<<< HEAD
    corPincel.r=(int)corGdk.red/257;
    corPincel.g=(int)corGdk.green/257;
    corPincel.b=(int)corGdk.blue/257;
=======
    corPincel.r=(int) corGdk.red/257;
    corPincel.g=(int) corGdk.green/257;
    corPincel.b=(int) corGdk.blue/257;
>>>>>>> refs/remotes/origin/master

  }

  gtk_widget_destroy(dialog);

}

<<<<<<< HEAD
// essa função abre uma janela para a seleção da imagem e carrega a mesma no sistema
void open (GtkButton* button, gpointer user_data){
=======
// essa funcao abre uma janela para a selecao da imagem e ela carrega no sistema
void abrirImagem (GtkButton* button, gpointer user_data){
>>>>>>> refs/remotes/origin/master


    GtkWidget *dialog;
    GtkFileFilter *filtro;

<<<<<<< HEAD
    dialog = gtk_file_chooser_dialog_new("Carregar imagem:", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_OPEN,GTK_RESPONSE_ACCEPT, GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL, NULL);
    //criando filtro de extenção de arquivo
=======
    dialog = gtk_file_chooser_dialog_new("Carregar imagem:", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_OPEN,GTK_RESPONSE_OK, GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL, NULL);
    //criando filtro de extencao de arquivo
>>>>>>> refs/remotes/origin/master
    filtro = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filtro, "*.ppm");
    //adicionando o filtro ao dialog
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filtro);
    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), "imagem.cpp");
    gtk_widget_show_all(dialog);

    // pegando a resposta
    gint resposta = gtk_dialog_run(GTK_DIALOG(dialog));
    if (resposta == GTK_RESPONSE_ACCEPT){
            // aqui vai carregar a imagem selecionada




   int countLinhaMatriz=0;
   int countColunaMatriz=0;
   cairo_t *cr;


          // aqui vai carregar a imagem selecionada
    char *caminhoArquivo;
    caminhoArquivo = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
    gtk_widget_destroy(dialog);

     FILE *file = fopen(caminhoArquivo,"r");
    if(file == NULL){
     printf("nao foi possivel carregar a imagem!");
    }else{


     // percorrendo e lendo o arquivo

     fscanf(file,"%s",imagem.identificador);
     fscanf(file,"%i %i",&imagem.largura,&imagem.altura);
     fscanf(file,"%i",&imagem.valorMaximo);

     printf("%s\n",imagem.identificador);
     printf("%i %i\n",imagem.largura,imagem.altura);
     largura=imagem.largura;
     altura=imagem.altura;
     printf("%i\n",imagem.valorMaximo);



     imagem.pixels = malloc (imagem.altura * sizeof (Pixel *)); // alocando o numeto de linhas/altura
     int i;
     for (i=0; i < imagem.altura; ++i){// percorrendo as linhas e alocando a altura
     imagem.pixels[i] = malloc (imagem.largura * sizeof (Pixel));
     }

     int l=0,a=0,r,g,b;
     i=0;
     while(fscanf(file,"%i %i %i",&r,&g,&b) != EOF & i< altura*largura){
         i++;
         imagem.pixels[a][l].color.r=r;
         imagem.pixels[a][l].color.g=g;
         imagem.pixels[a][l].color.b=b;
         //printf("[%i][%i] %i %i %i\n",a,l,imagem.pixels[a][l].color.r,imagem.pixels[a][l].color.g,imagem.pixels[a][l].color.b);
         if(l==imagem.largura-1){
            l=0;
            a++;
         }else{
            l++;
         }

     }
     printf("passou aqui\n");

 //    printf("[%i][%i] %i %i %i\n",a,l,imagem.pixels[a][l].color.r,imagem.pixels[a][l].color.g,imagem.pixels[a][l].color.b);
    fclose(file);
    }
    }else{
      // aqui se foi cancelado
    }

<<<<<<< HEAD
    //newImagem(G_OBJECT(botaoNovo),(gpointer) window);
    //gtk_widget_queue_draw_area (areaImagem,0,0,largura,altura);





=======
    fclose(file);
    }

        g_print("%s\n", gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)));
    }else{
      // aqui se foi cancelado
    }
>>>>>>> refs/remotes/origin/master

  int x=0,y=0;
       for(y=0;y<imagem.altura;y++){
        for(x=0;x<imagem.largura;x++){
            cairo_t *cr;
            cr = cairo_create (surface);
            cairo_set_line_width (cr, 1);
            cairo_set_source_rgb (cr,imagem.pixels[y][x].color.r,imagem.pixels[y][x].color.g,imagem.pixels[y][x].color.b);
            cairo_rectangle (cr,x,y, 1,1);
            cairo_stroke (cr);
            cairo_destroy(cr);

            // atualiza o componente gtk
            gtk_widget_queue_draw_area (areaImagem,x,y,largura,altura);

        }
     }




}



void fill(){

cairo_t *cr;
    switch(tipoUltipoDesenho){

case 1:

    cr = cairo_create (surface);
    cairo_set_line_width (cr, larguraLinha);
    cairo_set_source_rgb (cr,corPincel.r,corPincel.g,corPincel.b);

    int ite;
    for(ite=0;ite<countCoordenadasPintar;ite++){
       if(ite==0){
        cairo_move_to (cr, ultimaCoordenadaPintarX, ultimaCoordenadaPintarY);
        cairo_line_to (cr, coordenadas[ite].x, coordenadas[ite].y);
       }else{
        cairo_line_to (cr, coordenadas[ite].x, coordenadas[ite].y);
       }

    }

    // fecha com a coordenada inicial
    cairo_close_path (cr);
    cairo_fill(cr);
    //cairo_line_to (cr,cordenadaX+50,cordenadaY);
    cairo_stroke (cr);

    // atualiza o componente gtk
    gtk_widget_queue_draw_area (areaImagem,0,0,largura,altura);

    countCoordenadasPintar=0;
    ultimaCoordenadaPintarX=0;
    ultimaCoordenadaPintarY=0;
    tipoUltipoDesenho=0;
    break;
case 2:
    cr = cairo_create (surface);
    cairo_set_line_width (cr, larguraLinha);
    cairo_set_source_rgb (cr,corPincel.r,corPincel.g,corPincel.b);
    cairo_move_to (cr, ultimaCoordenadaPintarX, ultimaCoordenadaPintarY);
    cairo_arc (cr, ultimaCoordenadaPintarX, ultimaCoordenadaPintarY, 100, -1 * 3.14, 1 * 3.14);
    cairo_fill (cr);
    //cairo_line_to (cr,cordenadaX+50,cordenadaY);
    cairo_stroke (cr);
    // atualiza o componente gtk
    gtk_widget_queue_draw_area (areaImagem,0,0,largura,altura);

    countCoordenadasPintar=0;
    ultimaCoordenadaPintarX=0;
    ultimaCoordenadaPintarY=0;
    tipoUltipoDesenho=0;
    break;
case 3:
    cr = cairo_create (surface);
    cairo_set_line_width (cr, larguraLinha);
    cairo_set_source_rgb (cr, corPincel.r,corPincel.g,corPincel.b);
    cairo_rectangle (cr,ultimaCoordenadaPintarX,ultimaCoordenadaPintarY, 100, 100);
    cairo_fill (cr);
    cairo_stroke (cr);

    // atualiza o componente gtk
    gtk_widget_queue_draw_area (areaImagem,0,0,largura,altura);

    countCoordenadasPintar=0;
    ultimaCoordenadaPintarX=0;
    ultimaCoordenadaPintarY=0;
    break;

    }

}


void polygon(){
    cairo_t *cr;
    cr = cairo_create (surface);
    cairo_set_line_width (cr, larguraLinha);
    cairo_set_source_rgb (cr,corPincel.r,corPincel.g,corPincel.b);

    int ite;
    for(ite=0;ite<countCoordenadas;ite++){
       if(ite==0){
        cairo_move_to (cr, ultimaCoordenadaX, ultimaCoordenadaY);
        cairo_line_to (cr, coordenadas[ite].x, coordenadas[ite].y);
       }else{
        cairo_line_to (cr, coordenadas[ite].x, coordenadas[ite].y);
       }

    }

    // fecha com a coordenada inicial
    cairo_close_path (cr);
    //cairo_line_to (cr,cordenadaX+50,cordenadaY);
    cairo_stroke (cr);

    // atualiza o componente gtk
    gtk_widget_queue_draw_area (areaImagem,0,0,largura,altura);

    countCoordenadasPintar=countCoordenadas;
    ultimaCoordenadaPintarX=ultimaCoordenadaX;
    ultimaCoordenadaPintarY=ultimaCoordenadaY;
    countCoordenadas=0;
    ultimaCoordenadaX=0;
    ultimaCoordenadaY=0;
    tipoUltipoDesenho=1;


}

void circle(){
    cairo_t *cr;
    cr = cairo_create (surface);
    cairo_set_line_width (cr, larguraLinha);
    cairo_set_source_rgb (cr,corPincel.r,corPincel.g,corPincel.b);
    cairo_move_to (cr, ultimaCoordenadaX, ultimaCoordenadaY);
    cairo_arc (cr, ultimaCoordenadaX, ultimaCoordenadaY, 100, -1 * 3.14, 1 * 3.14);

    cairo_stroke (cr);
    // atualiza o componente gtk
    gtk_widget_queue_draw_area (areaImagem,0,0,largura,altura);

    countCoordenadasPintar=countCoordenadas;
    ultimaCoordenadaPintarX=ultimaCoordenadaX;
    ultimaCoordenadaPintarY=ultimaCoordenadaY;

    countCoordenadas=0;
    ultimaCoordenadaX=0;
    ultimaCoordenadaY=0;
    tipoUltipoDesenho=2;

}

void rect(){
    cairo_t *cr;
    cr = cairo_create (surface);
    cairo_set_line_width (cr, larguraLinha);
    cairo_set_source_rgb (cr, corPincel.r,corPincel.g,corPincel.b);
    cairo_rectangle (cr,ultimaCoordenadaX,ultimaCoordenadaY, 100, 100);
    cairo_stroke (cr);

    // atualiza o componente gtk
    gtk_widget_queue_draw_area (areaImagem,0,0,largura,altura);

    countCoordenadasPintar=countCoordenadas;
    ultimaCoordenadaPintarX=ultimaCoordenadaX;
    ultimaCoordenadaPintarY=ultimaCoordenadaY;

    countCoordenadas=0;
    ultimaCoordenadaX=0;
    ultimaCoordenadaY=0;
    tipoUltipoDesenho=3;

}




static void clear (void){
  cairo_t *cr;
  cr = cairo_create (surface);
  cairo_set_source_rgb (cr,corPadrao.r,corPadrao.g,corPadrao.b);
  cairo_paint (cr);
  cairo_destroy (cr);
  gtk_widget_set_size_request (areaImagem, largura,altura);

    countCoordenadas=0;
    ultimaCoordenadaX=0;
    ultimaCoordenadaY=0;
    tipoUltipoDesenho=0;
    countCoordenadasPintar=0;
    ultimaCoordenadaPintarX=0;
    ultimaCoordenadaPintarY=0;

}

<<<<<<< HEAD
// configura comportamento do click
=======
/* Cria uma nova superficie de pintura para armazenar as linhas */
>>>>>>> refs/remotes/origin/master
static gboolean configure_event_cb (GtkWidget*widget,GdkEventConfigure *event,gpointer data){
  if (surface)
    cairo_surface_destroy (surface);

  surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget),CAIRO_CONTENT_COLOR,gtk_widget_get_allocated_width (widget),gtk_widget_get_allocated_height (widget));

<<<<<<< HEAD

  clear ();
=======
  /* Inicializa uma nova superficie de pintura em branco */
  limpaImagem ();

>>>>>>> refs/remotes/origin/master

  return TRUE;
}

<<<<<<< HEAD
// pinta cordenada clicada
=======
/* Desenha a tela de pintura.
 * recebe um objeto cairo_t pronto para uso
 */
>>>>>>> refs/remotes/origin/master
static gboolean draw_cb (GtkWidget *widget,cairo_t *cr,gpointer data){
    cairo_set_source_rgb (cr,corPadrao.r,corPadrao.g,corPadrao.b);
    cairo_set_source_surface (cr, surface, 0, 0);
    cairo_paint(cr);
    return FALSE;
}


// faz a varredura da tela pintando o fundo da imagem
static void draw_brush (GtkWidget *widget,gdouble x,gdouble y){


  cairo_t *cr;
  cr = cairo_create (surface);
  cairo_set_source_rgb (cr, corPincel.r,corPincel.g,corPincel.b);
  cairo_rectangle (cr, x - larguraLinha, y - larguraLinha, larguraLinha, larguraLinha);
  cairo_fill (cr);
  cairo_destroy (cr);


  gtk_widget_queue_draw_area (widget, x - larguraLinha, y - larguraLinha, larguraLinha, larguraLinha);

  //armazena ultima cordenada do click
  Coordenada c={x-larguraLinha,y-larguraLinha};
  coordenadas[countCoordenadas]=c;
  countCoordenadas++;
  ultimaCoordenadaX=x-larguraLinha;
  ultimaCoordenadaY=y-larguraLinha;
}


// evento do click
static gboolean button_press_event_cb (GtkWidget *widget,GdkEventButton *event,gpointer data){

  if (surface == NULL)
    return FALSE;

  if (event->button == GDK_BUTTON_PRIMARY){
      draw_brush (widget, event->x, event->y);
    }else if (event->button == GDK_BUTTON_SECONDARY){
      clear();
      gtk_widget_queue_draw (widget);
    }


  return TRUE;
}


// escuta evento do mouse
static gboolean motion_notify_event_cb (GtkWidget *widget,GdkEventMotion *event,gpointer data){

  if (surface == NULL)
    return FALSE;

  if (event->state & GDK_BUTTON1_MASK)
    draw_brush (widget, event->x, event->y);

<<<<<<< HEAD
=======

>>>>>>> refs/remotes/origin/master
  return TRUE;
}




int main(int argc, char *argv[]){

   Pixel **matrixPixel;
   matrixPixel = malloc (altura * sizeof (Pixel *)); // alocando o numeto de linhas/altura
   int i;
   for (i=0; i < altura; ++i){// percorrendo as linhas e alocando a altura
     matrixPixel[i] = malloc (largura * sizeof (Pixel));
   }

   free(matrixPixel);

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "ProjetoITP.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "janelaPrincipal"));
    gtk_builder_connect_signals(builder, NULL);

    botaoPintar = GTK_WIDGET(gtk_builder_get_object(builder, "botaoPintar"));
    g_signal_connect (botaoPintar, "clicked",fill, NULL);

    botaoPoligono = GTK_WIDGET(gtk_builder_get_object(builder, "botaoPoligono"));
    g_signal_connect (botaoPoligono, "clicked", polygon, NULL);

    botaoCirculo = GTK_WIDGET(gtk_builder_get_object(builder, "botaoCirculo"));
    g_signal_connect (botaoCirculo, "clicked",circle, NULL);

    botaoRetangulo = GTK_WIDGET(gtk_builder_get_object(builder, "botaoRetangulo"));
    g_signal_connect (botaoRetangulo, "clicked",rect, NULL);

    botaoCor = GTK_WIDGET(gtk_builder_get_object(builder, "botaoCor"));
    g_signal_connect (botaoCor, "clicked",color, NULL);

    botaoAbrir = GTK_WIDGET(gtk_builder_get_object(builder, "botaoAbrir"));
    g_signal_connect(G_OBJECT(botaoAbrir), "clicked", G_CALLBACK(open), (gpointer) window);

    botaoNovo = GTK_WIDGET(gtk_builder_get_object(builder, "botaoNovo"));
    g_signal_connect(G_OBJECT(botaoNovo), "clicked", G_CALLBACK(image), (gpointer) window);

    botaoSalvar = GTK_WIDGET(gtk_builder_get_object(builder, "botaoSalvar"));
    g_signal_connect(G_OBJECT(botaoSalvar), "clicked", G_CALLBACK(save), (gpointer) window);

    botaoSair = GTK_WIDGET(gtk_builder_get_object(builder, "botaoSair"));
    g_signal_connect (botaoSair, "clicked", gtk_main_quit, NULL);

    areaImagem = GTK_WIDGET(gtk_builder_get_object(builder, "areaImagem"));
<<<<<<< HEAD

=======
  /*areaImagem = gtk_drawing_area_new ();
   * define um tamanho minimo */
>>>>>>> refs/remotes/origin/master
    gtk_widget_set_size_request (areaImagem, largura,altura);



  g_signal_connect (areaImagem, "draw",G_CALLBACK (draw_cb), NULL);
  g_signal_connect (areaImagem,"configure-event",G_CALLBACK (configure_event_cb), NULL);
  g_signal_connect (areaImagem, "motion-notify-event",G_CALLBACK (motion_notify_event_cb), NULL);
  g_signal_connect (areaImagem, "button-press-event",G_CALLBACK (button_press_event_cb), NULL);


  gtk_widget_set_events (areaImagem, gtk_widget_get_events (areaImagem)| GDK_BUTTON_PRESS_MASK| GDK_POINTER_MOTION_MASK);


    g_object_unref(builder);

    gtk_widget_show(window);
    gtk_main();


  return 0;
}




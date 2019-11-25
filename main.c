#include <stdio.h>
#include <stdlib.h>
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
static cairo_t *cr;





void save(GtkWidget *widget, gpointer window){



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
            if(corCount<6){
            matrixPixel[i][lg]=cores[corCount];
            }else{
            matrixPixel[i][lg]=pixelPadrao;
            }
            corCount++;
        }
    }


 Imagem im = {"P3",largura,altura,255,matrixPixel};
 imagem=im;


     GtkWidget *dialog;
    GtkFileFilter *filtro;

    dialog = gtk_file_chooser_dialog_new("Carregar imagem:", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_SAVE, GTK_STOCK_SAVE,GTK_RESPONSE_OK, GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL, NULL);
    //criando filtro de exten��o de arquivo
    filtro = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filtro, "*.ppm");
    //adicionando o filtro ao dialog
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filtro);
    // preenche a sugest�o de nome para o arquivo
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
    // exibe dialog de sucesso na opera��o
    GtkWidget *dialogConfirmacao;
  dialogConfirmacao = gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"Arquivo \"Imagem.ppm\" salvo com sucesso!");
  gtk_window_set_title(GTK_WINDOW(dialogConfirmacao), "Aviso");
  gtk_dialog_run(GTK_DIALOG(dialogConfirmacao));
  gtk_widget_destroy(dialogConfirmacao);


}



void newImagem(GtkWidget *widget, gpointer window){
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

/* FUNCAO PARA ABRIR O DIALOG SELETOR DE CORES */
void selecionarCor () {

  GtkWidget *dialog;
  GtkResponseType *tipoResultadoSelecao;
  GtkColorSelection *corSelecionada;
  GdkColor corGdk;
  dialog = gtk_color_selection_dialog_new ("Cor da Linha");

  corSelecionada = GTK_COLOR_SELECTION (gtk_color_selection_dialog_get_color_selection (GTK_COLOR_SELECTION_DIALOG (dialog)));

  gtk_color_selection_set_previous_color (corSelecionada, &corGdk);
  gtk_color_selection_set_current_color (corSelecionada, &corGdk);
  gtk_color_selection_set_has_palette (corSelecionada, TRUE);

  tipoResultadoSelecao = gtk_dialog_run(GTK_DIALOG (dialog));

  // pega o resultado da selecao da cor
  if(tipoResultadoSelecao == GTK_RESPONSE_OK){

    gtk_color_selection_get_current_color(corSelecionada,&corGdk);

    printf("%d",(int)corGdk.red);

    // alterarndo a cor da linha
    corPincel.r=(int) corGdk.red/257;
    corPincel.g=(int) corGdk.green/257;
    corPincel.b=(int) corGdk.blue/257;

  }

  gtk_widget_destroy(dialog);

}

// essa funcao abre uma janela para a selecao da imagem e ela carrega no sistema
void abrirImagem (GtkButton* button, gpointer user_data){


    GtkWidget *dialog;
    GtkFileFilter *filtro;

    dialog = gtk_file_chooser_dialog_new("Carregar imagem:", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_OPEN,GTK_RESPONSE_OK, GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL, NULL);
    //criando filtro de extencao de arquivo
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



   Pixel **matrixPixel;


          // aqui vai carregar a imagem selecionada
    char *caminhoArquivo;
    caminhoArquivo = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

     FILE *file = fopen(caminhoArquivo,"r");
    if(file == NULL){
     printf("nao foi possivel carregar a imagem!");
    }else{
    char tmp[11];

    int itr=0;
    while(fgets(tmp,11,file) != NULL){

    switch(itr){
  case 0:
    break;
  case 1:
/*
 int c=0;
 int qtd=0;
 int vlrAlt=0;
  while(str[c]!='\0'){
        if(c!=' '){
			switch(str[c]){
				case '0':
					qtd+=N[0];
					break;
				case '1':
					qtd+=N[1];
				  	break;
				case '2':
					qtd+=N[2];
				  	break;
				case '3':
					qtd+=N[3];
				  	break;
				case '4':
					qtd+=N[4];
				  	break;
				case '5':
					qtd+=N[5];
				  	break;
				case '6':
					qtd+=N[6];
				  	break;
				case '7':
					qtd+=N[7];
				  	break;
				case '8':
					qtd+=N[8];
				  	break;
				case '9':
					qtd+=N[9];
				  	break;
				default:
					break;
			}
			}else{

			}
      c++;

  }
/*

    int lg;
    int corCount=0;
    for(i=0; i< altura;i++){
        for(lg=0;lg<largura;lg++){
            if(corCount<6){
            matrixPixel[i][lg]=cores[corCount];
            }else{
            matrixPixel[i][lg]=pixelPadrao;
            }
            corCount++;
        }
    }


 */


      //num = strtol(cNum, NULL, 10);
    break;
case 2:
    break;
default:
    break;

    }
    itr++;


    }

    fclose(file);
    }

        g_print("%s\n", gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)));
    }else{
      // aqui se foi cancelado
    }

    gtk_widget_destroy(dialog);

}



void pintar(){

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


void desenharPoligono(){

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

void desenharCirculo(){
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

void desenharRetangulo(){
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




static void limpaImagem (void){
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

/* Cria uma nova superficie de pintura para armazenar as linhas */
static gboolean configure_event_cb (GtkWidget*widget,GdkEventConfigure *event,gpointer data){
  if (surface)
    cairo_surface_destroy (surface);

  surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget),CAIRO_CONTENT_COLOR,gtk_widget_get_allocated_width (widget),gtk_widget_get_allocated_height (widget));

  /* Inicializa uma nova superficie de pintura em branco */
  limpaImagem ();


  return TRUE;
}

/* Desenha a tela de pintura.
 * recebe um objeto cairo_t pronto para uso
 */
static gboolean draw_cb (GtkWidget *widget,cairo_t *cr,gpointer data){
    cairo_set_source_rgb (cr,corPadrao.r,corPadrao.g,corPadrao.b);
    cairo_set_source_surface (cr, surface, 0, 0);
    cairo_paint(cr);
    return FALSE;
}

/* Draw a rectangle on the surface at the given position */
static void draw_brush (GtkWidget *widget,gdouble x,gdouble y){
  /* Paint to the surface, where we store our state */
  cr = cairo_create (surface);
  cairo_set_source_rgb (cr, corPincel.r,corPincel.g,corPincel.b);
  cairo_rectangle (cr, x - larguraLinha, y - larguraLinha, larguraLinha, larguraLinha);
  cairo_fill (cr);
  cairo_destroy (cr);

  /* Now invalidate the affected region of the drawing area. */
  gtk_widget_queue_draw_area (widget, x - larguraLinha, y - larguraLinha, larguraLinha, larguraLinha);

  //armazena ultima cordenada do click
  Coordenada c={x-larguraLinha,y-larguraLinha};
  coordenadas[countCoordenadas]=c;
  countCoordenadas++;
  ultimaCoordenadaX=x-larguraLinha;
  ultimaCoordenadaY=y-larguraLinha;
}

/* Handle button press events by either drawing a rectangle
 * or clearing the surface, depending on which button was pressed.
 * The ::button-press signal handler receives a GdkEventButton
 * struct which contains this information.
 */
static gboolean button_press_event_cb (GtkWidget *widget,GdkEventButton *event,gpointer data){
  /* paranoia check, in case we haven't gotten a configure event */
  if (surface == NULL)
    return FALSE;

  if (event->button == GDK_BUTTON_PRIMARY){
      draw_brush (widget, event->x, event->y);
    }else if (event->button == GDK_BUTTON_SECONDARY){
      limpaImagem();
      gtk_widget_queue_draw (widget);
    }


  return TRUE;
}

/* Handle motion events by continuing to draw if button 1 is
 * still held down. The ::motion-notify signal handler receives
 * a GdkEventMotion struct which contains this information.
 */
static gboolean motion_notify_event_cb (GtkWidget *widget,GdkEventMotion *event,gpointer data){

  if (surface == NULL)
    return FALSE;

  if (event->state & GDK_BUTTON1_MASK)
    draw_brush (widget, event->x, event->y);


  return TRUE;
}




int main(int argc, char *argv[]){


    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "ProjetoITP.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "janelaPrincipal"));
    gtk_builder_connect_signals(builder, NULL);

    botaoPintar = GTK_WIDGET(gtk_builder_get_object(builder, "botaoPintar"));
    g_signal_connect (botaoPintar, "clicked",pintar, NULL);

    botaoPoligono = GTK_WIDGET(gtk_builder_get_object(builder, "botaoPoligono"));
    g_signal_connect (botaoPoligono, "clicked", desenharPoligono, NULL);

    botaoCirculo = GTK_WIDGET(gtk_builder_get_object(builder, "botaoCirculo"));
    g_signal_connect (botaoCirculo, "clicked",desenharCirculo, NULL);

    botaoRetangulo = GTK_WIDGET(gtk_builder_get_object(builder, "botaoRetangulo"));
    g_signal_connect (botaoRetangulo, "clicked",desenharRetangulo, NULL);

    botaoCor = GTK_WIDGET(gtk_builder_get_object(builder, "botaoCor"));
    g_signal_connect (botaoCor, "clicked",selecionarCor, NULL);

    botaoAbrir = GTK_WIDGET(gtk_builder_get_object(builder, "botaoAbrir"));
    g_signal_connect(G_OBJECT(botaoAbrir), "clicked", G_CALLBACK(abrirImagem), (gpointer) window);

    botaoNovo = GTK_WIDGET(gtk_builder_get_object(builder, "botaoNovo"));
    g_signal_connect(G_OBJECT(botaoNovo), "clicked", G_CALLBACK(newImagem), (gpointer) window);

    botaoSalvar = GTK_WIDGET(gtk_builder_get_object(builder, "botaoSalvar"));
    g_signal_connect(G_OBJECT(botaoSalvar), "clicked", G_CALLBACK(save), (gpointer) window);

    botaoSair = GTK_WIDGET(gtk_builder_get_object(builder, "botaoSair"));
    g_signal_connect (botaoSair, "clicked", gtk_main_quit, NULL);

    areaImagem = GTK_WIDGET(gtk_builder_get_object(builder, "areaImagem"));
  /*areaImagem = gtk_drawing_area_new ();
   * define um tamanho minimo */
    gtk_widget_set_size_request (areaImagem, largura,altura);


  /* Signals used to handle the backing surface */
  g_signal_connect (areaImagem, "draw",G_CALLBACK (draw_cb), NULL);
  g_signal_connect (areaImagem,"configure-event",G_CALLBACK (configure_event_cb), NULL);

  /* Event signals */
  g_signal_connect (areaImagem, "motion-notify-event",G_CALLBACK (motion_notify_event_cb), NULL);
  g_signal_connect (areaImagem, "button-press-event",G_CALLBACK (button_press_event_cb), NULL);

  /* Ask to receive events the drawing area doesn't normally
   * subscribe to. In particular, we need to ask for the
   * button press and motion notify events that want to handle.
   */
  gtk_widget_set_events (areaImagem, gtk_widget_get_events (areaImagem)| GDK_BUTTON_PRESS_MASK| GDK_POINTER_MOTION_MASK);

 //   painelImagem = GTK_WIDGET(gtk_builder_get_object(builder, "painelImagem"));
    //painelImagem = gtk_image_new_from_file ("magem_circle.png");
//    gtk_image_set_from_file (GTK_IMAGE (painelImagem), "magem_circle.png");
  //  gtk_image_set_from_file (GTK_IMAGE (painelImagem),"imagem_circle.png");

    g_object_unref(builder);

    gtk_widget_show(window);
    gtk_main();


  return 0;
}




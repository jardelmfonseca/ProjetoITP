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
Coordenada coordenadas[20];

int countCoordenadasPintar=0;
Coordenada coordenadasPintar[20];

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

 // coloca imagem no painel
 //  painelImagem = gtk_image_new_from_file ("imagem.ppm");




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
// coloca imagem no painel
//   painelImagem = gtk_image_new_from_file ("imagem.ppm");
    // exibe dialog de sucesso na operação
    GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"Arquivo \"Imagem.ppm\" salvo com sucesso!");
  gtk_window_set_title(GTK_WINDOW(dialog), "Aviso");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);


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


void abrirImagem (GtkButton* button, gpointer user_data){
	GtkWidget *image = GTK_WIDGET (user_data);
	GtkWidget *toplevel = gtk_widget_get_toplevel (image);
	GtkFileFilter *filtro = gtk_file_filter_new ();
	gtk_file_filter_add_pattern(filtro,".ccp");
	GtkWidget *dialog = gtk_file_chooser_dialog_new (("Abrir imagem"),GTK_WINDOW (toplevel),GTK_FILE_CHOOSER_ACTION_OPEN,GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,NULL);

	gtk_file_filter_add_pixbuf_formats (filtro);
	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog),filtro);

	switch (gtk_dialog_run (GTK_DIALOG (dialog))){
		case GTK_RESPONSE_ACCEPT:
		    {
            gchar *filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
			gtk_image_set_from_file (GTK_IMAGE (image), filename);
			break;
		    }

		default:
			break;
	}
	gtk_widget_destroy (dialog);
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

/* Create a new surface of the appropriate size to store our scribbles */
static gboolean configure_event_cb (GtkWidget*widget,GdkEventConfigure *event,gpointer data){
  if (surface)
    cairo_surface_destroy (surface);

  surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget),CAIRO_CONTENT_COLOR,gtk_widget_get_allocated_width (widget),gtk_widget_get_allocated_height (widget));

  /* Initialize the surface to white */
  limpaImagem ();

  /* We've handled the configure event, no need for further processing. */
  return TRUE;
}

/* Redraw the screen from the surface. Note that the ::draw
 * signal receives a ready-to-be-used cairo_t that is already
 * clipped to only draw the exposed areas of the widget
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

  /* We've handled the event, stop processing */
  return TRUE;
}

/* Handle motion events by continuing to draw if button 1 is
 * still held down. The ::motion-notify signal handler receives
 * a GdkEventMotion struct which contains this information.
 */
static gboolean motion_notify_event_cb (GtkWidget *widget,GdkEventMotion *event,gpointer data){
  /* paranoia check, in case we haven't gotten a configure event */
  if (surface == NULL)
    return FALSE;

  if (event->state & GDK_BUTTON1_MASK)
    draw_brush (widget, event->x, event->y);

  /* We've handled it, stop processing */
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
    g_signal_connect (botaoCor, "clicked", NULL, NULL);

    botaoAbrir = GTK_WIDGET(gtk_builder_get_object(builder, "botaoAbrir"));
    g_signal_connect(G_OBJECT(botaoAbrir), "clicked", G_CALLBACK(abrirImagem), (gpointer) window);

    botaoNovo = GTK_WIDGET(gtk_builder_get_object(builder, "botaoNovo"));
    g_signal_connect(G_OBJECT(botaoNovo), "clicked", G_CALLBACK(newImagem), (gpointer) window);

    botaoSalvar = GTK_WIDGET(gtk_builder_get_object(builder, "botaoSalvar"));
    g_signal_connect(G_OBJECT(botaoSalvar), "clicked", G_CALLBACK(save), (gpointer) window);

    botaoSair = GTK_WIDGET(gtk_builder_get_object(builder, "botaoSair"));
    g_signal_connect (botaoSair, "clicked", gtk_main_quit, NULL);

    areaImagem = GTK_WIDGET(gtk_builder_get_object(builder, "areaImagem"));
    //areaImagem = gtk_drawing_area_new ();
  /* set a minimum size */
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




/*File:helloworld.c
 *Date:2013-11-23
 *Author:sjin
 *Mail:413977243@qq.com
 */
 
/*my first test program*/
#include <gtk/gtk.h>
GtkWidget *label;
GtkWidget *window;
GtkWidget *button;
char str[100];
/* 这是一个回调函数。data 参数在本示例中被忽略。
 * 后面有更多的回调函数示例。*/
void hello( GtkWidget *widget,gpointer   data )
{
    printf("Hello World\n");
}

int main( int   argc, char *argv[] )
{
    /* GtkWidget 是构件的存储类型 */
    
    /* 这个函数在所有的 GTK 程序都要调用。参数由命令行中解析出来并且送到该程序中*/
    gtk_init (&argc, &argv);
        /* 创建一个新窗口 */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
 
    gtk_window_set_title(GTK_WINDOW(window),"My first program helloworld!");
    
    
    g_signal_connect (G_OBJECT (window), "delete_event",G_CALLBACK (gtk_main_quit), NULL);
    
    
    g_signal_connect (G_OBJECT (window), "destroy",G_CALLBACK (gtk_main_quit), NULL);
    
    /* 设置窗口（按钮）边框的宽度。*/
    gtk_container_set_border_width (GTK_CONTAINER (window), 150);
    
    /* 创建一个标签为 "Hello World" 的新按钮。*/
    button = gtk_button_new_with_label ("Hello World");
    
    /* 当按钮收到 "clicked" 信号时会调用 hello() 函数，并将NULL传给
     * 它作为参数。hello() 函数在前面定义了。*/
    g_signal_connect (G_OBJECT (button), "clicked",G_CALLBACK (hello), NULL);
    
    /* 当点击按钮时，会通过调用 gtk_widget_destroy(window) 来关闭窗口。
     * "destroy" 信号会从这里或从窗口管理器发出。*/
    g_signal_connect_swapped (G_OBJECT (button), "clicked", G_CALLBACK (gtk_widget_destroy),window);
    // label=gtk_label_new(str);
    // sprintf(str,"%s","Hello World\n");
    // gtk_container_add(GTK_CONTAINER(button), label); 	// 将按钮放在布局容器里
    // gtk_widget_show(label);
    /* 把按钮放入窗口 (一个 gtk 容器) 中。*/
    gtk_container_add (GTK_CONTAINER (window), button);
    
    /* 最后一步是显示新创建的按钮和窗口 */
    gtk_widget_show (button);
    gtk_widget_show (window);
    
    /* 所有的 GTK 程序必须有一个 gtk_main() 函数。程序运行停在这里
     * 等待事件 (如键盘事件或鼠标事件) 的发生。*/
    gtk_main ();
    
    return 0;
}
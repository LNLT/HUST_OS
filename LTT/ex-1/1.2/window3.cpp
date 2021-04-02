#include <gtk/gtk.h>
#include<pthread.h>

char str[1000];
GtkWidget *label;
GtkWidget *window;
GtkWidget *button;
pthread_t p1;


void *go(void * argc){
    for(int i=10;i<60;i++){
        sleep(1);
        sprintf(str,"数字i的十六进制为%X",i*2);//显示十六进制数
        gtk_label_set_text(GTK_LABEL(label),str);
    }
}

int main (int argc, char *argv[])
{
    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);//生成一个样式为GTK_WINDOW_TOPLEVEL的窗口
    gtk_window_set_title(GTK_WINDOW(window),"window3");//设置窗口标题 
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 400);//设置窗口默认大小
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);//设置窗口在显示器中的位置为居中
    /* GTK_WIN_POS_NONE            ：不固定
       GTK_WIN_POS_CENTER          : 居中
       GTK_WIN_POS_MOUSE           : 出现在鼠标位置
       GTK_WIN_POS_CENTER_ALWAYS   : 窗口改变大小的时候仍然居中
    */
    g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(gtk_main_quit),NULL);//将窗口的delete_event信号绑定处理函数gtk_main_quit
   
    
    label=gtk_label_new(str);//创建标签
    int ret=pthread_create(&p1,NULL,&go,NULL);
    gtk_container_add(GTK_CONTAINER(window), label); 	// 将按钮放在布局容器里

    gtk_widget_show(label);//显示标签
    gtk_widget_show(window);//显示生成的这个窗口
    gtk_main();// 进入消息循环

    return 0;
}
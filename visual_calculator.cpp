#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>
#include <thread>

class pixels{
    public:
        char data;
};
pixels texts[1230];
int write_cols, write_rows = 0;
int draw(int rows,int cols,int process,char data){
    pixels mapping[(rows-1)*cols];
    int i,j,k = 0;
    for (i=0; i < rows-1 ; i++){
        for (j=0 ; j < cols ;j++){
            mapping[j+i*cols].data = '*';
        }
    }
    if (rows > 32 && cols > 42){
        for(i=((cols/2)-20);i <= (cols/2)+20; i++){
            mapping[i+((rows/2)-16)*cols].data = '-';
        }
        for(i=((cols/2)-20);i <= (cols/2)+20; i++){
            mapping[i+((rows/2)+15)*cols].data = '-';
        }
        for (i=((rows)/2)-16;i< (rows/2)+16;i++){
            mapping[(cols/2)-21+i*cols].data = '|';
        }
        for (i=((rows)/2)-16;i< (rows/2)+16;i++){
            mapping[(cols/2)+21+i*cols].data = '|';
        }
        for (i=((rows)/2)-15;i<(rows/2)+15;i++){
            for (j=(cols/2) - 20; j <= (cols/2) + 20 ;j++){
                mapping[j+i*cols].data = texts[k].data;
                k++;
            }
        }
    }
    printf ("\33c\e[3J");
    for (i=0 ; i < rows-1; i++){
        for (j=0 ; j < cols ;j++){
            printf("%c",mapping[j+i*cols].data);
        }
        printf("\r\n");
    }
    return 1;
}
void winsize_to_write(){
    struct winsize winsz;
    ioctl(0,TIOCGWINSZ, &winsz);
    int rows = winsz.ws_row;
    int cols = winsz.ws_col;
    draw(rows,cols,0,0);
}
static void sig_handler(int sig){
    if (SIGWINCH == sig){
        winsize_to_write();
    }
}
void read_input(){
    char key;
    int i, writing = 0;
    for(i=0; i<1230; i++){
        texts[i].data = ' ';
    }
    while (1){
        scanf("%c",&key);
        texts[writing].data = key;
        writing++;
        winsize_to_write();
    }
}
void signal_take(){
    signal(SIGWINCH, sig_handler);
}

int main(){
    std::thread t1(read_input);
    std::thread t2(signal_take);
    system("stty raw");
    while (1){
        pause();
    }
    return 0;
}
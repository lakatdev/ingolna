#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

int main() {
    const char *fb_path = "/dev/fb0";
    int fb_fd = open(fb_path, O_RDWR);
    if (fb_fd == -1) {
        perror("Error opening framebuffer device");
        return 1;
    }

    struct fb_var_screeninfo vinfo;
    if (ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo)) {
        perror("Error reading variable screen info");
        close(fb_fd);
        return 1;
    }

    size_t screensize = vinfo.xres * vinfo.yres * (vinfo.bits_per_pixel / 8);
    printf("Detected resolution: %dx%d, color depth: %d bits\n",
           vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

    unsigned char *fb_data = malloc(screensize);
    if (!fb_data) {
        perror("Error allocating memory");
        close(fb_fd);
        return 1;
    }

    srand(time(NULL));

    for (size_t i = 0; i < screensize; i++) {
        fb_data[i] = rand() % 256;
    }

    ssize_t bytes_written = write(fb_fd, fb_data, screensize);
    if (bytes_written != screensize) {
        perror("Error writing to framebuffer");
        free(fb_data);
        close(fb_fd);
        return 1;
    }

    printf("Framebuffer filled with noise.\n");

    free(fb_data);
    close(fb_fd);

    return 0;
}

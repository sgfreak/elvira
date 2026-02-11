// I will not comment after this for code minimalism xD
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

struct termios o;
char **b, *f;
int n, r, c, m, R, C;

void t(int e) {
    if (e) {
        tcgetattr(0, &o);
        struct termios s = o;
        s.c_lflag &= ~(ICANON | ECHO | ISIG);
        tcsetattr(0, TCSANOW, &s);
        write(1, "\033[?1049h", 8);
    } else {
        tcsetattr(0, TCSANOW, &o);
        write(1, "\033[?1049l", 8);
    }
}

void d() {
    struct winsize w; ioctl(0, TIOCGWINSZ, &w); R = w.ws_row; C = w.ws_col;
    write(1, "\033[H", 3);
    for (int i = 0; i < R - 1; i++) {
        write(1, "\033[K", 3);
        if (i < n) write(1, b[i], strlen(b[i]));
        write(1, "\r\n", 2);
    }
    char s[64];
    write(1, "\033[K", 3);
    int l = sprintf(s, "[%c] %d,%d %s", m ? 'I' : 'N', r + 1, c + 1, f);
    write(1, s, l);
    l = sprintf(s, "\033[%d;%dH", r + 1, c + 1);
    write(1, s, l);
}

void w() {
    FILE *p = fopen(f, "w");
    if (!p) return;
    for (int i = 0; i < n; i++) fprintf(p, "%s\n", b[i]);
    fclose(p);
}

int main(int a, char **v) {
    f = a > 1 ? v[1] : "untitled";
    FILE *p = fopen(f, "r");
    char l[8192];
    b = malloc(10000 * sizeof(char*));
    while (p && fgets(l, 8192, p)) {
        l[strcspn(l, "\r\n")] = 0;
        b[n++] = strdup(l);
    }
    if (p) fclose(p);
    if (!n) b[n++] = strdup("");
    t(1);
    while (1) {
        d();
        char k[8] = {0};
        int s = read(0, k, 8);
        if (s <= 0) continue;
        if (!m) {
            if (k[0] == 'q') break;
            if (k[0] == 'i') m = 1;
            if (k[0] == 'h' && c > 0) c--;
            if (k[0] == 'l' && c < (int)strlen(b[r])) c++;
            if (k[0] == 'j' && r < n - 1) { r++; if (c > (int)strlen(b[r])) c = strlen(b[r]); }
            if (k[0] == 'k' && r > 0) { r--; if (c > (int)strlen(b[r])) c = strlen(b[r]); }
            if (k[0] == 'x' && strlen(b[r]) > 0) memmove(b[r] + c, b[r] + c + 1, strlen(b[r]) - c);
            if (k[0] == 'o') { memmove(b + r + 2, b + r + 1, (n - r - 1) * sizeof(char*)); b[++r] = strdup(""); n++; c = 0; m = 1; }
            if (k[0] == ':') { read(0, k, 1); if (k[0] == 'w') w(); if (k[0] == 'q') break; }
        } else {
            if (k[0] == 27) m = 0;
            else if (k[0] == 127 || k[0] == 8) {
                if (c > 0) { memmove(b[r] + c - 1, b[r] + c, strlen(b[r]) - c + 1); c--; }
                else if (r > 0) {
                    int p = strlen(b[r - 1]);
                    b[r - 1] = realloc(b[r - 1], p + strlen(b[r]) + 1);
                    strcat(b[r - 1], b[r]);
                    free(b[r]);
                    memmove(b + r, b + r + 1, (n - r - 1) * sizeof(char*));
                    n--; r--; c = p;
                }
            } else if (k[0] == 13) {
                memmove(b + r + 2, b + r + 1, (n - r - 1) * sizeof(char*));
                b[r + 1] = strdup(b[r] + c);
                b[r] = realloc(b[r], c + 1); b[r][c] = 0;
                n++; r++; c = 0;
            } else {
                b[r] = realloc(b[r], strlen(b[r]) + s + 1);
                memmove(b[r] + c + s, b[r] + c, strlen(b[r]) - c + 1);
                memcpy(b[r] + c, k, s);
                c += s;
            }
        }
    }
    t(0);
    return 0;
}

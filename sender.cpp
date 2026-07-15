#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>

using namespace std;

const int PAYLOAD = 160;
const int BUF_SIZE = 8192;

struct Slot { bool valid; uint32_t seq; unsigned char payload[PAYLOAD]; };
Slot hist[BUF_SIZE];

int main() {
    int h_fd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in h_in = {0}; h_in.sin_family = AF_INET; h_in.sin_port = htons(47010); h_in.sin_addr.s_addr = inet_addr("127.0.0.1");
    bind(h_fd, (struct sockaddr*)&h_in, sizeof(h_in));

    int fb_fd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in fb_in = {0}; fb_in.sin_family = AF_INET; fb_in.sin_port = htons(47004); fb_in.sin_addr.s_addr = inet_addr("127.0.0.1");
    bind(fb_fd, (struct sockaddr*)&fb_in, sizeof(fb_in));

    int out_fd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in r_out = {0}; r_out.sin_family = AF_INET; r_out.sin_port = htons(47001); r_out.sin_addr.s_addr = inet_addr("127.0.0.1");

    memset(hist, 0, sizeof(hist));
    for(;;) {
        fd_set fds; FD_ZERO(&fds); FD_SET(h_fd, &fds); FD_SET(fb_fd, &fds);
        if (select(max(h_fd, fb_fd) + 1, &fds, 0, 0, 0) < 0) continue;

        if (FD_ISSET(h_fd, &fds)) {
            unsigned char raw[164];
            if (recvfrom(h_fd, raw, sizeof(raw), 0, 0, 0) == 164) {
                uint32_t seq; memcpy(&seq, raw, 4); seq = ntohl(seq);
                uint32_t idx = seq % BUF_SIZE; hist[idx] = {true, seq}; memcpy(hist[idx].payload, raw + 4, PAYLOAD);
                sendto(out_fd, raw, 164, 0, (struct sockaddr*)&r_out, sizeof(r_out));
            }
        }
        if (FD_ISSET(fb_fd, &fds)) {
            uint32_t batch[100]; ssize_t n = recvfrom(fb_fd, batch, sizeof(batch), 0, 0, 0);
            for (int i = 0; i < n / 4; i++) {
                uint32_t miss = ntohl(batch[i]); uint32_t idx = miss % BUF_SIZE;
                if (hist[idx].valid && hist[idx].seq == miss) {
                    unsigned char out[164]; uint32_t s = htonl(miss);
                    memcpy(out, &s, 4); memcpy(out + 4, hist[idx].payload, PAYLOAD);
                    sendto(out_fd, out, 164, 0, (struct sockaddr*)&r_out, sizeof(r_out));
                }
            }
        }
    }
}
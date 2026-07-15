#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>

using namespace std;

const int PAYLOAD = 160;
const int BUF_SIZE = 8192; 

struct PacketSlot {
    bool valid;
    unsigned char payload[PAYLOAD];
};

double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

PacketSlot buf[BUF_SIZE];
double last_nack[BUF_SIZE]; 

int main() {
    ios_base::sync_with_stdio(false); cin.tie(NULL);

    int in_fd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in in_addr = {0};
    in_addr.sin_family = AF_INET; in_addr.sin_port = htons(47002); in_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    bind(in_fd, (struct sockaddr*)&in_addr, sizeof(in_addr));

    int out_fd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in player = {0};
    player.sin_family = AF_INET; player.sin_port = htons(47020); player.sin_addr.s_addr = inet_addr("127.0.0.1");

    int fb_fd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in sender_fb = {0};
    sender_fb.sin_family = AF_INET; sender_fb.sin_port = htons(47003); sender_fb.sin_addr.s_addr = inet_addr("127.0.0.1");

    char* t0_str = getenv("T0"); char* d_str = getenv("DELAY_MS");
    double t0 = t0_str ? atof(t0_str) : get_time();
    double delay_s = d_str ? atof(d_str) / 1000.0 : 0.060;

    memset(buf, 0, sizeof(buf)); memset(last_nack, 0, sizeof(last_nack));
    uint32_t nxt = 0, highest_seq = 0;

    for(;;) {
        fd_set readfds; FD_ZERO(&readfds); FD_SET(in_fd, &readfds);
        struct timeval tv = {0, 2000}; 
        select(in_fd + 1, &readfds, NULL, NULL, &tv);
        
        if (FD_ISSET(in_fd, &readfds)) {
            unsigned char raw[2048];
            ssize_t n = recvfrom(in_fd, raw, sizeof(raw), 0, 0, 0);
            if (n >= 164) {
                uint32_t seq; memcpy(&seq, raw, 4); seq = ntohl(seq);
                if (seq >= nxt) {
                    uint32_t idx = seq % BUF_SIZE;
                    buf[idx].valid = true;
                    memcpy(buf[idx].payload, raw + 4, PAYLOAD);
                    highest_seq = max(highest_seq, seq);
                }
            }
        }

        while (true) {
            uint32_t idx = nxt % BUF_SIZE;
            if (buf[idx].valid) {
                unsigned char out[164]; uint32_t out_seq = htonl(nxt);
                memcpy(out, &out_seq, 4); memcpy(out + 4, buf[idx].payload, PAYLOAD);
                sendto(out_fd, out, 164, 0, (struct sockaddr*)&player, sizeof(player));
                buf[idx].valid = false; nxt++;
            } else {
                if (get_time() >= (t0 + delay_s + (nxt * 0.020)) - 0.002) nxt++; 
                else break;
            }
        }

        uint32_t nack_batch[50], batch_count = 0;
        for (uint32_t i = nxt; i < min(highest_seq, nxt + 50); i++) {
            uint32_t idx = i % BUF_SIZE;
            if (!buf[idx].valid && (get_time() - last_nack[idx] > 0.030)) {
                nack_batch[batch_count++] = htonl(i); last_nack[idx] = get_time();
            }
        }
        if (batch_count > 0) sendto(fb_fd, nack_batch, batch_count * 4, 0, (struct sockaddr*)&sender_fb, sizeof(sender_fb));
    }
}
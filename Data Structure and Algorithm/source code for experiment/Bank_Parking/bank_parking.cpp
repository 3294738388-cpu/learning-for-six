#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include "C:\Users\TX\Desktop\Data Structure\include\LinkQueue.h"
#define maxnumber 3
#define MAXWAITING_TIME 120 // 最大等待时间，单位：分钟
using namespace std;

typedef struct Guest {
    int number;
    int arrival_time; // 分钟，整数
    int duration;     // 服务时长，整数分钟
    int vip = 0; // 1 for VIP, 0 for regular
    int state = 2; // 0 waiting, 1 being served, 2 finished
    int waiting_time = 0; // 累计等待时间（分钟）
    int service_end = -1; // 服务结束时间（分钟），-1 表示未在服务中
} Guest;

int findGuestIndex(Guest g[], int n, int number) {
    for (int i = 0; i < n; i++) {
        if (g[i].number == number) {
            return i;
        }
    }
    return -1;
}

// 返回指向 guest g 所在队列的指针：
// - 返回 &vipQueue 表示在 VIP 队列中
// - 返回 &regularQueue[i] 表示在第 i 个普通队列中
// - 返回 nullptr 表示未在任何队列中找到
LinkQueue* findQueue(const Guest &g, LinkQueue regularQueue[], LinkQueue &vipQueue) {
    // 检查 VIP 队列（遍历链表节点，不出队）
    LNode *p = vipQueue.front->next;
    while (p != nullptr) {
        if (p->data == g.number) return &vipQueue;
        p = p->next;
    }

    // 检查普通队列
    for (int i = 0; i < maxnumber; i++) {
        p = regularQueue[i].front->next;
        while (p != nullptr) {
            if (p->data == g.number) return &regularQueue[i];
            p = p->next;
        }
    }

    return nullptr;
}

int main() {
    srand((unsigned)time(NULL));

    const int N = 10;
    Guest g[N];
    for (int i = 0; i < N; i++) {
        g[i].number = i + 1;
        g[i].arrival_time = rand() % 480; // 0..479
        g[i].duration = (rand() % 20) + 30; // 30..149
        g[i].vip = (rand() % 10 == 0) ? 1 : 0;
        g[i].waiting_time = 0;
        g[i].state = 2;
        g[i].service_end = -1;
    }

    LinkQueue regularQueue[maxnumber], vipQueue;
    for (int i = 0; i < maxnumber; i++) InitQueue_L(regularQueue[i]);
    InitQueue_L(vipQueue);

    // 按 arrival_time 排序（升序）
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < i; j++) {
            if (g[i].arrival_time < g[j].arrival_time) {
                Guest tmp = g[i];
                g[i] = g[j];
                g[j] = tmp;
            }
        }
    }

    // 每个窗口正在服务的顾客编号（-1 表示空闲）
    int serving_in_window[maxnumber];
    int empty [480] = {0}; // 用于记录每分钟的空闲窗口
    for (int i = 0; i < maxnumber; i++) serving_in_window[i] = -1;

    // 仿真循环（每分钟）
    for (int min = 0; min < 480; min++) {
        for (int i = 0; i < N; i++) {
            if (g[i].waiting_time >= MAXWAITING_TIME) {
                LinkQueue *q = findQueue(g[i], regularQueue, vipQueue);
                if (q != nullptr) {
                    LinkList p = q->front;
                    while (p -> next -> data != g[i].number) {
                        LinkList temp = p -> next;
                        p -> next = temp -> next;
                        delete temp;
                    }
                }
                g[i].state = 3; // 标记为完成
            }
        }
        // 1) 处理本分钟结束的服务：释放对应窗口，标记顾客完成
        for (int q = 0; q < maxnumber; q++) {
            int num = serving_in_window[q];
            if (num != -1) {
                int idx = findGuestIndex(g, N, num);
                if (idx != -1 && g[idx].state == 1 && g[idx].service_end == min) {
                    g[idx].state = 2; // 完成
                    g[idx].service_end = -1;
                    serving_in_window[q] = -1; // 窗口空闲
                }
            }
        }

        // 2) 处理到达并入队（到达时 waiting_time 置 0，state 设为等待或立即服务将在后面分配）
        for (int i = 0; i < N; i++) {
            if (g[i].arrival_time == min) {
                g[i].waiting_time = 0;
                g[i].state = 0; // 先设为等待（后面会分配到窗口改为 1）
                if (g[i].vip == 1) {
                    EnQueue_L(vipQueue, g[i].number);
                } else {
                    // 选择最短队列（当前仅 maxnumber 窗口）
                    int minIndex = 0;
                    int minLen = QueueLength_L(regularQueue[0]);
                    for (int j = 1; j < maxnumber; j++) {
                        int len = QueueLength_L(regularQueue[j]);
                        if (len < minLen) {
                            minLen = len;
                            minIndex = j;
                        }
                    }
                    EnQueue_L(regularQueue[minIndex], g[i].number);
                }
            }
        }

        // 3) 为每个空闲窗口分配顾客（VIP 优先）
        for (int q = 0; q < maxnumber; q++) {
            if (serving_in_window[q] == -1) { 
                int num;
                if (!QueueEmpty_L(vipQueue)) {
                    if (DeQueue_L(vipQueue, num)) {
                        int idx = findGuestIndex(g, N, num);
                        if (idx != -1) {
                            serving_in_window[q] = num;
                            g[idx].state = 1;
                            g[idx].service_end = min + g[idx].duration;
                        }
                    }
                } else if (!QueueEmpty_L(regularQueue[q])) {
                        if (DeQueue_L(regularQueue[q], num)) {
                        int idx = findGuestIndex(g, N, num);
                        if (idx != -1) {
                            serving_in_window[q] = num;
                            g[idx].state = 1;
                            g[idx].service_end = min + g[idx].duration;
                        }
                    }
                }
            }
        }

        // 4) 本分钟结束，对仍在等待的顾客累加 waiting_time（新到达且被立即分配的不会计入）
        for (int i = 0; i < N; i++) {
            if (g[i].state == 0) {
                g[i].waiting_time += 1;
            }
        }

        for (int i = 0; i < maxnumber; i++) {
            if (serving_in_window[i] == -1) {
                empty[min]++; // 记录空闲窗口
            }
        }
    }

    for (int i = 0; i < N; i++) {
        cout << "Guest " << setw(2) << g[i].number 
             << " | Arrival: " << setw(3) << g[i].arrival_time 
             << " | Duration: " << setw(3) << g[i].duration 
             << " | VIP: " << g[i].vip 
             << " | State: " << g[i].state
             << " | Total Waiting Time: " << setw(3) << g[i].waiting_time << " mins" 
             << endl;
    }
    int sum = 0;
    for (int i = 0; i < N; i++) {
        sum += g[i].waiting_time;
    }
    cout << "Average Waiting Time: " << (double)sum / N << " mins" << endl;

    sum = 0;
    for (int i = 0; i < 480; i++) {
        sum += empty[i];
    }
    cout << " Windows Average Occupancy Rate: " << (double)sum / 480 / maxnumber << endl;

    return 0;
}

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

struct PCB {
    string name;        // 进程名
    int serviceTime;    // 需要服务的时间 (总运行时间)
    int runTime;        // 已运行时间
    int priority;       // 当前优先级
    char state;         // 状态: 'W' (Wait/Ready), 'R' (Run), 'F' (Finish)
    PCB* next;          // 指向下一个PCB的指针 (用于链表队列)

    PCB(string n, int s, int p) : 
        name(n), serviceTime(s), runTime(0), priority(p), state('W'), next(nullptr) {}
};

// 全局变量：就绪队列头指针
PCB* readyQueue = nullptr;

// 函数声明
void insertProcess(PCB* process);
void createProcesses();
void runScheduler();
void printStatus(PCB* current);
void cleanup();

int main() {
    cout << "=== 操作系统课程设计：进程调度算法模拟 (动态优先级) ===" << endl;
    
    createProcesses();

    if (readyQueue != nullptr) {
        cout << "\n[System] 调度开始..." << endl;
        runScheduler();
    } else {
        cout << "[System] 没有进程需要调度。" << endl;
    }

    cleanup();
    
    return 0;
}

void insertProcess(PCB* process) {
    if (readyQueue == nullptr) {
        readyQueue = process;
        return;
    }

    if (process->priority > readyQueue->priority) {
        process->next = readyQueue;
        readyQueue = process;
    } else {
        PCB* current = readyQueue;
        while (current->next != nullptr && current->next->priority >= process->priority) {
            current = current->next;
        }
        process->next = current->next;
        current->next = process;
    }
}

void createProcesses() {
    int n;
    cout << "请输入进程数量: ";
    cin >> n;

    for (int i = 0; i < n; ++i) {
        string name;
        int time, prio;
        cout << "请输入第 " << (i + 1) << " 个进程的信息 (名称 服务时间 初始优先级): ";
        cin >> name >> time >> prio;
        
        PCB* newPCB = new PCB(name, time, prio);
        insertProcess(newPCB);
    }
    
    cout << "[System] 进程初始化完成，进入就绪队列。" << endl;
}

void runScheduler() {
    int timeSlice = 1; // 模拟时间片计数
    
    while (readyQueue != nullptr) {
        PCB* current = readyQueue;
        readyQueue = readyQueue->next; // 从队列移除
        
        current->state = 'R';
        current->next = nullptr; // 断开连接

        cout << "\n--> [Time " << setw(2) << timeSlice++ << "] 正在运行进程: " << current->name 
             << " (优先级: " << current->priority << ", 剩余时间: " << (current->serviceTime - current->runTime) << ")" << endl;

        // 更新状态
        current->priority -= 1; 
        current->runTime += 1; 
        
        if (current->runTime >= current->serviceTime) {
            current->state = 'F';
            cout << "    [Finish] 进程 " << current->name << " 已完成！" << endl;
            delete current; 
            current = nullptr;
        } else {
            current->state = 'W';
            cout << "    [Switch] 进程 " << current->name << " 时间片用完，重新排队 (新优先级: " << current->priority << ")" << endl;
            insertProcess(current);
        }

        if (readyQueue) {
             cout << "    [Queue] 等待队列: ";
             PCB* p = readyQueue;
             while(p) {
                 cout << p->name << "(" << p->priority << ") -> ";
                 p = p->next;
             }
             cout << "NULL" << endl;
        }
    }
    
    cout << "\n[System] 所有进程调度结束。" << endl;
}

void cleanup() {
    while (readyQueue != nullptr) {
        PCB* temp = readyQueue;
        readyQueue = readyQueue->next;
        delete temp;
    }
}

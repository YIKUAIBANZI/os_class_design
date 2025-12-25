#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

// 内存节点结构体
struct MemNode {
    int startAddr;      // 起始地址
    int size;           // 分区大小 (KB)
    bool isFree;        // 状态: true=空闲, false=已分配
    int processID;      // 占用进程ID (-1表示空闲)
    MemNode* next;      // 下一个节点指针

    MemNode(int start, int sz) : 
        startAddr(start), size(sz), isFree(true), processID(-1), next(nullptr) {}
};

// 内存管理器类
class MemoryManager {
private:
    MemNode* head;      // 内存链表头指针
    int totalMemory;    // 总内存大小

public:
    MemoryManager(int totalSize) {
        totalMemory = totalSize;
        // 初始化时，整个内存是一个大的空闲块
        head = new MemNode(0, totalSize);
    }

    ~MemoryManager() {
        MemNode* current = head;
        while (current != nullptr) {
            MemNode* temp = current;
            current = current->next;
            delete temp;
        }
    }

    // 首次适应算法 (First Fit) 分配内存
    bool allocate(int pid, int requestSize) {
        MemNode* current = head;
        
        while (current != nullptr) {
            // 找到第一个空闲且大小足够的块
            if (current->isFree && current->size >= requestSize) {
                // 情况1: 分区很大，需要切割
                if (current->size > requestSize) {
                    // 创建一个新的空闲节点，表示剩余的部分
                    MemNode* newFreeNode = new MemNode(current->startAddr + requestSize, current->size - requestSize);
                    newFreeNode->next = current->next;
                    
                    // 更新当前节点为已分配
                    current->size = requestSize;
                    current->isFree = false;
                    current->processID = pid;
                    current->next = newFreeNode;
                } 
                // 情况2: 分区刚好合适，不需要切割
                else {
                    current->isFree = false;
                    current->processID = pid;
                }
                
                cout << "[Success] 进程 " << pid << " 申请 " << requestSize << "KB 成功。地址: " << current->startAddr << endl;
                return true;
            }
            current = current->next;
        }
        
        cout << "[Failed] 进程 " << pid << " 申请 " << requestSize << "KB 失败！无足够连续空间。" << endl;
        return false;
    }

    // 回收内存
    void freeMemory(int pid) {
        MemNode* current = head;
        MemNode* prev = nullptr;
        bool found = false;

        while (current != nullptr) {
            if (!current->isFree && current->processID == pid) {
                current->isFree = true;
                current->processID = -1;
                found = true;
                cout << "[Free] 进程 " << pid << " 内存已释放。地址: " << current->startAddr << endl;
                break; // 假设一个进程只分配了一块，释放后退出
            }
            prev = current;
            current = current->next;
        }

        if (found) {
            mergeFreeBlocks(); // 释放后立即尝试合并
        } else {
            cout << "[Error] 未找到进程 " << pid << " 的内存分配记录。" << endl;
        }
    }

    // 合并相邻的空闲分区
    void mergeFreeBlocks() {
        MemNode* current = head;
        while (current != nullptr && current->next != nullptr) {
            // 如果当前块是空闲，且下一个块也是空闲
            if (current->isFree && current->next->isFree) {
                MemNode* temp = current->next;
                // 合并大小
                current->size += temp->size;
                // 删除下一个节点
                current->next = temp->next;
                delete temp;
                cout << "    -> [Merge] 发生内存合并，新空闲块大小: " << current->size << "KB" << endl;
            } else {
                current = current->next;
            }
        }
    }

    // 显示当前内存状态
    void showStatus() {
        cout << "\n------------------ 当前内存状态 ------------------" << endl;
        cout << left << setw(10) << "起始地址" << setw(10) << "大小(KB)" << setw(10) << "状态" << setw(10) << "进程ID" << endl;
        
        MemNode* current = head;
        while (current != nullptr) {
            cout << left << setw(10) << current->startAddr 
                 << setw(10) << current->size 
                 << setw(10) << (current->isFree ? "Free" : "Used") 
                 << setw(10) << (current->isFree ? "-" : to_string(current->processID)) << endl;
            current = current->next;
        }
        cout << "--------------------------------------------------\n" << endl;
    }
};

int main() {
    cout << "=== 操作系统课程设计：内存管理模拟 (First Fit) ===" << endl;
    
    // 初始化 640KB 内存
    MemoryManager mm(640);
    mm.showStatus();

    // 模拟报告中的测试步骤
    // 1. 作业1申请 130KB
    mm.allocate(1, 130);
    // 2. 作业2申请 60KB
    mm.allocate(2, 60);
    // 3. 作业3申请 100KB
    mm.allocate(3, 100);
    
    mm.showStatus();

    // 4. 作业2释放 60KB (此时应产生外部碎片)
    mm.freeMemory(2);
    mm.showStatus();

    // 5. 作业4申请 200KB 
    // (First Fit 应该跳过中间 60KB 的空洞，在后面分配)
    mm.allocate(4, 200);
    mm.showStatus();

    // 6. 释放所有，观察合并
    mm.freeMemory(1);
    mm.freeMemory(3);
    mm.freeMemory(4);
    
    mm.showStatus();

    return 0;
}

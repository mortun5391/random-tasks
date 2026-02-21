#include <vector>
#include <random>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>


using namespace std;

template <typename T>
struct SkipListNode {
    SkipListNode* next;
    SkipListNode* low;
    T value;

    SkipListNode(T value_) : value(value_), next(nullptr), low(nullptr) {}
    SkipListNode(SkipListNode* node) : value(node->value), next(node->next), low(node->low) {}
};

template <typename T>
class SkipList {
private:
    SkipListNode<T>* head_;
    mt19937 rng_;
    uniform_int_distribution<int> dist_;

    SkipListNode<T>* lower_bound_(T value) {
        SkipListNode<T>* cur = head_;

        while (cur != nullptr) {
            while (cur->next != nullptr && cur->next->value <= value) {
                cur = cur->next;
            }
            if (cur->low == nullptr) {
                return cur;
            }
            cur = cur->low;
        }
        return nullptr;
    }

    SkipListNode<T>* insert_(SkipListNode<T>* cur, T value) {
        while (cur->next != nullptr && cur->next->value < value) {
            cur = cur->next;
        }
        if (cur->low == nullptr) {
            SkipListNode<T>* next = cur->next;
            cur->next = new SkipListNode(value);
            cur->next->next = next;
            return cur->next;

        }
        SkipListNode<T>* node = insert_(cur->low, value);
        if (node == nullptr) {
            return nullptr;
        }

        if (dist_(rng_)) {
            SkipListNode<T>* next = cur->next;
            cur->next = new SkipListNode(value);
            cur->next->next = next;
            cur->next->low = node;
            return cur->next;
        }

        return nullptr;
    }

    void erase_(T value) {
        SkipListNode<T>* cur = head_;
        bool flag = true;
        while (cur != nullptr) {
            while (cur->next != nullptr && cur->next->value < value) {
                cur = cur->next;
                flag = false;
            }
            if (cur->next != nullptr && cur->next->value == value) {
                SkipListNode<T>* next = cur->next->next;
                delete cur->next;
                cur->next = next;
            }

            if (cur->low == nullptr) {
                return;
            }
            if (cur->next == nullptr && flag) {
                head_ = cur->low;
                
                delete cur;
                cur = head_;
                continue;
            }
            cur = cur->low;
        }
    }


public:

    SkipList() {
        head_ = new SkipListNode(T());
        dist_ = uniform_int_distribution<int>(0, 1);
        std::random_device rd;
        rng_ = std::mt19937(rd());
    }

    void insert(T value) {
        SkipListNode<T>* node = insert_(head_, value);
        if (node != nullptr) {
            if (dist_(rng_)) {
                SkipListNode<T>* new_head = new SkipListNode(T());
                new_head->low = head_;
                head_ = new_head;
                head_->next = new SkipListNode(value);
                head_->next->low = node;
            }
        }
    }

    void erase(T value) {
        erase_(value);
    }

    T lower_bound(T value) {
        return lower_bound_(value)->value;
    }

    void print() {
        if (head_ == nullptr) {
            std::cout << "Empty Skip List" << std::endl;
            return;
        }

        int height = 0;
        SkipListNode<T>* top = head_;
        while (top != nullptr) {
            height++;
            top = top->low;
        }

        std::vector<std::vector<std::string>> levels(height);

        SkipListNode<T>* level = head_;
        int levelIdx = height - 1;

        while (level != nullptr) {
            SkipListNode<T>* curr = level->next;
            while (curr != nullptr) {
                std::stringstream ss;
                ss << curr->value;
                levels[levelIdx].push_back(ss.str());
                curr = curr->next;
            }
            level = level->low;
            levelIdx--;
        }

        std::cout << "Skip List Structure:" << std::endl;
        std::cout << "====================" << std::endl;

        for (int i = height - 1; i >= 0; i--) {
            std::cout << "Level " << i << ": ";

            int pos = 0;
            auto& currentLevel = levels[i];
            auto& lowerLevel = (i > 0) ? levels[i-1] : levels[i];

            for (const auto& val : currentLevel) {
                auto it = std::find(lowerLevel.begin(), lowerLevel.end(), val);
                if (it != lowerLevel.end()) {
                    int lowerPos = std::distance(lowerLevel.begin(), it);
                    while (pos < lowerPos) {
                        std::cout << "    ";
                        pos++;
                    }
                }
                std::cout << "[" << val << "] ";
                pos++;
            }
            std::cout << std::endl;
        }
        std::cout << "====================" << std::endl;
    }


};


int main() {
    SkipList<int> sl;
    for (int i = 1; i <= 10; ++i) {
        sl.insert(i);
    }

    sl.print();

    cout << sl.lower_bound(4) << endl;
    sl.erase(5);
    sl.erase(10);
    sl.erase(24);
    sl.erase(28);

    sl.print();
}



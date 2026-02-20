#include <vector>
#include <random>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

template <typename T>
struct Node {
    Node* next;
    Node* low;
    T value;

    Node(T value_) : value(value_), next(nullptr), low(nullptr) {}
    Node(Node* node) : value(node->value), next(node->next), low(node->low) {}
};

template <typename T>
class SkipList {
private:
    Node<T>* head_;
    mt19937 rng_;
    uniform_int_distribution<int> dist_;

    Node<T>* lower_bound_(T value) {
        Node<T>* cur = head_;

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

    Node<T>* insert_(Node<T>* cur, T value) {
        while (cur->next != nullptr && cur->next->value < value) {
            cur = cur->next;
        }
        if (cur->low == nullptr) {
            Node<T>* next = cur->next;
            cur->next = new Node(value);
            cur->next->next = next;
            return cur->next;

        }
        Node<T>* node = insert_(cur->low, value);
        if (node == nullptr) {
            return nullptr;
        }

        if (dist_(rng_)) {
            Node<T>* next = cur->next;
            cur->next = new Node(value);
            cur->next->next = next;
            cur->next->low = node;
            return cur->next;
        }

        return nullptr;
    }


public:

    SkipList() {
        head_ = new Node(T());
        dist_ = uniform_int_distribution<int>(0, 1);
        std::random_device rd;
        rng_ = std::mt19937(rd());
    }

    void insert(T value) {
        Node<T>* node = insert_(head_, value);
        if (node != nullptr) {
            if (dist_(rng_)) {
                Node<T>* new_head = new Node(T());
                new_head->low = head_;
                head_ = new_head;
                head_->next = new Node(value);
                head_->next->low = node;
            }
        }
    }

    void erase(T value) {

    }

    T lower_bound(T value) {
        return lower_bound_(value)->value;
    }

    void print() {
        if (head_ == nullptr) {
            std::cout << "Empty Skip List" << std::endl;
            return;
        }

        // Сначала найдем максимальную высоту
        int height = 0;
        Node<T>* top = head_;
        while (top != nullptr) {
            height++;
            top = top->low;
        }

        // Создадим вектор для хранения всех уровней
        std::vector<std::vector<std::string>> levels(height);

        // Соберем все значения по уровням
        Node<T>* level = head_;
        int levelIdx = height - 1;

        while (level != nullptr) {
            Node<T>* curr = level->next;
            while (curr != nullptr) {
                // Преобразуем значение в строку (для универсальности)
                std::stringstream ss;
                ss << curr->value;
                levels[levelIdx].push_back(ss.str());
                curr = curr->next;
            }
            level = level->low;
            levelIdx--;
        }

        // Вывод в красивом формате
        std::cout << "Skip List Structure:" << std::endl;
        std::cout << "====================" << std::endl;

        for (int i = height - 1; i >= 0; i--) {
            std::cout << "Level " << i << ": ";

            // Для выравнивания, будем отслеживать позицию
            int pos = 0;
            auto& currentLevel = levels[i];
            auto& lowerLevel = (i > 0) ? levels[i-1] : levels[i];

            for (const auto& val : currentLevel) {
                // Найдем позицию этого значения на нижнем уровне
                auto it = std::find(lowerLevel.begin(), lowerLevel.end(), val);
                if (it != lowerLevel.end()) {
                    int lowerPos = std::distance(lowerLevel.begin(), it);
                    // Добавим отступы
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
    sl.insert(1);
    sl.insert(2);
    sl.insert(5);
    sl.insert(4);

    cout << sl.lower_bound(4) << endl;

    sl.print();
}



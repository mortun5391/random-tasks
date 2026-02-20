#include <vector>
#include <random>
#include <iostream>
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
    Node<T>* head;
    mt19937 rng;
    uniform_int_distribution<int> dist(0, 1);

    Node<T>* lower_bound(T value) {
        Node<T>* cur = head;
        
        while (cur != nullptr) {
            while (cur->next != nullptr && cur->next->value < value) {
                cur = cur->next;
            }
            if (cur->low == nullptr) {
                return cur;
            }
            cur = cur->low;
        }
        return nullptr;
    }

    Node<T>* insert(Node<T>* cur, T value) {
        while (cur->next != nullptr && cur->next->value < value) {
            cur = cur->next;
        }
        if (cur->low == nullptr) {
            Node<T>* next = cur->next;
            cur->next = Node(value);
            cur->next->next = next;
            return cur->next;

        }
        Node<T>* node = insert(cur->low, value);
        if (node == nullptr) {
            return nullptr;
        }

        if (dist(rng)) {
            Node<T>* next = cur->next;
            cur->next = Node(value);
            cur->next->next = next;
            cur->next->low = node;
            return cur->next;
        }

        return nullptr
    }


public:

    SkipList() : rng(random_device()), head(nullptr) {
        head = new Node(T());
    }


    void insert(T value) {
        Node<T>* node = insert(head, value);
        if (node != nullptr) {
            if (dist(rng)) {
                Node<T>* new_head(T());
                new_head->low = head;
                head = new_head;
                head->next = Node(value);
                head->next->low = node;
            }
        }        
    }

    void erase(T value) {

    }

    T lower_bound() {

    }

    void print() {
        if (head == nullptr || head->low == nullptr) {
            cout << "Empty skip list" << endl;
            return;
        }

        // Получаем все уровни
        vector<Node<T>*> levels = get_levels();
        
        // Находим самый нижний уровень для определения всех значений
        Node<T>* bottom = levels.back();
        vector<T> values;
        Node<T>* curr = bottom->next;  // пропускаем головной узел
        while (curr != nullptr) {
            values.push_back(curr->value);
            curr = curr->next;
        }

        if (values.empty()) {
            cout << "Empty skip list" << endl;
            return;
        }

        // Печатаем каждый уровень
        for (size_t i = 0; i < levels.size(); i++) {
            cout << "Level " << i << ": ";
            
            Node<T>* level_node = levels[i]->next;  // пропускаем головной узел
            size_t value_index = 0;
            
            while (level_node != nullptr && value_index < values.size()) {
                // Печатаем текущее значение
                cout << "[" << setw(2) << level_node->value << "]";
                
                // Переходим к следующему узлу на этом уровне
                level_node = level_node->next;
                value_index++;
                
                // Если есть ещё значения, добавляем разделитель
                if (level_node != nullptr && value_index < values.size()) {
                    cout << "---";
                }
            }
            cout << endl;
        }
        
        // Альтернативный красивый вывод с выравниванием
        cout << "\nVertical view:" << endl;
        
        // Печатаем значения на нижнем уровне для справки
        cout << "Values: ";
        for (const auto& val : values) {
            cout << "[" << setw(2) << val << "] ";
        }
        cout << endl;
        
        // Печатаем каждый уровень с выравниванием
        for (size_t i = 0; i < levels.size(); i++) {
            cout << "L" << i << ":    ";
            
            Node<T>* level_node = levels[i]->next;
            size_t pos = 0;
            
            for (size_t j = 0; j < values.size(); j++) {
                if (level_node != nullptr && level_node->value == values[j]) {
                    cout << "[" << setw(2) << level_node->value << "] ";
                    level_node = level_node->next;
                } else {
                    cout << "      ";  // пробелы для пропущенных значений
                }
            }
            cout << endl;
        }
    }


};


int main() {
    SkipList<int> sl;

    sl.print();
}



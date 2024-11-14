#include "main.hpp"

/* TODO: Please design your data structure carefully so that you can work with the given dataset
 *       in this assignment. The below structures are just some suggestions.
 */

template<typename T>
class List {
public:
    virtual ~List() = default;
    virtual void push_back(T value) = 0;
    virtual void push_front(T value) = 0;
    virtual void insert(int index, T value) = 0;
    virtual void remove(int index) = 0;
    virtual T& get(int index) const = 0;
    virtual int length() const = 0;
    virtual void clear() = 0;
    virtual void print() const = 0;
    virtual void reverse() = 0;

};

template<typename T>
class MyList : public List<T> {
public:
    class Node {
    public:
        T value;
        Node* next;
        Node* prev;

        Node(const T& val, Node* next = nullptr, Node* prev = nullptr) : value(val), next(next), prev(prev) {}
    };

private:
    Node* head;
    Node* tail;
    int size;

public:
    MyList() : head(nullptr), tail(nullptr), size(0) {}

    ~MyList() {
        clear();
    }

    void push_back(T value) override {
        Node* newNode = new Node(value);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        size++;
    }

    void push_front(T value) override {
        Node* newNode = new Node(value);
        if (!head) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        size++;
    }

    void insert(int index, T value) override {
        if (index < 0 || index > size) {
            std::cerr << "Invalid index." << std::endl;
            return;
        }

        if (index == 0) {
            push_front(value);
        } else if (index == size) {
            push_back(value);
        } else {
            Node* current = head;
            for (int i = 0; i < index - 1; i++) {
                current = current->next;
            }
            Node* newNode = new Node(value, current->next, current);
            current->next->prev = newNode;
            current->next = newNode;
            size++;
        }
    }

    void remove(int index) override {
        if (index < 0 || index >= size) {
            std::cerr << "Invalid index." << std::endl;
            return;
        }

        Node* current = head;

        if (index == 0) {
            head = head->next;
            if (head) {
                head->prev = nullptr;
            } else {
                tail = nullptr;
            }
            delete current;
        } else if (index == size - 1) {
            current = tail;
            tail = tail->prev;
            tail->next = nullptr;
            delete current;
        } else {
            for (int i = 0; i < index; i++) {
                current = current->next;
            }
            current->prev->next = current->next;
            current->next->prev = current->prev;
            delete current;
        }

        size--;
    }

    T& get(int index) const override {
        if (index < 0 || index >= size) {
            std::cerr << "Invalid index." << std::endl;
            // Returning a default value for simplicity.
            // In a real implementation, you should handle this differently.
            static T default_value;
            return default_value;
        }
        Node* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        return current->value;
    }

    int length() const override {
        return size;
    }

    void clear() override {
        Node* current = head;
        while (current) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }
        head = tail = nullptr;
        size = 0;
    }
    bool empty(){return size==0;};
    void print() const override {
        Node* current = head;
        while (current) {
            std::cout << current->value << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }

    void reverse() override {
        if (!head || head == tail) {
            return;
        }

        Node* current = head;
        Node* temp = nullptr;

        while (current != nullptr) {
            temp = current->next;
            current->next = current->prev;
            current->prev = temp;
            current = temp;
        }

        temp = head;
        head = tail;
        tail = temp;
    }


};
class Dataset {
private:
    MyList<MyList<int>*>* data;
    MyList<string>* labels;
    //You may need to define more
public:
    Dataset(){
        this->data = new MyList<MyList<int> *>();
        this->labels = new  MyList<string>();
        };
    ~Dataset(){clear();};
    Dataset(const Dataset& other);
    void clear();
    Dataset& operator=(const Dataset& other);
    bool loadFromCSV(const char* fileName);
    void printHead(int nRows = 5, int nCols = 5) const;
    void printTail(int nRows = 5, int nCols = 5) const;
    void getShape(int& nRows, int& nCols) const;
    void columns() const;
    bool drop(int axis = 0, int index = 0, std::string columns = "");
    Dataset extract(int startRow = 0, int endRow = -1, int startCol = 0, int endCol = -1) const;
    MyList<MyList<int>*>* getData() const{return data;};
    MyList<string>* getLabels() const{ return labels;};
};

class kNN {
private:
    int k;
    Dataset X_train;
    Dataset  y_train;   
    double (*distance)(const MyList<int>& xi, const MyList<int>& xj, int nCols);
public:
    void setK(int _k){this->k=_k;}
    int getK() const{return this->k;}
    static double euclideanDistance(const MyList<int>& xi, const MyList<int>& xj, int nCols);
    //You may need to define more
public:
    kNN(int k = 5, double (*distanceFunc)(const MyList<int>&, const MyList<int>&, int) = &euclideanDistance) : k(k), distance(distanceFunc) {}
    void fit(const Dataset& X_train, const Dataset& y_train);
    Dataset predict(const Dataset& X_test);
    double score(const Dataset& y_test, const Dataset& y_pred);
    ~kNN(){}
};
        
void train_test_split(Dataset& X, Dataset& y, double test_size, 
                        Dataset& X_train, Dataset& X_test, Dataset& y_train, Dataset& y_test);

// Please add more or modify as needed
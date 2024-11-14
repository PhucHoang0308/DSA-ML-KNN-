#include "kNN.hpp"

/* TODO: You can implement methods, functions that support your data structures here.
 * */
void sort(double *&array, int size) {
    for (int i = 0; i < size - 1; ++i) {
        for (int j = i + 1; j < size; ++j) {
            if (array[i] < array[j]) {
                double temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }
    }
}
int findIndex(double *arr, int size, double value) {
    for (int i = 0; i < size; ++i) {
        if (arr[i] == value) {
            return i; // Trả về chỉ số của giá trị tìm thấy
        }
    }
    return -1; // Trả về -1 nếu không tìm thấy giá trị
}
int mode(double *array, int size) {
    // Tạo một mảng đếm số lần xuất hiện của mỗi phần tử
    int *count = new int[size];
    for (int i = 0; i < size; ++i) {
        count[i] = 0;
    }

    // Đếm số lần xuất hiện của mỗi phần tử trong mảng
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (array[j] == array[i]) {
                count[i]++;
            }
        }
    }

    // Tìm phần tử có số lần xuất hiện nhiều nhất
    double modeValue = array[0];
    int maxCount = count[0];
    for (int i = 1; i < size; ++i) {
        if (count[i] > maxCount) {
            maxCount = count[i];
            modeValue = array[i];
        }
    }

    // Xóa bộ đếm và trả về giá trị mode
    delete[] count;
    return modeValue;
}
Dataset::Dataset(const Dataset &other){
// Khởi tạo data mới
        this->data =new MyList<MyList<int> *>();

        // Sao chép dữ liệu từ other.data vào data
        int length = other.data->length();
        for (int i = 0; i < length; ++i) {
            MyList<int>* sublist = other.data->get(i);
            MyList<int>* newSublist = new MyList<int>();
            int sublistLength = sublist->length();
            for (int j = 0; j < sublistLength; ++j) {
                newSublist->push_back(sublist->get(j));
            }
            data->push_back(newSublist);
        }
}
void Dataset::clear() {
  for (int i = 0; i < data->length(); ++i) {
    MyList<int>* sublist = data->get(i);
    delete sublist;
  }
  data->clear();
}
Dataset& Dataset::operator=(const Dataset& other) {
    if (this != &other) {
        // Xóa dữ liệu hiện tại trước khi sao chép dữ liệu từ đối tượng khác
        clear();

        // Sao chép dữ liệu từ đối tượng other
        if (other.data) {
            this->data = new MyList<MyList<int> *>();
            for (int i = 0; i < other.data->length(); ++i) {
                MyList<int>* newRow = new MyList<int>();
                for (int j = 0; j < other.data->get(i)->length(); ++j) {
                    newRow->push_back(other.data->get(i)->get(j));
                }
                this->data->push_back(newRow);
            }
        }

        if (other.labels) {
            this->labels = new MyList<string>();
            for (int i = 0; i < other.labels->length(); ++i) {
                this->labels->push_back(other.labels->get(i));
            }
        }
    }
    return *this;
}


bool Dataset::loadFromCSV(const char *fileName)
{
    ifstream file(fileName);
    if (!file.is_open()) {
            return false; 
        }
    string line;
    getline(file, line);
    //đưa label vào list<string> trước
    stringstream sslabel(line);
    string labelcell;
    this->labels = new MyList<string>();
    while (getline(sslabel, labelcell,',')) {
        labels->push_back(labelcell);}
    //duyệt phần tử int
    while (getline(file, line)) { // Đọc từng dòng từ tệp tin
        stringstream ss(line);
        string cell;
        MyList<int>* sublist = new MyList<int>();
        while (getline(ss, cell, ',')) { // Phân tích dữ liệu từng cell trên dòng
            int value; 
            value = stoi(cell);
            sublist->push_back(value);
        }
        data->push_back(sublist);
    }
    file.close(); // Đóng tệp tin
    return true;
}
void Dataset::printHead(int nRows, int nCols) const{
    if(nRows > data->length()) nRows = data->length();
    if(nCols > labels->length()) nCols = labels->length();
    
    // In nhãn
    for (int i = 0; i < nCols - 1; ++i) {
        cout << labels->get(i) << ' ';
    }
    cout << labels->get(nCols - 1) << endl;

    // In dữ liệu
    bool lastRow = false;
    for (int i = 0; i < data->length() && i < nRows; ++i) {
        MyList<int>* row = data->get(i);
        for (int j = 0; j < row->length() && j < nCols - 1; ++j) {
            cout << row->get(j) << " ";
        }
        // Kiểm tra nếu dòng hiện tại là dòng cuối cùng
        if (i == data->length() - 1 || i == nRows - 1) {
            cout << row->get(labels->length() - 1);
            lastRow = true;
        } else {
            cout << row->get(labels->length() - 1) << endl;
            lastRow = false;
        }
    }
    if (!lastRow) cout << endl;
}
void Dataset::printTail(int nRows, int nCols) const{
    if(nRows > data->length()) nRows = data->length();
    if(nCols > labels->length()) nCols = labels->length();
    
    // In nhãn
    for (int i = labels->length() - nCols; i < labels->length() - 1; ++i) {
        cout << labels->get(i) << ' ';
    }
    cout << labels->get(labels->length() - 1) << endl;

    // In dữ liệu
    bool lastRow = false;
    for (int i = data->length() - nRows; i < data->length(); ++i) {
        MyList<int>* row = data->get(i);
        for (int j = labels->length() - nCols; j < labels->length() - 1; ++j) {
            cout << row->get(j) << " ";
        }
        // Kiểm tra nếu dòng hiện tại là dòng cuối cùng
        if (i == data->length() - 1) {
            cout << row->get(labels->length() - 1);
            lastRow = true;
        } else {
            cout << row->get(labels->length() - 1) << endl;
            lastRow = false;
        }
    }
    if (!lastRow) cout << endl;
}

void Dataset::getShape(int &nRows, int &nCols) const
{
    nRows = data->length();
    nCols = labels->length();
}

void Dataset::columns() const
{
    int nCols=labels->length();
    for (int i = 0; i < nCols-1; ++i) {
        cout << labels->get(i) << ' ';
    }
    cout << labels->get(nCols - 1) << endl;
}

bool Dataset::drop(int axis, int index, std::string columnName) {
    if (axis != 0 && axis != 1) {
        return false;
    }

    if (axis == 0) {
        if (index >= data->length() || index < 0) {
            return false;
        }
        data->remove(index);
        return true;
    }

    if (axis == 1) {
        // Tìm vị trí của cột columnName trong danh sách các cột
        int columnIndex = -1;
        for (int i = 0; i < labels->length(); ++i) {
            if (labels->get(i) == columnName) {
                columnIndex = i;
                break;
            }
        }
        if (columnIndex == -1) {
            // Không tìm thấy tên cột columnName
            return false;
        }
        
        // Xóa cột columnName khỏi data
        for (int i = 0; i < data->length(); ++i) {
            MyList<int>* row = data->get(i);
            row->remove(columnIndex);
        }
        
        // Xóa tên cột columnName khỏi labels
        labels->remove(columnIndex);
        return true;
    }
    return false;
}

Dataset Dataset::extract(int startRow, int endRow, int startCol, int endCol) const
{
    Dataset extractedDataset;

    // Xác định số hàng và cột cần trích xuất
    int numRows = (endRow == -1) ? data->length() - startRow : endRow - startRow + 1;
    int numCols = (endCol == -1) ? labels->length() - startCol : endCol - startCol + 1;

    // Trích xuất dữ liệu từ data
    for (int i = startRow; i < startRow + numRows; ++i) {
        MyList<int>* newRow = new MyList<int>();
        for (int j = startCol; j < startCol + numCols; ++j) {
            newRow->push_back(data->get(i)->get(j));
        }
        extractedDataset.data->push_back(newRow);
    }

    // Trích xuất dữ liệu từ labels
    for (int j = startCol; j < startCol + numCols; ++j) {
        extractedDataset.labels->push_back(labels->get(j));
    }

    return extractedDataset;
}

double kNN::euclideanDistance(const MyList<int> &xi, const MyList<int> &xj, int nCols)
{
    double sum = 0.0;
    for (int i = 0; i < nCols; ++i) {
        double diff = xi.get(i) - xj.get(i);
        sum += diff * diff;
    }
    return sqrt(sum);
}

void kNN::fit(const Dataset& X_train, const Dataset& y_train) {
    this->X_train = X_train;
    
    this->y_train = y_train;
}


Dataset kNN::predict(const Dataset &X_test)
{
    Dataset predictions;
    int cols, rows;
    X_train.getShape(rows, cols);
    string label = "label"; // Replace this with your label for the prediction
    predictions.getLabels()->push_back(label); // Add label directly to the predictions dataset
    
    for (int i = 0; i < X_test.getData()->length(); ++i) {
        MyList<int>* result = new MyList<int>();
        MyList<int>* row_test = X_test.getData()->get(i);
        double *distance = new double[X_train.getData()->length()];
        
        for (int j = 0; j < X_train.getData()->length(); ++j) {
            MyList<int>* row_train = X_train.getData()->get(j);
            distance[j] = euclideanDistance(*row_test, *row_train, cols);
        }
        
        int size = X_train.getData()->length();
        double *temp = new double[size];
        for (int j = 0; j < size; ++j) {
            temp[j] = distance[j];
        }
        sort(temp, size);
        
        double indexK[k];
        for (int l = 0; l < k; ++l) {
            indexK[l] = findIndex(distance, size, temp[size - l - 1]);
        }
        
        double *Numintest = new double[k];
        for (int l = 0; l < k; ++l) {
            MyList<int>* row_traintest = y_train.getData()->get(indexK[l]);
            Numintest[l] = row_traintest->get(0);
        }
        
        int num = mode(Numintest, k);
        result->push_back(num);

        
        delete[] distance;
        delete[] temp;
        delete[] Numintest;
        predictions.getData()->push_back(result);
    }
    
    return predictions;
}
double kNN::score(const Dataset &y_test, const Dataset &y_pred)
{
    std::cout << "Starting computation...\n";
    
    int correct = 0;
    cout<<correct<<endl;
    for (int i = 0; i < y_test.getData()->length(); ++i) {
        int a = y_test.getData()->get(i)->get(0);
        int b = y_pred.getData()->get(i)->get(0);
        
        if (a == b) ++correct;
        
    }
    cout<<correct<<endl;
    double score = static_cast<double>(correct) / (y_test.getData()->length()); // Corrected calculation
    return score;
}

void train_test_split(Dataset &X, Dataset &y, double test_size, Dataset &X_train, Dataset &X_test, Dataset &y_train, Dataset &y_test)
{
    // Tính số lượng mẫu trong tập huấn luyện và tập dự đoán
    int total_samples = X.getData()->length();
    int num_train_samples = static_cast<int>((1-test_size) * total_samples);
    int num_test_samples = total_samples - num_train_samples;

    // Trích xuất các phần của X và y tương ứng với tập huấn luyện và tập dự đoán
    X_train = X.extract(0, num_train_samples - 1);
    X_test = X.extract(num_train_samples, num_train_samples + num_test_samples - 1);
    y_train = y.extract(0, num_train_samples - 1);
    y_test = y.extract(num_train_samples, num_train_samples + num_test_samples - 1);

}
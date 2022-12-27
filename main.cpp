#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;


///////////////////////////////////////////////////// Merge sort
//
template <typename T>
void merge(std::vector <T> &arr, size_t left, size_t right, size_t middle) {
    std::vector<T> a(middle - left + 1);
    std::vector<T> b(right - middle);
    size_t i, j, k = left;

    for (i = 0; i < a.size(); i++) {
        a[i] = arr[i + left];
    }

    for (i = 0; i < b.size(); i++) {
        b[i] = arr[i + middle + 1];
    }

    i = j = 0;

    while (i < a.size() && j < b.size()) {
        if (a[i] <= b[j]) {
            arr[k++] = a[i++];
        } else {
            arr[k++] = b[j++];
        }
    }

    while (i < a.size()) {
        arr[k++] = a[i++];
    }

    while (j < b.size()) {
        arr[k++] = b[j++];
    }
}

template <typename T>
void merge_sort(std::vector<T> &arr, size_t left, size_t right) {
    if (left < right) {
        size_t middle = left + (right - left) / 2;
        merge_sort(arr, left, middle);
        merge_sort(arr, middle + 1, right);
        if (right - left + 1 <= 16) {
            merge(arr, left, right, middle);
        }
    }
}

template <typename T>
void temp_sort(std::vector<T> &arr, size_t block) {
    for (size_t i = 0; i < arr.size(); i += block) {
        for (size_t j = i ; j < i + block; j++) {
            for (size_t z = i; z < i + block - 1 - (j - i); z++) {
                if (arr[z] > arr[z + 1]) {
                    std::swap(arr[z], arr[z + 1]);
                }
            }
        }
    }
}

template <typename T>
bool checked_sort(const std::vector<T> &arr, size_t block) {
    for (size_t i = 0; i < arr.size(); i += block) {
        for (size_t j = i + 1; j < i + block; j++) {
            if (arr[j] < arr[j - 1]) {
                std::cout << "ERROR INDEX: " << j << std::endl; // ДЛЯ ДЕБАГА
                return false;
            }
        }
    }
    return true;
}

std::string kernel_code = // Попытка сделать слияние одним ядром
        "__kernel void merge(__global const int *arr, "
        "                    int size_arr, "
        "                    int size_area, "
        "                    int count_area, "
        "                    int size_wk)"
        "{                              "
        "   int i = get_global_id(0); "
        "   int id = get_group_id(0);  "
        //            "   __local int a[size_wk];"
        //            "   __local int b[size_wk];"
        "   if (id == 1) {                            "
        "       int x1 = size_wk * id + size_wk - 1, y1 = x1 + 1; "
        "       int x2 = size_wk * id, y2 = y1 + size_wk - 1;"
        "       while (((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)) > 1)"
        "       {"
        "           int mid_x = (x1 + x2) / 2;"
        "           int mid_y = (y1 + y2) / 2;"
        "           if (arr[mid_y] >= arr[mid_x]) {"
        "               x1 = mid_x;"
        "               y1 = mid_y;"
        "           } else {"
        "               x2 = mid_x;"
        "               y2 = mid_x;"
        "           }"
        "       }"
        "   }                                    "
        "       "


        "   if (id <= mid) {"
        "       a[id - left] = arr[id];"
        "   } else {"
        "       b[id - left] = arr[id];"
        "   }"
        "   int n1 = mid - left + 1; "
        "   int n2 = right - mid; "
        "   "
        "   "
        //            "   int x1 = size_wk * id + size_wk, y1 = "
        //            "   int left = size_wk * id; "
        //            "   int right = left + size_wk; "
        //            "   int mid = left + (right - left) / 2; "
        //            "   __local int a[mid - left + 1]; "
        //            "   __local int b[right - mid];"
        //            "                                      "
        "}";


void output_binary_matrix(const vector<int> &arr, int size_area, int size_wk, int step) {
    std::cout << "///////////////////" << std::endl << "\t";
    for (size_t i = step; i < step + size_wk; i++) {
        std::cout << arr[i] << "\t";
    } std::cout << std::endl;
    for (size_t i = step + size_area; i < (step + size_area + size_wk); i++) {
        std::cout << arr[i] << "\t";
        for (size_t j = step; j < step + size_wk; j++) {
            //cout << "i = " << i << " j = " << j << endl;
            if (arr[i] >= arr[j]) {
                std::cout << "1\t";
            } else {
                std::cout << "0\t";
            }
        } std::cout << endl;
    } std::cout << endl;
    std::cout << "///////////////////" << std::endl;
}

void output_binary_diagonal(const vector<int> &arr, int x1, int y1, int x2, int y2, int size_area, int size_wk, int step) {
    std::cout << "///////////////////" << endl;
    int count = 0;
    cout << "step + size_wk = " << step + size_wk << " step + size_area + size_wk = " << step + size_area + size_wk << endl;
    cout << "x1 = " << x1 << " y1 = " << y1 << " x2 = " << x2 << " y2 = " << y2 << endl << "\t";
    for (size_t i = step; i < step + size_wk; i++) {
        std::cout << arr[i] << "\t";
    } std::cout << std::endl;
    for (size_t i = step + size_area; i < (step + size_area + size_wk); i++) {
        std::cout << arr[i] << "\t";
        for (size_t j = step; j < step + size_wk; j++) {
//            cout << "i = " << i << " j = " << j << endl;
//            cout << "x1 = " << x1 << " y1 = " << y1 << endl;
//            if (j == x1 && i == y1) {
//                cout << "CHECK" << endl;
//            }
            if (j == x1 && i == y1 && x1 >= x2 && y1 <= y2) {
                count++;
                if (arr[i] > arr[j]) {
                    std::cout << "+\t";
                } else {
                    std::cout << "-\t";
                }
                x1 -= 1; y1 += 1;
            } else {
                if (arr[i] > arr[j]) {
                    std::cout << "1\t";
                } else {
                    std::cout << "0\t";
                }
            }
        } std::cout << endl;
    } std::cout << endl;
    std::cout << "///////////////////" << std::endl;
    if (count < 2) {
        cout << "НЕПРАВИЛЬНО БИНАРИШЬ!!! count = " << count << endl;
        exit(1);
    }
}


std::pair<bool, bool> search_bin(const vector<int> &arr, int size_arr, int size_area, int size_wk, int step) {
    std::pair<bool, bool> pair_if(false, false);

    int x1 = step + size_wk - 1, y1 = step + size_area;
    int x2 = step, y2 = step + size_area + size_wk - 1; //size_wk + 2 * size_area - 1;
    output_binary_diagonal(arr, x1, y1, x2, y2, size_area, size_wk, step);
    while (abs(x1 - x2) != 1) {
        int mid_x = (x1 + x2) / 2;
        int mid_y = (y1 + y2) / 2;
        cout << "x1 = " << x1 << " x2 = " << x2 << endl;
        cout << "y1 = " << y1 << " y2 = " << y2 << endl;
        if ((x1 + x2) % 2 != 0) {
            mid_x += 1;
        }
        cout << "mid_x = " << mid_x << " mid_y = " << mid_y << " arr[mid_y] = " << arr[mid_y] << " arr[mid_x] = " << arr[mid_x] << endl;
        if (arr[mid_y] >= arr[mid_x]) {
            x2 = mid_x;
            y2 = mid_y;
        } else {
            x1 = mid_x;
            y1 = mid_y;
        }
        output_binary_diagonal(arr, x1, y1, x2, y2, size_area, size_wk, step);
    }
    if (arr[y1] >= arr[x1]) {
        pair_if.first = true;
    } else {
        pair_if.first = false;
    }

    if (arr[y2] >= arr[x2]) {
        pair_if.second = true;
    } else {
        pair_if.second = false;
    }

    return pair_if;
}


int main() {
    vector <int> arr(1000);
    for (auto &it : arr) {
        it = rand() % 1000 + 1;
    }
    temp_sort(arr, 64);
    if (!checked_sort(arr, 64)) {
        cout << "НЕПРАВИЛЬНО ОТСОРТИРОВАННЫЙ МАССИВ!!!" << endl;
        return 0;
    }
    for (size_t i = 0; i < arr.size(); i += 64) {
        cout << "BLOCK " << i / 64 + 1 << ": ";
        for (size_t j = i; j < i + 64; j++) {
            cout << arr[j] << " ";
        } cout << endl;
    }
//    vector <int> arr(128);
//    for (size_t i = 0; i < 64; i++) {
//        arr[i] = i;
//    }
//    for (size_t i = 64, j = 0; i < 128; i++, j++) {
//        arr[i] = j;
//    }
    int size_area = 64;
    int size_wk = 8;
    int step = 128;
    output_binary_matrix(arr, size_area, size_wk, step);
    cout << checked_sort(arr, 64) << " size block: "  << 64 << endl;
    std::pair<bool, bool> ind = search_bin(arr, arr.size(), size_area, size_wk, step);
    std::cout << ind.first << " " << ind.second << endl;
    return 0;
}




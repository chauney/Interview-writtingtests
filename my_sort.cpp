/*
* 排序算法
*/
#include <vector>
#include <iostream>

void swapValue(int& a, int& b)
{
    int tmp = a;
    a = b;
    b = tmp;
}

// 冒泡排序
// 通过相邻元素比较交换，将最大值冒泡到队列的最后，执行n-1轮
void bubbleSort(std::vector<int>& arr)
{
    int n = arr.size();
    // 外层循环控制轮数（每轮确定一个最大值的位置）
    for (int  i = 0; i < n - 1; i++) {
        // 内层循环遍历未排序部分
        for (int j = 0; j < n - 1 - i; j++) {
            // 如果当前元素比后一个大，交换它们
            if (arr[j] > arr[j+1]) {
                swapValue(arr[j], arr[j + 1]);
            }
        }
    }
}

// 选择排序
// 每轮选择未排序部分的最小值，与当前起始位置交换。
void selectionSort(std::vector<int>& arr)
{
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int index = i; // 记录当前轮次最小值的索引
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[index]) {
                index = j; // 更新最小值位置
            }
        }
        swapValue(arr[index], arr[i]);
    }
}

// 插入排序
// 将未排序的数字插入到已经排序的数组中
void insertSort(std::vector<int>& arr)
{
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// 希尔排序
// 通过将数组元素按特定间隔分组，对每组进行插入排序，随着间隔逐渐减小，最终完成整体排序。
// 间隔序列（Gap Sequence）​​：使用递减的间隔（如Knuth序列：gap = gap * 3 + 1），逐步将数组变为“基本有序”，减少元素移动次数。
// ​​时间复杂度​​：取决于间隔序列，通常为 ​​O(n log n) ~ O(n²)​​，最优可达到 ​​O(n log² n)​​。
void shellSort(std::vector<int>& arr)
{
    int n = arr.size();
    int gap = 1;

    while (gap < n / 3) {
        gap = gap * 3 + 1;
    }
    // 逐渐缩小间隔
    while (gap >= 1) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j = i;
            while (j >= gap && arr[j - gap] > temp) {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = temp;
        }
        gap /= 3;
    }
}

// 快速排序
// 选择一个基准值，将数组快速分为两个部分，然后进行递归排序
int partition(std::vector<int>& arr, int low, int high)
{
    int paivot = arr[high];
    int i = low - 1;
    for (int j=low; j < high; j++) {
        if (arr[j] <= paivot) {
            i++;
            swapValue(arr[i], arr[j]);
        }
    }
    swapValue(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(std::vector<int>& arr, int low, int high)
{
    if (low < high) {
        int paivot = partition(arr, low, high);
        quickSort(arr, low, paivot - 1);
        quickSort(arr, paivot, high);
    }
}

void merge(std::vector<int>& arr, int l, int m, int r)
{
    // std::vector<int> temp(r - l + 1);
    // int i = l;
    // int j = m + 1;
    // int k = 0;

    // while(i <= m && j <= r) {
    //     temp[k++] = (arr[i] <= arr[j]) ? arr[i++] : arr[j++];
    // }

    // while(i <= m) {
    //     temp[k++] = arr[i++];
    // }

    // while (j <= r) {
    //     temp[k++] = arr[j++];
    // }

    // for (int p = 0; p < k; p++) {
    //     arr[l + p] = temp[p];
    // }

    std::vector<int> temp(r - l + 1); // 临时存储合并结果
    int i = l, j = m+1, k = 0; // i左数组指针，j右数组指针，k临时数组指针

    // 合并两个子数组
    while (i <= m && j <= r) {
        temp[k++] = (arr[i] <= arr[j]) ? arr[i++] : arr[j++];
    }
    // 拷贝左数组剩余元素
    while (i <= m) temp[k++] = arr[i++];
    // 拷贝右数组剩余元素
    while (j <= r) temp[k++] = arr[j++];
    // 将临时数组拷贝回原数组
    for (int p = 0; p < k; p++) {
        arr[l + p] = temp[p];
    }
}

void mergeSort(std::vector<int>& arr, int l, int r)
{
    if (l >= r) return; // 递归终止条件
    int m = l + (r - l)/2; // 计算中间点（防溢出）
    mergeSort(arr, l, m);   // 分割左半部分
    mergeSort(arr, m+1, r); // 分割右半部分
    merge(arr, l, m, r);    // 合并结果
}

// 维护以i为根的子树满足最大堆性质
void heapify(std::vector<int>& arr, int n, int i) {
    int largest = i;        // 初始化最大元素为根
    int left = 2*i + 1;     // 左子节点索引
    int right = 2*i + 2;    // 右子节点索引

    // 如果左子节点比根大，更新largest
    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }
    // 如果右子节点比根大，更新largest
    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }
    // 如果最大值不是根节点，交换并递归调整
    if (largest != i) {
        swapValue(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(std::vector<int>& arr) {
    int n = arr.size();
    // 构建最大堆（从最后一个非叶子节点开始调整）
    for (int i = n/2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
    // 逐个提取堆顶元素（最大值）
    for (int i = n-1; i > 0; i--) {
        swapValue(arr[0], arr[i]); // 将最大值移到数组末尾
        heapify(arr, i, 0);   // 调整剩余元素为最大堆
    }
}

int main()
{
    std::vector<int> arr{12, 11, 30, 22, 55, 68, 39, 9, 6, 99, 30, 55};

    // bubbleSort(arr);
    // std::cout << "bubbleSort: ";
    // selectionSort(arr);
    // std::cout << "selectionSort: ";
    // insertSort(arr);
    // std::cout << "insertSort: ";
    // quickSort(arr, 0, 12);
    // std::cout << "quickSort: ";
    // mergeSort(arr, 0, 12);
    // std::cout << "mergeSort: ";
    // heapSort(arr);
    // std::cout << "heapSort: ";
    shellSort(arr);
    std::cout << "shellSort: ";
    for (int i = 0; i < arr.size(); i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << " --" << std::endl;

}
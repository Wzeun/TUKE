#include <stdio.h>
#include <math.h>

// task 1
float lift_a_car(const int stick_length, const int human_weight, const int car_weight) {
    if (human_weight <= 0 || car_weight <= 0 || stick_length <= 0) {
        return -1.0f;
    }
    float r2 = (float)(stick_length * human_weight) / (human_weight + car_weight);
    return roundf(r2 * 100) / 100;
}

// task 2
float unit_price(const float pack_price, const int rolls_count, const int pieces_count) {
    if (rolls_count <= 0 || pieces_count <= 0 || pack_price < 0) {
        return -1.0f; 
    }
    int total_pieces = rolls_count * pieces_count;
    float price_per_piece = pack_price / total_pieces;
    float price_per_100_pieces = price_per_piece * 100;
    return roundf(price_per_100_pieces * 100) / 100; 
}

// task 3
int bank_notes(const int price) {
    int denominations[] = {200, 100, 50, 20, 10};
    int count = 0;
    int remaining = price;

    if (remaining < 10 || remaining % 10 != 0) return -1; 

    for (int i = 0; i < 5; i++) {
        while (remaining >= denominations[i]) {
            remaining -= denominations[i];
            count++;
        }
    }

    return count;
}

// task 4
int euler(const int n) {
    if (n <= 0) return 0;

    int result = n;
    int temp_n = n;

    for (int p = 2; p * p <= temp_n; p++) {
        if (temp_n % p == 0) {
            while (temp_n % p == 0) {
                temp_n /= p;
            }
            result -= result / p;
        }
    }
    if (temp_n > 1) result -= result / temp_n;

    return result;
}

// task 5
int find_missing_number(const int n, const int arr[]) {
    int total_sum = n * (n + 1) / 2;
    int array_sum = 0;
    for (int i = 0; i < n - 1; i++) { // n - 1 because one number is missing
        array_sum += arr[i];
    }
    return total_sum - array_sum;
}

// task 6
unsigned long sum_squared(const int line) {
    if (line < 0) return 0;

    unsigned long sum = 0;
    unsigned long coefficient = 1;

    for (int i = 0; i <= line; ++i) {
        sum += coefficient * coefficient;
        coefficient = coefficient * (line - i) / (i + 1);
    }
    return sum;
}

// task 7
int array_min(const int input_array[], const int array_size) {
    if (input_array == NULL || array_size <= 0) return -1;

    int min = input_array[0];
    for (int i = 1; i < array_size; i++) {
        if (input_array[i] < min) min = input_array[i];
    }
    return min;
}

int array_max(const int input_array[], const int array_size) {
    if (input_array == NULL || array_size <= 0) return -1;

    int max = input_array[0];
    for (int i = 1; i < array_size; i++) {
        if (input_array[i] > max) max = input_array[i];
    }
    return max;
}

// task 8
int factorize_count(const int n) {
    if (n < 2) return 0;

    int count = 0; 
    int num = n;  

    if (num % 2 == 0) {
        count++;
        while (num % 2 == 0) {
            num /= 2;
        }
    }

    for (int i = 3; i * i <= num; i += 2) {
        if (num % i == 0) {
            count++;
            while (num % i == 0) {
                num /= i;
            }
        }
    }

    if (num > 2) count ++; 
    return count;
}

// task 9
void podium(const int n, int arr[]) {
    int first = (n + 2) / 3;
    int second = first - 1;
    int third = n - first - second;
    if (first <= 0 || second <= 0 || third < 0 || second <= third) {
        first = (n + 1) / 3 + 1;
        second = (n + 1) / 3;
        third = n - first - second;
    }
    arr[0] = first;
    arr[1] = second;
    arr[2] = third;
}

int main() {
    // task 1
    printf("%.2f\n", lift_a_car(2, 80, 1400));
    printf("%.2f\n", lift_a_car(4, 90, 1650));

    // task 2
    printf("%.4f\n", unit_price(4.79, 16, 150)); 
    printf("%.4f\n", unit_price(5.63, 20, 200)); 

    // task 3
    printf("%d\n", bank_notes(540)); 
    printf("%d\n", bank_notes(5));   

    // task 4
    printf("%d\n", euler(6));
    printf("%d\n", euler(12));

    // task 5
    int input_array[] = {3, 0, 1};
    printf("%d\n", find_missing_number(3, input_array)); 

    // task 6
    printf("%lu\n", sum_squared(1));
    printf("%lu\n", sum_squared(4));
    printf("%lu\n", sum_squared(33));

    // task 7
    int input_array2[] = {1, 2, 3, 4, 5};
    printf("%d\n", array_min(input_array2, 5));
    printf("%d\n", array_max(input_array2, 5));
    printf("%d\n", array_max(NULL, 5));

    // task 8
    printf("%d\n", factorize_count(12));

    // task 9
    int heights[3];
    int material = 6;
    podium(material, heights);
    for(int i = 0; i < 3; i++){
        printf("%d ", heights[i]);
    }
    printf("\n");

    return 0;
}

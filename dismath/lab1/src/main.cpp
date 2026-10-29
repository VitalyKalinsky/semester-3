// Реализовать программу генерации бинарного кода Грея для заполнения универсума мультмножеств (заданной пользователем разрядности). На основе универсума формируются два мультимножества двумя способами заполнения - вручную и автоматически (выбирает пользователь). Мощности множеств задает пользователь. В результате на экран выводятся результаты действий над множествами: объединение, пересечение, разность (оба варианта), симметрическая разность, дополнение (оба варианта), арифметические сумма, разность, произведение и деление. Кроме того, требуется реализовать защиту от некорректного пользовательского ввода и пользовательское меню.
// input_number()
// input_sequence()

// union()
// insertion()
// difference()
// sym_difference()
// complement()
// operator+()

#include <string>
#include <limits>
#include <iostream>
#include <vector>
#include <bitset>
#include <format>
#include <algorithm>
#include <chrono>
#include "MultiSet.hpp"

int U_SIZE = 0;
MultiSet multi_universe;

using namespace std;
int getValidatedInt(const string &prompt = "", int min_val = 0, int max_val = numeric_limits<int>::max())
{
    int value;
    while (true)
    {
        cout << prompt;
        if (cin >> value && value >= min_val && value <= max_val)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "Ошибка ввода! Пожалуйста, введите корректное число от " << min_val << " до " << max_val << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int Q(int i)
{
    int q = 1;
    int j = i;
    while (j % 2 == 0)
    {
        j = j / 2;
        q = q + 1;
    }
    return q;
}

void generateGrayCode(int n, vector<int> &target)
{
    target.clear();
    int B = 0;
    target.push_back(B);

    int total_steps = 1 << n;

    for (int i = 1; i < total_steps; i++)
    {
        int p_index = Q(i) - 1;
        B ^= 1 << p_index;
        target.push_back(B);
    }
}

void printBinary(int code, int bitrate)
{
    for (int i = bitrate - 1; i >= 0; i--)
    {
        cout << ((code >> i) & 1);
    }
}

void printMultiSet(MultiSet &ms, const vector<int> &u, int bitrate)
{
    if (ms.size() == 0)
    {
        cout << "Мультимножество пустое!\n";
        return;
    }

    size_t limit = u.size();
    bool is_truncated = ms.isBigSize();

    if (is_truncated)
    {
        cout << "\nМножество слишком большое для вывода, будут выведены первые 32 элемента:\n";
        limit = 32;
    }

    bool first = true;
    cout << "{";
    int count = 0;
    for (size_t i = 0; count < limit && i < u.size(); ++i)
    {
        if (ms[i] > 0)
        {
            if (!first)
                cout << ", ";
            cout << "'";
            printBinary(u[i], bitrate);
            cout << "': " << ms[i];
            first = false;
            count++;
        }
    }

    if (is_truncated)
    {
        if (!first)
            cout << ", ...";
        else
            cout << "...";
    }
    cout << "}\n";
}

long long createUniverse(MultiSet &u)
{
    u.resize(U_SIZE, 0);
    if (U_SIZE == 0)
        return 0;
    long long total_capacity = 0;
    for (int i = 0; i < U_SIZE; ++i)
    {
        u[i] = 1 + rand() % 10;
        total_capacity += u[i];
    }
    return total_capacity;
}

void fillManually(MultiSet &ms, const vector<int> &universe, int bitrate, int target_capacity)
{
    int current_capacity = 0;

    cout << "\nДоступные элементы универсума (код и максимальная кратность в U):\n";
    for (int i = 0; i < universe.size(); ++i)
    {
        cout << i + 1 << ". [";
        printBinary(universe[i], bitrate);
        cout << "] — кратность в U: " << multi_universe[i] << "\n";
    }

    while (current_capacity < target_capacity)
    {
        cout << "\nТекущая мощность: " << current_capacity << " из " << target_capacity << "\n";
        int idx = getValidatedInt(format("Выберите номер элемента для добавления (от {} до {}): ", 1, universe.size()), 1, universe.size()) - 1;

        int remaining_in_u = multi_universe[idx] - ms[idx];
        if (remaining_in_u <= 0)
        {
            cout << "Этот элемент недоступен (оставшаяся ёмкость в универсуме равна 0). Выберите другой элемент.\n";
            continue;
        }

        int max_allowed = min(remaining_in_u, target_capacity - current_capacity);
        string prompt = format("Введите кратность от 1 до {} (оставшаяся ёмкость в U: {}): ", max_allowed, remaining_in_u);
        int count = getValidatedInt(prompt, 1, max_allowed);

        ms[idx] += count;
        current_capacity += count;
    }

    cout << "==============================================\n";
}

void fillAutomatically(MultiSet &ms, const vector<int> &universe, int target_capacity)
{
    int current_capacity = 0;
    while (current_capacity < target_capacity)
    {
        int idx = rand() % U_SIZE;
        int max_delta = min(multi_universe[idx] - ms[idx], target_capacity - current_capacity);
        if (max_delta <= 0)
            continue;
        int delta = 1 + rand() % max_delta;
        ms[idx] += delta;
        current_capacity += delta;
    }
    cout << "\nАвтоматическое заполнение выполнено! \n";
    cout << "==============================================\n";
}

int createMultiSets(MultiSet &A, MultiSet &B, vector<int> &universe)
{
    // при bitrate = 28 время вычисления кода грея составило в среднем 7.3 секунды. при 27 - 3.6 сек.
    A.clear();
    B.clear();
    universe.clear();
    multi_universe.clear();
    U_SIZE = 0;
    int bitrate = getValidatedInt("Введите разрядность кода Грея (0-27): ", 0, 27);
    auto start = chrono::steady_clock::now();
    if (bitrate == 0)
    {
        cout << "\nУниверсум пуст (мощность равна 0). Множества A и B автоматически оставлены пустыми.\n";
    }
    else
    {
        universe.resize(1 << bitrate, 0);
        generateGrayCode(bitrate, universe);
        U_SIZE = universe.size();

        A.resize(U_SIZE, 0);
        B.resize(U_SIZE, 0);
        long long total_u_capacity = createUniverse(multi_universe);

        auto end = chrono::steady_clock::now();
        chrono::duration<double> elapsed = end - start;
        cout << "Количество кодов: " << U_SIZE << ". Прошло времени: " << elapsed.count() << " сек.\n";

        cout << "\nСгенерированный универсум U: ";
        printMultiSet(multi_universe, universe, bitrate);

        int lower_capacity = 0;
        int upper_capacity;
        if (100LL < total_u_capacity)
            upper_capacity = 100;
        else
            upper_capacity = total_u_capacity;

        cout << "\n\n================ Заполнение A ================\n";
        int capacityA = getValidatedInt(format("\nВведите мощность мультимножества A (от {} до {}): ", lower_capacity, upper_capacity),
                                        lower_capacity, upper_capacity);

        if (capacityA > 0)
        {
            int choiceA = getValidatedInt("\nВыберите способ заполнения множества A (1 - Вручную, 2 - Автоматически): ", 1, 2);
            if (choiceA == 1)
                fillManually(A, universe, bitrate, capacityA);
            else
                fillAutomatically(A, universe, capacityA);
        }

        cout << "\n\n================ Заполнение B ================\n";
        int capacityB = getValidatedInt(format("\nВведите мощность мультимножества B (от {} до {}): ", lower_capacity, upper_capacity),
                                        lower_capacity, upper_capacity);

        if (capacityB > 0)
        {
            int choiceB = getValidatedInt("\nВыберите способ заполнения множества B (1 - Вручную, 2 - Автоматически): ", 1, 2);
            if (choiceB == 1)
                fillManually(B, universe, bitrate, capacityB);
            else
                fillAutomatically(B, universe, capacityB);
        }
    }
    return bitrate;
}
int main(int argc, char const *argv[])
{
    srand(time(nullptr));
    MultiSet A, B;
    vector<int> universe;
    int bitrate = createMultiSets(A, B, universe);
    while (true)
    {
        cout << "\n\n================ Главное меню ================\n";
        cout << "0. Выход\n";
        cout << "1. Показать исходные мультимножества\n";
        cout << "2. Объединение (A u B)\n";
        cout << "3. Пересечение (A n B)\n";
        cout << "4. Разность (A \\ B и B \\ A)\n";
        cout << "5. Симметрическая разность (A /\\ B)\n";
        cout << "6. Дополнение (U \\ A и U \\ B)\n";
        cout << "7. Арифметическая сумма\n";
        cout << "8. Арифметическая разность\n";
        cout << "9. Арифметическое произведение\n";
        cout << "10. Арифметическое деление\n";
        cout << "==============================================\n";

        int menuChoice = getValidatedInt("Выберите действие: ", 0, 10);

        if (menuChoice == 0)
        {
            cout << "\n\n========== Вы хотите начать заново? ==========\n";
            cout << "0. Выход\n";
            cout << "1. Начать заново\n";
            cout << "==============================================\n";
            menuChoice = getValidatedInt("Выберите действие: ", 0, 1);
            if (menuChoice == 1)
            {
                bitrate = createMultiSets(A, B, universe);
                continue;
            }
            else
            {
                cout << "Программа завершена.\n";
                break;
            }
        }

        MultiSet result;

        switch (menuChoice)
        {
        case 1:
            cout << "\nМультимножество A: ";
            printMultiSet(A, universe, bitrate);
            cout << "Мультимножество B: ";
            printMultiSet(B, universe, bitrate);
            cout << "Универсум: ";
            printMultiSet(multi_universe, universe, bitrate);
            break;
        case 2:
            result = uni(A, B);
            cout << "Объединение (A u B): ";
            printMultiSet(result, universe, bitrate);
            break;
        case 3:
            result = insertion(A, B);
            cout << "Пересечение (A n B): ";
            printMultiSet(result, universe, bitrate);
            break;
        case 4:
            result = difference(A, B);
            cout << "Разность (A \\ B): ";
            printMultiSet(result, universe, bitrate);

            result = difference(B, A);
            cout << "Разность (B \\ A): ";
            printMultiSet(result, universe, bitrate);
            break;
        case 5:
            result = sym_difference(A, B);
            cout << "Симметрическая разность (A /\\ B): ";
            printMultiSet(result, universe, bitrate);
            break;
        case 6:
            result = complement(A);
            cout << "Дополнение (U \\ A): ";
            printMultiSet(result, universe, bitrate);

            result = complement(B);
            cout << "Дополнение (U \\ B): ";
            printMultiSet(result, universe, bitrate);
            break;
        case 7:
            result = A + B;
            cout << "Арифметическая сумма (A + B): ";
            printMultiSet(result, universe, bitrate);
            break;
        case 8:
            result = A - B;
            cout << "Арифметическая разность (A - B): ";
            printMultiSet(result, universe, bitrate);

            result = B - A;
            cout << "Арифметическая разность (B - A): ";
            printMultiSet(result, universe, bitrate);
            break;
        case 9:
            result = A * B;
            cout << "Арифметическое произведение (A * B): ";
            printMultiSet(result, universe, bitrate);
            break;
        case 10:
            result = A / B;
            cout << "Арифметическое деление (A / B): ";
            printMultiSet(result, universe, bitrate);

            result = B / A;
            cout << "Арифметическое деление (B / A): ";
            printMultiSet(result, universe, bitrate);
            break;
        }
    }

    return 0;
}

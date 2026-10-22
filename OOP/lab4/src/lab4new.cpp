//========================================================================================
//	Лабораторная №4. Концепты. Диапазоны. Трёхстороннее сравнение С++.
//	C++20	         Назначенные инициализаторы. Корутины. Шаблонные лямбды. Атрибуты.
//========================================================================================
#include <iostream>
#include <vector>
#include <string>
#include <concepts>
#include <ranges>
#include <algorithm>
#include <span>
#include <coroutine>
#include <numeric>
#include <compare>
#include <optional>
#include <cstdio>
#include <string>
#include <queue>
#include <stack>
#include <array>
#include <exception>
#include "point.hpp"
#include "point3d.hpp"
#include <thread>

using namespace std;

template <class T>
concept Printable = requires(T val) {
    cout << val;
};

template <Printable T>
void printValue(T val)
{
    cout << val << '\n';
}

template <class T>
concept PrintableContainer = requires(T val) {
    val.pop();
    val.empty();
};

template <PrintableContainer T>
void print_adapter(T stack, const string &message = "")
{
    cout << "\n======================================" << endl;
    if (!message.empty())
        printf("%*s\n", int((40 + message.length()) / 2), message.c_str());
    else
        cout << "print_adapter\n";
    cout << "======================================" << endl;
    while (!stack.empty())
    {
        if constexpr (requires { stack.top(); })
        {
            cout << stack.top() << endl;
        }
        else
        {
            cout << stack.front() << endl;
        }

        stack.pop();
    }

    cout << "======================================" << endl
         << endl;
}

template <class T>
concept PrintableRange = ranges::input_range<T> && requires(ranges::range_reference_t<T> element) {
    { cout << element };
};

template <PrintableRange T>
void pr(T &v)
{
    for (auto el : v)
        cout << el << " ";
    cout << "\n";
}

template <class T, class K>
concept Summable = requires(T t, K k) {
    { t + k } -> convertible_to<T>;
};

template <class T, class K>
    requires Summable<T, K>
T sumTwo(T a, K b)
{
    return a + b;
}

template <integral T, T N>
constexpr T factorial()
{
    static_assert(N >= 0, "Ошибка: n не может быть отрицательным\n");
    static_assert(N <= 20, "Ошибка: n превышает максимум\n");

    if constexpr (N <= 1)
        return 1;
    else
        return N * factorial<T, N - 1>();
}

struct QuadraticRoots
{
    double x1 = 0.0;
    double x2 = 0.0;
    bool has_roots = false;
};

constexpr double const_sqrt(double val)
{
    if (val <= 0.0)
        return 0.0;
    double curr = val > 1.0 ? val : 1.0;
    // вычисляем корень по методу Ньютона, тк стандартная sqrt не constexpr (только в с++26, которая ещё официально не выпущена)
    for (int i = 0; i < 25; i++)
    {
        curr = 0.5 * (curr + val / curr);
    }
    return curr;
}

consteval QuadraticRoots solve_quadratic(double a, double b, double c)
{
    double d = b * b - 4.0 * a * c;
    if (d < 0.0)
        return {0.0, 0.0, false};
    double sq = const_sqrt(d);
    return {(-b + sq) / (2.0 * a), (-b - sq) / (2.0 * a), true};
}

constexpr int fibonacci(int n)
{
    if (n <= 0)
        return 0;
    if (n == 1)
        return 1;
    int a = 0, b = 1;
    for (int i = 2; i <= n; i++)
    {
        int next_val = a + b;
        a = b;
        b = next_val;
    }
    return b;
}

struct Box
{
    double width = 0.0;
    double height = 0.0;
    double depth = 0.0;

    double volume() const
    {
        return width * height * depth;
    }

    partial_ordering operator<=>(const Box &other) const
    {
        if (volume() == 0 || other.volume() == 0)
        {
            return partial_ordering::unordered;
        }
        return volume() <=> other.volume();
    }

    bool operator==(const Box &other) const
    {
        return (*this <=> other) == 0;
    }
};

struct Config
{
    string host = "localhost";
    int port = 8080;
    bool ssl = false;
    int timeout = 30;
};

void printSpan(span<int> s)
{
    for (int val : s)
        cout << val << ' ';
    cout << '\n';
}

double average(span<const double> data)
{
    if (data.empty())
        return 0.0;
    double sum = 0.0;
    for (double val : data)
        sum += val;
    return sum / static_cast<double>(data.size());
}

struct Product
{
    string name;
    double price = 0.0;
};

void reduce_price(span<Product> items, double discount)
{
    for (auto &item : items)
    {
        item.price = max(0.0, item.price - discount);
    }
}

template <class T>
struct Generator
{
    struct promise_type
    {
        T current_value;

        Generator get_return_object()
        {
            return Generator{coroutine_handle<promise_type>::from_promise(*this)};
        }

        suspend_always initial_suspend() { return {}; }

        suspend_always final_suspend() noexcept { return {}; }

        suspend_always yield_value(T v)
        {
            current_value = v;
            return {};
        }

        void return_void() {}

        void unhandled_exception() { terminate(); }
    };

    coroutine_handle<promise_type> handle;

    explicit Generator(coroutine_handle<promise_type> h) : handle(h) {}

    ~Generator()
    {
        if (handle)
            handle.destroy();
    }

    Generator(const Generator &) = delete;
    Generator &operator=(const Generator &) = delete;

    Generator(Generator &&other) noexcept : handle(other.handle) { other.handle = nullptr; }
    Generator &operator=(Generator &&other) noexcept
    {
        if (this != &other)
        {
            if (handle)
                handle.destroy();
            handle = other.handle;
            other.handle = nullptr;
        }
        return *this;
    }

    bool next()
    {
        handle.resume();
        return !handle.done();
    }

    T value()
    {
        return handle.promise().current_value;
    }
};

// Ваша функция-корутина (T заменяется на int)
Generator<int> count_to(int limit)
{
    if (limit < 1)
        co_return;
    for (int i = 1; i <= limit; i++)
    {
        co_yield i;
    }
}

enum class Status
{
    Success,
    Timeout,
    CriticalError
};

void handle_status(Status n)
{
    switch (n)
    {
    case Status::Success:
        [[likely]] cout << "Status: Success\n";
        break;
    case Status::Timeout:
        [[unlikely]] cout << "Status: Timeout\n";
        break;
    case Status::CriticalError:
        [[unlikely]] cout << "Status: CriticalError\n";
        break;
    }
}

int main()
{

    // РАЗДЕЛ 1: CONCEPTS (Концепт)
    // Задание 1.1
    // Определите концепт Printable, который проверяет, что тип T можно вывести
    // через std::cout (т.е. оператор << определён для std::ostream и T).
    // Затем напишите шаблонную функцию printValue(T val), которая принимает
    // только типы, удовлетворяющие концепту Printable, и выводит значение.
    // Перепишите шаблон функции для вывода значений stack, queue, priority_queue из Лаб. 3 через концепты и constexpr в единую функцию
    cout << "\n/////////////////////////////////////\n"
         << "1.1" << "\n/////////////////////////////////////\n";
    Point p(1.0, 2.0);
    printValue(p);

    stack<int> s_adapter;
    s_adapter.push(1);
    s_adapter.push(2);

    queue<int> q_adapter;
    q_adapter.push(3);
    q_adapter.push(4);

    priority_queue<int> pq_adapter;
    pq_adapter.push(5);
    pq_adapter.push(6);

    print_adapter(s_adapter);
    print_adapter(q_adapter);
    print_adapter(pq_adapter);
    // Задание 1.2
    // Напишите концепт Summable, который проверяет, что два значения типа T или K
    // можно сложить оператором +, где K можно привести к T, результат приводится к типу T.
    // Затем напишите шаблонную функцию sumTwo, возвращающую их сумму.
    cout << "\n/////////////////////////////////////\n"
         << "1.2" << "\n/////////////////////////////////////\n";

    cout << "sumTwo: " << sumTwo(10, 5.5) << '\n';
    // Задание 1.3
    // Используйте стандартный концепт std::integral для ограничения вычислений рекурсивной функции
    // factorial(T n), которая вычисляет факториал только для целых чисел и указывает максимальное значение n.
    cout << "\n/////////////////////////////////////\n"
         << "1.3" << "\n/////////////////////////////////////\n";
    constexpr int evnum = 5;
    auto fact = factorial<int, evnum>();
    cout << "factorial(5): " << fact << '\n';
    cout << "factorial(0): " << factorial<int, 4>() << '\n';

    // РАЗДЕЛ 2: RANGES (Диапазоны)

    // Задание 2.1
    // Создайте вектор из целых чисел, заполните вектор какими-либо значениями.
    // С помощью std::views::filter и std::views::transform:
    //   - оставьте только чётные числа,
    //   - умножьте каждое из них на 3,
    //   - выведите результат на экран.
    cout << "\n/////////////////////////////////////\n"
         << "2.1" << "\n/////////////////////////////////////\n";

    vector<int> vec_nums = {1, 2, 3, 7, 8, 9, 10, 4, 5, 6};
    auto filtered_transformed = vec_nums | views::filter([](int x)
                                                         { return x % 2 == 0; }) |
                                views::transform([](int x)
                                                 { return x * 3; });

    pr(filtered_transformed);

    // Задание 2.2
    // С помощью std::views::drop и std::views::take выведите элементы вектора
    // начиная с 3-го и заканчивая 6-м (включительно).
    cout << "\n/////////////////////////////////////\n"
         << "2.2" << "\n/////////////////////////////////////\n";

    auto slice_view = vec_nums | views::drop(2) | views::take(4);
    pr(slice_view);
    // Задание 2.3
    // Создайте вектор строк (std::string_view), заполните вектор какими-либо значениями.
    // Используйте std::ranges::sort для сортировки вектора строк в алфавитном порядке.
    // Объедините вектор строк в единый объект string_view используя std::views::join.
    // Затем используйте views::split и views::transform создайте новое представление ranges::transform_view
    // в котором строки приведены к верхнему регистру.
    // Используйте std::ranges::find, чтобы найти введенную пользователем строку в полученном представлении и выведите её индекс.
    cout << "\n/////////////////////////////////////\n"
         << "2.3" << "\n/////////////////////////////////////\n";

    vector<string_view> words_vec = {"cherry.", "apple.", "banana.", "peach.", "orange.", "orange.", "apricot."};

    ranges::sort(words_vec);
    pr(words_vec);

    auto joined_view = words_vec | views::join;
    pr(joined_view);

    string joined_buffer(joined_view.begin(), joined_view.end());
    string_view joined_sv = joined_buffer;
    auto upper_view = joined_sv | views::split('.') | views::filter([](auto r)
                                                                    { return !ranges::empty(r); }) |
                      views::transform([](auto r)
                                       {     string s;
    for (char c : r) 
        s += static_cast<char>(toupper(c));
    return s; });

    pr(upper_view);

    string target_word = "ORANGE";
    auto it_word = ranges::find(upper_view, target_word);
    if (it_word != upper_view.end())
    {
        cout << "Индекс " << target_word << ": " << ranges::distance(upper_view.begin(), it_word) << '\n';
    }
    // РАЗДЕЛ 3: consteval и constexpr

    // Задание 3.1
    cout << "\n/////////////////////////////////////\n"
         << "3.1" << "\n/////////////////////////////////////\n";
    // Напишите функцию вычисления квадратного уравнения через consteval
    // ТОЛЬКО на этапе компиляции. Создайте constexpr переменную со значением и выведите результат.
    constexpr QuadraticRoots quad_roots = solve_quadratic(1.0, -5.0, 6.0);
    cout << "Корни: " << quad_roots.x1 << ", " << quad_roots.x2 << '\n';
    // Задание 3.2
    cout << "\n/////////////////////////////////////\n"
         << "3.2" << "\n/////////////////////////////////////\n";
    // Напишите constexpr-функцию fibonacci(int n), возвращающую n-е число
    // Фибоначчи итеративно. Убедитесь, что она работает как в compile-time
    // (constexpr переменная), так и в run-time (обычный вызов).
    // fibonacci(10) = 55,  fibonacci(0) = 0,  fibonacci(1) = 1
    constexpr int fib_ct_10 = fibonacci(10);
    constexpr int fib_ct_0 = fibonacci(0);
    constexpr int fib_ct_1 = fibonacci(1);
    int runtime_n_10 = 10;
    int runtime_n_0 = 0;
    int runtime_n_1 = 1;
    int fib_rt_10 = fibonacci(runtime_n_10);
    int fib_rt_0 = fibonacci(runtime_n_0);
    int fib_rt_1 = fibonacci(runtime_n_1);
    cout << "fib compile-time: " << fib_ct_10 << ", run-time: " << fib_rt_10 << '\n';
    cout << "fib compile-time: " << fib_ct_0 << ", run-time: " << fib_rt_0 << '\n';
    cout << "fib compile-time: " << fib_ct_1 << ", run-time: " << fib_rt_1 << '\n';
    // РАЗДЕЛ 4: Трёхстороннее сравнение (Оператор космического корабля <=>)

    // Задание 4.1
    cout << "\n/////////////////////////////////////\n"
         << "4.1" << "\n/////////////////////////////////////\n";
    // Добавьте перегрузку оператора <=> для класса Point
    // Проверьте результаты сравнений точек между собой: < > == !=
    // Модифицируйте оператор через std::weak_ordering, где критерием сравнения будет являться
    // порядок точек плоскости, определяемый удалённостью от центра.
    Point pt_a{3.0, 4.0}, pt_b{0.0, 5.0}, pt_c{1.0, 1.0};
    cout << (pt_a >= pt_b) << ' '
         << (pt_c <= pt_a) << ' '
         << (pt_a != pt_c) << ' '
         << (pt_a == pt_c) << '\n';
    // Задание 4.2
    cout << "\n/////////////////////////////////////\n"
         << "4.2" << "\n/////////////////////////////////////\n";
    // Создайте структуру Box с полями double width, height и depth.
    // Добавьте метод double volume() const.
    // Определите оператор <=>, сравнивающий Box по объёму вручную, с неопределенным unordered поведением для width, height, depth значения равного 0.
    // Проверьте результаты сравнений Box между собой: < > == !=
    // Отсортируйте вектор из трёх Box (std::ranges::sort) и выведите объёмы.

    vector<Box> boxes_vec = {{3.0, 4.0, 5.0}, {1.0, 2.0, 3.0}, {2.0, 2.0, 2.0}};
    vector<Box> boxes_vec1 = {{}, {}};
    cout << (boxes_vec1[0] == boxes_vec1[1]) << endl;
    ranges::sort(boxes_vec);

    for (const auto &b : boxes_vec)
        cout << b.volume() << ' ';
    cout << '\n';
    // РАЗДЕЛ 5: Designated Initializers (Назначенные инициализаторы)

    // Задание 5.1
    cout << "\n/////////////////////////////////////\n"
         << "5.1" << "\n/////////////////////////////////////\n";
    // Объявите структуру Point3D { double x = 0; double y = 0; double z = 0; }.
    // Создайте три объекта, используя designated initializers:
    //   - p1: только x = 1.0
    //   - p2: x = 2.0, z = 5.0
    //   - p3: все три поля (3.0, 4.0, 7.0)
    // Выведите координаты каждого объекта.

    Point3D p1{.x = 1.0};
    Point3D p2{.x = 2.0, .z = 5.0};
    Point3D p3{.x = 3.0, .y = 4.0, .z = 7.0};

    cout << "p1: " << p1.x << ", " << p1.y << ", " << p1.z << '\n';
    cout << "p2: " << p2.x << ", " << p2.y << ", " << p2.z << '\n';
    cout << "p3: " << p3.x << ", " << p3.y << ", " << p3.z << '\n';

    // Задание 5.2
    cout << "\n/////////////////////////////////////\n"
         << "5.2" << "\n/////////////////////////////////////\n";
    // Объявите структуру Config:
    //   std::string host = "localhost"; int port = 8080;
    //   bool ssl = false; int timeout = 30;
    // Создайте объект, установив через designated initializers только
    // host = "example.com" и ssl = true. Выведите все поля.

    Config cfg{.host = "example.com", .ssl = true};
    cout << cfg.host << ' ' << cfg.port << ' ' << cfg.ssl << ' ' << cfg.timeout << '\n';
    // РАЗДЕЛ 6: std::span

    // Задание 6.1
    cout << "\n/////////////////////////////////////\n"
         << "6.1" << "\n/////////////////////////////////////\n";
    // Напишите функцию printSpan(std::span<int> s), которая выводит все элементы
    // через пробел. Вызовите её с:
    //   1) int arr[] = {1, 2, 3, 4, 5, ...}
    //   2) std::vector<int>{10, 20, 30, ....}
    int raw_arr[] = {1, 2, 3, 4, 5};
    vector<int> raw_vec = {10, 20, 30, 40, 50, 60};
    printSpan(raw_arr);
    printSpan(raw_vec);

    // Задание 6.2
    cout << "\n/////////////////////////////////////\n"
         << "6.2" << "\n/////////////////////////////////////\n";
    // Напишите функцию double average(std::span<const double> data).
    // Если data пустой — вернуть 0.0. Иначе вернуть среднее арифметическое.
    // Проверьте на {1.0, 2.5, 3.0, 4.5, 5.0}.
    vector<double> avg_vals = {1.0, 2.5, 3.0, 4.5, 5.0};
    cout << "Average: " << average(avg_vals) << '\n';

    // Задание 6.3
    cout << "\n/////////////////////////////////////\n"
         << "6.3" << "\n/////////////////////////////////////\n";
    // Создайте структуру Product содержащую имя товара и цену. Создайте std::array из товаров заполненных произвольным образом.
    // Реализовать функцию, которая уменьшает цену каждого переданного товара на введенное значение, но не ниже нуля.
    // Создать std::span, представляющий весь массив. Через subspan выбрать два товара, начиная с индекса 1.
    // Уменьшить цены выбранных товаров на указанную константу. Вывести названия и цены всех товаров из исходного массива.
    array<Product, 4> products_arr = {{{"Product1", 100.0}, {"Product2", 150.0}, {"Product3", 200.0}, {"Product4", 250.0}}};
    span<Product> prod_span(products_arr);
    auto prod_subspan = prod_span.subspan(1, 2);
    reduce_price(prod_subspan, 60.0);
    for (const auto &p : prod_subspan)
    {
        cout << p.name << ": " << p.price << '\n';
    }

    // РАЗДЕЛ 7: Coroutines (Корутины)

    // Задание 7.1
    cout << "\n/////////////////////////////////////\n"
         << "7.1" << "\n/////////////////////////////////////\n";
    // Напишите корутину, которая по очереди выдаёт целые числа от 1 до 5.
    // Необходимо реализовать класс Generator. Он должен содержать:
    // - Вложенный тип promise_type.
    // - Дескриптор корутины std::coroutine_handle<promise_type>.
    // - Метод next(), который возобновляет корутину и возвращает true, если получено очередное число, или false, если выполнение завершено.
    // - Метод value(), возвращающий последнее выданное число.
    // - Деструктор, освобождающий корутину через destroy().
    // Реализуйте тип promise_type и необходимые методы:
    // - get_return_object().
    // - initial_suspend() — корутина должна начинать выполнение только при вызове next().
    // - final_suspend() — корутина должна оставаться приостановленной до освобождения её дескриптора.
    // - yield_value(int) — сохраняет число и приостанавливает выполнение.
    // - return_void().
    // - unhandled_exception() — для простоты вызывает std::terminate().
    // Для приостановки используется std::suspend_always.
    // Реализуйте корутину count_to. Она должна в цикле выдавать числа через co_yield.
    // Если limit меньше 1, она не должна выдавать ни одного числа.
    // Подсказка: в next() проверяйте, что корутина ещё не завершилась, прежде чем вызывать resume(). Вызывайте value() только после того, как next() вернул true.
    auto count_gen = count_to(5);
    while (count_gen.next())
    {
        cout << count_gen.value() << ' ';
    }

    cout << '\n';
    // РАЗДЕЛ 8: Шаблонные лямбды (C++20)

    // Задание 8.1
    cout << "\n/////////////////////////////////////\n"
         << "8.1" << "\n/////////////////////////////////////\n";
    // Напиши лямбду, которая преобразует элементы std::array с помощью переданной операции и возвращает новый массив.
    // Тип элементов результата может отличаться от типа исходных элементов. Заготовка шаблонной лямбды: auto transform_array = []<typename T, std::size_t N, typename F>,
    // где T — тип исходных элементов; N — размер массива; F — тип переданной операции.
    // Выведите элементы массива.
    auto transform_array = []<typename T, size_t N, typename F>(const array<T, N> &arr, F op)
    {
        array<invoke_result_t<F, T>, N> res{};
        for (size_t i = 0; i < N; i++)
            res[i] = op(arr[i]);
        return res;
    };
    array<int, 7> arr_input = {10, 20, 30, 40, 50, 60};
    auto arr_output = transform_array(arr_input, [](int x)
                                      { return x / 10; });
    for (auto val : arr_output)
        cout << val << ' ';
    cout << '\n';

    // Задание 8.2
    cout << "\n/////////////////////////////////////\n"
         << "8.2" << "\n/////////////////////////////////////\n";
    // Напишите лямбду-счётчик: при каждом вызове возвращает следующее число
    // начиная с 0 (0, 1, 2, 3, ...). Используйте захват count = 0 + mutable.
    auto counter_lambda = [count = 0]() mutable
    { return count++; };
    for (int i = 0; i < 10; i++)
        cout << counter_lambda() << ' ';
    cout << "\n";

    // Задание 8.3
    cout << "\n/////////////////////////////////////\n"
         << "8.3" << "\n/////////////////////////////////////\n";
    // Напиши одной лямбдой на C++20, которая вычисляет сумму элементов двух массивов (std::array) разных типов с явными шаблонными параметрами.
    auto sum_arrays = []<class T1, size_t N1, class T2, size_t N2>(const array<T1, N1> &a1, const array<T2, N2> &a2)
    {
        using res_type = decltype(declval<T1>() + declval<T2>());
        array<res_type, N1> sum_array;

        for (int i = 0; i < N1; i++)
        {
            sum_array[i] = a1[i] + a2[i];
        }

        return sum_array;
    };

    array<int, 2> arr1 = {1, 2};
    array<double, 2> arr2 = {3.5, 4.5};
    auto result = sum_arrays(arr1, arr2);
    for (int i = 0; i < 2; i++)
        cout << result[i] << ' ';
    cout << "\n";

    // РАЗДЕЛ 9: Атрибуты [[likely]] / [[unlikely]] и std::jthread

    // Задание 9.1
    cout << "\n/////////////////////////////////////\n"
         << "9.1" << "\n/////////////////////////////////////\n";
    // Напишите функцию handle_status(Status n), которая проверяет типы статусов в операторе switch/case:
    // Status::Success пометьте ветку [[likely]]
    // Status::Timeout пометьте ветку [[unlikely]]
    // Status::CriticalError пометьте ветку [[unlikely]]
    handle_status(Status::Success);
    handle_status(Status::Timeout);
    handle_status(Status::CriticalError);

    // Задание 9.2
    cout << "\n/////////////////////////////////////\n"
         << "9.2" << "\n/////////////////////////////////////\n";
    // Используя std::jthread, запустите поток, который выводит генерирует случаные числа в заданном пользователем диапазоне.
    // Передайте токен управления std::to_token в функцию с потоком. Задайте задержку между генерациями this_thread::sleep_for в 200мс
    // Напишите явный запрос остановки потока из основной функции через 3 секунды.
    int range_start = 1, range_end = 100;
    jthread worker_thread([range_start, range_end](stop_token stop_tok)
                          {while (!stop_tok.stop_requested()){
            int rand_val = range_start + rand() % (range_end - range_start + 1);
            cout << rand_val << ' ' << flush;
            this_thread::sleep_for(chrono::milliseconds(200));
        } });
    this_thread::sleep_for(chrono::seconds(3));
    worker_thread.request_stop();
    cout << "STOP!" << endl;
    // необязательно, но я хочу, чтобы вывод был последовательным
    cout << '\n';

    // РАЗДЕЛ 10: Range-for с инициализатором и std::views::iota

    // Задание 10.1
    cout << "\n/////////////////////////////////////\n"
         << "10.1" << "\n/////////////////////////////////////\n";
    // Напишите функцию getNumbers(), возвращающую vector<int> со случайными значениями.
    // Используйте range-for с инициализатором C++20. Выведите все элементы.
    auto getNumbers = [](int len)
    {
        vector<int> out;
        for (int i = 0; i < len; i++)
            out.push_back(rand() % 10);

        return out;
    };

    for (auto num : getNumbers(5))
    {
        cout << num << ' ';
    }
    cout << '\n';

    // Задание 10.2
    cout << "\n/////////////////////////////////////\n"
         << "10.2" << "\n/////////////////////////////////////\n";
    // Используйте std::views::iota для создания последовательности 1..n.
    // Вычислите сумму через std::accumulate и выведите.
    int n = 10;
    auto seq_iota = views::iota(1, n + 1);
    int total_iota = accumulate(seq_iota.begin(), seq_iota.end(), 0);
    cout << "Сумма 1..n: " << total_iota << '\n';
    // РАЗДЕЛ 11: std::optional

    // Задание 11.1
    cout << "\n/////////////////////////////////////\n"
         << "11.1" << "\n/////////////////////////////////////\n";
    // Напишите шаблонную функцию принимающую два контейнера из целых чисел и выполняющую поэлементное деление всех элементов одного контейнера на другой
    // В случае, если произошла ошибка: разный размер контейнеров, либо деление на ноль, то вернуть std::nullopt и применить монадическую операцию or_else
    // Вывести номера индексов, с которыми не получилось выполнить деление.
    vector<int> div_c1 = {10, 20, 30, 40, 50};
    vector<int> div_c2 = {2, 0, 5, 0};
    auto divide_containers = []<class C1, class C2>(const C1 &c1, const C2 &c2) -> optional<vector<int>>
    {
        if (c1.size() != c2.size())
        {
            return nullopt;
        }
        vector<int> res;
        for (size_t i = 0; i < c1.size(); i++)
        {
            if (c2[i] == 0)
                return nullopt;
            res.push_back(c1[i] / c2[i]);
        }
        return res;
    };

    auto div_res = divide_containers(div_c1, div_c2);

    div_res
        .and_then([&div_res, &div_c1, &div_c2](const vector<int> &res) -> optional<vector<int>>
                  {
                        cout << "Результат деления: ";
                        for (int val : div_res.value())
                        {
                            cout << val << ' ';
                        }
                        cout << '\n'; 
                        return res; })
        .or_else([&div_res, &div_c1, &div_c2]() -> optional<vector<int>>
                 {
                if (div_c1.size() != div_c2.size())
                {
                    cout << "Ошибка: размеры контейнеров не совпадают ("
                            << div_c1.size() << " и " << div_c2.size() << ")\n";
                }
                cout << "Ошибки деления на индексах: ";
                for (size_t i = 0; i < min(div_c1.size(), div_c2.size()); i++)
                {
                    if (div_c2[i] == 0)
                    {
                        cout << i << ' ';
                    }
                }
                cout << '\n';
                return nullopt; });

    return 0;
}

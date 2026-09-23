//=======================================================================
//	Лабораторная №3. Шаблоны функций. Шаблоны классов. Стандартные шаблоны С++.
//				Обработка исключений.
//=======================================================================
// Используйте недостающие файлы из лабораторной 2
// cmake -DCMAKE_CXX_FLAGS="-DNDEBUG -fsanitize=address" ..
#include "MyVector.h"
#include "mystring.hpp"
#include <iostream>
#include "MyStack.hpp"
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
using namespace std;
template <class T>
void pr(T &v, MyString s)
{
    cout << "\n\n\t" << s << "  # Sequence:\n";
    // Итератор любого контейнера
    if (v.begin() == v.end())
    {
        cout << "[]\n";
        return;
    }

    class T::iterator p = v.begin();
    int i;
    cout << "[" << *p++;
    for (i = 0; p != v.end(); p++, i++)
        cout << ", " << *p;
    cout << "]\n";
}

template <class T>
void Swap(T &a, T &b)
{
    T temp = a;
    a = b;
    b = temp;
    cout << "a=" << a << ", b=" << b << endl;
}

int main()
{

    //===========================================================
    // Шаблоны функций
    //===========================================================
    // Создайте шаблон функции перестановки двух параметров - Swap().
    // Проверьте работоспособность созданного шаблона с помощью
    // приведенного ниже фрагмента.

    {
        int i = 1, j = -1;
        Swap(i, j);

        double a = 0.5, b = -5.5;
        Swap(a, b);

        Vector u(1, 2), w(-3, -4);
        Swap(u, w);

        // Если вы достаточно развили класс MyString в предыдущей работе,
        // то следующий фрагмент тоже должен работать корректно.

        MyString s1("Your fault"), s2("My forgiveness");
        Swap(s1, s2);
    }
    //===========================================================
    // Шаблоны классов
    //===========================================================
    // Создайте шаблон класса MyStack для хранения элементов любого типа T.
    // В качестве основы для стека может быть выбран массив.
    // Для задания максимального размера стека может быть использован
    // параметр-константа шаблона
    // Обязательными операциями со стеком являются "Push" и "Pop","GetSize" и "Capacity"
    // Необязательной - может быть выбор по индексу (operator[]).
    // Для того, чтобы гарантировать корректное выполнение этих операций
    // следует генерировать исключительные ситуации.

    // С помощью шаблона MyStack создайте стек переменных типа int, затем
    // стек переменных типа double и, наконец, стек из переменных типа Vector
    // Если вы подготовите три класса для обработки исключений,
    // то следующий фрагмент должен работать
    try
    {
        cout << "\tTest MyStack\n";
        MyStack<int, 3> stack;

        cout << "\nInteger Stack capacity: " << stack.get_capacity() << endl;

        stack.push(1);
        stack.push(2);
        stack.push(3);

        cout << "\nInteger Stack has: " << stack.get_size() << " elements\n";

        // stack.push(4);	// Здесь должно быть "выброшено" исключение

        cout << "\nInteger Stack pops: " << stack.pop() << endl;
        cout << "\nInteger Stack pops: " << stack.pop() << endl;

        cout << "\nInteger Stack has: " << stack.get_size() << " elements\n";
        stack.pop();
        // stack.pop();		// Здесь должно быть "выброшено" исключение
        stack.push(2);

        // int i = stack[3]; // Здесь должно быть "выброшено" исключение

        MyStack<Vector, 5> ptStack;

        cout << "\nVector Stack capacity: " << ptStack.get_capacity();

        ptStack.push(Vector(1, 1));
        ptStack.push(Vector(2, 2));

        cout << "\nVector Stack pops: ";
        // Используйте метод класса Vector для вывода элемента
        ptStack.pop().Out();

        cout << "\nVector Stack has: " << ptStack.get_size() << " elements\n";
    }
    catch (StackOverflowException o)
    {
        cerr << o.what() << endl;
    }
    catch (StackUnderflowException o)
    {
        cerr << o.what() << endl;
    }
    catch (StackOutOfRangeException o)
    {
        cerr << o.what() << endl;
    }

    //=======================================================================
    // Контейнеры стандартной библиотеки. Последовательности типа vector
    //=======================================================================

    // Создайте пустой вектор целых чисел. Узнайте его размер с помощью метода size(),
    // С помощью метода push_back() заполните вектор какими-либо значениями.
    // Получите новый размер вектора и выведите значения его элементов.
    // В процессе работы с вектором вы можете кроме количества реально заполненных
    // элементов (size()) узнать максимально возможное количество элементов (max_size()),
    // а также зарезервированную память (capacity()).

    vector<int> v;
    size_t n = v.size(); // 0
    v.push_back(-1);     // [-1]
    v.push_back(-2);     // [-1, -2]
    n = v.size();        // 2
    n = v.capacity();    // 2
    n = v.max_size();    // int: -1, size_t: 2305843009213693951

    // Так как мы часто будем выводить последовательности, то целесообразно
    // создать шаблон функции для вывода любого контейнера.
    // Проанализируйте коды такого шабдлона (pr), который приведен выше
    // Используйте его для вывода вашего вектора

    pr(v, "Vector of ints");

    // Используем другой конструктор для создания вектора вещественных
    // с начальным размером в 2 элемента и заполнением (222.).
    // Проверим параметры вектора. Затем изменим размер вектора и его заполнение
    // (метод - resize()) и вновь проверим параметры.

    vector<double> vd(2, 222.0);
    pr(vd, "Vector of doubles");
    n = vd.size();     // 2
    n = vd.capacity(); // 2
    n = vd.max_size(); // 1152921504606846975

    vd.resize(10, 123.123);
    pr(vd, "After resize");
    n = vd.size();     // 10
    n = vd.capacity(); // 10
    n = vd.max_size(); // 1152921504606846975

    // Используя метод at(), а также операцию выбора [], измените значения
    // некоторых элементов вектора и проверьте результат.
    vd.at(5) = 55.55;
    vd[7] = 7777.7777;
    pr(vd, "After at");

    // Создайте вектор вещественных, который является копией существующего.
    vector<double> wd = vector<double>(vd);
    pr(wd, "Copy");

    // Создайте вектор, который копирует часть существующей последовательности
    vector<double> ud(wd.begin() + 4, wd.end() - 2);
    pr(ud, "Copy part");

    // Создайте вектор вещественных, который является копией части обычного массива.
    double ar[] = {0., 1., 2., 3., 4., 5.};

    vector<double> va(ar + 1, ar + 5);
    pr(va, "Copy part of array");

    // Создайте вектор символов, который является копией части обычной строки
    char s[] = "Array is a succession of chars";

    vector<char> vc1(s + 9, s + 21);
    pr(vc1, "Copy part of c-style string");

    // Создайте вектор элементов типа Vector и инициализируйте
    // его вектором с координатами (1,1).
    vector<Vector> vv(3, Vector(1, 1));
    cout << "\n\nvector of Vectors\n";
    int i;
    for (i = 0; i < vv.size(); i++)
        vv[i].Out();

    // Создайте вектор указателей на Vector и инициализируйте его адресами
    // объектов класса Vector
    Vector v1(1, 1), v2(2, 2), v3(3, 3);
    vector<Vector *> vp;
    vp.push_back(&v1);
    vp.push_back(&v2);
    vp.push_back(&v3);

    cout
        << "\n\nvector of pointers to Vector\n";

    for (i = 0; i < vp.size(); i++)
        vp[i]->Out();

    // Научитесь пользоваться методом assign и операцией
    // присваивания = для контейнеров типа vector.
    // vp.assign(count, value)
    vp.assign(6, &v2);
    cout << "\n\nAfter assign\n";
    for (i = 0; i < vp.size(); i++)
        vp[i]->Out();

    // Декларируйте новый вектор указателей на Vector и инициализируйте его
    // с помощью второй версии assign
    // vpNew.assign(begin, end)

    vector<Vector *> vpNew;
    vpNew.assign(vp.begin(), vp.end() - 1);
    cout << "\n\nNew vector after assign\n";
    for (size_t i = 0; i < vpNew.size(); i++)
        vpNew[i]->Out();

    // На базе шаблона vector создание двухмерный массив и
    // заполните его значениями разными способами.
    // Первый вариант - прямоугольная матрица
    // Второй вариант - ступенчатая матрица

    //========= Прямоугольная матрица
    cout << "\n\n\tRectangular matrix (3x4):\n";
    vector<vector<int>> rectangular_matrix(3, vector<int>(4, 0));
    for (size_t row = 0; row < rectangular_matrix.size(); row++)
    {
        for (size_t col = 0; col < rectangular_matrix[row].size(); col++)
        {
            rectangular_matrix[row][col] = (row + 1) * 10 + (col + 1);
            cout << rectangular_matrix[row][col] << "\t";
        }
        cout << endl;
    }

    //========= Ступенчатая матрица
    // vector <vector
    vector<vector<double>> vdd(5);
    for (i = 0; i < vdd.size(); i++)
        vdd[i] = vector<double>(i + 1, double(i + 0.1 * i));

    cout << "\n\n\tTest vector of vector<double>\n";
    for (i = 0; i < vdd.size(); i++)
    {
        cout << endl;
        for (int j = 0; j < vdd[i].size(); j++)
            cout << vdd[i][j] << "  ";
    }

    //===================================
    // Простейшие действия с контейнерами
    //===================================
    // 3б. Получение значения первого и последнего элементов последовательности.
    // Получение размера последовательности. Присваивание значений
    // элементов одной последовательности элементам другой - assign().

    // Создайте и проинициализируйте вектор из элементов char. Размер -
    // по желанию.
    vector<char> vChar1 = {'A', 'B', 'C', 'D'};

    // Создайте и проинициализируйте массив из элементов char. Размер -
    // по желанию.
    char cMas[] = {'E', 'F', 'G', 'H', 'I', 'J'};

    // Получите значение первого элемента вектора ( front() )
    cout << "\nvChar1 front: " << vChar1.front();

    // Получите значение последнего элемента вектора ( back() )
    cout << "\nvChar1 back: " << vChar1.back();

    // Получите размер вектора
    cout << "\nvChar1 size: " << vChar1.size();

    // Присвойте вектору любой диапазон из значений массива cMas.
    vChar1.assign(cMas + 1, cMas + 5);

    // Проверьте размер вектора, первый и последний элементы.
    cout << "\nvChar1 after assign: size = " << vChar1.size()
         << ", front = " << vChar1.front()
         << ", back = " << vChar1.back() << endl;

    // 3в. Доступ к произвольным элементам вектора с проверкой - at()
    // и без проверки - []
    // Создайте неинициализированный вектор из 8 элементов char - vChar2.
    vector<char> vChar2(8);
    // С помощью at() присвойте четным элементам вектора значения
    // элементов vChar1 из предыдущего задания,

    for (i = 0; i < 8; i++)
        if (i % 2 == 0)
            vChar2.at(i) = (i / 2 < vChar2.size() ? vChar1.at(i / 2) : vChar1.at(i / 2 - 1) + 1);

    // а с помощью [] присвойте нечетным элементам вектора vChar2 значения
    // массива {'K','U','K','U'}.
    char kuku[] = {'K', 'U', 'K', 'U'};

    // Попробуйте "выйти" за границы вектора с помощью at() и
    // с помощью []. Обратите внимание: что происходит при
    // попытке обращения к несуществующему элементу в обоих случаях

    // vChar2.at(200);
    // terminate called after throwing an instance of 'std::out_of_range'
    //   what():  vector::_M_range_check: __n (which is 200) >= this->size() (which is 8)

    // vChar2[200]; // выводит мусор (ub)

    // 3г.Добавьте в конец вектора vChar2  - букву Z (push_back()). Для
    // расширения кругозора можете ее сразу же и выкинуть (pop_back())
    cout << "\nvChar2 back before push_back('Z'): " << vChar2.back() << ", size = " << vChar2.size();
    vChar2.push_back('Z');
    cout << "\nvChar2 back after push_back('Z'): " << vChar2.back() << ", size = " << vChar2.size();
    vChar2.pop_back();
    cout << "\nvChar2 back after pop_back(): " << vChar2.back() << ", size = " << vChar2.size();

    // 3д. Вставка-удаление элемента последовательности insert() - erase()
    // Очистка последовательности - clear()

    // Вставьте перед каждым элементом вектора vChar2 букву 'W'
    pr(vChar2, "\nvChar2 before insertion");
    for (i = 0; i < vChar2.size(); i += 2)
        vChar2.insert(vChar2.begin() + i, 'W');

    pr(vChar2, "vChar2 after 'W' insertion");
    // Вставьте перед 5-ым элементом вектора vChar2 3 буквы 'X'
    vChar2.insert(vChar2.begin() + 4, 3, 'X');
    pr(vChar2, "vChar2 after 3 'X' insertion");

    // Вставьте перед 2-ым элементом вектора vChar2 с третьего по
    // шестой элементы массива "aaabbbccc"
    const char arr_str[] = "aaabbbccc";
    vChar2.insert(vChar2.begin() + 1, arr_str + 2, arr_str + 6);
    pr(vChar2, "vChar2 after abc insertion");

    // Сотрите c первого по десятый элементы vChar2
    vChar2.erase(vChar2.begin(), vChar2.begin() + 10);
    pr(vChar2, "vChar2 after erase");

    // Уничтожьте все элементы последовательности - clear()

    vChar2.clear();
    pr(vChar2, "vChar2 after clear");
    // Создание двухмерного массива
    vector<vector<int>> matrix(3, vector<int>(4, 0));
    ///////////////////////////////////////////////////////////////////

    // Задание 4. Списки. Операции, характерные для списков.
    // Создайте два пустых списка из элементов Vector - ptList1 и
    // ptList2

    list<Vector> ptList1;
    list<Vector> ptList2;

    // Наполните оба списка значениями с помощью методов push_back(),
    // push_front, insrert()

    for (i = 0; i < 2; i++)
    {
        ptList1.push_back(Vector(0, 0));
        ptList1.push_front(Vector(1, 1));
        ptList1.insert(ptList1.begin(), Vector(2, 2));

        ptList2.push_back(Vector(i, i));
        ptList2.push_front(Vector(i - 1, i - 1));
        ptList2.insert(ptList2.begin(), Vector(i + 2, i + 2));
    }
    pr(ptList1, "ptList1");
    pr(ptList2, "ptList2");

    // Отсортируйте списки - sort().
    // Подсказка: для того, чтобы работала сортировка, в классе Vector
    // должен быть переопределен оператор "<"

    ptList1.sort();
    ptList2.sort();
    pr(ptList1, "sorted ptList1");
    pr(ptList2, "sorted ptList2");

    // Объедините отсортированные списки - merge(). Посмотрите: что
    // при этом происходит со вторым списком.

    ptList1.merge(ptList2);

    pr(ptList1, "merged ptList1");
    pr(ptList2, "merged ptList2");

    // Исключение элемента из списка - remove()
    // Исключите из списка элемент с определенным значением.
    // Подсказка: для этого необходимо также переопределить
    // в классе Vector оператор "=="

    ptList1.remove(Vector(0, 0));
    pr(ptList1, "ptList1 after remove");

    ///////////////////////////////////////////////////////////////////

    // Задание 5. Стандартные алгоритмы.Подключите заголовочный файл
    //  <algorithm>
    // 5а. Выведите на экран элементы ptList1 из предыдущего
    // задания с помощью алгоритма for_each()

    for_each(ptList1.begin(), ptList1.end(), [](const Vector &pt)
             { pt.Out(); }); 

    // 5б.С помощью алгоритма find() найдите итератор на элемент Vector с
    // определенным значением. С помощью алгоритма find_if() найдите
    // итератор на элемент, удовлетворяющий определенному условию,
    // например, обе координаты точки должны быть больше 2.
    // Подсказка: напишите функцию-предикат, которая проверяет условие
    // и возвращает boolean-значение (предикат может быть как глобальной
    // функцией, так и методом класса)

    // Создайте список из указателей на элеметы Vector. С помощью
    // алгоритма find_if() и предиката (можно использовать предикат -
    // метод класса Vector, определенный в предыдущем задании) найдите в
    // последовательности элемент, удовлетворяющий условию

    // 5в. Создайте список элементов Vector. Наполните список
    // значениями. С помощью алгоритма replace() замените элемент
    // с определенным значением новым значением. С помощью алгоритма
    // replace_if() замените элемент, удовлетворяющий какому-либо
    // условию на определенное значение. Подсказка: условие
    // задается предикатом.

    // Сформировали значения элементов списка

    // 5г. Создайте вектор строк (string). С помощью алгоритма count()
    // сосчитайте количество одинаковых строк. С помощью алгоритма
    // count_if() сосчитайте количество строк, начинающихся с заданной
    // буквы

    // 5д. С помощью алгоритма count_if() сосчитайте количество строк,
    // которые совпадают с заданной строкой. Подсказка: смотри тему
    // объекты-функции

    cout
        << "\n\n";
}
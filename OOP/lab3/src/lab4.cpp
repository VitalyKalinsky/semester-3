// Контейнеры STL:
// deque, stack, queue, priority_queue
// set, multiset, map, multimap
// Итераторы. Стандартные алгоритмы. Предикаты.
#include <iostream>
#include <deque>
#include <stack>
#include <vector>
#include <queue>
#include <cstring>
#include <list>
#include <set>
#include <map>
#include <iterator>
#include <algorithm>

#include "MyString.hpp"
#include "point.hpp"
#include "rect.hpp"

using namespace std;
#define stop asm("int3");

template <class T>
void pr(T &v, MyString s)
{
    cout << "\n\t" << s << "  # Sequence:\n";
    // Итератор любого контейнера
    if (v.begin() == v.end())
    {
        cout << "[]\n";
        return;
    }

    class T::iterator p = v.begin();
    cout << "[" << *p++;
    for (; p != v.end(); p++)
        cout << ", " << *p;
    cout << "]\n";
}

template <class T>
void print_stack(T stack, const string &message = "")
{
    cout << "\n======================================" << endl;
    if (!message.empty())
        printf("%*s\n", int((40 + message.length()) / 2), message.c_str());

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

struct CmpStr
{
    bool operator()(const char *a, const char *b) const
    {
        return strcmp(a, b) > 0;
    }
};

template <typename T>
class PrintElem
{
public:
    void operator()(const T &val) const
    {
        cout << val << " ";
    }
};

template <typename T>
class ShiftPoint
{
    T delta;

public:
    ShiftPoint(T val) : delta(val) {};
    void operator()(Point &p) const
    {
        p.setX(p.getX() + delta);
        p.setY(p.getY() + delta);
    }
};

int g_n = 2;
int g_m = 10;

bool Pred1_1(const Point &p)
{
    return (p.getX() >= -g_n && p.getX() <= g_m) && (p.getY() >= -g_n && p.getY() <= g_m);
}

void toLowerString(string &s)
{
    transform(s.begin(), s.end(), s.begin(), [](char c)
              { return static_cast<char>(tolower(c)); });
}

int main()
{
    printf("\n\n//////////////////////////////////////////////////////////////////\n \t deque \n//////////////////////////////////////////////////////////////////\n");
    // Очередь с двумя концами - контейнер deque
    // Создайте пустой deque с элементами типа Point. С помощью
    // assign заполните deque копиями элементов вектора. С помощью
    // разработанного Вами в предыдущем задании универсального шаблона
    // выведите значения элементов на печать
    {
        vector<Point> pVector = {{2, 3}, {1, 5}, {4, 1}, {1, 2}};
        deque<Point> pDeque;
        pDeque.assign(pVector.begin(), pVector.end());
        pr(pDeque, "pDeque");
    }

    printf("\n\n//////////////////////////////////////////////////////////////////\n \t deque+MyString \n//////////////////////////////////////////////////////////////////\n");
    // Создайте deque с элементами типа MyString. Заполните его значениями
    // с помощью push_back(), push_front(), insert()
    // С помощью erase удалите из deque все элементы, в которых строчки
    // начинаются с 'A' или 'a'

    deque<MyString> mstrDeque;
    stack<MyString> mstrStack;
    queue<MyString> mstrQueue;
    priority_queue<MyString> mstrPrQueue;
    const char *values[] = {"Banana",
                            "Butterbroad",
                            "Cucumber",
                            "Orange",
                            "Apple",
                            "AlpenGold",
                            "aaananas",
                            "a",
                            "",
                            "Beer"};
    for (auto &&i : values)
    {
        mstrDeque.push_back(i);
        mstrStack.push(i);
        mstrQueue.push(i);
        mstrPrQueue.push(i);
    }

    pr(mstrDeque, "mstrDeque before erase");
    // не прибавляем потому что тогда в конце будет outofrange
    for (auto item = mstrDeque.begin(); item < mstrDeque.end();)
    {
        if (item->get_length() > 0 && (((*item)[0] == 'A') || ((*item)[0] == 'a')))
        {
            item = mstrDeque.erase(item);
        }
        else
        {
            item++;
        }
    }

    pr(mstrDeque, "mstrDeque after erase");

    printf("\n\n//////////////////////////////////////////////////////////////////\n \t stack, queue, priority_queue \n//////////////////////////////////////////////////////////////////\n");
    ////////////////////////////////////////////////////////////////////////////////////
    // Напишите шаблон функции для вывода значений stack, queue, priority_queue
    // Подумайте, как "получать" данное "с верхушки"?
    // 1. Создать 3 функции, каждая из которых печатает stack, queue, priority_queue
    // 2. Создать общую функцию, принимающую другую функцию, которая вызывает метод подглядывания элемента (top, front)
    // 3. Создать обёртку над stack, queue, priority_queue, у которой будет один общий метод peek, вызывающий соответственные методы. Функция вывода была бы одна и принимала бы обёртки и сообщение.
    // 4. Использовать синтаксический сахар и выбрать метод на этапе компиляции. Необходимо поменять в CMake версию C++ на 20
    // Что происходит с контейнерами после вывода значений?
    // Передаются по значению, оригиналы остаются прежними. это отражено в выводе двух стеков подряд
    {
        cout << endl;
        print_stack(mstrStack, "mstrStack");
        print_stack(mstrStack, "mstrStack");
        print_stack(mstrQueue, "mstrQueue");
        print_stack(mstrPrQueue, "mstrPrQueue");
    }
    ////////////////////////////////////////////////////////////////////////////////////
    // stack
    printf("\n\n//////////////////////////////////////////////////////////////////\n \t stack \n//////////////////////////////////////////////////////////////////\n");
    {
        vector<int> iVector = {10, 20, 30, 40, 50};
        // Создайте стек таким образом, чтобы
        // а) элементы стека стали копиями элементов вектора
        stack<int> iStack(deque<int>(iVector.rbegin(), iVector.rend()));
        // б) при выводе значений как вектора, так и стека порядок значений был одинаковым
        pr(iVector, "iVector");
        print_stack(iStack, "iStack");
    }

    // Сравнение и копирование стеков
    // а) создайте стек и любым способом задайте значения элементов
    // б) создайте новый стек таким образом, чтобы он стал копией первого
    // в) сравните стеки на равенство
    // г) модифицируйте любой из стеком любым образом (push, pop, top)
    // д) проверьте, какой из стеков больше (подумайте, какой смысл вкладывается в такое сравнение)
    {
        vector<int> iVector = {10, 20, 30, 40, 50};
        stack<int> iStack(deque<int>(iVector.begin(), iVector.end()));
        stack<int> iStackCopy(iStack);
        cout << "iStack == iStackCopy: " << (iStack == iStackCopy) << "\n";

        iStackCopy.pop();
        iStackCopy.push(50);

        print_stack(iStack, "iStack");
        print_stack(iStackCopy, "iStackCopy");
        // лексикографическое поэлементное от новых к старым
        cout << "iStack < iStackCopy: " << (iStack < iStackCopy) << "\n";
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // queue
    printf("\n\n//////////////////////////////////////////////////////////////////\n \t queue \n//////////////////////////////////////////////////////////////////\n");

    // Создайте очередь, которая содержит указатели на объекты типа Point,
    // при этом явно задайте базовый контейнер.
    // Измените значения первого и последнего элементов посредством front() и back()
    // Подумайте, что требуется сделать при уничтожении такой очереди?
    {
        queue<Point *, deque<Point *>> ptrQueue;
        ptrQueue.push(new Point(1, 1));
        ptrQueue.push(new Point(2, 2));
        ptrQueue.push(new Point(3, 3));

        *ptrQueue.front() = Point(10, 10);
        *ptrQueue.back() = Point(30, 30);

        cout << "front: " << *ptrQueue.front() << ", bask: " << *ptrQueue.back() << "\n";

        // нужно очистить память
        while (!ptrQueue.empty())
        {
            delete ptrQueue.front();
            ptrQueue.pop();
        }
    }
    ////////////////////////////////////////////////////////////////////////////////////
    // priority_queue
    printf("\n\n//////////////////////////////////////////////////////////////////\n \t priority_queue \n//////////////////////////////////////////////////////////////////\n");
    // а) создайте очередь с приоритетами, которая будет хранить адреса строковых литералов - const char*
    // б) проинициализируйте очередь при создании с помощью вспомогательного массива с элементами const char*
    // в) проверьте "упорядоченность" значений (с помощью pop() ) - если они оказываются не упорядоченными, подумайте:
    //		что сравнивается при вставке?

    {
        const char *words[] = {"Banana",
                               "Butterbroad",
                               "Cucumber",
                               "Orange",
                               "Apple",
                               "AlpenGold",
                               "aaananas",
                               "a",
                               "Beer"};

        priority_queue<const char *> strPrQueue(words, words + 9);
        print_stack(strPrQueue, "Popped strPrQueue");
        priority_queue<const char *, vector<const char *>, CmpStr> pqSorted(words, words + 9);
        print_stack(pqSorted, "Popped strPrQueue sorted");
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // set
    printf("\n\n//////////////////////////////////////////////////////////////////\n \t set \n//////////////////////////////////////////////////////////////////\n");
    // a) создайте множество с элементами типа Point - подумайте, что необходимо определить
    //		в классе Point (и каким образом)
    // б) распечатайте значения элементов с помощью шаблона, реализованного в предыдущей лаб. работе
    // в) попробуйте изменить любое значение...
    // г) Создайте два множества, которые будут содержать одинаковые значения
    //		типа int, но занесенные в разном порядке
    // д) Вставьте в любое множество диапазон элементов из любого другого
    //	контейнера, например, элементов массива	(что происходит, если в массиве имеются дубли?)
    {
        // нужно определить operator < для точки
        set<Point> sPoint = {{2, 3}, {1, 5}, {4, 1}, {1, 2}};
        pr(sPoint, "sPoint");

        // *sPoint.begin() = Point(99, 99); // объекты set только const

        set<int> setA = {5, 1, 4, 2, 3};
        set<int> setB = {1, 2, 3, 4, 5};
        pr(setA, "setA");
        pr(setB, "setB");
        cout << "setA == setB: " << (setA == setB) << "\n";

        int arrDups[] = {10, 20, 10, 30, 20, 40};
        set<int> setFromArr;
        setFromArr.insert(arrDups, arrDups + 6);
        pr(setFromArr, "setFromArr (no dups)");
    }
    ////////////////////////////////////////////////////////////////////////////////////
    // multiset

    ////////////////////////////////////////////////////////////////////////////////////
    printf("\n\n//////////////////////////////////////////////////////////////////\n \t map \n//////////////////////////////////////////////////////////////////\n");
    // map
    // а) создайте map, который хранит пары "фамилия, зарплата" - pair<const char*, int>,
    //	при этом строки задаются строковыми литералами
    // б) заполните контейнер значениями посредством operator[] и insert()
    // в) распечатайте содержимое
    // е) замените один из КЛЮЧЕЙ на новый (была "Иванова", вышла замуж => стала "Петрова")

    {
        map<const char *, int, CmpStr> salaries;
        salaries["Ivanova"] = 55000;
        salaries["Petrov"] = 72000;
        salaries.insert(make_pair("Sidorov", 68000));

        cout << "salaries:\n";
        for (const auto &item : salaries)
        {
            cout << "  " << item.first << ": " << item.second << " rub.\n";
        }

        auto ivanova = salaries.find("Ivanova");
        if (ivanova != salaries.end())
        {
            int salary = ivanova->second;
            salaries.erase(ivanova);
            salaries["Petrova"] = salary;
        }

        cout << "Ivanova -> Petrova:\n";
        for (const auto &item : salaries)
        {
            cout << "  " << item.first << ": " << item.second << " rub.\n";
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////
    printf("\n\n//////////////////////////////////////////////////////////////////\n \t multimap \n//////////////////////////////////////////////////////////////////\n");
    // multimap
    // а) создайте "англо-русский" словарь, где одному и тому же ключу будут соответствовать
    //		несколько русских значений - pair<string,string>, например: strange: чужой, странный...
    // б) Заполните словарь парами с помощью метода insert или проинициализируйте с помощью
    //		вспомогательного массива пара (пары можно конструировать или создавать с помощью шаблона make_pair)
    // в) Выведите все содержимое словаря на экран
    // г) Выведите на экран только варианты "переводов" для заданного ключа. Подсказка: для нахождения диапазона
    //		итераторов можно использовать методы lower_bound() и upper_bound()
    {
        multimap<string, string> dict;
        dict.insert(make_pair("strange", "чужой"));
        dict.insert(make_pair("strange", "странный"));
        dict.insert(make_pair("strange", "необычный"));
        dict.insert(make_pair("run", "бежать"));
        dict.insert(make_pair("run", "запускать"));
        dict.insert(make_pair("build", "строить"));
        dict.insert(make_pair("build", "компилировать"));

        cout << "Все пары словаря:\n";
        for (const auto &kv : dict)
        {
            cout << "  " << kv.first << " -> " << kv.second << "\n";
        }

        // Поиск переводов для ключа "strange" через lower_bound и upper_bound
        cout << "Варианты перевода для 'strange': ";
        auto low = dict.lower_bound("strange");
        auto up = dict.upper_bound("strange");
        for (auto it = low; it != up; ++it)
        {
            cout << it->second << "; ";
        }
        cout << "\n";
    }
    ///////////////////////////////////////////////////////////////////

    printf("\n\n//////////////////////////////////////////////////////////////////\n \t iterators \n//////////////////////////////////////////////////////////////////\n");
    // Итераторы
    {
        // Реверсивные итераторы. Сформируйте set<Point>. Подумайте, что
        // нужно перегрузить в классе Point. Создайте вектор, элементы которого
        // являются копиями элементов set, но упорядочены по убыванию
        set<Point> pointSet = {{2, 3}, {1, 5}, {4, 1}, {1, 2}};
        vector<Point> pointVector(pointSet.rbegin(), pointSet.rend());
        pr(pointVector, "pointVector (set по убыванию)");

        // Потоковые итераторы. С помощью ostream_iterator выведите содержимое
        // vector и set из предыдущего задания на экран.
        cout << "Вывод через ostream_iterator:\n  vector: ";
        copy(pointVector.begin(), pointVector.end(), ostream_iterator<Point>(cout, " "));
        cout << "\n  set:    ";
        copy(pointSet.begin(), pointSet.end(), ostream_iterator<Point>(cout, " "));
        cout << "\n";

        // Итераторы вставки. С помощью возвращаемых функциями:
        // back_inserter()
        // front_inserter()
        // inserter()
        // итераторов вставки добавьте элементы в любой из созданных контейнеров. Подумайте:
        // какие из итераторов вставки можно использовать с каждым контейнером.
        vector<int> src = {1, 2, 3};

        // back_inserter (требует push_back: vector, deque, list)
        list<int> iBack;
        copy(src.begin(), src.end(), back_inserter(iBack));
        pr(iBack, "iBack (через back_inserter)");

        // front_inserter (требует push_front: deque, list)
        list<int> iFront;
        copy(src.begin(), src.end(), front_inserter(iFront));
        pr(iFront, "iFront (через front_inserter)");

        // inserter (требует insert(pos, val): vector, deque, list, set, map)
        set<int> iSet;
        copy(src.begin(), src.end(), inserter(iSet, iSet.end()));
        pr(iSet, "iSet (через inserter)");
    }

    ///////////////////////////////////////////////////////////////////

    // Обобщенные алгоритмы (заголовочный файл <algorithm>). Предикаты.

    printf("\n\n//////////////////////////////////////////////////////////////////\n \t for_each \n//////////////////////////////////////////////////////////////////\n");
    // алгоритм for_each() - вызов заданной функции для каждого элемента любой последовательности
    // (массив, vector, list...)
    // С помощью алгоритма for_each в любой последовательности с элементами любого типа
    // распечатайте значения элементов
    // Подсказка : неплохо вызываемую функцию определить как шаблон
    {
        vector<int> nums = {10, 20, 30, 40, 50};
        cout << "for_each печать nums: ";
        for_each(nums.begin(), nums.end(), PrintElem<int>());
        cout << "\n";
    }
    // stop

    // С помощью алгоритма for_each в любой последовательности с элементами типа Point
    // измените "координаты" на указанное значение (такой предикат тоже стоит реализовать
    // как шаблон) и выведите результат с помощью предыдущего предиката
    vector<Point> ptsVector = {{2, 3}, {1, 5}, {4, 1}, {1, 2}, {1, 5}, {5, 5}, {6, 7}};
    {
        cout << "ptsVector до сдвига: ";
        for_each(ptsVector.begin(), ptsVector.end(), PrintElem<Point>());
        cout << "\n";
        int shift = -3;
        for_each(ptsVector.begin(), ptsVector.end(), ShiftPoint<int>(shift));

        cout << "ptsVector после сдвига " << shift << ": ";
        for_each(ptsVector.begin(), ptsVector.end(), PrintElem<Point>());
        cout << "\n";
    }
    printf("\n\n//////////////////////////////////////////////////////////////////\n \t find \n//////////////////////////////////////////////////////////////////\n");
    // С помощью алгоритма find() найдите в любой последовательности элементов Point
    // все итераторы на элемент Point с указанным значением.
    {
        Point target(-2, 2);
        auto found = ptsVector.begin();
        while ((found = find(found, ptsVector.end(), target)) != ptsVector.end())
        {
            cout << "Найден элемент " << target << " на позиции " << found++ - ptsVector.begin() << endl;
        }
    }
    
    printf("\n\n//////////////////////////////////////////////////////////////////\n \t sort \n//////////////////////////////////////////////////////////////////\n");
    // С помощью алгоритма sort() отсортируйте любую последовательность элементов Point.
    ////По умолчанию алгоритм сортирует последовательность по возрастанию.
    // Что должно быть определено в классе Point?
    //  Замечание: обобщенный алгоритм sort не работает со списком, так как
    // это было бы не эффективно => для списка сортировка реализована методом класса!!!
    {
        sort(ptsVector.begin(), ptsVector.end());
        pr(ptsVector, "ptsVector (после sort)");
    }

    // Создайте глобальную функцию вида: bool Pred1_1(const Point& ), которая будет вызываться
    // алгоритмом find_if(), передавая в качестве параметра очередной элемент последовательности.
    // С помощью алгоритма find_if() найдите в любой последовательности элементов Point
    // итератор на элемент Point, удовлетворяющий условию: координаты x и y лежат в промежутке
    //[-n, +m].
    printf("\n\n//////////////////////////////////////////////////////////////////\n \t Points [-n, +m] \n//////////////////////////////////////////////////////////////////\n");
    {
        g_n = 1;
        g_m = 10;
        auto itPred = find_if(ptsVector.begin(), ptsVector.end(), Pred1_1);
        if (itPred != ptsVector.end())
        {
            cout << "Первый элемент в промежутке [-" << g_n << ", +" << g_m << "]: " << *itPred << "\n";
        }
    }
    // С помощью алгоритма sort() отсортируйте любую последовательность элементов Rect,
    // располагая прямоугольники по удалению центра от начала координат.

    printf("\n\n//////////////////////////////////////////////////////////////////\n \t Rect sort \n//////////////////////////////////////////////////////////////////\n");
    {
        vector<Rect> rectVector = {Rect(2, 3, 1, 5),
                                   Rect(4, 1, 1, 2),
                                   Rect(1, 5, 5, 5),
                                   Rect(5, 4, 1, 1),
                                   Rect(5, 5, 6, 7)};
        pr(rectVector, "rectVector before sort");
        sort(rectVector.begin(), rectVector.end());
        pr(rectVector, "rectVector after sort");
    }
    
    printf("\n\n//////////////////////////////////////////////////////////////////\n \t transform \n//////////////////////////////////////////////////////////////////\n");
    { // transform
      // Напишите функцию, которая с помощью алгоритма transform переводит
      // содержимое объекта string в нижний регистр.
      // Подсказка: класс string - это "почти" контейнер, поэтому для него
      //  определены методы begin() и end()
        string sample = "HeLlO hOw Are YoU!?ZZZ";
        toLowerString(sample);
        cout << "sample в нижнем регистре: " << sample << "\n";

        // Заполните list объектами string. С помощью алгоритма transform сформируте
        // значения "пустого" set, конвертируя строки в нижний регистр
        list<string> lWords = {"BaNAna",
                               "ORanGE",
                               "cuCUMber",
                               "bEer",
                               "AlpenGold",
                               "OrANGe",
                               "AlPenGold",
                               "ORANGE",
                               "bEEr"};
        set<string> sWords;

        transform(lWords.begin(), lWords.end(), inserter(sWords, sWords.begin()), [](string &s)
                  {toLowerString(s);
                    return s; });
        pr(sWords, "sWords (уникальные строки в нижнем регистре)");
        // stop
    }
    
    printf("\n\n//////////////////////////////////////////////////////////////////\n \t frequences \n//////////////////////////////////////////////////////////////////\n");
    { // map

        // Сформируйте любым способом вектор с элементами типа string.
        // Создайте (и распечатайте для проверки) map<string, int>, который будет
        // содержать упорядоченные по алфавиту строки и
        // количество повторений каждой строки в векторе
        list<string> lWords = {"BaNAna",
                               "ORanGE",
                               "cuCUMber",
                               "bEer",
                               "AlpenGold",
                               "OrANGe",
                               "AlPenGold",
                               "ORANGE",
                               "bEEr"};
        vector<string> vWords;
        transform(lWords.begin(), lWords.end(), back_inserter(vWords), [](string &s)
                  {toLowerString(s);
                    return s; });
        pr(vWords, "vWords");

        map<string, int> frequences;
        for_each(vWords.begin(), vWords.end(), [&frequences](string &str)
                 { frequences[str]++; });

        cout << "frequences:\n";
        for (const auto &item : frequences)
        {
            cout << "  " << item.first << " - " << item.second << " р.\n";
        }
    }

    return 0;
}

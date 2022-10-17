#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <cmath>

// Testing data.
//const std::string PATH_TO_DATA0 = "./data/price.txt";
//const std::string PATH_TO_DATA1 = "./data/year.txt";
//const std::string PATH_TO_DATA2 = "./data/odom.txt";

//const std::string PATH_TO_DATA0 = "./data/salary.txt";
//const std::string PATH_TO_DATA1 = "./data/family.txt";
//const std::string PATH_TO_DATA2 = "./data/medicine.txt";
//const std::string PATH_TO_DATA3 = "./data/housecoef.txt";
//const std::string PATH_TO_DATA4 = "./data/ecology.txt";
//const std::string PATH_TO_DATA5 = "./data/dacha.txt";

// My.
const std::string PATH_TO_DATA0 = "./data/using.txt";
const std::string PATH_TO_DATA1 = "./data/com_support.txt";
const std::string PATH_TO_DATA2 = "./data/scaling.txt";
const std::string PATH_TO_DATA3 = "./data/native_vault.txt";
const std::string PATH_TO_DATA4 = "./data/doc.txt";
const std::string PATH_TO_DATA5 = "./data/native_ui.txt";

const char PATH_TO_Power_DATA[] = "./data/power.txt";
const std::string ORCH_NAMES = "./data/orch_names.txt";

const int VARIANT = 5;
const int BO = 6;

// Test for lada.
//const int VARIANT = 3;
//const int BO = 3;


//выделение памяти дл¤ динамического двумерного массива
double** createArr(int n, int m);
//подсчет весовые коэффициенты
void readPower(double* arr, double n, std::ifstream& in);
//посчитать массив размером NxM из вход¤щего потока
void readFile(double** arr, double n, double m, std::ifstream& in);
//определение доминирующего варианта
std::vector<int> dominate(double** arr, double n, double m);
//определение блокирующего варианта
std::vector<int> block(double** arr, double n, double m);
//определение турнирного варианта
std::vector<double> turnir(double** arr, const double power[BO], double n,
                           double m, int number);
//составление массива дл¤ варианта в случае механизма K-max
double** createKarray(double** arr, double n, double m);
//определение K-opt вариантов
void createKopt(double** arr, double n, int* kopt_Array);
//вывести двумерный массив
void writeArr(double** arr, double n, double m);
//вывести двумерный массив K-opt механизм
void writeArrKopt(double** arr, double n, double m, int* opt);
//уничтожить двумерный массив
void distractionArray(double** arr, int n);
//расстановка мест
void placeRating(const double arr[VARIANT], int A[VARIANT]);

void read_names( std::vector<std::string> &vec, std::ifstream& in )
{
    std::string data {};
    while( in >> data )
    {
        vec.emplace_back( data );
    }
}

int main()
{
    setlocale(LC_ALL, "RUS");
    double rating[VARIANT] = { 0 };
    double ratingBlock[VARIANT] = { 0 };
    double ratingTurnir[VARIANT] = { 0 };
    double kmax[VARIANT] = { 0 };
    double kopt[VARIANT] = { 0 };

    // Read names.
    std::vector<std::string> names;
    std::ifstream in_names;
//    in_names.exceptions(std::ios::failbit | std::ios::badbit);
    in_names.open(ORCH_NAMES.c_str());
    read_names( names, in_names );

    //считывание весовых коэффициентов
    std::ifstream in_power;
    in_power.exceptions(std::ios::failbit | std::ios::badbit);
    in_power.open(PATH_TO_Power_DATA);

    double* powerArr = new double[BO];
    //powerArr - массив с весовыми коэффициентами
    readPower(powerArr, BO, in_power);

    double sum { 0 };
    sum = std::accumulate(powerArr, powerArr + BO, sum );
    std::cout << "Sum of weight coeff: " << sum << std::endl;
    std::cout << "Весовые коэффициенты:" << std::endl;
    for (int i = 0; i < BO; ++i)
    {
        std::cout << powerArr[i] << std::endl;
    }
    in_power.close();
    for (int k = 0; k < BO; k++)
    {
        //подготовка данных
        std::ifstream in; //поток дл¤ считывани¤ основных данных
        in.exceptions(std::ios::failbit | std::ios::badbit);
        std::string str;
        switch (k)
        {
        case 0:
            str = PATH_TO_DATA0;
            break;
        case 1:
            str = PATH_TO_DATA1;
            break;
        case 2:
            str = PATH_TO_DATA2;
            break;
        case 3:
            str = PATH_TO_DATA3;
            break;
        case 4:
            str = PATH_TO_DATA4;
            break;
        case 5:
            str = PATH_TO_DATA5;
            break;
        default:
            break;
        }
        std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
        std::cout << str << std::endl;
        in.open(str);
        double** Dom_data = createArr(VARIANT, VARIANT);
        readFile(Dom_data, VARIANT, VARIANT, in);
        writeArr(Dom_data, VARIANT, VARIANT);
        in.close();
        //обработка данных
        std::vector<int> dom_Array;
        std::vector<int> block_Array;
        std::vector<double> turnir_Array;
        //std::vector<int> kopt_Array;
        int* kopt_Array = new int[VARIANT];
        std::fill_n( kopt_Array, VARIANT, -1 );
        double** Karray = createKarray(Dom_data, VARIANT, VARIANT);
        createKopt(Karray, VARIANT, kopt_Array);
        std::cout << "Kopt" << std::endl;
        for (int i = 0; i < VARIANT; ++i)
        {
            std::cout << '[' << i << "] = " << kopt_Array[i] << std::endl;
        }
        std::cout << "+++++++" << std::endl;
        std::cout << "K-max механизм " << std::endl;
        writeArrKopt(Karray, VARIANT, 4, kopt_Array);
        std::cout << "======== " << std::endl;
        dom_Array = dominate(Dom_data, VARIANT, VARIANT);//определение доминирующих вариантов
        block_Array = block(Dom_data, VARIANT, VARIANT);//определение блокирующих вариантов
        turnir_Array = turnir(Dom_data, powerArr, VARIANT, VARIANT, k);//определение турнирных вариантов
        std::cout << "Доминирующий механизм" << std::endl;
        for (int i = 0; i < dom_Array.size(); ++i)
        {
            std::cout << dom_Array[i] << std::endl;
            rating[dom_Array[i]] += powerArr[k];
        }
        std::cout << "Блокирующий механизм " << std::endl;
        for (int i = 0; i < block_Array.size(); ++i)
        {
            std::cout << block_Array[i] << std::endl;
            ratingBlock[block_Array[i]] += powerArr[k];
        }
        //данные по турнирному механизму
        std::cout << "Турнирный механизм " << std::endl;
        for (int i = 0; i < turnir_Array.size(); ++i)
        {
            std::cout << turnir_Array[i] << std::endl;
            ratingTurnir[i] += turnir_Array[i];
        }
        //данные по K-max механизму
        for (int i = 0; i < VARIANT; ++i)
        {
            for (int j = 0; j < 4; j++)
            {
                kmax[i] += Karray[i][j] * powerArr[k];
            }
        }
        //данные по K-opt
        for (int i = 0; i < VARIANT; ++i)
        {
            if ((kopt_Array[i] == 1) || (kopt_Array[i] == 2) ||
                    (kopt_Array[i] == 3) || (kopt_Array[i] == 4))
            {
                for (int j = 0; j < 4; j++)
                {
                    kopt[i] += Karray[i][j] * powerArr[k];
                }
            }
        }
        distractionArray(Dom_data, VARIANT);
        distractionArray(Karray, VARIANT);
    }
    int rating_place[VARIANT] = { 0 };
    placeRating(rating, rating_place);
    std::cout << "_____Механизм доминирования_____" << std::endl;
    std::cout << "Баллы вариантов с учетом весовых коэффициентов и места вариантов" << std::endl;
    for (int i = 0; i < VARIANT; ++i)
    {
        std::cout << std::setw(0) << names[i] << std::setw(25 - names[i].size()) << rating[i] <<
                     std::setw(8) << rating_place[i] << std::endl;
    }
    int rating_place_block[VARIANT] = { 0 };
    placeRating(ratingBlock, rating_place_block);
    std::cout << "_____Механизм блокировки______" << std::endl;
    std::cout << "Баллы вариантов с учетом весовых коэффициентов и места вариантов" << std::endl;
    for (int i = 0; i < VARIANT; ++i)
    {
        std::cout << std::setw(0) << names[i] << std::setw(25 - names[i].size()) << ratingBlock[i] <<
                     std::setw(8) << rating_place_block[i] << std::endl;
    }
    int rating_place_turnir[VARIANT] = { 0 };
    placeRating(ratingTurnir, rating_place_turnir);
    std::cout << "______Турнирный механизм______" << std::endl;
    std::cout << "Баллы вариантов с учетом весовых коэффициентов и места вариантов" << std::endl;
    for (int i = 0; i < VARIANT; ++i)
    {
        std::cout << std::setw(0) << names[i] << std::setw(25 - names[i].size()) << ratingTurnir[i] <<
                     std::setw(8) << rating_place_turnir[i] << std::endl;
    }
    int rating_place_kmax[VARIANT] = { 0 };
    int rating_place_kopt[VARIANT] = { 0 };
    placeRating(kmax, rating_place_kmax);
    placeRating(kopt, rating_place_kopt);
    std::cout << "______Механизм K-MAX______" << std::endl;
    std::cout << "Баллы вариантов с учетом весовых коэффициентов и места вариантов" << std::endl;
    for (int i = 0; i < VARIANT; ++i)
    {
        std::cout << std::setw(0) << names[i] << std::setw(25 - names[i].size()) << kmax[i] <<
                     std::setw(8) << rating_place_kmax[i] << std::setw(8) << kopt[i] <<
                     std::setw(8) << rating_place_kopt[i] << std::endl;
    }
    std::cout << "______Бальная система______" << std::endl;
    std::cout << "===============================================================================" << std::endl;
    std::cout << "             ||  Блок  ||  Дом  ||  Тур  ||  Sjp  ||  SjM  ||  Сумма баллов  ||" << std::endl;
    std::cout << "===============================================================================" << std::endl;

    int winner = VARIANT + 1 - rating_place[0] +
                 VARIANT + 1 - rating_place_block[0] +
            VARIANT + 1 - rating_place_turnir[0] +
            VARIANT + 1 - rating_place_kmax[0] +
            VARIANT + 1 - rating_place_kopt[0]
            ;
    int index { 0 };
    for (int i = 1; i < VARIANT; ++i)
    {
        int dom_value = VARIANT + 1 - rating_place[i];
        int block_value = VARIANT + 1 - rating_place_block[i];
        int turn_value = VARIANT + 1 - rating_place_turnir[i];
        int kmax_value = VARIANT + 1 - rating_place_kmax[i];
        int kopt_value = VARIANT + 1 - rating_place_kopt[i];
        int sum = dom_value + block_value + turn_value + kmax_value +
                kopt_value;
        if ( winner < sum ) {
            winner = sum;
            index = i;
        }
    }

    for (int i = 0; i < VARIANT; ++i)
    {
        int dom_value = VARIANT + 1 - rating_place[i];
        int block_value = VARIANT + 1 - rating_place_block[i];
        int turn_value = VARIANT + 1 - rating_place_turnir[i];
        int kmax_value = VARIANT + 1 - rating_place_kmax[i];
        int kopt_value = VARIANT + 1 - rating_place_kopt[i];
        int sum = dom_value + block_value + turn_value + kmax_value +
                kopt_value;
        if ( i == index ) {
            std::cout << "\033[1;31m"  << std::setw(0) << names[i] << std::setw(20 - names[i].size()) << block_value <<
                         std::setw(8) << dom_value << std::setw(9) << turn_value << std::setw(9) <<
                         kmax_value << std::setw(9) << kopt_value << std::setw(13) << sum << "\033[0m" << std::endl;
        } else {
            std::cout << std::setw(0) << names[i] << std::setw(20 - names[i].size()) << block_value <<
                         std::setw(8) << dom_value << std::setw(9) << turn_value << std::setw(9) <<
                         kmax_value << std::setw(9) << kopt_value << std::setw(13) << sum << std::endl;
        }
    }
    return 0;
}
//выделение памяти для динамического двухмерного массива
double** createArr(int n, int m)
{
    double** A;
    A = new double* [n];
    for (int i = 0; i < n; i++)
    {
        A[i] = new double[m];
    }
    return A;
}
//считать весовые коэффициенты
void readPower(double* arr, double n, std::ifstream& in)
{
    for (int j = 0; j < n; j++)
    {
        in >> arr[j];
    }
}
//считать массив размером NxM из входящего потока
void readFile(double** arr, double n, double m, std::ifstream& in)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            in >> arr[i][j];
        }
    }
}
//определение доминирующего варианта
std::vector<int> dominate(double** arr, double n, double m)
{
    std::vector<int> dom_str_Array;
    bool dom_str;
    for (int i = 0; i < n; i++)
    {
        dom_str = true;
        for (int j = 0; j < m; j++)
        {
            if (i == j) continue;
            if (arr[i][j] != 1)
            {
                dom_str = false;
                break;
            }
        }
        if (dom_str) dom_str_Array.push_back(i);
    }
    return dom_str_Array;
}
//определение блокирующего варианта
std::vector<int> block(double** arr, double n, double m)
{
    std::vector<int> block_str_Array;
    bool block_str;
    for (int i = 0; i < n; i++)
    {
        block_str = true;
        for (int j = 0; j < m; j++)
        {
            if (i == j) continue;
            if (arr[j][i] != 0)
            {
                block_str = false;
                break;
            }
        }
        if (block_str) block_str_Array.push_back(i);
    }
    return block_str_Array;
}
//определение турнирного варианта
std::vector<double> turnir(double** arr, const double power[BO], double n,
                           double m, int number)
{
    std::vector<double> turnir_str_Array;
    bool turnir_str;
    for (int i = 0; i < n; i++)
    {
        double sum = 0;
        for (int j = 0; j < m; j++)
        {
            if (i == j) continue;
            if (arr[i][j] == 1)
            {
                if (arr[j][i] == 0)
                {
                    sum += power[number];
                }
                else if (arr[j][i] == 1)
                {
                    sum += power[number] / 2;
                }
            }
        }
        turnir_str_Array.push_back(sum);
    }
    return turnir_str_Array;
}
//оставление массива для варианта в случае механизма K-max
double** createKarray(double** arr, double n, double m)
{
    double** A = createArr(VARIANT, 4);
    for (int i = 0; i < n; i++)
    {
        double HR0 = 0;
        double ER = 0;
        double NK = 0;
        for (int j = 0; j < m; j++)
        {
            if (i == j) continue;
            if (arr[i][j] == 1)
            {
                if (arr[j][i] == 0)
                {
                    HR0 += 1;
                }
                else if (arr[j][i] == 1)
                {
                    ER += 1;
                }
            }
            if (arr[i][j] == -1)
            {
                NK += 1;
            }
        }
        for (int j = 0; j < 4; j++)
        {
            switch (j)
            {
            case 0:
                A[i][j] = HR0 + ER + NK;
                break;
            case 1:
                A[i][j] = HR0 + NK;
                break;
            case 2:
                A[i][j] = HR0 + ER;
                break;
            case 3:
                A[i][j] = HR0;
                break;
            default:
                break;
            }
        }
    }
    return A;
};
//определение K-opt вариантов
void createKopt(double** arr, double n, int* kopt_Array)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            switch (j)
            {
            case 0:
                if (arr[i][j] == n)
                {
                    kopt_Array[i] = 1;
                }
                break;
            case 1:
                if ((arr[i][j] == (n - 1)) && (arr[i][j] > arr[i][j + 2]))
                {
                    kopt_Array[i] = 2;
                }
                break;
            case 2:
                if ((arr[i][j] == n) && (arr[i][j] > arr[i][j + 1]))
                {
                    kopt_Array[i] = 3;
                }
                break;
            case 3:
                if ((arr[i][j] == (n - 1)) && (arr[i][j] == arr[i][j - 1]) &&
                        (arr[i][j] == arr[i][j - 2]))
                {
                    kopt_Array[i] = 4;
                }
                break;
            default:
                kopt_Array[i] = 0;
                break;
            }
        }
    }
}
//вывести двумерный массив
void writeArr(double** arr, double n, double m)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            std::cout << arr[i][j] << "\t";
        }
        std::cout << std::endl;
    }
}
//вывести двумерный массив K-opt механизм
void writeArrKopt(double** arr, double n, double m, int* opt)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            std::cout << arr[i][j] << "\t";
        }
        switch (opt[i])
        {
        case 0:
            break;
        case 1:
            std::cout << "максимальный" << "\t";
            break;
        case 2:
            std::cout << "строго максимальный" << "\t";
            break;
        case 3:
            std::cout << "наибольший" << "\t";
            break;
        case 4:
            std::cout << "строго наибольший" << "\t";
            break;
        default:
            break;
        }
        std::cout << std::endl;
    }
}
//уничтожить двумерный массив
void distractionArray(double** arr, int n)
{
    for (int i = 0; i < n; ++i)
    {
        delete[] arr[i];
    }
    delete[] arr;
}
//расстановка мест
void placeRating(const double arr[VARIANT], int A[VARIANT])
{
    double place[VARIANT] = { 0 };
    int number[VARIANT];
    for (int i = 0; i < VARIANT; ++i)
    {
        number[i] = i + 1;
    }
    for (int i = 0; i < VARIANT; i++)
    {
        place[i] = arr[i];
    }
    std::sort(std::begin(place), std::end(place));
    std::reverse(std::begin(place), std::end(place));
    int pl = 0;
    for (int i = 0; i < VARIANT; ++i)//по массиву place
    {
        if ((place[i] == place[i - 1]) && (i != 0))
        {
            continue;
        }
        for (int j = 0; j < VARIANT; j++)//по массиву arr
        {
            if (arr[j] == place[i])
            {
                //A[j] = i + 1;
                A[j] = number[pl];
            }
        }
        pl++;
        if (place[i] == 0) break;
    }
}

// belyaeva_vera_lab1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <string>
#include <clocale>
#include <fstream>

using namespace std;

struct Pipe {
    string name;
    double length;
    double diameter;
    bool repairing;

    Pipe() : name(""), length(0.0), diameter(0.0), repairing(false) {}

    void input() {
        cout << "Введите название трубы:  ";
        cin.ignore();
        getline(cin, name);

        cout << "Введите длину трубы в километрах: ";
        while (!(cin >> length) || length <= 0 || cin.peek() != '\n') {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Ошибка! Введите положительное число: ";
        }

        cout << "Введите диаметр трубы в миллиметрах: ";
        while (!(cin >> diameter) || diameter <= 0 || cin.peek() != '\n') {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Ошибка! Введите положительное число: ";
        }

        repairing = false;
    }

    void print() const {
        cout << "\nТруба\n"
            << "Название: " << name << "\n"
            << "Длина: " << length << " км\n"
            << "Диаметр: " << diameter << " мм\n"
            << "Состояние: "
            << (repairing ? "В ремонте" : "В работе")
            << "\n";
    }

    void inTheRepair() {
        repairing = !repairing;
        cout << "Признак изменён. Труба теперь: "
            << (repairing ? "В ремонте" : "В работе") << "\n";
    }

    void saveToFile(ofstream& file) {
        file << "1\n";
        file << name << "\n";
        file << length << "\n";
        file << diameter << "\n";
        file << repairing << "\n";
    }

    bool loadFromFile(ifstream& file) {
        int marker;
        file >> marker;
        file.ignore();

        if (marker == 0) {
            return false;
        }

        getline(file, name);
        file >> length;
        file >> diameter;
        file >> repairing;
        file.ignore();
        return true;
    }
};

struct CompressorStation {
    string name;
    int numberOfWorkshops;
    int operatingWorkshops;
    int stationClass;

    CompressorStation() : name(""), numberOfWorkshops(0), operatingWorkshops(0), stationClass(0) {}

    void input() {
        cout << "Введите название кс: ";
        cin.ignore();
        getline(cin, name);

        cout << "Количество цехов: ";
        while (!(cin >> numberOfWorkshops) || numberOfWorkshops <= 0 || cin.peek() != '\n') {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Ошибка! Введите число: ";
        }

        operatingWorkshops = 0;

        cout << "Класс станции: ";
        while (!(cin >> stationClass) || stationClass <= 0 || cin.peek() != '\n') {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Ошибка! Введите число: ";
        }
    }

    void print() const {
        cout << "\n КС \n"
            << "Название: " << name << "\n"
            << "Количество цехов: " << numberOfWorkshops << "\n"
            << "Количество работающих цехов: " << operatingWorkshops << "\n"
            << "Класс станции: " << stationClass << "\n"
            << "Станция "
            << (operatingWorkshops > 0 ? "Работает " : "Простаивает")
            << "\n";
    }

    void startWorkshop() {
        bool canStart = (operatingWorkshops < numberOfWorkshops);
        cout << (canStart ? "Цех запущен.\n"
            : "Нельзя, все цеха в работе\n");
        if (canStart) operatingWorkshops++;
    }

    void stopWorkshop() {
        bool canStop = (operatingWorkshops > 0);
        cout << (canStop ? "Цех остановлен. \n"
            : "Нельзя, нет работающих цехов\n");
        if (canStop) operatingWorkshops--;
    }

    void saveToFile(ofstream& file) {
        file << "1\n";
        file << name << "\n";
        file << numberOfWorkshops << "\n";
        file << operatingWorkshops << "\n";
        file << stationClass << "\n";
    }

    bool loadFromFile(ifstream& file) {
        int marker;
        file >> marker;
        file.ignore();

        if (marker == 0) {
            return false;
        }

        getline(file, name);
        file >> numberOfWorkshops;
        file >> operatingWorkshops;
        file >> stationClass;
        file.ignore();
        return true;
    }
};

int main()
{
    setlocale(LC_ALL, "Russian");

    Pipe pipe;
    CompressorStation compressorStation;

    bool pipeFilled = false;
    bool csFilled = false;

    while (true) {
        cout << "\nМеню\n"
            << "1.Добавить трубу\n"
            << "2.Добавить кс\n"
            << "3.Просмотр всех объектов\n"
            << "4.Редактировать трубу (в ремонте/не в ремонте)\n"
            << "5.Редактировать кс (запуск/ остановка цеха)\n"
            << "6. Сохранить в файл\n"
            << "7. Загрузить из файла\n"
            << "0. Выход\n"
            << "\n";

        int choice;
        cout << "Ваш выбор: \n";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Ошибка! Введите число: ";
            continue;
        }

        switch (choice) {

        case 1:
            pipe.input();
            pipeFilled = true;
            cout << "Труба добавлена\n";
            break;

        case 2:
            compressorStation.input();
            csFilled = true;
            cout << "Добавлена кс\n";
            break;

        case 3:
            if (!pipeFilled && !csFilled) {
                cout << "Пока ничего не введено\n";
            }
            else {
                if (pipeFilled) pipe.print();
                if (csFilled) compressorStation.print();
            }
            break;

        case 4:
            if (!pipeFilled) {
                cout << "Сначала добавьте трубу (п.1)\n";
            }
            else {
                pipe.inTheRepair();
            }
            break;

        case 5:
            if (!csFilled) {
                cout << "Сначала добавьте кс\n";
            }
            else {
                cout << "1.Запустить цех\n"
                    << "2. Остановить цех\n";
                int sub;
                cout << "Выбор\n ";
                if (!(cin >> sub)) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Ошибка! Введите число: ";
                    break;
                }
                switch (sub) {
                case 1:
                    compressorStation.startWorkshop();
                    break;
                case 2:
                    compressorStation.stopWorkshop();
                    break;
                default:
                    cout << "Неверный пункт меню\n";
                }
            }
            break;

        case 6: {
            ofstream file("data.txt");
            if (file.is_open()) {

                if (pipeFilled) {
                    pipe.saveToFile(file);
                }
                else {
                    file << "0\n";
                }

                if (csFilled) {
                    compressorStation.saveToFile(file);
                }
                else {
                    file << "0\n";
                }

                file.close();
                cout << "Данные сохранены\n";
            }
            else {
                cout << "Ошибка открытия файла\n";
            }
            break;
        }

        case 7: {
            ifstream file("data.txt");
            if (file.is_open()) {
                bool loadedPipe = pipe.loadFromFile(file);
                bool loadedCs = compressorStation.loadFromFile(file);

                pipeFilled = loadedPipe;
                csFilled = loadedCs;

                if (!loadedCs) {
                    compressorStation = CompressorStation();
                }

                if (loadedPipe || loadedCs) {
                    cout << "Данные загружены\n";
                }
                else {
                    cout << "Файл пуст или повреждён\n";
                }
                file.close();
            }
            else {
                cout << "Файл не найден\n";
            }
            break;
        }

        case 0:
            cout << "Выход из программы\n";
            return 0;

        default:
            cout << "Неверный пункт меню!\n";
        }
    }
    return 0;
}
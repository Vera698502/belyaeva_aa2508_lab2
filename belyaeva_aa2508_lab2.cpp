// belyaeva_vera_lab1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <string>
#include <clocale>
#include <fstream>
#include <map>

using namespace std;

class Pipe {
private:
    int id;
    string name;
    double length;
    double diameter;
    bool repairing;  

public:
    Pipe(int newId=0) : id(newId), name(""), length(0.0), diameter(0.0), repairing(false) {}

    string getName() const { return name; }
    double getLength() const { return length; }
    double getDiametr() const { return diameter; }
    bool isRepairing() const { return repairing; }
    int getId() const { return id; }

    void setName(const string& newName) { name = newName; }
    void setLength(double newLength) { length = newLength; }
    void setDiameter(double newDiameter) { diameter = newDiameter; }
    void setRepairing(bool newRepairing) { repairing = newRepairing; }

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
        cout << "\nТруба с Id"<<id<<"\n"
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
        file << id << "\n";
        file << name << "\n";
        file << length << "\n";
        file << diameter << "\n";
        file << repairing << "\n";
    }

    bool loadFromFile(ifstream& file) {
        file >> id;
        file.ignore();
        getline(file, name);
        file >> length;
        file >> diameter;
        file >> repairing;
        file.ignore();
        return true;
    }
};

class CompressorStation {
private:
    int id;
    string name;
    int numberOfWorkshops;
    int operatingWorkshops;
    int stationClass;

public:
    CompressorStation(int newId=0) : id(newId), name(""), numberOfWorkshops(0), operatingWorkshops(0), stationClass(0) {}

    string getName() const { return name; }
    int getNumberOfWorkshops() const { return numberOfWorkshops; }
    int getOperatingWorkshops() const { return operatingWorkshops; }
    int getStationClass() const { return stationClass; }
    int getId() const { return id; }

    void setName(const string& newName) { name = newName; }
    void setNumberOfWorkshops(int n) { numberOfWorkshops = n; }
    void setOperatingWorkshops(int n) { operatingWorkshops = n; }
    void setStationClass(int n) { stationClass = n; }

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
        cout << "\n КС с Id"<<id<<"\n"
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
        file << id << "\n";
        file << name << "\n";
        file << numberOfWorkshops << "\n";
        file << operatingWorkshops << "\n";
        file << stationClass << "\n";
    }

    bool loadFromFile(ifstream& file) {
        file >> id;
        file.ignore();
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

    map<int, Pipe> pipe;
    map<int, CompressorStation> compressorStation;

    int nextPipeId = 1;
    int nextCsId = 1;

    while (true) {
        cout << "\nМеню\n"
            << "1.Добавить трубу\n"
            << "2.Добавить кс\n"
            << "3.Просмотр всех объектов\n"
            << "4.Редактировать трубу (в ремонте/не в ремонте)\n"
            << "5.Редактировать кс (запуск/ остановка цеха)\n"
            << "6. Удалить трубу по id\n"
            << "7. Удалить кс по id\n"
            <<"8. Загрузить в файл\n"
            <<"9. Выгрузить из файла\n"
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

        case 1: {
            int id = nextPipeId++;
            Pipe newPipe(id);
            newPipe.input();
            pipe.emplace(id, newPipe);
            cout << "Труба добавлена (Id" << id << ")\n";
            break;
        }

        case 2: {
            int id = nextCsId++;
            CompressorStation newCs(id);
            newCs.input();
            compressorStation.emplace(id, newCs);
            cout << "Добавлена кс (Id" << id << ")\n";
            break;
        }

        case 3: {
            if (pipe.empty() && compressorStation.empty()) {
                cout << "Пока ничего не введено\n";
            }
            else {
                for (const auto& pair : pipe) {
                    cout << "\n[ID " << pair.first << "]";
                    pair.second.print();
                }
                for (const auto& pair : compressorStation) {
                    cout << "\n[id " << pair.first << "]";
                    pair.second.print();
                }
            }
            break;
        }

        case 4: {
            if (pipe.empty()) {
                cout << "Сначала добавьте трубу (п.1)\n";
                break;
            }
            cout << "Введите id трубы\n";
            int id;
            cin >> id;

            auto it = pipe.find(id);
            if (it == pipe.end()) {
                cout << "Труба с Id " << id << "не найдена\n";
            }
            else {
                it->second.inTheRepair();
            }
            break;
        }

        case 5: {
            if (compressorStation.empty()) {
                cout << "Сначала добавьте кс\n";
                break;
            }
            cout << "Введите id кс\n";
            int id;
            cin >> id;

            auto it = compressorStation.find(id);
            if (it == compressorStation.end()) {
                cout << "КС с Id" << id << "не найдена\n";
                break;
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
                    it->second.startWorkshop();
                    break;
                case 2:
                    it->second.stopWorkshop();
                    break;
                default:
                    cout << "Неверный пункт меню\n";
                }
            }
            break;
        }

        case 6: {
            if (pipe.empty()) {
                cout << "Сначала добавьте трубу(п 1)\n";
                break;
            }

            cout << "Введите id трубы для удаления\n";
            int id;
            cin >> id;

            if (pipe.erase(id) > 0) {
                cout << "Труба с ID " << id << " удалена\n";
            }
            else {
                cout << "Труба с ID " << id << " не найдена\n";
            }
            break;
        }

        case 7: {
            if (compressorStation.empty()) {
                cout << "Сначала добавьте кс\n";
                break;
            }

            cout << "Введите id кс для удаления\n";
            int id;
            cin >> id;

            if (compressorStation.erase(id) > 0) {
                cout << "Кс с id " << id << "удалена\n";
            }
            else {
                cout << "Кс с таким id не найдено\n";
            }
            break;
        }

        case 8: {
            ofstream file("data.txt");
            if (!file.is_open()) {
                cout << "Ошибка открытия файла\n";
                break;
            }

            file << pipe.size() << "\n";

            for ( auto& pair : pipe) {
                pair.second.saveToFile(file);
            }

            file << compressorStation.size() << "\n";
            for ( auto& pair : compressorStation) {
                pair.second.saveToFile(file);
            }

            file.close();
            cout << "Данные сохранены\n";
            
            break;
        }

        case 9: {
            ifstream file("data.txt");
            if (!file.is_open()) {
                cout << "Файл не найден\n";
                break;
            }

            pipe.clear();
            compressorStation.clear();

            int pipeCount;
            file >> pipeCount;
            file.ignore();

            for (int i = 0; i < pipeCount; ++i) {
                Pipe p;
                p.loadFromFile(file);
                pipe.emplace(p.getId(), p);
            }

            int csCount;
            file >> csCount;
            file.ignore();

            for (int i = 0; i < csCount; ++i) {
                CompressorStation cs;
                cs.loadFromFile(file);
                compressorStation.emplace(cs.getId(), cs);
            }

            file.close();

            if (!pipe.empty()) {
                nextPipeId = pipe.rbegin()->first + 1;
            }

            if (!compressorStation.empty()) {
                nextCsId = compressorStation.rbegin()->first + 1;
            }

            cout << "Загружено труб:" << pipe.size()
                << ", КС" << compressorStation.size();
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
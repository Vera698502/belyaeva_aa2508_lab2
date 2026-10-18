#include <iostream>
#include <string>
#include <clocale>
#include <fstream>
#include <map>
#include <vector>

using namespace std;

class Pipe {
private:
    int id;
    string name;
    double length;
    double diameter;
    bool repairing;

    inline static int nextId = 1;

public:
    Pipe() : id(nextId++), name(""), length(0.0), diameter(0.0), repairing(false) {}

    int getId() const { return id; }
    string getName() const { return name; }
    bool isRepairing() const { return repairing; }

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
        cout << "\nТруба с Id" << id << "\n"
            << "Название: " << name << "\n"
            << "Длина: " << length << " км\n"
            << "Диаметр: " << diameter << " мм\n"
            << "Состояние: " << (repairing ? "В ремонте" : "В работе") << "\n";
    }

    void inTheRepair() {
        repairing = !repairing;
        cout << "Признак изменён. Труба теперь: " << (repairing ? "В ремонте" : "В работе") << "\n";
    }

    void saveToFile(ofstream& file) {
        file << id << "\n" << name << "\n" << length << "\n" << diameter << "\n" << repairing << "\n";
    }

    bool loadFromFile(ifstream& file) {
        if (!(file >> id)) return false;
        file.ignore();
        if (!getline(file, name)) return false;
        if (!(file >> length)) return false;
        if (!(file >> diameter)) return false;
        if (!(file >> repairing)) return false;
        file.ignore();

        if (id >= nextId) nextId = id + 1;
        return true;
    }

    bool matchesName(const string& query) const { return name == query; }
    bool matchesRepair(bool repairState) const { return repairing == repairState; }
};

class CompressorStation {
private:
    int id;
    string name;
    int numberOfWorkshops;
    int operatingWorkshops;
    int stationClass;

    inline static int nextId = 1;

public:
    CompressorStation() : id(nextId++), name(""), numberOfWorkshops(0), operatingWorkshops(0), stationClass(0) {}

    int getId() const { return id; }
    string getName() const { return name; }

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
        cout << "\n КС с Id" << id << "\n"
            << "Название: " << name << "\n"
            << "Количество цехов: " << numberOfWorkshops << "\n"
            << "Количество работающих цехов: " << operatingWorkshops << "\n"
            << "Класс станции: " << stationClass << "\n"
            << "Станция " << (operatingWorkshops > 0 ? "Работает " : "Простаивает") << "\n";
    }

    void startWorkshop() {
        bool canStart = (operatingWorkshops < numberOfWorkshops);
        cout << (canStart ? "Цех запущен.\n" : "Нельзя, все цеха в работе\n");
        if (canStart) operatingWorkshops++;
    }

    void stopWorkshop() {
        bool canStop = (operatingWorkshops > 0);
        cout << (canStop ? "Цех остановлен.\n" : "Нельзя, нет работающих цехов\n");
        if (canStop) operatingWorkshops--;
    }

    void saveToFile(ofstream& file) {
        file << id << "\n" << name << "\n" << numberOfWorkshops << "\n" << operatingWorkshops << "\n" << stationClass << "\n";
    }

    bool loadFromFile(ifstream& file) {
        if (!(file >> id)) return false;
        file.ignore();
        if (!getline(file, name)) return false;
        if (!(file >> numberOfWorkshops)) return false;
        if (!(file >> operatingWorkshops)) return false;
        if (!(file >> stationClass)) return false;
        file.ignore();

        if (id >= nextId) nextId = id + 1;
        return true;
    }

    double getIdlePercent() const {
        if (numberOfWorkshops == 0) return 0;
        return (numberOfWorkshops - operatingWorkshops) * 100.0 / numberOfWorkshops;
    }

    bool matchesName(const string& query) const { return name == query; }
    bool matchesIdlePercent(double minPercent) const { return getIdlePercent() >= minPercent; }
};

int readInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) return value;
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Ошибка! Введите целое число.\n";
    }
}

vector<int> findPipes(const map<int, Pipe>& pipe) {
    vector<int> foundIds;
    cout << "Искать по названию-1, по ремонту-2: ";
    int choice = readInt("");

    if (choice == 1) {
        cout << "Введите название для поиска: ";
        cin.ignore();
        string query;
        getline(cin, query);
        for (const auto& pair : pipe) {
            if (pair.second.matchesName(query)) foundIds.push_back(pair.first);
        }
    }
    else if (choice == 2) {
        cout << "Искать в ремонте? (1 - да, 0 - нет): ";
        int state = readInt("");
        for (const auto& pair : pipe) {
            if (pair.second.matchesRepair(state)) foundIds.push_back(pair.first);
        }
    }
    else {
        cout << "Неверный фильтр\n";
    }
    return foundIds;
}

int main()
{
    ofstream log("log.txt", ios::app);
    log << "\nЗапуск программы\n";
    setlocale(LC_ALL, "Russian");

    map<int, Pipe> pipe;
    map<int, CompressorStation> compressorStation;

    while (true) {
        cout << "\nМеню\n"
            << "1.Добавить трубу\n"
            << "2.Добавить кс\n"
            << "3.Просмотр всех объектов\n"
            << "4.Редактировать трубу (в ремонте/не в ремонте)\n"
            << "5.Редактировать кс (запуск/ остановка цеха)\n"
            << "6. Удалить трубу по id\n"
            << "7. Удалить кс по id\n"
            << "8. Сохранить в файл\n"
            << "9. Загрузить из файла\n"
            << "10. Поиск труб\n"
            << "11. Поиск КС\n"
            << "12. Пакетное редактирование труб\n"
            << "0. Выход\n\n";

        int choice = readInt("Ваш выбор: \n");

        switch (choice) {

        case 1: {
            Pipe newPipe; 
            newPipe.input();
            pipe.emplace(newPipe.getId(), newPipe); 
            cout << "Труба добавлена (Id" << newPipe.getId() << ")\n";
            log << "Добавлена труба ID: " << newPipe.getId() << "\n";
            break;
        }

        case 2: {
            CompressorStation newCs; 
            newCs.input();
            compressorStation.emplace(newCs.getId(), newCs);
            cout << "Добавлена кс (Id" << newCs.getId() << ")\n";
            log << "Добавлена кс ID: " << newCs.getId() << "\n";
            break;
        }

        case 3: {
            if (pipe.empty() && compressorStation.empty()) {
                cout << "Пока ничего не введено\n";
            }
            else {
                for (const auto& pair : pipe) pair.second.print();
                for (const auto& pair : compressorStation) pair.second.print();
            }
            break;
        }

        case 4: {
            if (pipe.empty()) {
                cout << "Сначала добавьте трубу (п.1)\n";
                break;
            }
            int id = readInt("Введите id трубы: ");

            auto it = pipe.find(id);
            if (it == pipe.end()) {
                cout << "Труба с Id " << id << " не найдена\n";
            }
            else {
                it->second.inTheRepair();
                log << "Статус трубы ID " << id << " изменен (ремонт)\n";
            }
            break;
        }

        case 5: {
            if (compressorStation.empty()) {
                cout << "Сначала добавьте кс\n";
                break;
            }
            int id = readInt("Введите id кс: ");

            auto it = compressorStation.find(id);
            if (it == compressorStation.end()) {
                cout << "КС с Id " << id << " не найдена\n";
                break;
            }
            else {
                cout << "1.Запустить цех\n"
                    << "2. Остановить цех\n";
                int sub = readInt("Выбор: ");
                switch (sub) {
                case 1:
                    it->second.startWorkshop();
                    log << "Запущен цех на КС ID " << id << "\n";
                    break;
                case 2:
                    it->second.stopWorkshop();
                    log << "Остановлен цех на КС ID " << id << "\n";
                    break;
                default:
                    cout << "Неверный пункт меню\n";
                }
            }
            break;
        }

        case 6: {
            if (pipe.empty()) {
                cout << "Сначала добавьте трубу (п.1)\n";
                break;
            }
            int id = readInt("Введите id трубы для удаления: ");

            if (pipe.erase(id) > 0) {
                cout << "Труба с ID " << id << " удалена\n";
                log << "Удалена труба ID " << id << "\n";
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
            int id = readInt("Введите id кс для удаления: ");

            if (compressorStation.erase(id) > 0) {
                cout << "Кс с id " << id << " удалена\n";
                log << "Удалена КС ID " << id << "\n";
            }
            else {
                cout << "Кс с таким id не найдено\n";
            }
            break;
        }

        case 8: {
            cout << "Введите имя файла для сохранения: ";
            string filename;
            cin >> filename;
            ofstream file(filename);
            if (!file.is_open()) {
                cout << "Ошибка открытия файла\n";
                break;
            }

            file << pipe.size() << "\n";
            for (auto& pair : pipe) pair.second.saveToFile(file);

            file << compressorStation.size() << "\n";
            for (auto& pair : compressorStation) pair.second.saveToFile(file);

            file.close();
            cout << "Данные сохранены\n";
            log << "Данные сохранены в файл: " << filename << "\n";
            break;
        }

        case 9: {
            cout << "Введите имя файла для загрузки: ";
            string filename;
            cin >> filename;
            ifstream file(filename);
            if (!file.is_open()) {
                cout << "Файл не найден\n";
                break;
            }

            int pipeCount, csCount;
            if (!(file >> pipeCount)) {
                cout << "Ошибка: файл поврежден (нет данных о трубах)!\n";
                break;
            }
            file.ignore();

            pipe.clear();
            compressorStation.clear();
            bool isBroken = false;

            for (int i = 0; i < pipeCount; ++i) {
                Pipe p;
                if (!p.loadFromFile(file)) {
                    isBroken = true;
                    break;
                }
                pipe.emplace(p.getId(), p);
            }

            if (isBroken) {
                cout << "Ошибка чтения труб! Файл поврежден.\n";
                pipe.clear();
                break;
            }

            if (!(file >> csCount)) {
                cout << "Ошибка: файл поврежден (нет данных о КС)!\n";
                pipe.clear();
                break;
            }
            file.ignore();

            for (int i = 0; i < csCount; ++i) {
                CompressorStation cs;
                if (!cs.loadFromFile(file)) {
                    isBroken = true;
                    break;
                }
                compressorStation.emplace(cs.getId(), cs);
            }

            if (isBroken) {
                cout << "Ошибка чтения КС! Файл поврежден.\n";
                pipe.clear();
                compressorStation.clear();
                break;
            }

            file.close();

            cout << "Загружено труб: " << pipe.size() << ", КС: " << compressorStation.size() << "\n";
            log << "Загружены данные из файла: " << filename << "\n";
            break;
        }

        case 10: {
            if (pipe.empty()) {
                cout << "Нет труб для поиска\n";
                break;
            }
            vector<int> found = findPipes(pipe);
            if (found.empty()) {
                cout << "Труб не найдено!\n";
            }
            else {
                cout << "Найдено труб: " << found.size() << "\n";
                for (int id : found) pipe.at(id).print();
            }
            log << "Выполнен поиск труб\n";
            break;
        }

        case 11: {
            if (compressorStation.empty()) {
                cout << "Нет кс для поиска\n";
                break;
            }
            cout << "Искать по названию -1, по проценту незадействованных -2: ";
            int filterType = readInt("");

            vector<int> foundId;

            if (filterType == 1) {
                cout << "Введите название для поиска: ";
                cin.ignore();
                string query;
                getline(cin, query);
                for (const auto& pair : compressorStation) {
                    if (pair.second.matchesName(query)) foundId.push_back(pair.first);
                }
            }
            else if (filterType == 2) {
                cout << "Введите минимальный процент незадействованных цехов: ";
                double percent; cin >> percent;
                for (const auto& pair : compressorStation) {
                    if (pair.second.matchesIdlePercent(percent)) foundId.push_back(pair.first);
                }
            }
            else {
                cout << "Неверный тип фильтра\n";
                break;
            }

            if (foundId.empty()) {
                cout << "Ничего не найдено\n";
            }
            else {
                cout << "Найдено кс: " << foundId.size() << "\n";
                for (int id : foundId) compressorStation.at(id).print();
            }
            log << "Выполнен поиск КС\n";
            break;
        }

        case 12: {
            if (pipe.empty()) {
                cout << "Нет труб для поиска\n";
                break;
            }
            cout << "Шаг 1. Поиск труб.\n";

            vector<int> foundIds = findPipes(pipe);

            if (foundIds.empty()) {
                cout << "Ничего не найдено. Пакетное редактирование отменено.\n";
                break;
            }

            cout << "Найдено труб: " << foundIds.size() << "\n";
            for (int id : foundIds) pipe.at(id).print();

            cout << "\nЧто сделать с этими трубами?\n"
                << "1 - Отправить в ремонт\n"
                << "2 - Вернуть из ремонта\n"
                << "3 - Удалить\n"
                << "0 - Отмена\n";
            int action = readInt("Выбор: ");

            if (action == 0) break;

            cout << "\nПрименить ко всем найденным(1) или к выбранным(2): ";
            int scopeChoice = readInt("");

            vector<int> finalList;

            if (scopeChoice == 1) {
                finalList = foundIds;
            }
            else if (scopeChoice == 2) {
                cout << "Введи ID труб по одному, 0 - закончить\n";
                while (true) {
                    int inputId = readInt("");
                    if (inputId == 0) break;

                    bool isFound = false;
                    for (int id : foundIds) {
                        if (id == inputId) {
                            isFound = true;
                            break;
                        }
                    }

                    if (isFound) finalList.push_back(inputId);
                    else cout << "ID " << inputId << " нет в списке найденных!\n";
                }
            }
            else {
                cout << "Неверный выбор\n";
                break;
            }

            if (finalList.empty()) {
                cout << "Список пуст. Отмена\n";
                break;
            }

            int count = 0;
            for (int id : finalList) {
                if (action == 1) { pipe.at(id).setRepairing(true); count++; }
                else if (action == 2) { pipe.at(id).setRepairing(false); count++; }
                else if (action == 3) { pipe.erase(id); count++; }
            }
            cout << "Успешно обработано труб: " << count << "\n";
            log << "Пакетное редактирование. Обработано труб: " << count << "\n";
            break;
        }

        case 0:
            cout << "Выход из программы\n";
            log << "Завершение работы программы\n";
            log.close();
            return 0;

        default:
            cout << "Неверный пункт меню!\n";
        }
    }
    return 0;
}
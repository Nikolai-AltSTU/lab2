// lab2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#pragma comment(linker, "/STACK:16777216")

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <set>
#include <windows.h>

#define V_START INT32_MIN
#define V_FINISH INT32_MAX
using namespace std;
class Task;
class Event;

map<int, Event*> events;
map<int, Task*> tasks;

class Event
{
    int id = 0;
public:
    Event(int id);
    int getId() { return id; }
    int earlyMoment = INT32_MIN;
    int lateMoment = INT32_MAX;
    int reservedTime = 0;
    int newNumber = 0;
    int level = V_START;
    map<int, Task*> prevTasksOfEvent;
    map<int, Task*> nextTasksOfEvent;
    friend std::ostream& operator<< (std::ostream& out, const Event *event);
    bool isAfterOrEqualThanEvent(const Event* event);
    ~Event();
};

class Task {
public:
    int id = V_START;
    int prevEventIndex = V_START;
    int nextEventIndex = V_FINISH;
    Event* prevEvent;
    Event* nextEvent;
    int timeOfDuration = 0;
    int fullRezervedTime = 0;
    int independentRezervedTime = 0;
    Task(int id);
    Task(int id, int prevEventIndex, int nextEventIndex, int timeOfDuration);
    friend std::ostream& operator<< (std::ostream& out, const Task* task);
    friend std::istream& operator>> (std::istream& in, Task& task);
    ~Task();
};

Event::Event(int id) {
    this->id = id;
}
bool Event::isAfterOrEqualThanEvent(const Event* event)
{
    if (event == this)
        return true;
    for (int i = 0; i < prevTasksOfEvent.size(); i++)
    {
        if (prevTasksOfEvent[i]->prevEvent->isAfterOrEqualThanEvent(event))
            return true;
    }
    return false;
}
ostream& operator<<(ostream& out, const Event *event) {
    out << "ID события: " << event->id;
    if (event->level != V_START)
        out << " \tуровень упорядочивания: " << event->level;
    if (event->id == V_START)
    {
        out << " ( фиктивное начальное) ";
    }
    else if (event->id == V_FINISH)
    {
        out << " ( фиктивное конечное) ";
    }
    out << endl;
    out << " ранний срок: " << event->earlyMoment << endl;
    out << " поздний срок: " << event->lateMoment << endl;
    out << " резерв времени: " << event->reservedTime << endl;
    int lateMoment = INT32_MAX;
    int reservedTime = 0;

    //out << endl;
    /*
    out << " \tПредыдущие работы: " << endl;
    for (auto p : event->prevTasksOfEvent)
    {
        out << " \t\t" << (p.second) << endl;
    }
    out << " \tПоследующие работы: " << endl;
    for (auto p : event->nextTasksOfEvent)
    {
        out << " \t\t" << (p.second) << endl;
    }
    */
    return out;
}
Event::~Event() {
    while (!prevTasksOfEvent.empty() && prevTasksOfEvent.size() != 3722304989)
    {
        delete prevTasksOfEvent.begin()->second;
    }
    if (prevTasksOfEvent.size() == 0)
        prevTasksOfEvent.clear();
    while(!nextTasksOfEvent.empty() && nextTasksOfEvent.size() != 3722304989)
    {
        delete nextTasksOfEvent.begin()->second;
    }
    if (nextTasksOfEvent.size() == 0)
        nextTasksOfEvent.clear();
}

Task::Task(int id)
{
    this->id = id;
}
Task::Task(int id, int prevEventIndex, int nextEventIndex, int timeOfDuration)
{
    this->id = id;
    this->prevEventIndex = prevEventIndex;
    this->prevEvent = events[prevEventIndex];
    this->nextEventIndex = nextEventIndex;
    this->nextEvent = events[nextEventIndex];
    this->timeOfDuration = timeOfDuration;
}
ostream& operator<< (ostream& out, const Task* task) {
    out << "ID работы: " << task->id << "\t ";
    out << "Предыдущее событие: " << task->prevEventIndex << "\t ";
    out << "Следующее событие: " << task->nextEventIndex << "\t ";
    out << "Время: " << task->timeOfDuration << "\t ";
    out << "Полный резерв: " << task->fullRezervedTime << "\t ";
    out << "Независимый резерв: " << task->independentRezervedTime << endl;
    return out;
}
istream& operator>> (std::istream& in, Task& task)
{
    in >> task.prevEventIndex;
    in >> task.nextEventIndex;
    in >> task.timeOfDuration;
    task.prevEvent = events[task.prevEventIndex];
    task.nextEvent = events[task.nextEventIndex];
    return in;
}
Task::~Task() {
    for (auto key_event : prevEvent->nextTasksOfEvent)
    {
        if (key_event.second == this)
        {
            prevEvent->nextTasksOfEvent.erase(key_event.first);
            if (prevEvent->nextTasksOfEvent.size() == 0)
                prevEvent->nextTasksOfEvent.clear();
            break;
        }
        
    }
    
    for (auto key_event : nextEvent->prevTasksOfEvent)
    {
        if (key_event.second == this)
        {
            nextEvent->prevTasksOfEvent.erase(key_event.first);
            if (nextEvent->prevTasksOfEvent.size() == 0)
                nextEvent->prevTasksOfEvent.clear();
            break;
        }
    }
    tasks.erase(id);
}


int message(const string header, Event* eventToService, vector<string> variants)
{
    MessageBeep(1);
    cout << endl << "\t " << header << " \n\t\t" << eventToService << endl;
    cout << " Варианты разрешения конфликта:" << endl;
    for (int i = 1; i <= variants.size(); i++)
    {
        cout << " \t\t" << i << ") \t" << variants[i-1] << endl;
    }
    int variant = 0;
    do {
        cout << " Введите нужный номер варианта разрешения конфликта: ";
        cin >> variant;
    } while (variant < 1 || variant > variants.size());
    return variant - 1;
}
int message(const string header, Event* eventToService)
{
    cout << endl  << "\t " << header << " \n\t\t" << eventToService << endl;
    return 0;
}
int message(const string header, vector<Task*> vectorOfTasks)
{
    MessageBeep(1);
    cout << endl << "\t " << header << endl;
    cout << " Варианты разрешения конфликта:" << endl;
    for (int i = 1; i <= vectorOfTasks.size(); i++)
    {
        cout << " \t\t" << i << ") \t" << " Удалить работу \t "  << vectorOfTasks[i - 1] << endl;
    }
    int variant = 0;
    do {
        cout << " Введите нужный номер варианта разрешения конфликта: ";
        cin >> variant;
    } while (variant < 1 || variant > vectorOfTasks.size());
    return variant - 1;
}
int messageForStartFinish(const string header, vector<Event*> vectorOfEvents)
{
    MessageBeep(1);
    cout << endl << "\t " << header << endl;
    cout << " \t\t1) \t Оставить все события\t " << endl;
    for (int i = 2; i <= vectorOfEvents.size() + 1; i++)
    {
        cout << " \t\t" << i << ") \t" << " Удалить событие \t " << vectorOfEvents[i - 2] << endl;
    }
    int variant = 0;
    do {
        cout << " Введите нужный номер варианта разрешения конфликта: ";
        cin >> variant;
    } while (variant < 1 || variant > vectorOfEvents.size() + 1);
    return variant - 2;
}

int findTask(int index, map<int, Task*> tasks)
{
    for (auto it : tasks)
    {
        if (it.first == index)
            continue;
        if (it.second->nextEventIndex == tasks[index]->nextEventIndex
            && it.second->prevEventIndex == tasks[index]->prevEventIndex)
        {
            return it.first;
        }
    }
    return -1;
}

void inputTasks(int numberOfEvents, int numberOfTasks, ifstream* fin, map<int, Event*>& events, map<int, Task*>& tasks)
{
    for (int i = 0; i < numberOfEvents; i++)
    {
        events[i] = new Event(i);
    }

    for (int i = 0; i < numberOfTasks; i++)
    {
        tasks[i] = new Task(i);
        *fin >> *tasks[i];
        tasks[i]->nextEvent->prevTasksOfEvent[tasks[i]->prevEventIndex] = tasks[i];
        tasks[i]->prevEvent->nextTasksOfEvent[tasks[i]->nextEventIndex] = tasks[i];

        int theSameTask = findTask(i, tasks);
        if (theSameTask >= 0)
        {
            int numberOfTaskToRemove = message(" Конфликт!  Найдена дублирующая работа ", { tasks[theSameTask],  tasks[i] });
            if (numberOfTaskToRemove == 0) {
                numberOfTaskToRemove = theSameTask;
            }
            else
            {
                numberOfTaskToRemove = i;
            }
            delete tasks[numberOfTaskToRemove];
            tasks.erase(numberOfTaskToRemove);
        }
    }

    // удаление несуществующих событий, добавленных для ввода
    for (int i = 0; i < numberOfEvents; i++)
    {
        if (events[i]->nextTasksOfEvent.empty() && events[i]->prevTasksOfEvent.empty())
        {
            delete events[i];
            events.erase(i);
        }
    }

}

map<int, vector<Task*>> ways;
vector<Task*>* findCircle(Event* begin, Event* event)
{
    if (event->newNumber) {
        event->newNumber = 0;
        for (auto nextTask : event->nextTasksOfEvent)
        {
            if (nextTask.second->nextEvent == begin)
            {
                ways[begin->getId()].push_back(nextTask.second);
                return &ways[begin->getId()];
            }
            else {
                vector<Task*>* res = findCircle(begin, nextTask.second->nextEvent);
                if (res != nullptr)
                {
                    res->push_back(nextTask.second);
                    return res;
                }
            }
        }
    }
    return nullptr;
}

int removeCircles(map<int, Event*> &events, map<int, Task*> &tasks) {
    int numberOfRemovedTasks = 0;
    
    for(map<int, Event*>::iterator key_event = events.begin(); key_event != events.cend(); )
    {
        for (auto it_eventP : events)
            it_eventP.second->newNumber = 1;
        ways.clear();
        vector<Task*>* way = findCircle(key_event->second, key_event->second);
        //vector<Task*>* way = new vector<Task*>(findCircle(key_event->second, key_event->second));

        if (way != nullptr && !way->empty())
        {
            string header = " \t Конфликт! \n\t Найден цикл из " + to_string(way->size()) + " работ";
            int numberOfTaskToRemove = message(header , *way);
            tasks.erase((*way)[numberOfTaskToRemove]->id);
            delete (*way)[numberOfTaskToRemove];
            numberOfRemovedTasks++;
            key_event = events.begin();
        }
        else
        {
            key_event++;
        }
    }
    if (numberOfRemovedTasks == 0)
    {
        return 0;
    }
    else {
        return numberOfRemovedTasks + removeCircles(events, tasks);
    }
}

int findStartAndFinish(map<int, Event*>& events, Event*& start, Event*& finish, Event*& virtualStart, Event*& virtualFinish)
{
    vector<Event*> foundStartEvents;
    vector<Event*> foundFinishEvents;

    int numberOfDeleted = 0;
    for (auto key_event : events)
    {
        // поиск начального события
        if (key_event.second->prevTasksOfEvent.empty())
        {
            foundStartEvents.push_back(key_event.second);
        }
        if (key_event.second->nextTasksOfEvent.empty())
        {
            foundFinishEvents.push_back(key_event.second);
        }
    }

    if (foundStartEvents.size() == 1)
    {
        start = foundStartEvents[0];
    }
    else
    {
        cout << endl << endl << "\t\t Найденные начальные события: " << endl;
        for (auto key_event : foundStartEvents)
            cout << "\t\t " << key_event << endl;

        int answer = messageForStartFinish("\t\t Выберите вариант ", foundStartEvents);
        if (answer >= 0)
        {
            int id = foundStartEvents[answer]->getId();
            delete foundStartEvents[answer];
            events[id] = nullptr;
            numberOfDeleted++;
            events.erase(id);
            return numberOfDeleted;
        }
        else {
            virtualStart = new Event(V_START);
            events[V_START] = virtualStart;
            start = virtualStart;
            for (auto key_event : foundStartEvents)
            {
                virtualStart->nextTasksOfEvent[key_event->getId()] = new Task(V_START, V_START, key_event->getId(), 0);
                key_event->prevTasksOfEvent[V_START] = virtualStart->nextTasksOfEvent[key_event->getId()];
            }
        }
    }

    if (foundFinishEvents.size() == 1)
    {
        finish = foundFinishEvents[0];
    }
    else
    {
        cout << endl << endl << "\t\t Найденные конечные события: " << endl;
        for (auto key_event : foundFinishEvents)
            cout << "\t\t " << key_event << endl;

        int answer = messageForStartFinish("\t\t Выберите вариант ", foundFinishEvents);
        if (answer >= 0)
        {
            int id = foundFinishEvents[answer]->getId();
            delete foundFinishEvents[answer];
            events[id] = nullptr;
            numberOfDeleted++;
            events.erase(id);
            return numberOfDeleted;
        }
        else {
            virtualFinish = new Event(V_FINISH);
            events[V_FINISH] = virtualFinish;
            finish = virtualFinish;
            for (auto key_event : foundFinishEvents)
            {
                virtualFinish->prevTasksOfEvent[key_event->getId()] = new Task(V_FINISH, key_event->getId(), V_FINISH, 0);
                key_event->nextTasksOfEvent[V_FINISH] = virtualFinish->prevTasksOfEvent[key_event->getId()];
            }
        }
    }
    return numberOfDeleted;
}

vector<Event*> sortEvents(map<int, Event*>& events, Event*& start, Event*& finish, Event*& virtualStart, Event*& virtualFinish)
// сортировка вершин 
{
    int level = 1;
    set<Event*> notSortedEvents;
    vector<Event*> sortedEvents;
    sortedEvents.push_back(start);
    start->level = 0;

    // добавление событий в множество несортированных
    for (auto event : events)
    {
        if (event.second != start)
            notSortedEvents.insert(event.second);
    }
    if (virtualFinish)
    {
        notSortedEvents.insert(virtualFinish);
    }

    // образовался цикл, поэтому не работает
    while (!notSortedEvents.empty())
    {
        int numberOfNewAddedEvents = 0;
        for(auto event : notSortedEvents)
        {
            bool dontAdd = false;
            for (auto key_prevTask : event->prevTasksOfEvent)
            {
                // вот тут падает
                if (notSortedEvents.find(key_prevTask.second->prevEvent) != notSortedEvents.cend())
                {
                    dontAdd = true;
                    break;
                }
            }
            if (dontAdd)
            {
                continue;
            }
            else
            {
                numberOfNewAddedEvents++;
                sortedEvents.push_back(event);
            }
        }
        for (int i = sortedEvents.size() - numberOfNewAddedEvents; i < sortedEvents.size(); i++)
        {
            sortedEvents[i]->level = level;
            notSortedEvents.erase(sortedEvents[i]);
        }
        level++;
    }
    for (int i = 0; i < sortedEvents.size(); i++)
    {
        sortedEvents[i]->newNumber = i + 1;
    }
    return sortedEvents;
}

map<int, vector<vector<Event*>>> waysFromEvent;
vector<vector<Event*>> findFullWay(vector<Event*> *sortedEvents, Event* start, Event* finish)
{
    waysFromEvent.clear();
    waysFromEvent[finish->getId()].push_back({ finish });

    for (int i = sortedEvents->size() - 2; i >= 0; i--)
    {
        Event* pEvent = (*sortedEvents)[i];
        for (auto nextTask : pEvent->nextTasksOfEvent)
        {
            for (auto way : waysFromEvent[nextTask.second->nextEvent->getId()])
            {
                waysFromEvent[pEvent->getId()].push_back(way);
                waysFromEvent[pEvent->getId()].back().push_back(pEvent);
            }
        }
    }
    return waysFromEvent[start->getId()];
}

map<int, vector<vector<Event*>>> criticalWaysFromEvent;
vector<vector<Event*>> findCriticalWays(vector<Event*>* sortedEvents, Event* start, Event* finish)
{
    criticalWaysFromEvent.clear();
    criticalWaysFromEvent[finish->getId()].push_back({ finish });

    for (int i = sortedEvents->size() - 2; i >= 0; i--)
    {
        Event* pEvent = (*sortedEvents)[i];
        for (auto nextTask : pEvent->nextTasksOfEvent)
        {
            if (nextTask.second->fullRezervedTime > 0)
                continue;
            for (auto way : criticalWaysFromEvent[nextTask.second->nextEvent->getId()])
            {
                criticalWaysFromEvent[pEvent->getId()].push_back(way);
                criticalWaysFromEvent[pEvent->getId()].back().push_back(pEvent);
            }
        }
    }
    return criticalWaysFromEvent[start->getId()];
}


void printTasks(vector<Task*>& tasks, ofstream *fout = NULL)
{
    for (auto t : tasks)
    {
        if (fout != NULL)
            *fout << t << endl;
        else
            cout << t << endl;
    }
}
void printTasks(map<int, Task*>& tasks, ofstream* fout = NULL)
{
    for (auto t : tasks)
    {
        if (fout != NULL)
            *fout << t.second << endl;
        else
            cout << t.second << endl;
    }
}
void printEvents(vector<Event*>& sortedEvents, ofstream* fout = NULL)
{
    for (auto t : tasks)
    {
        if (fout != NULL)
            *fout << t.second << endl;
        else
            cout << t.second << endl;
    }
}

void findParamsForEvents(vector<Event*> &sortedEvents)
{
    // прямой ход
    sortedEvents[0]->earlyMoment = 0;
    for (int i = 1; i < sortedEvents.size(); i++)
    {
        for (auto prevTask : sortedEvents[i]->prevTasksOfEvent)
        {
            sortedEvents[i]->earlyMoment = max(sortedEvents[i]->earlyMoment,
                prevTask.second->prevEvent->earlyMoment + prevTask.second->timeOfDuration);
        }
    }

    // обратный ход и вычисление резерва
    sortedEvents[sortedEvents.size() - 1]->lateMoment = sortedEvents[sortedEvents.size() - 1]->earlyMoment;
    for (int i = sortedEvents.size() - 2; i >= 0; i--)
    {
        for (auto nextTask : sortedEvents[i]->nextTasksOfEvent)
        {
            sortedEvents[i]->lateMoment = min(sortedEvents[i]->lateMoment,
                nextTask.second->nextEvent->lateMoment - nextTask.second->timeOfDuration);
        }
        sortedEvents[i]->reservedTime = sortedEvents[i]->lateMoment - sortedEvents[i]->earlyMoment;
    }
}

void findParamsForTasks(map<int, Task*> &tasks)
{
    for (auto itTask : tasks)
    {
        itTask.second->fullRezervedTime = itTask.second->nextEvent->lateMoment - itTask.second->prevEvent->earlyMoment
            - itTask.second->timeOfDuration;
        itTask.second->independentRezervedTime = itTask.second->nextEvent->earlyMoment - itTask.second->prevEvent->lateMoment
            - itTask.second->timeOfDuration;
    }
}

vector<Task*> getSortedTasks(const vector<Event*> sortedEvents)
{
    vector<Task*> sortedTasks;
    for (int i = 0; i < sortedEvents.size(); i++)
    {
        for (auto it : sortedEvents[i]->nextTasksOfEvent)
        {
            sortedTasks.push_back(it.second);
        }
    }
    return sortedTasks;
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    ifstream *fin = new ifstream("input.dat", fstream::in);
    ofstream *fout = new ofstream("output.dat", fstream::trunc);
    ofstream *foutGraph = new ofstream("graph.dat", fstream::trunc);
    int numberOfEvents = 0, numberOfTasks = 0;
    *fin >> numberOfEvents >> numberOfTasks;

    inputTasks(numberOfEvents, numberOfTasks, fin, events, tasks);

    // вывод исходного списка работ
    cout << " Исходный список работ: " << endl;
    printTasks(tasks);
    int lineN = 30; while (cout << '-', lineN--); cout << endl;
    
    removeCircles(events, tasks);

    Event* start = nullptr, * finish = nullptr;
    Event* virtualStart = nullptr, * virtualFinish = nullptr;
    while(findStartAndFinish(events, start, finish, virtualStart, virtualFinish));
   
    vector<Event*> sortedEvents = sortEvents(events, start, finish, virtualStart, virtualFinish);
            
    vector<Task*> sortedTasks = getSortedTasks(sortedEvents);

    // Печать частично упорядоченного списка работ
    cout << " Частично упорядоченного списка работ:" << endl;
    printTasks(sortedTasks);

    // Вывод в файл частично упорядоченного списка работ
    *foutGraph << sortedEvents.size() << " \t" << sortedTasks.size() << endl;
    for (int i = 0; i < sortedEvents.size(); i++)
    {
        *fout << sortedEvents[i] << endl;
        for (auto it : sortedEvents[i]->nextTasksOfEvent)
        {
            *foutGraph << it.second->prevEvent->getId() << " \t" << it.second->nextEvent->getId() << " \t" << it.second->timeOfDuration << endl;
        }
    }

    findParamsForEvents(sortedEvents);
    findParamsForTasks(tasks);

    vector<vector<Event*>> criticalWays = findCriticalWays(&sortedEvents, start, finish);
    /*
    cout << endl << " \t Список полных путей " << endl;
    for (auto fullWay : findFullWay(&sortedEvents, start, finish))
    {
        for (int i = fullWay.size() - 1; i >= 0; i--)
        {
            cout << fullWay[i]->getId() << " \t";
        }
        cout << endl;
    }
    
    cout << " Длина критического пути: " << finish->earlyMoment << endl;
    cout << endl << " \t Список критических путей " << endl;
    for (auto criticalWay : findCriticalWay(&sortedEvents, start, finish))
    {
        for (int i = criticalWay.size() - 1; i >= 0; i--)
        {
            cout << criticalWay[i]->getId() << " \t";
        }
        cout << endl;
    }
    */



    fin->close(); delete fin;
    fout->close();  delete fout;
    foutGraph->close(); delete foutGraph;
}

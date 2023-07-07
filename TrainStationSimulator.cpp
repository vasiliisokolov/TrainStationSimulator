#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::mutex staition_access;
std::vector<Train*> trains;
std::vector<std::thread> railways;

class Train
{
public:
    char litera;
    int time;

    void trip(RailwayStation* station)
    {
        while (time > 0)
        {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            time -= 5;
        }
        arrive(station);
    }

    void arrive(RailwayStation* station)
    {
        if (!staition_access.try_lock())
        {
            std::cout << "Train " << litera << " waiting to arrive!" << std::endl;
        }
        else
        {
            char answer;
            staition_access.lock();
            station->setTrain(this);
            //staition_access.unlock();
            std::cout << "Train " << litera << " requesting permission to send!(Y/N)";
            std::cin >> answer;
            if(answer == 'Y') station->departTrain();
        }
    }
    
    void depart()
    {        
         staition_access.unlock();
    }

    Train(char inlitera, int inTime) : litera(inlitera), time(inTime)
    {

    }
};

class RailwayStation
{
    Train* train;

public:
    int count;

    void setTrain(Train* inTrain)
    {
        train = inTrain;
        count++;
        std::cout << "Train " << train->litera << " arrived at the station!" << std::endl;
    }

    void departTrain()
    {
        std::cout << "Train " << train->litera << " departs!" << std::endl;
        train->depart();
    }

    RailwayStation()
    {
        train = nullptr;
        count = 0;
    }
};

int main()
{
    std::cout << "Railway Station simulation!\n";
    RailwayStation* station = new RailwayStation();

    for (int i = 0; i < 3; i++)
    {
        char tempChar;
        int tempTime;
        std::cout << "Enter a train's litera: ";
        std::cin >> tempChar;
        std::cout << "Enter a time to arrive: ";
        std::cin >> tempTime;
        Train* train = new Train(tempChar, tempTime);
        trains.push_back(train);
    }

    for (int i = 0; i < 3; i++)
    {
        railways.emplace_back(&Train::trip, trains[i], station);
    }

    while(station->count > 3)
    for (int i = 0; i < 3; i++)
    {
        railways[i].join();
    }

    

    for (int i = 2; i >= 0; i--)
    {
        delete trains[i];
    }
    delete station;
}


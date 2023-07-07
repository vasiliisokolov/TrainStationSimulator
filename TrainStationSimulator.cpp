#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::mutex staition_access;

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
            staition_access.lock();
            station->setTrain(this);
            //staition_access.unlock();
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

}


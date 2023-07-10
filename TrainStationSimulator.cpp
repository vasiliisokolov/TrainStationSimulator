#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::mutex staition_access;
std::vector<char> station;
std::vector<std::thread> railways;

class Train
{
public:
    char litera;
    int time;

    void trip()
    {
        while (time > 0)
        {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            time -= 5;
            std::cout << "Train's " << litera << " time to arrive: " << time << std::endl;
        }
        std::cout << "Train " << litera << " arrived at the station!" << std::endl;
        arrive();
    }

    void arrive()
    {
        if (staition_access.try_lock())
        {
            
            station.push_back(litera);
            std::this_thread::sleep_for(std::chrono::seconds(5));
            std::cout << "Train " << litera << " departs!" << std::endl;
            staition_access.unlock();
            
        }
        else
        {
            std::cout << "Train " << litera << " waiting to arrive!" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(5));
            arrive();
        }
        
        
    }

    Train(char inlitera, int inTime) : litera(inlitera), time(inTime)
    {

    }
};


int main()
{
    std::vector<Train*> trains;
    std::cout << "Railway Station simulation!\n";
    

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
        railways.emplace_back(&Train::trip, trains[i]);
    }
       
    for (int i = 0; i < 3; i++)
    {
        railways[i].join();
    }
    
    for (int i = 2; i >= 0; i--)
    {
        delete trains[i];
    }
    
}


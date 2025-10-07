#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>

class ChocolateBoiler
{
public:
    static ChocolateBoiler &GetInstance()
    {
        static ChocolateBoiler _instance;
        return _instance;
    }

    void Fill()
    {
        if (IsEmpty())
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _empty = false;
            _boiled = false;
            std::cout << "Filling the boiler with a chocolate mixture" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }

    void Drain()
    {
        if (!IsEmpty() && IsBoiled())
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _empty = true;
            std::cout << "Draining the boiled chocolate and milk" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }

    void Boil()
    {
        if (!IsEmpty() && !IsBoiled())
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _boiled = true;
            std::cout << "Boiling the chocolate mixture" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    bool IsEmpty() const
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return _empty;
    }

    bool IsBoiled() const
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return _boiled;
    }

    ChocolateBoiler(const ChocolateBoiler &) = delete;
    ChocolateBoiler &operator=(const ChocolateBoiler &) = delete;

private:
    ChocolateBoiler()
    {
        std::cout << "Creating Chocolate Boiler" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        _empty = true;
        _boiled = false;
        std::cout << "Chocolate Boiler created" << std::endl;
    }

    bool _empty;
    bool _boiled;
    mutable std::mutex _mutex;
};

void useBoiler()
{
    std::cout << "Thread " << std::this_thread::get_id() << " is using the boiler." << std::endl;
    ChocolateBoiler &boiler = ChocolateBoiler::GetInstance();
    boiler.Fill();
    boiler.Boil();
    boiler.Drain();
}

int main()
{
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i)
    {
        threads.emplace_back(useBoiler);
    }

    for (auto &t : threads)
    {
        t.join();
    }

    return 0;
}
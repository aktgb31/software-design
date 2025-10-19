#include <iostream>
#include <string>

class CaffeineBeverage
{
public:
    virtual void prepare() final
    {
        boilWater();
        brew();
        addCondiments();
        pourInCup();
    }

    void boilWater()
    {
        std::cout << "Boiling water" << std::endl;
    }

    virtual void brew() = 0;

    virtual void addCondiments() = 0;

    void pourInCup()
    {
        std::cout << "Pouring in cup" << std::endl;
    }
};

class Coffee : public CaffeineBeverage
{
public:
    void brew() override
    {
        std::cout << "Dripping Coffee through filter" << std::endl;
    }

    void addCondiments() override
    {
        std::cout << "Adding Sugar and Milk" << std::endl;
    }
};

class Tea : public CaffeineBeverage
{
public:
    void brew() override
    {
        std::cout << "Steeping the tea" << std::endl;
    }

    void addCondiments() override
    {
        std::cout << "Adding Lemon" << std::endl;
    }
};

int main()
{
    CaffeineBeverage *beverage1 = new Coffee();
    beverage1->prepare();
    std::cout << std::endl;

    CaffeineBeverage *beverage2 = new Tea();
    beverage2->prepare();
    std::cout << std::endl;

    delete beverage1;
    delete beverage2;

    return 0;
}
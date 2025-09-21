#include <iostream>
#include <memory>

class FlyBehaviour
{
public:
    virtual void Fly()
    {
        std::cout << "Flying Simply\n";
    }
};

class FlyWithWings : public FlyBehaviour
{
public:
    virtual void Fly()
    {
        std::cout << "Flying with Wings\n";
    }
};

class FlyNoWay : public FlyBehaviour
{
public:
    virtual void Fly()
    {
        std::cout << "Can't fly\n";
    }
};

class FlyRocketPowered : public FlyBehaviour
{
public:
    virtual void Fly()
    {
        std::cout << "Flying with a Rocket\n";
    }
};

class QuackBehaviour
{
public:
    virtual void Quack()
    {
        std::cout << "Quacking Simply\n";
    }
};

class Squeak : public QuackBehaviour
{
public:
    virtual void Quack()
    {
        std::cout << "Squeaking\n";
    }
};

class Duck
{
protected:
    std::unique_ptr<FlyBehaviour> flyBehaviour;
    std::unique_ptr<QuackBehaviour> quackBehaviour;

public:
    virtual void Swim()
    {
        std::cout << "Swimming\n";
    }

    virtual void Display()
    {
        std::cout << "Display a simple duck\n";
    }
    void PerformFly()
    {
        flyBehaviour->Fly();
    }
    void PerformQuack()
    {
        quackBehaviour->Quack();
    }
    void SetFlyBehaviour(FlyBehaviour *fb)
    {
        flyBehaviour.reset(fb);
    }
    void SetQuackBehaviour(QuackBehaviour *qb)
    {
        quackBehaviour.reset(qb);
    }
};

class MallardDuck : public Duck
{
public:
    MallardDuck()
    {
        flyBehaviour = std::make_unique<FlyWithWings>();
        quackBehaviour = std::make_unique<QuackBehaviour>();
    }
    virtual void Display()
    {
        std::cout << "Display a Mallard duck\n";
    }
};

class RubberDuck : public Duck
{
public:
    RubberDuck()
    {
        flyBehaviour = std::make_unique<FlyNoWay>();
        quackBehaviour = std::make_unique<Squeak>();
    }
    virtual void Display()
    {
        std::cout << "Display a Rubber duck\n";
    }
};

class ModelDuck : public Duck
{
public:
    ModelDuck()
    {
        flyBehaviour = std::make_unique<FlyNoWay>();
        quackBehaviour = std::make_unique<QuackBehaviour>();
    }
    virtual void Display()
    {
        std::cout << "Display a Model duck\n";
    }
};

int main()
{
    std::unique_ptr<Duck> mallard = std::make_unique<MallardDuck>();
    mallard->Display();
    mallard->Swim();
    mallard->PerformFly();
    mallard->PerformQuack();

    std::unique_ptr<Duck> rubber = std::make_unique<RubberDuck>();
    rubber->Display();
    rubber->Swim();
    rubber->PerformFly();
    rubber->PerformQuack();

    std::unique_ptr<Duck> model = std::make_unique<ModelDuck>();
    model->Display();
    model->Swim();
    model->PerformFly();
    model->PerformQuack();
    model->SetFlyBehaviour(new FlyRocketPowered());
    model->PerformFly();
    return 0;
}
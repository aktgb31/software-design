#include <stdio.h>
#include <string.h>

class Duck
{
public:
    virtual void quack() = 0;
    virtual void fly() = 0;
    virtual ~Duck() {}
};

class WildDuck : public Duck
{
public:
    void quack() override
    {
        printf("Quack Quack\n");
    }
    void fly() override
    {
        printf("I'm flying\n");
    }
};

class Turkey
{
public:
    virtual void gobble() = 0;
    virtual void fly() = 0;
    virtual ~Turkey() {}
};

class WildTurkey : public Turkey
{
public:
    void gobble()
    {
        printf("Gobble Gobble\n");
    }
    void fly()
    {
        printf("I'm flying a short distance\n");
    }
};

class TurkeyAdapter : public Duck
{
private:
    Turkey *turkey;

public:
    TurkeyAdapter(Turkey *t) : turkey(t) {}
    void quack() override
    {
        turkey->gobble();
    }
    void fly() override
    {
        turkey->fly();
    }
};

class DuckAdapter : public Turkey
{
private:
    Duck *duck;

public:
    DuckAdapter(Duck *d) : duck(d) {}
    void gobble() override
    {
        duck->quack();
    }
    void fly() override
    {
        duck->fly();
    }
};

int main()
{
    Duck *duck = new WildDuck();
    duck->quack();
    duck->fly();

    Turkey *turkey = new WildTurkey();
    Duck *turkeyAdapter = new TurkeyAdapter(turkey);
    turkeyAdapter->quack();
    turkeyAdapter->fly();

    delete duck;
    delete turkeyAdapter;
    delete turkey;

    return 0;
}
#include <iostream>
#include <memory>

class WeaponBehavior
{
public:
    virtual void useWeapon() = 0;
    virtual ~WeaponBehavior() {}
};

class AxeBehaviour : public WeaponBehavior
{
public:
    virtual void useWeapon() override
    {
        std::cout << "Swinging an axe!\n";
    }
};

class SwordBehaviour : public WeaponBehavior
{
public:
    virtual void useWeapon() override
    {
        std::cout << "Swinging a sword!\n";
    }
};

class KnifeBehaviour : public WeaponBehavior
{
public:
    virtual void useWeapon() override
    {
        std::cout << "Cutting with a knife!\n";
    }
};

class BowAndArrowBehaviour : public WeaponBehavior
{
public:
    virtual void useWeapon() override
    {
        std::cout << "Shooting an arrow with a bow!\n";
    }
};

class Character
{
protected:
    std::unique_ptr<WeaponBehavior> weapon;

public:
    virtual void fight() = 0;
    virtual ~Character() {}

    void SetWeapon(WeaponBehavior *wb)
    {
        weapon.reset(wb);
    }
};

class King : public Character
{
public:
    virtual void fight() override
    {
        std::cout << "King fighting: ";
        weapon->useWeapon();
    }
};

class Queen : public Character
{
public:
    virtual void fight() override
    {
        std::cout << "Queen fighting: ";
        weapon->useWeapon();
    }
};

class Troll : public Character
{
public:
    virtual void fight() override
    {
        std::cout << "Troll fighting: ";
        weapon->useWeapon();
    }
};

class Knight : public Character
{
public:
    virtual void fight() override
    {
        std::cout << "Knight fighting: ";
        weapon->useWeapon();
    }
};

int main()
{
    std::unique_ptr<Character> king = std::make_unique<King>();
    king->SetWeapon(new SwordBehaviour());
    king->fight();

    std::unique_ptr<Character> queen = std::make_unique<Queen>();
    queen->SetWeapon(new BowAndArrowBehaviour());
    queen->fight();

    std::unique_ptr<Character> troll = std::make_unique<Troll>();
    troll->SetWeapon(new AxeBehaviour());
    troll->fight();

    std::unique_ptr<Character> knight = std::make_unique<Knight>();
    knight->SetWeapon(new KnifeBehaviour());
    knight->fight();

    // Changing weapon at runtime
    knight->SetWeapon(new SwordBehaviour());
    knight->fight();

    return 0;
}
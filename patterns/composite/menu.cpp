#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <algorithm>
#include <stack>

class Iterator
{
public:
    virtual bool hasNext() = 0;
    virtual void *next() = 0;
    virtual void remove() = 0;
    virtual ~Iterator() = default;
};

class MenuComponent
{
public:
    virtual std::string getName() const
    {
        throw std::runtime_error("Unsupported Operation");
    }
    virtual std::string getDescription() const
    {
        throw std::runtime_error("Unsupported Operation");
    }
    virtual bool isVegetarian() const
    {
        throw std::runtime_error("Unsupported Operation");
    }
    virtual double getPrice() const
    {
        throw std::runtime_error("Unsupported Operation");
    }
    virtual void print() const
    {
        throw std::runtime_error("Unsupported Operation");
    }
    virtual void addComponent(MenuComponent *component)
    {
        throw std::runtime_error("Unsupported Operation");
    }
    virtual void removeComponent(MenuComponent *component)
    {
        throw std::runtime_error("Unsupported Operation");
    }
    virtual MenuComponent *getChild(int index)
    {
        throw std::runtime_error("Unsupported Operation");
    }
    virtual Iterator *createIterator()
    {
        throw std::runtime_error("Unsupported Operation");
    }

    virtual ~MenuComponent() = default;
};

class NullIterator : public Iterator
{
public:
    bool hasNext() override { return false; }
    void *next() override { return nullptr; }
    void remove() override { throw std::runtime_error("Unsupported Operation"); }
};

class MenuItem : public MenuComponent
{
    std::string name;
    std::string description;
    bool vegetarian;
    double price;

public:
    MenuItem(const std::string &name, const std::string &description, bool vegetarian, double price)
        : name(name), description(description), vegetarian(vegetarian), price(price) {}

    std::string getName() const override
    {
        return name;
    }

    std::string getDescription() const override
    {
        return description;
    }

    bool isVegetarian() const override
    {
        return vegetarian;
    }

    double getPrice() const override
    {
        return price;
    }

    void print() const override
    {
        std::cout << "  " << getName();
        if (isVegetarian())
        {
            std::cout << " (v)";
        }
        std::cout << ", " << getPrice() << std::endl;
        std::cout << "     -- " << getDescription() << std::endl;
    }

    Iterator *createIterator()
    {
        return new NullIterator();
    }
};

class Menu : public MenuComponent
{
    std::string name;
    std::string description;

    typedef std::vector<std::unique_ptr<MenuComponent>> ComponentList;
    ComponentList components;

    class MenuIterator : public Iterator
    {
        ComponentList &components;
        ComponentList::iterator _it;

    public:
        MenuIterator(ComponentList &components) : components(components)
        {
            _it = components.begin();
        }

        void *next() override
        {
            if (!hasNext())
                return nullptr;
            return (_it++)->get();
        }

        bool hasNext() override
        {
            return _it != components.end();
        }
        void remove() override { throw std::runtime_error("Unsupported Operation"); }
    };

public:
    Menu(const std::string &name, const std::string &description) : name(name), description(description) {}

    std::string getName() const override
    {
        return name;
    }

    std::string getDescription() const override
    {
        return description;
    }

    void addComponent(MenuComponent *component) override
    {
        components.emplace_back((component));
    }

    void removeComponent(MenuComponent *component) override
    {
        components.erase(std::remove_if(components.begin(), components.end(),
                                        [component](const std::unique_ptr<MenuComponent> &ptr)
                                        { return ptr.get() == component; }),
                         components.end());
    }

    MenuComponent *getChild(int index) override
    {
        if (index < 0 || index >= static_cast<int>(components.size()))
        {
            throw std::out_of_range("Index out of range");
        }
        return components[index].get();
    }

    void print() const override
    {
        std::cout << std::endl
                  << getName() << ", " << getDescription() << std::endl;
        std::cout << "---------------------" << std::endl;

        for (const auto &component : components)
        {
            component->print();
        }
    }

    Iterator *createIterator();
};

class CompositeIterator : public Iterator
{
    std::stack<std::unique_ptr<Iterator>> _stack;

public:
    CompositeIterator(Iterator *iterator)
    {
        _stack.emplace(iterator);
    }

    void *next() override
    {
        if (!hasNext())
            return nullptr;
        auto &iterator = _stack.top();
        MenuComponent *component = static_cast<MenuComponent *>(iterator->next());
        if (Menu *menu = dynamic_cast<Menu *>(component))
        {
            _stack.emplace(component->createIterator());
        }
        return component;
    }

    bool hasNext() override
    {
        if (_stack.empty())
            return false;
        auto &iterator = _stack.top();
        if (iterator->hasNext())
            return true;
        else
        {
            _stack.pop();
            return hasNext();
        }
    }
    void remove() override { throw std::runtime_error("Unsupported Operation"); }
};

Iterator *Menu::createIterator()
{
    return new CompositeIterator(new MenuIterator(components));
}

class Waitress
{
    MenuComponent *menus;

public:
    Waitress(MenuComponent *menus) : menus(menus) {}
    void printMenu() const
    {
        menus->print();
    }

    void printVegetarianMenu()
    {
        std::unique_ptr<Iterator> iterator(menus->createIterator());
        while (iterator->hasNext())
        {
            MenuComponent *menuComponent = static_cast<MenuComponent *>(iterator->next());
            try
            {
                if (menuComponent->isVegetarian())
                    menuComponent->print();
            }
            catch (const std::runtime_error &e)
            {
            }
        }
    }
};

int main()
{

    MenuComponent *pancakeHouseMenu = new Menu("Pancake House", "Breakfast");
    MenuComponent *dinerMenu = new Menu("Diner", "Lunch");
    MenuComponent *cafeMenu = new Menu("Cafe", "Evenings");
    MenuComponent *dessertMenu = new Menu("Dessert Menu", "Dessert of course!");

    MenuComponent *allMenus = new Menu("All Menus", "All available menus");
    allMenus->addComponent(pancakeHouseMenu);
    allMenus->addComponent(dinerMenu);
    allMenus->addComponent(cafeMenu);

    pancakeHouseMenu->addComponent(new MenuItem("Fruit Bowl", "A bowl of fresh fruit", true, 3.99));
    dinerMenu->addComponent(new MenuItem("Pasta", "Spaghetti with Marinara Sauce, and a slice of sourdough bread", true, 3.89));
    cafeMenu->addComponent(new MenuItem("Veggie Burger and Air Fries", "Veggie burger on a whole wheat bun, lettuce, tomato, and fries", true, 6.95));

    dinerMenu->addComponent(dessertMenu);
    dessertMenu->addComponent(new MenuItem("Apple Pie", "Apple pie with a flaky crust, topped with vanilla ice cream", true, 1.59));

    Waitress waitress(allMenus);
    waitress.printMenu();

    std::cout << std::endl;

    waitress.printVegetarianMenu();
    std::cout << std::endl;

    waitress.printVegetarianMenu();
    std::cout << std::endl;

    waitress.printVegetarianMenu();
    std::cout << std::endl;

    delete allMenus;

    std::cout << "Exiting\n";

    return 0;
}

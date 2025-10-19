#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

class MenuItem
{
    std::string name;
    std::string description;
    bool vegetarian;
    double price;

public:
    MenuItem() = default;
    MenuItem(const std::string &name, const std::string &description, bool vegetarian, double price)
        : name(name), description(description), vegetarian(vegetarian), price(price) {}

    std::string getName() const { return name; }
    std::string getDescription() const { return description; }
    bool isVegetarian() const { return vegetarian; }
    double getPrice() const { return price; }
};

class Iterator
{
public:
    virtual bool hasNext() = 0;
    virtual void *next() = 0;
    virtual void remove() {}
    virtual ~Iterator() = default;
};

class Menu
{
public:
    virtual Iterator *createIterator() = 0;
};

class PancakeHouseMenuIterator : public Iterator
{
    MenuItem *items;
    int position;
    int maxItems;

public:
    PancakeHouseMenuIterator(MenuItem *items, int maxItems) : items(items),
                                                              position(0),
                                                              maxItems(maxItems) {}
    bool hasNext() override
    {
        return position < maxItems && !items[position].getName().empty();
    }

    void *next() override
    {
        return &items[position++];
    }
};

class PancakeHouseMenu : public Menu
{
    static const int MAX_ITEMS = 6;
    MenuItem menuItems[MAX_ITEMS];
    int numberOfItems;

public:
    PancakeHouseMenu() : numberOfItems(0)
    {
        addItem("K&B's Pancake Breakfast", "Pancakes with scrambled eggs, and toast", true, 2.99);
        addItem("Regular Pancake Breakfast", "Pancakes with fried eggs, sausage", false, 2.99);
        addItem("Blueberry Pancakes", "Pancakes made with fresh blueberries", true, 3.49);
        addItem("Waffles", "Waffles with your choice of blueberries or strawberries", true, 3.59);
    }

    void addItem(const std::string &name, const std::string &description, bool vegetarian, double price)
    {
        if (numberOfItems >= MAX_ITEMS)
        {
            std::cout << "Menu is full! Cannot add item to menu." << std::endl;
            return;
        }
        menuItems[numberOfItems++] = std::move(MenuItem(name, description, vegetarian, price));
    }

    Iterator *createIterator()
    {
        return new PancakeHouseMenuIterator(menuItems, numberOfItems);
    }

    int getNumberOfItems() const { return numberOfItems; }
};

class DinerMenuIterator : public Iterator
{
    std::vector<MenuItem> &items;
    size_t position;

public:
    DinerMenuIterator(std::vector<MenuItem> &items) : items(items), position(0) {}

    bool hasNext() override
    {
        return position < items.size();
    }

    void *next() override
    {
        return const_cast<MenuItem *>(&items[position++]);
    }
};

class DinerMenu : public Menu
{
    std::vector<MenuItem> menuItems;

public:
    DinerMenu()
    {
        addItem("Vegetarian BLT", "Fakin' Bacon with lettuce & tomato on whole wheat", true, 2.99);
        addItem("BLT", "Bacon with lettuce & tomato on whole wheat", false, 2.99);
        addItem("Soup of the day", "Soup of the day, with a side of potato salad", false, 3.29);
        addItem("Hotdog", "A hot dog, with sauerkraut, relish, onions, topped with cheese", false, 3.05);
    }

    void addItem(const std::string &name, const std::string &description, bool vegetarian, double price)
    {
        menuItems.emplace_back(name, description, vegetarian, price);
    }

    Iterator *createIterator()
    {
        return new DinerMenuIterator(menuItems);
    }

    int getNumberOfItems() const { return static_cast<int>(menuItems.size()); }
};

class CafeMenuIterator : public Iterator
{
    std::unordered_map<std::string, MenuItem> itemsMap;
    std::unordered_map<std::string, MenuItem>::iterator position;

public:
    CafeMenuIterator(const std::unordered_map<std::string, MenuItem> &menuMap) : itemsMap(menuMap)
    {
        position = itemsMap.begin();
    }

    bool hasNext() override
    {
        return position != itemsMap.end();
    }

    void *next() override
    {
        return &((position++)->second);
    }
};

class CafeMenu : public Menu
{
    std::unordered_map<std::string, MenuItem> menuItems;

public:
    CafeMenu()
    {
        addItem("Cappuccino", "Espresso with steamed milk foam", true, 3.50);
        addItem("Espresso", "Strong black coffee", true,
                2.75);
        addItem("Turkey Sandwich", "Turkey sandwich with lettuce and tomato", false, 5.25);
    }

    void addItem(const std::string &name, const std::string &description, bool vegetarian, double price)
    {
        menuItems.emplace(name, MenuItem(name, description, vegetarian, price));
    }

    Iterator *createIterator() override
    {
        return new CafeMenuIterator(menuItems);
    }
};

class Waitress
{
    std::unordered_map<std::string, Menu *> menu_by_name;

    void printMenu(Iterator &iterator)
    {
        while (iterator.hasNext())
        {
            MenuItem *item = static_cast<MenuItem *>(iterator.next());
            std::cout << item->getName() << ", " << item->getPrice() << " -- " << item->getDescription() << std::endl;
        }
    }

    bool isVegetarian(Iterator &iterator, const std::string &itemName)
    {
        while (iterator.hasNext())
        {
            MenuItem *item = static_cast<MenuItem *>(iterator.next());
            if (item->getName() == itemName)
            {
                return item->isVegetarian();
            }
        }
        return false;
    }

public:
    Waitress(std::unordered_map<std::string, Menu *> menus)
        : menu_by_name(std::move(menus)) {}

    void printMenu()
    {
        for (const auto &[name, menu] : menu_by_name)
        {
            std::cout << name << " Menu:" << std::endl;
            std::unique_ptr<Iterator> iterator(menu->createIterator());
            printMenu(*iterator);
            std::cout << std::endl;
        }
    }

    void printVegetarianMenu()
    {
        std::cout << "Vegetarian Menu:" << std::endl;
        for (const auto &[name, menu] : menu_by_name)
        {
            std::unique_ptr<Iterator> iterator(menu->createIterator());
            while (iterator->hasNext())
            {
                MenuItem *item = static_cast<MenuItem *>(iterator->next());
                if (item->isVegetarian())
                {
                    std::cout << item->getName() << ", " << item->getPrice() << " -- " << item->getDescription() << std::endl;
                }
            }
        }
    }

    void printBreakfastMenu()
    {
        std::cout << "Breakfast Menu:" << std::endl;
        std::unique_ptr<Iterator> pancakeIterator(menu_by_name["Pancake House"]->createIterator());
        printMenu(*pancakeIterator);
    }

    void printLunchMenu()
    {
        std::cout << "Lunch Menu:" << std::endl;
        std::unique_ptr<Iterator> cafeIterator(menu_by_name["Cafe"]->createIterator());
        printMenu(*cafeIterator);
    }

    void printDinnerMenu()
    {
        std::cout << "Dinner Menu:" << std::endl;
        std::unique_ptr<Iterator> dinerIterator(menu_by_name["Diner"]->createIterator());
        printMenu(*dinerIterator);
    }

    bool isItemVegetarian(const std::string &name)
    {
        for (const auto &[menuName, menu] : menu_by_name)
        {
            std::unique_ptr<Iterator> iterator(menu->createIterator());
            if (isVegetarian(*iterator, name))
            {
                return true;
            }
        }
        return false;
    }
};

int main()
{
    PancakeHouseMenu *pancakeHouseMenu = new PancakeHouseMenu();
    DinerMenu *dinerMenu = new DinerMenu();
    CafeMenu *cafeMenu = new CafeMenu();
    Waitress *waitress = new Waitress({{"Pancake House", pancakeHouseMenu},
                                       {"Diner", dinerMenu},
                                       {"Cafe", cafeMenu}});

    waitress->printMenu();
    std::cout << std::endl;

    waitress->printVegetarianMenu();
    std::cout << std::endl;

    std::string itemName = "Vegetarian BLT";
    std::cout << itemName << " is "
              << (waitress->isItemVegetarian(itemName) ? "vegetarian." : "not vegetarian.") << std::endl;

    delete waitress;
    delete dinerMenu;
    delete pancakeHouseMenu;
    delete cafeMenu;

    return 0;
}

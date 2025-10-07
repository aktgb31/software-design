#include <iostream>
#include <string>
#include <vector>
#include <memory>

class Dough
{
public:
    Dough(const std::string &type) : _type(type) {}
    std::string GetType() const { return _type; }

protected:
    std::string _type;
};

class Sauce
{
public:
    Sauce(const std::string &type) : _type(type) {}
    std::string GetType() const { return _type; }

protected:
    std::string _type;
};

class Topping
{
public:
    Topping(const std::string &name) : _name(name) {}
    std::string GetName() const { return _name; }

protected:
    std::string _name;
};

class Cheese
{
public:
    Cheese(const std::string &type) : _type(type) {}
    std::string GetType() const { return _type; }

protected:
    std::string _type;
};

class Pepperoni
{
public:
    Pepperoni(const std::string &type) : _type(type) {}
    std::string GetType() const { return _type; }

protected:
    std::string _type;
};

class Clams
{
public:
    Clams(const std::string &type) : _type(type) {}
    std::string GetType() const { return _type; }

protected:
    std::string _type;
};

class Veggies
{
public:
    Veggies(const std::string &name) : _name(name) {}
    std::string GetName() const { return _name; }

protected:
    std::string _name;
};

class Pizza
{
public:
    virtual void Prepare() = 0;

    void Bake()
    {
        std::cout << "Baking " << _name << std::endl;
    }
    void Cut()
    {
        std::cout << "Cutting diagonally " << _name << std::endl;
    }
    void Box()
    {
        std::cout << "Boxing " << _name << std::endl;
    }
    std::string GetName() const { return _name; }

    void SetName(const std::string &name) { _name = name; }

    std::string toString() const
    {
        std::string result = "---- " + _name + " ----\n";
        if (_dough)
            result += "Dough: " + _dough->GetType() + "\n";
        if (_sauce)
            result += "Sauce: " + _sauce->GetType() + "\n";
        if (_cheese)
            result += "Cheese: " + _cheese->GetType() + "\n";
        if (!_veggies.empty())
        {
            result += "Veggies: ";
            for (const auto &veggie : _veggies)
            {
                result += veggie->GetName() + " ";
            }
            result += "\n";
        }
        if (_pepperoni)
            result += "Pepperoni: " + _pepperoni->GetType() + "\n";
        if (_clam)
            result += "Clams: " + _clam->GetType() + "\n";
        return result;
    }

    virtual ~Pizza()
    {
        if (_dough)
            delete _dough;
        if (_sauce)
            delete _sauce;
        if (_cheese)
            delete _cheese;
        for (auto &veggie : _veggies)
        {
            if (veggie)
                delete veggie;
        }
        if (_pepperoni)
            delete _pepperoni;
        if (_clam)
            delete _clam;
    }

protected:
    std::string _name;
    Dough *_dough = nullptr;
    Sauce *_sauce = nullptr;
    Cheese *_cheese = nullptr;
    std::vector<Veggies *> _veggies;
    Pepperoni *_pepperoni = nullptr;
    Clams *_clam = nullptr;
};

class PizzaIngredientFactory
{
public:
    virtual ~PizzaIngredientFactory() = default;
    virtual Dough *CreateDough() = 0;
    virtual Sauce *CreateSauce() = 0;
    virtual Cheese *CreateCheese() = 0;
    virtual std::vector<Veggies *> CreateVeggies() = 0;
    virtual Pepperoni *CreatePepperoni() = 0;
    virtual Clams *CreateClam() = 0;
};

class ThinCrustDough : public Dough
{
public:
    ThinCrustDough() : Dough("Thin Crust Dough") {}
};

class ThickCrustDough : public Dough
{
public:
    ThickCrustDough() : Dough("Thick Crust Dough") {}
};

class PlumTomatoSauce : public Sauce
{
public:
    PlumTomatoSauce() : Sauce("Plum Tomato Sauce") {}
};

class MarinaraSauce : public Sauce
{
public:
    MarinaraSauce() : Sauce("Marinara Sauce") {}
};

class BruschettaSauce : public Sauce
{
public:
    BruschettaSauce() : Sauce("Bruschetta Sauce") {}
};

class MozzarellaCheese : public Cheese
{
public:
    MozzarellaCheese() : Cheese("Mozzarella Cheese") {}
};

class ReggianoCheese : public Cheese
{
public:
    ReggianoCheese() : Cheese("Reggiano Cheese") {}
};

class GoatCheese : public Cheese
{
public:
    GoatCheese() : Cheese("Goat Cheese") {}
};

class SlicedPepperoni : public Pepperoni
{
public:
    SlicedPepperoni() : Pepperoni("Sliced Pepperoni") {}
};

class FreshClams : public Clams
{
public:
    FreshClams() : Clams("Fresh Clams") {}
};

class FrozenClams : public Clams
{
public:
    FrozenClams() : Clams("Frozen Clams") {}
};

class Garlic : public Veggies
{
public:
    Garlic() : Veggies("Garlic") {}
};

class Onion : public Veggies
{
public:
    Onion() : Veggies("Onion") {}
};

class Mushroom : public Veggies
{
public:
    Mushroom() : Veggies("Mushroom") {}
};

class RedPepper : public Veggies
{
public:
    RedPepper() : Veggies("Red Pepper") {}
};

class CheezePizza : public Pizza
{
public:
    CheezePizza(PizzaIngredientFactory *ingredientFactory) : _ingredientFactory(ingredientFactory) {}
    void Prepare() override
    {
        std::cout << "Preparing " << _name << std::endl;
        _dough = _ingredientFactory->CreateDough();
        _sauce = _ingredientFactory->CreateSauce();
        _cheese = _ingredientFactory->CreateCheese();
    }

    ~CheezePizza()
    {
        delete _ingredientFactory;
    }

protected:
    PizzaIngredientFactory *_ingredientFactory;
};

class ClamPizza : public Pizza
{
public:
    ClamPizza(PizzaIngredientFactory *ingredientFactory) : _ingredientFactory(ingredientFactory) {}
    void Prepare() override
    {
        std::cout << "Preparing " << _name << std::endl;
        _dough = _ingredientFactory->CreateDough();
        _sauce = _ingredientFactory->CreateSauce();
        _cheese = _ingredientFactory->CreateCheese();
        _clam = _ingredientFactory->CreateClam();
    }

    ~ClamPizza()
    {
        delete _ingredientFactory;
    }

protected:
    PizzaIngredientFactory *_ingredientFactory;
};

class VeggiePizza : public Pizza
{
public:
    VeggiePizza(PizzaIngredientFactory *ingredientFactory) : _ingredientFactory(ingredientFactory) {}
    void Prepare() override
    {
        std::cout << "Preparing " << _name << std::endl;
        _dough = _ingredientFactory->CreateDough();
        _sauce = _ingredientFactory->CreateSauce();
        _cheese = _ingredientFactory->CreateCheese();
        _veggies = _ingredientFactory->CreateVeggies();
    }

    ~VeggiePizza()
    {
        delete _ingredientFactory;
    }

protected:
    PizzaIngredientFactory *_ingredientFactory;
};

class PizzaStore
{
public:
    PizzaStore() = default;
    virtual ~PizzaStore() = default;

    Pizza *OrderPizza(const std::string &type)
    {
        Pizza *pizza = CreatePizza(type);
        if (pizza)
        {
            pizza->Prepare();
            pizza->Bake();
            pizza->Cut();
            pizza->Box();
        }
        return pizza;
    }

protected:
    virtual Pizza *CreatePizza(const std::string &type) = 0;
};

class NyPizzaIngredientFactory : public PizzaIngredientFactory
{
public:
    Dough *CreateDough() override
    {
        return new ThinCrustDough();
    }
    Sauce *CreateSauce() override
    {
        return new MarinaraSauce();
    }
    Cheese *CreateCheese() override
    {
        return new ReggianoCheese();
    }
    std::vector<Veggies *> CreateVeggies() override
    {
        return {new Garlic(), new Onion(), new Mushroom(), new RedPepper()};
    }
    Pepperoni *CreatePepperoni() override
    {
        return new SlicedPepperoni();
    }
    Clams *CreateClam() override
    {
        return new FreshClams();
    }
};

class ChicagoPizzaIngredientFactory : public PizzaIngredientFactory
{
public:
    Dough *CreateDough() override
    {
        return new ThickCrustDough();
    }
    Sauce *CreateSauce() override
    {
        return new PlumTomatoSauce();
    }
    Cheese *CreateCheese() override
    {
        return new MozzarellaCheese();
    }
    std::vector<Veggies *> CreateVeggies() override
    {
        return {new Garlic(), new Onion(), new Mushroom(), new RedPepper()};
    }
    Pepperoni *CreatePepperoni() override
    {
        return new SlicedPepperoni();
    }
    Clams *CreateClam() override
    {
        return new FrozenClams();
    }
};

class CaliforniaPizzaIngredientFactory : public PizzaIngredientFactory
{
public:
    Dough *CreateDough() override
    {
        return new ThinCrustDough();
    }
    Sauce *CreateSauce() override
    {
        return new BruschettaSauce();
    }
    Cheese *CreateCheese() override
    {
        return new GoatCheese();
    }
    std::vector<Veggies *> CreateVeggies() override
    {
        return {new Garlic(), new Onion(), new Mushroom(), new RedPepper()};
    }
    Pepperoni *CreatePepperoni() override
    {
        return new SlicedPepperoni();
    }
    Clams *CreateClam() override
    {
        return new FreshClams();
    }
};

class NYPizzaStore : public PizzaStore
{
public:
    NYPizzaStore() = default;
    ~NYPizzaStore() override = default;

protected:
    Pizza *CreatePizza(const std::string &type) override
    {
        PizzaIngredientFactory *ingredientFactory = new NyPizzaIngredientFactory();
        if (type == "cheese")
        {
            Pizza *pizza = new CheezePizza(ingredientFactory);
            pizza->SetName("New York Style Cheese Pizza");
            return pizza;
        }
        else if (type == "clam")
        {
            Pizza *pizza = new ClamPizza(ingredientFactory);
            pizza->SetName("New York Style Clam Pizza");
            return pizza;
        }
        else if (type == "veggie")
        {
            Pizza *pizza = new VeggiePizza(ingredientFactory);
            pizza->SetName("New York Style Veggie Pizza");
            return pizza;
        }
        else
        {
            return nullptr;
        }
    }
};

class ChicagoPizzaStore : public PizzaStore
{
public:
    ChicagoPizzaStore() = default;
    ~ChicagoPizzaStore() override = default;

protected:
    Pizza *CreatePizza(const std::string &type) override
    {
        PizzaIngredientFactory *ingredientFactory = new ChicagoPizzaIngredientFactory();
        if (type == "cheese")
        {
            Pizza *pizza = new CheezePizza(ingredientFactory);
            pizza->SetName("Chicago Style Cheese Pizza");
            return pizza;
        }
        else if (type == "pepperoni")
        {
            Pizza *pizza = new ClamPizza(ingredientFactory);
            pizza->SetName("Chicago Style Pepperoni Pizza");
            return pizza;
        }
        else
        {
            return nullptr;
        }
    }
};

class CaliforniaPizzaStore : public PizzaStore
{
public:
    CaliforniaPizzaStore() = default;
    ~CaliforniaPizzaStore() override = default;

protected:
    Pizza *CreatePizza(const std::string &type) override
    {
        PizzaIngredientFactory *ingredientFactory = new CaliforniaPizzaIngredientFactory();
        if (type == "cheese")
        {
            Pizza *pizza = new CheezePizza(ingredientFactory);
            pizza->SetName("California Style Cheese Pizza");
            return pizza;
        }
        else if (type == "pepperoni")
        {
            Pizza *pizza = new ClamPizza(ingredientFactory);
            pizza->SetName("California Style Pepperoni Pizza");
            return pizza;
        }
        else
        {
            return nullptr;
        }
    }
};

int main()
{
    PizzaStore *nyStore = new NYPizzaStore();
    PizzaStore *chicagoStore = new ChicagoPizzaStore();
    PizzaStore *californiaStore = new CaliforniaPizzaStore();

    Pizza *pizza = nyStore->OrderPizza("cheese");
    std::cout << "Ethan ordered a " << pizza->GetName() << "\n\n";
    delete pizza;

    pizza = chicagoStore->OrderPizza("cheese");
    std::cout << "Joel ordered a " << pizza->GetName() << "\n\n";
    delete pizza;

    pizza = californiaStore->OrderPizza("pepperoni");
    std::cout << "Alex ordered a " << pizza->GetName() << "\n\n";
    delete pizza;

    delete nyStore;
    delete chicagoStore;
    delete californiaStore;

    return 0;
}

#include <iostream>
#include <string>
#include <memory>

enum Size { UNKNOWN,TALL, GRANDE, VENTI };

class Beverage {
public:
    virtual std::string getDescription() const {
        return description;
    }
    virtual double cost() = 0; // Pure virtual function
    virtual ~Beverage() = default; // Virtual destructor

    virtual void setSize(Size size) {
        this->size = size;
    }
    virtual Size getSize() const {
        return size;
    }

protected:
    std::string description = "Unknown Beverage";
private:
    enum Size size = UNKNOWN;
};  

class CondimentDecorator : public Beverage {
public:
    virtual std::string getDescription() const override = 0; // Pure virtual function
};

class Espresso : public Beverage {
public:
    Espresso() {
        description = "Espresso";
    }
    double cost() override {
        return 1.99;       
    }
};

class HouseBlend : public Beverage {
public:
    HouseBlend() {
        description = "House Blend Coffee";
    }
    double cost() override {
        return 0.89;
    }
};

class Decaf : public Beverage {
public:
    Decaf() {
        description = "Decaf Coffee";
    }
    double cost() override {
        return 1.05;
    }
};

class DarkRoast : public Beverage {
public:
    DarkRoast() {
        description = "Dark Roast Coffee";
    }
    double cost() override {
        return 0.99;
    }
};

class Mocha : public CondimentDecorator {
public:
    Mocha(std::unique_ptr<Beverage> beverage) : beverage_(std::move(beverage)) {}
    std::string getDescription() const override {
        return beverage_->getDescription() + ", Mocha";
    }
    double cost() override {
        return 0.20 + beverage_->cost();
    }
private:
    std::unique_ptr<Beverage> beverage_;
};

class Soy : public CondimentDecorator {
public:
    Soy(std::unique_ptr<Beverage> beverage) : beverage_(std::move(beverage)) {}
    std::string getDescription() const override {     
        return beverage_->getDescription() + ", Soy";
    }
    double cost() override {
        double soyCost = 0.0;
        switch (beverage_->getSize())
        {
        case TALL:
            soyCost = 0.10;
            break;
        case GRANDE:
            soyCost = 0.15;
            break;
        case VENTI:
            soyCost = 0.20;
            break;
        case UNKNOWN:
        default:
            soyCost = 0.0;
            break;
        }
        return soyCost + beverage_->cost();
    }
private:
    std::unique_ptr<Beverage> beverage_;
};

class Whip : public CondimentDecorator {
public:
    Whip(std::unique_ptr<Beverage> beverage) : beverage_(std::move(beverage)) {}
    std::string getDescription() const override {
        return beverage_->getDescription() + ", Whip";
    }
    double cost() override {
        return 0.10 + beverage_->cost();
    }   
private:
    std::unique_ptr<Beverage> beverage_;
};

class SteamedMilk : public CondimentDecorator {
public:
    SteamedMilk(std::unique_ptr<Beverage> beverage) : beverage_(std::move(beverage)) {}
    std::string getDescription() const override {
        return beverage_->getDescription() + ", Steamed Milk";
    }
    double cost() override {    
        return 0.10 + beverage_->cost();
    }
private:
    std::unique_ptr<Beverage> beverage_;
};

int main() {
    std::unique_ptr<Beverage> beverage1 = std::make_unique<Espresso>();
    std::cout << beverage1->getDescription() << " $" << beverage1->cost() << std::endl;

    std::unique_ptr<Beverage> beverage2 = std::make_unique<DarkRoast>();
    beverage2 = std::make_unique<Mocha>(std::move(beverage2));
    beverage2 = std::make_unique<Mocha>(std::move(beverage2));
    beverage2 = std::make_unique<Whip>(std::move(beverage2));
    std::cout << beverage2->getDescription() << " $" << beverage2->cost() << std::endl;

    std::unique_ptr<Beverage> beverage3 = std::make_unique<HouseBlend>();
    beverage3->setSize(VENTI);
    beverage3 = std::make_unique<Soy>(std::move(beverage3));
    beverage3 = std::make_unique<Mocha>(std::move(beverage3));
    beverage3 = std::make_unique<Whip>(std::move(beverage3));
    std::cout << beverage3->getDescription() << " $" << beverage3->cost () << std::endl;

    return 0;
}
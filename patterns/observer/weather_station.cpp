#include<iostream>
#include<vector>
#include<algorithm>

class Observer {
public:
    virtual void update(float temperature, float humidity, float pressure) = 0;
};

class Subject {
public:
    virtual void registerObserver(Observer* o) = 0;
    virtual void removeObserver(Observer* o) = 0;
    virtual void notifyObservers() = 0;
};

class DisplayElement {
public:
    virtual void display() = 0;
};

class WeatherData : public Subject {

public:
    void registerObserver(Observer* o) override {
        observers.push_back(o);
    }

    void removeObserver(Observer* o) override {
        observers.erase(std::remove(observers.begin(), observers.end(), o), observers.end()); 
    }

    void notifyObservers() override {
        for (Observer* observer : observers) {
            observer->update(temperature, humidity, pressure);
        }
    }

    void measurementsChanged() {
        notifyObservers();
    }

    void setMeasurements(float temp, float hum, float pres) {
        temperature = temp;
        humidity = hum;
        pressure = pres;
        measurementsChanged();
    }

private:
    std::vector<Observer*> observers;
    float temperature;
    float humidity;
    float pressure;
};

class CurrentConditionsDisplay : public Observer, public DisplayElement {

public:
    CurrentConditionsDisplay(Subject& weatherData) : weatherData(weatherData) {
        weatherData.registerObserver(this);
    }

    void update(float temperature, float humidity, float pressure) override {
        this->temperature = temperature;
        this->humidity = humidity;
        display();
    }

    void display() override {
        std::cout << "Current conditions: " << temperature << "C degrees and " << humidity << "% humidity" << std::endl;
    }

    ~CurrentConditionsDisplay() {
        weatherData.removeObserver(this);
    }

private:
    float temperature;
    float humidity;
    Subject& weatherData;
};


class StatisticsDisplay : public Observer, public DisplayElement {
public:
    StatisticsDisplay(Subject& weatherData) : weatherData(weatherData), maxTemp(-1000), minTemp(1000), tempSum(0), numReadings(0) {
        weatherData.registerObserver(this);
    }

    void update(float temperature, float humidity, float pressure) override {
        tempSum += temperature;
        numReadings++;

        if (temperature > maxTemp) {
            maxTemp = temperature;
        }

        if (temperature < minTemp) {
            minTemp = temperature;
        }

        display();
    }

    void display() override {
        std::cout << "Avg/Max/Min temperature = " << (tempSum / numReadings) << "/" << maxTemp << "/" << minTemp << std::endl;
    }

    ~StatisticsDisplay() {
        weatherData.removeObserver(this);
    }

private:
    float maxTemp;
    float minTemp;
    float tempSum;
    int numReadings;
    Subject& weatherData;
};

class ForecastDisplay : public Observer, public DisplayElement {
public:
    ForecastDisplay(Subject& weatherData) : weatherData(weatherData) {
        weatherData.registerObserver(this);
    }
    void update(float temperature, float humidity, float pressure) override {
        lastPressure = currentPressure;
        currentPressure = pressure;

        display();
    }
    void display() override {
        std::cout << "Forecast: ";
        if (currentPressure > lastPressure) {
            std::cout << "Improving weather on the way!" << std::endl;
        } else if (currentPressure == lastPressure) {
            std::cout << "More of the same" << std::endl;
        } else if (currentPressure < lastPressure) {
            std::cout << "Watch out for cooler, rainy weather" << std::endl;
        }
    }
    ~ForecastDisplay() {
        weatherData.removeObserver(this);
    }
private:
    float currentPressure = 29.92f;
    float lastPressure;
    Subject& weatherData;
};

class HeatIndexDisplay : public Observer, public DisplayElement {
public:
    HeatIndexDisplay(Subject& weatherData) : weatherData(weatherData) {
        weatherData.registerObserver(this);
    }
    void update(float temperature, float humidity, float pressure) override {
        heatIndex = computeHeatIndex(temperature, humidity);
        display();
    }
    void display() override {
        std::cout << "Heat index is " << heatIndex << std::endl;
    }
    ~HeatIndexDisplay() {
        weatherData.removeObserver(this);
    }
private:
    float heatIndex;
    Subject& weatherData;
    float computeHeatIndex(float t, float rh) {
        return 16.923 + (0.185212 * t) + (5.37941 * rh) - (0.100254 * t * rh) +
               (0.00941695 * (t * t)) + (0.00728898 * (rh * rh)) +
               (0.000345372 * (t * t * rh)) - (0.000814971 * (t * rh * rh)) +
               (0.0000102102 * (t * t * rh * rh)) - (0.000038646 * (t * t * t)) +
               (0.0000291583 * (rh * rh * rh)) + (0.00000142721 * (t * t * t * rh)) +
               (0.000000197483 * (t * rh * rh * rh)) - (0.0000000218429 * (t * t * t * rh * rh)) +
               (0.000000000843296 * (t * t * rh * rh * rh)) -
               (0.0000000000481975 * (t * t * t * rh * rh * rh));
    }
};

int main() {
    WeatherData weatherData;

    CurrentConditionsDisplay currentDisplay(weatherData);
    StatisticsDisplay statisticsDisplay(weatherData);
    ForecastDisplay forecastDisplay(weatherData);
    HeatIndexDisplay heatIndexDisplay(weatherData);
    
    weatherData.setMeasurements(80, 65, 30.4f);
    weatherData.setMeasurements(82, 70, 29.2f);
    weatherData.setMeasurements(78, 90, 29.2f);
    
    return 0;
}
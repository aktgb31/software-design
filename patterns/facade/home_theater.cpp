#include <iostream>
#include <string>

class Amplifier
{
public:
    void on() { std::cout << "Amplifier on\n"; }
    void off() { std::cout << "Amplifier off\n"; }
    void setVolume(int level) { std::cout << "Amplifier volume set to "
                                          << level << "\n"; }
};

class Tuner
{
public:
    void on() { std::cout << "Tuner on\n"; }
    void off() { std::cout << "Tuner off\n"; }
    void setFrequency(double frequency)
    {
        std::cout << "Tuner frequency set to " << frequency << " MHz\n";
    }
};

class DVDPlayer
{
public:
    void on() { std::cout << "DVD Player on\n"; }
    void off() { std::cout << "DVD Player off\n"; }
    void play(const std::string &movie)
    {
        std::cout << "Playing movie: " << movie << "\n";
    }
    void stop() { std::cout << "DVD Player stopped\n"; }
};

class Projector
{
public:
    void on() { std::cout << "Projector on\n"; }
    void off() { std::cout << "Projector off\n"; }
    void wideScreenMode() { std::cout << "Projector in widescreen mode\n"; }
};

class HomeTheaterFacade
{
private:
    Amplifier *amp;
    Tuner *tuner;
    DVDPlayer *dvd;
    Projector *projector;

public:
    HomeTheaterFacade(Amplifier *a, Tuner *t, DVDPlayer *d, Projector *p)
        : amp(a), tuner(t), dvd(d), projector(p) {}

    void watchMovie(const std::string &movie)
    {
        std::cout << "Get ready to watch a movie...\n";
        amp->on();
        amp->setVolume(5);
        projector->on();
        projector->wideScreenMode();
        dvd->on();
        dvd->play(movie);
    }

    void endMovie()
    {
        std::cout << "Shutting movie theater down...\n";
        dvd->stop();
        dvd->off();
        projector->off();
        amp->off();
    }
};

int main()
{
    Amplifier amp;
    Tuner tuner;
    DVDPlayer dvd;
    Projector projector;

    HomeTheaterFacade homeTheater(&amp, &tuner, &dvd, &projector);

    homeTheater.watchMovie("Inception");
    std::cout << "\n";
    homeTheater.endMovie();

    return 0;
}

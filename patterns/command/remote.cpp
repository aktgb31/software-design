#include <iostream>
#include <string>
#include <memory>

class Command
{
public:
    virtual void execute() = 0;
    virtual void undo() = 0;
};

class Light
{
public:
    void on()
    {
        std::cout << "Light is ON" << std::endl;
    }
    void off()
    {
        std::cout << "Light is OFF" << std::endl;
    }
};

class CeilingFan
{
public:
    void high()
    {
        std::cout << "Ceiling Fan is on High" << std::endl;
    }

    void medium()
    {
        std::cout << "Ceiling Fan is on Medium" << std::endl;
    }

    void low()
    {
        std::cout << "Ceiling Fan is on Low" << std::endl;
    }

    void off()
    {
        std::cout << "Ceiling Fan is OFF" << std::endl;
    }
};

class Stereo
{
public:
    void on()
    {
        std::cout << "Stereo is ON" << std::endl;
    }
    void off()
    {
        std::cout << "Stereo is OFF" << std::endl;
    }
    void setCD()
    {
        std::cout << "Stereo is set for CD input" << std::endl;
    }
    void setVolume(int volume)
    {
        std::cout << "Stereo volume set to " << volume << std::endl;
    }
};

class GarageDoor
{
public:
    void up()
    {
        std::cout << "Garage Door is Open" << std::endl;
    }

    void down()
    {
        std::cout << "Garage Door is Closed" << std::endl;
    }
};

class LightOnCommand : public Command
{
public:
    LightOnCommand(Light &light) : light_(light) {}
    void execute() override
    {
        light_.on();
    }

    void undo() override
    {
        light_.off();
    }

private:
    Light &light_;
};

class LightOffCommand : public Command
{
public:
    LightOffCommand(Light &light) : light_(light) {}
    void execute() override
    {
        light_.off();
    }

    void undo() override
    {
        light_.on();
    }

private:
    Light &light_;
};

class CeilingFanHighCommand : public Command
{
public:
    CeilingFanHighCommand(CeilingFan &ceilingFan) : ceilingFan_(ceilingFan) {}
    void execute() override
    {
        ceilingFan_.high();
    }

    void undo() override
    {
        ceilingFan_.off();
    }

private:
    CeilingFan &ceilingFan_;
};

class CeilingFanOffCommand : public Command
{
public:
    CeilingFanOffCommand(CeilingFan &ceilingFan) : ceilingFan_(ceilingFan) {}
    void execute() override
    {
        ceilingFan_.off();
    }

    void undo() override
    {
        ceilingFan_.high();
    }

private:
    CeilingFan &ceilingFan_;
};

class GarageDoorUpCommand : public Command
{
public:
    GarageDoorUpCommand(GarageDoor &garageDoor) : garageDoor_(garageDoor) {}
    void execute() override
    {
        garageDoor_.up();
    }

    void undo() override
    {
        garageDoor_.down();
    }

private:
    GarageDoor &garageDoor_;
};

class GarageDoorDownCommand : public Command
{
public:
    GarageDoorDownCommand(GarageDoor &garageDoor) : garageDoor_(garageDoor) {}
    void execute() override
    {
        garageDoor_.down();
    }

    void undo() override
    {
        garageDoor_.up();
    }

private:
    GarageDoor &garageDoor_;
};

class StereoOnWithCDCommand : public Command
{
public:
    StereoOnWithCDCommand(Stereo &stereo) : stereo_(stereo)
    {
    }
    void execute() override
    {
        stereo_.on();
        stereo_.setCD();
        stereo_.setVolume(11);
    }

    void undo() override
    {
        stereo_.off();
    }

private:
    Stereo &stereo_;
};

class StereoOffCommand : public Command
{
public:
    StereoOffCommand(Stereo &stereo) : stereo_(stereo) {}
    void execute() override
    {
        stereo_.off();
    }

    void undo() override
    {
        stereo_.on();
        stereo_.setCD();
        stereo_.setVolume(11);
    }

private:
    Stereo &stereo_;
};

class NoCommand : public Command
{
public:
    void execute() override
    {
        // Do nothing
    }

    void undo() override
    {
        // Do nothing
    }
};

class SimpleRemoteControl
{
public:
    SimpleRemoteControl(int size)
    {
        onSlot_ = new Command *[size];
        offSlot_ = new Command *[size];

        for (int i = 0; i < size; ++i)
        {
            onSlot_[i] = &noCommand;
            offSlot_[i] = &noCommand;
        }
        undoCommand_ = &noCommand;
    }

    void setCommand(int i, Command *onCommand, Command *offCommand)
    {
        onSlot_[i] = onCommand;
        offSlot_[i] = offCommand;
    }

    void onButtonWasPressed(int i)
    {
        onSlot_[i]->execute();
        undoCommand_ = onSlot_[i];
    }

    void offButtonWasPressed(int i)
    {

        offSlot_[i]->execute();
        undoCommand_ = offSlot_[i];
    }
    void undoButtonWasPressed()
    {
        undoCommand_->undo();
    }

    ~SimpleRemoteControl()
    {
        delete[] onSlot_;
        delete[] offSlot_;
    }

private:
    Command **onSlot_ = nullptr;
    Command **offSlot_ = nullptr;
    NoCommand noCommand;
    Command *undoCommand_ = nullptr;
};

int main()
{
    SimpleRemoteControl remote(5);
    Light light;
    Stereo stereo;
    GarageDoor garageDoor;
    CeilingFan ceilingFan;

    LightOnCommand lightOn(light);
    LightOffCommand lightOff(light);
    CeilingFanHighCommand ceilingFanHigh(ceilingFan);
    CeilingFanOffCommand ceilingFanOff(ceilingFan);
    GarageDoorUpCommand garageDoorUp(garageDoor);
    GarageDoorDownCommand garageDoorDown(garageDoor);
    StereoOnWithCDCommand stereoOn(stereo);
    StereoOffCommand stereoOff(stereo);

    remote.setCommand(0, &lightOn, &lightOff);
    remote.setCommand(1, &ceilingFanHigh, &ceilingFanOff);
    remote.setCommand(2, &stereoOn, &stereoOff);
    remote.setCommand(3, &garageDoorUp, &garageDoorDown);

    remote.onButtonWasPressed(0);
    remote.offButtonWasPressed(0);
    remote.undoButtonWasPressed();
    remote.onButtonWasPressed(1);
    remote.offButtonWasPressed(1);
    remote.onButtonWasPressed(2);
    remote.offButtonWasPressed(2);
    remote.undoButtonWasPressed();
    remote.onButtonWasPressed(3);
    remote.offButtonWasPressed(3);
    remote.offButtonWasPressed(4); // No command assigned
    return 0;
}
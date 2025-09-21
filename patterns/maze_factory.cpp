// A simple Maze
#include <vector>
#include <iostream>

enum Direction
{
    North,
    South,
    East,
    West
};

// Base Class
class MapSite
{
public:
    virtual void Enter() = 0;
};

class Room : public MapSite
{
public:
    Room(int roomNo) : _roomNumber(roomNo) {};

    MapSite *GetSide(Direction d) const { return _sides[d]; };
    void SetSide(Direction d, MapSite *m) { _sides[d] = m; }

    virtual void Enter() override
    {
        std::cout << "Entered in room " << _roomNumber << "\n";
    }

    int RoomNumber() const
    {
        return _roomNumber;
    }

private:
    MapSite *_sides[4];
    int _roomNumber;
};

class Wall : public MapSite
{
public:
    Wall() {};
    virtual void Enter() override
    {
        std::cout << "Oh No!! Hit a wall\n";
    }
};

class Door : public MapSite
{
public:
    Door(Room *r1 = nullptr, Room *r2 = nullptr) : _room1(r1), _room2(r2) {};
    virtual void Enter() override
    {
        if (_room1 and _room2)
            std::cout << "Entering a door from " << _room1->RoomNumber() << " to " << _room2->RoomNumber() << "\n";
    }
    Room *OtherSideFrom(Room *r)
    {
        return (r == _room1) ? _room2 : _room1;
    }

private:
    Room *_room1;
    Room *_room2;
    bool _isOpen;
};

class Maze
{
public:
    Maze() {};

    void AddRoom(Room *room)
    {
        _rooms.push_back(room);
    };

    Room *RoomNo(int roomNo) const
    {
        for (auto room : _rooms)
            if (room->RoomNumber() == roomNo)
            {
                return room;
            }
        return nullptr;
    };

private:
    std::vector<Room *> _rooms;
};

class MazeFactory
{
public:
    MazeFactory() {};

    virtual Maze *MakeMaze() const
    {
        return new Maze();
    }
    virtual Wall *MakeWall() const
    {
        return new Wall();
    }
    virtual Room *MakeRoom(int n) const
    {
        return new Room(n);
    }
    virtual Door *MakeDoor(Room *rl, Room *r2) const
    {
        return new Door(rl, r2);
    }
};

class MazeGame
{
public:
    Maze *CreateMaze(MazeFactory &factory)
    {
        Maze *aMaze = factory.MakeMaze();
        Room *rl = factory.MakeRoom(1);
        Room *r2 = factory.MakeRoom(2);
        Door *aDoor = factory.MakeDoor(rl, r2);
        aMaze->AddRoom(rl);
        aMaze->AddRoom(r2);
        rl->SetSide(North, factory.MakeWall());
        rl->SetSide(East, aDoor);
        rl->SetSide(South, factory.MakeWall());
        rl->SetSide(West, factory.MakeWall());
        r2->SetSide(North, factory.MakeWall());
        r2->SetSide(East, factory.MakeWall());
        r2->SetSide(South, factory.MakeWall());
        r2->SetSide(West, aDoor);
        return aMaze;
    }
};

int main()
{
    MazeFactory simple_factory;
    MazeGame maze_game = MazeGame();

    Maze *maze = maze_game.CreateMaze(simple_factory);

    maze->RoomNo(1)->Enter();
    maze->RoomNo(1)->GetSide(Direction::East)->Enter();
    maze->RoomNo(2)->GetSide(Direction::East)->Enter();

    return 0;
}
#pragma once

#include <QByteArray>
#include <QDateTime>

#include <string>
#include <unordered_map>

namespace dh {

struct Coordinate
{
    float _latitude = 0.f;
    float _longitude = 0.f;

    static float distanceTo(const Coordinate& oneC, const Coordinate& secondC);
};

enum class Character : int
{
    Passion = 0,
    Friendly,
    Playful
};

enum class Male : int
{
    Man = 0,
    Woman,
    Other
};

class DogOwner;

class Dog
{
public:
    Dog(const std::string &name, const DogOwner &dogOwner)
        : _name(name), _dogOwner(dogOwner) {}

    Dog(const Dog&) = default;
    const Dog &operator=(const Dog&) const = delete;

    std::string _name;
    std::string _breed;
    Male _male = Male::Man;
    float _wheight = 0.;
    int age = 0;
    Character _character = Character::Passion;
    QByteArray _icon; // path to local bitmaps cache (byte array to simplicy)
    bool _sterilized = false;

    const std::string &getName() const { return _name; }
    const DogOwner &getOwner() const { return _dogOwner; }

private:
    const DogOwner &_dogOwner;
};

using DogPtr = std::shared_ptr<Dog>;

class DogOwner
{
public:
    DogOwner() = default;
    DogOwner(const std::string &email, const std::string &password)
        : _email(email), _password(password) {}

    DogOwner(const DogOwner&) = delete;
    const DogOwner &operator=(const DogOwner&) const = delete;

    std::string _name;
    Male _male = Male::Man;
    QByteArray _icon; // path to local bitmaps cache (byte array to simplicy)

    std::string _email;
    std::string _password;
    std::unordered_map<std::string, DogPtr> _dogs;

    const std::string &getEmail() const { return _email; };
    const std::string & getPassword() const { return _password; };

    DogPtr getDog(const std::string &name) const;
    DogPtr addDog(const std::string &name);
    const std::unordered_map<std::string, DogPtr> &getDogs() const { return _dogs; };
};

using DogOwnerPtr = std::shared_ptr<DogOwner>;

class Walk
{
public:
    Walk(const std::string &dogOwnerId, const std::string &dogName)
        : _dogOwnerId(dogOwnerId) , _dogName(dogName) {}

    QDateTime _date;
    Coordinate _coordinate;
    std::string _dogOwnerId;
    std::string _dogName;
};

class ModelData
{
public:

    static ModelData getDummyModelData(); // for tests

    std::vector<Walk> getWalks(const Coordinate &coorinate, float radius, const QDateTime &date) const;

    std::unordered_map<std::string, DogOwnerPtr> _dogOwners;
    std::vector<Walk> _walks;
    std::string _superUser;
};

} // namespace dh

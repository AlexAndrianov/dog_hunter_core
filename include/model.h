#include <QDateTime>

#include <string>
#include <unordered_map>

namespace dh {

struct Coordinate
{
    float _latitude = 0.f;
    float _longitude = 0.f;
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

    std::string _breed;
    Male _male = Male::Man;
    float _wheight = 0.;
    int age = 0;
    Character _character = Character::Passion;
    std::string _icon; // path to local bitmaps cache
    bool _sterilized = false;

    const std::string &getName() const { return _name; }
    const DogOwner &getOwner() const { return _dogOwner; }

private:
    std::string _name;
    const DogOwner &_dogOwner;
};

using DogPtr = std::shared_ptr<Dog>;


class DogOwner
{
public:
    DogOwner(const DogOwner&) = delete;
    const DogOwner &operator=(const DogOwner&) const = delete;

    std::string _name;
    Male _male = Male::Man;
    std::string _icon; // path to local bitmaps cache

    const std::string &getEmail() const { return _email; };
    const std::string & getPassword() const { return _password; };

    DogPtr getDog(const std::string &name) const;
    DogPtr addDog(const std::string &name);

private:
    DogOwner(const std::string &email, const std::string &password)
        : _email(email), _password(password) {}

    std::string _email;
    std::string _password;
    std::unordered_map<std::string, DogPtr> _dogs;
};

using DogOwnerPtr = std::shared_ptr<DogOwner>;

class Walk
{
public:
    Walk(const std::string &dogOwnerId, const std::string &dogName)
        : _dogOwnerId(dogOwnerId) , _dogName(dogName) {}

    QDateTime _date;
    Coordinate _coordinate;
private:
    std::string _dogOwnerId;
    std::string _dogName;
};

class ModelData
{
public:

    std::unordered_map<std::string, DogOwnerPtr> _dogOwners;
    std::vector<Walk> _walks;
};

} // namespace dh

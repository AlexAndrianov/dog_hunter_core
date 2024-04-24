#include "model.h"

namespace dh {

DogPtr DogOwner::getDog(const std::string &name) const
{
    auto it = _dogs.find(name);
    if(it == _dogs.end())
        return nullptr;

    return it->second;
}

DogPtr DogOwner::addDog(const std::string &name)
{
    auto res = _dogs.emplace(name, std::make_shared<Dog>(name, *this));
    return res.first->second;
}
}

#include "model.h"
#include <ranges>

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

ModelData ModelData::getDummyModelData()
{
    ModelData modelData;

    auto prepareDummyData = [&modelData](auto name, auto email,
                                         auto password, auto dogName, auto dogAge, auto coordinate){
        auto dogOwner = std::make_shared<DogOwner>();
        dogOwner->_name = name;
        dogOwner->_email = email;
        dogOwner->_password = password;

        auto dog = std::make_shared<Dog>(dogName, *dogOwner);
        dog->age = dogAge;
        dogOwner->_dogs.emplace(dogName, dog);

        Walk walk {email, dogName};
        walk._date = QDateTime(QDate(2024, 5, 12), QTime(12, 0));
        walk._coordinate = coordinate;

        modelData._dogOwners.emplace(email, dogOwner);
        modelData._walks.push_back(walk);
    };

    prepareDummyData("Sam", "dummy1@gmail.com", "123", "Pussy", 3, Coordinate{-37.804863, 144.946003});
    prepareDummyData("John", "dummy2@gmail.com", "123", "Stinky", 2, Coordinate{-37.812218, 144.971386});
    prepareDummyData("Alex", "dummy3@gmail.com", "123", "Beasty", 4, Coordinate{-37.807099, 144.960818});
    prepareDummyData("Dr. Lector", "dummy4@gmail.com", "123", "Ginny", 5, Coordinate{-37.822073, 144.965616});
    prepareDummyData("Lara", "dummy5@gmail.com", "123", "Molly", 9, Coordinate{-37.818675, 144.957879});
    prepareDummyData("Gina", "dummy6@gmail.com", "123", "Graph", 1, Coordinate{-37.805981, 144.953247});

    return modelData;
}

float Coordinate::distanceTo(const Coordinate& oneC, const Coordinate& secondC) {
    const auto earthRadius = 6371.f;
    const auto dLat = qDegreesToRadians(secondC._latitude - oneC._latitude);
    const auto dLon = qDegreesToRadians(secondC._longitude - oneC._longitude);
    const auto lat1 = qDegreesToRadians(oneC._latitude);
    const auto lat2 = qDegreesToRadians(secondC._latitude);

    auto a = sinf(dLat / 2) * sinf(dLat / 2) +
               sinf(dLon / 2) * sinf(dLon / 2) * cosf(lat1) * cosf(lat2);
    auto c = 2 * atan2f(sqrtf(a), sqrtf(1 - a));
    return earthRadius * c;
}

std::vector<Walk> ModelData::getWalks(const Coordinate &coorinate, float radius, const QDateTime &date) const
{
    auto filteredData = _walks | std::views::filter([&](const auto& walk) {
                            if(walk._dogOwnerId == _superUser)
                                return false;

                            auto minDiff = walk._date.secsTo(date) / 60;

                            if(minDiff > 40)
                                return false;

                            return Coordinate::distanceTo(walk._coordinate, coorinate) <= radius / 1000.f;
                        });

    return std::vector<Walk>(filteredData.begin(), filteredData.end());
}

}

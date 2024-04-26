#include "RequestHandler.h"

#include <QJsonArray>
#include <QJsonObject>

#include <functional>
#include <unordered_map>
#include <string>


namespace dh {

const static auto gHandlersFabrique =
    std::unordered_map<std::string, std::function<RequestHandlerPtr()>> {
        {"LoginRequest", [](){ return  QSharedPointer<LoginRequest>::create(); }},
        {"LoginResponce", [](){ return  QSharedPointer<LoginResponce>::create(); }}
};

RequestHandlerPtr RequestHandler::create(const QJsonDocument &jsonDoc)
{
    if(!jsonDoc.isObject())
    {
        qDebug() << "Wrong json document!";
        return nullptr;
    }

    QJsonObject jsonObj = jsonDoc.object();

    // Access individual fields
    QString type = jsonObj["handler_type"].toString();

    auto itCreator = gHandlersFabrique.find(type.toStdString());
    if(itCreator == gHandlersFabrique.end())
    {
        qDebug() << "Handler with Type: " + type << " doesn't exist!";
        return nullptr;
    }

    auto handler = itCreator->second();
    handler->deserialize(jsonObj);

    return handler;
}

void serializeDog(const Dog &dog, QJsonObject &jsonObject)
{
    jsonObject["name"] = QString::fromStdString(dog.getName());
    jsonObject["owner"] = QString::fromStdString(dog.getOwner().getEmail());

    jsonObject["breed"] = QString::fromStdString(dog._breed);
    jsonObject["male"] = static_cast<int>(dog._male);
    jsonObject["wheight"] = dog._wheight;
    jsonObject["age"] = dog.age;
    jsonObject["_character"] = static_cast<int>(dog._character);
    jsonObject["_sterilized"] = dog._sterilized;

    QString base64String = dog._icon.toBase64();
    jsonObject["icon"] = base64String;
}

void derializeDog(Dog &dog, const QJsonObject &jsonObject)
{
    dog._name = jsonObject["name"].toString().toStdString();
    dog._breed = jsonObject["breed"].toString().toStdString();
    dog._male = static_cast<Male>(jsonObject["male"].toInt());
    dog._wheight = jsonObject["wheight"].toDouble();
    dog.age = jsonObject["age"].toInt();
    dog._character = static_cast<Character>(jsonObject["_character"].toInt());
    dog._sterilized = jsonObject["_sterilized"].toBool();

    QString base64String = jsonObject["icon"].toString();
    dog._icon = QByteArray::fromBase64(base64String.toUtf8());
}

void serializeDogOwner(const DogOwner &dogOwner, QJsonObject &jsonObject)
{
    jsonObject["email"] = QString::fromStdString(dogOwner.getEmail());
    jsonObject["password"] = QString::fromStdString(dogOwner.getPassword());
    jsonObject["name"] = QString::fromStdString(dogOwner._name);
    jsonObject["male"] = static_cast<int>(dogOwner._male);

    QString base64String = dogOwner._icon.toBase64();
    jsonObject["icon"] = base64String;

    // dogs
    QJsonArray jsonDogs;
    for(const auto &dogIt : dogOwner.getDogs())
    {
        QJsonObject dogJsonObject;
        serializeDog(*dogIt.second, dogJsonObject);
        jsonDogs.append(dogJsonObject);
    }

    jsonObject["dogs"] = jsonDogs;
}

void derializeDogOwner(DogOwner &dogOwner, const QJsonObject &jsonObj)
{
    dogOwner._email = jsonObj["email"].toString().toStdString();
    dogOwner._password = jsonObj["password"].toString().toStdString();

    dogOwner._name = jsonObj["name"].toString().toStdString();
    dogOwner._male = static_cast<Male>(jsonObj["male"].toInt());

    QString base64String = jsonObj["icon"].toString();
    dogOwner._icon = QByteArray::fromBase64(base64String.toUtf8());

    QJsonArray jsonArray = jsonObj["dogs"].toArray();
    for (const QJsonValue &value : jsonArray) {
        if (!value.isObject())
        {
            qDebug() << "The json has wrong serialized objects";
            continue;
        }

        QJsonObject obj = value.toObject();
        auto dog = std::make_shared<Dog>("", dogOwner);

        derializeDog(*dog, obj);
    }
}

QJsonObject LoginResponce::serialize() const
{
    QJsonObject jsonObject;
    jsonObject["handler_type"] = "LoginResponce";
    jsonObject["status"] = static_cast<int>(_status);

    if(_status == ResponceStatus::Valid && _dogOwner)
        serializeDogOwner(*_dogOwner, jsonObject);

    return jsonObject;
}

void LoginResponce::deserialize(const QJsonObject &jsonObj)
{
    _status = static_cast<ResponceStatus>(jsonObj["status"].toInt());
    if(_status != ResponceStatus::Valid)
        return;

    _dogOwner = std::make_shared<DogOwner>();
    derializeDogOwner(*_dogOwner, jsonObj);
}

}

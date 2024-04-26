#pragma once

#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QSharedPointer>
#include <QString>

#include "model.h"

namespace dh {

class RequestHandler;
using RequestHandlerPtr = QSharedPointer<RequestHandler>;

class RequestHandler : public QObject {
public:

    virtual ~RequestHandler() {}

    virtual QJsonObject serialize() const = 0;

    static RequestHandlerPtr create(const QJsonDocument &jsonDoc);

protected:
    virtual void deserialize(const QJsonObject &jsonObj) = 0;
};

class LoginRequest : public RequestHandler
{
public:
    LoginRequest() = default;
    LoginRequest(const QString &login, const QString &password)
        : _login(login), _password(password) {}

    virtual QJsonObject serialize() const override
    {
        QJsonObject jsonObject;
        jsonObject["handler_type"] = "LoginRequest";
        jsonObject["login"] = _login;
        jsonObject["password"] = _password;

        return jsonObject;
    }

    QString _login;
    QString _password;

protected:
    virtual void deserialize(const QJsonObject &jsonObj) override
    {
        _login = jsonObj["login"].toString();
        _password = jsonObj["password"].toString();
    }
};

class LoginResponce : public RequestHandler
{
public:

    virtual QJsonObject serialize() const override;

    enum class ResponceStatus : int
    {
        Valid = 0,
        InvalidLogin,
        InvalidPassword
    };

    ResponceStatus _status;
    DogOwnerPtr _dogOwner;

protected:
    virtual void deserialize(const QJsonObject &jsonObj) override;
};

}

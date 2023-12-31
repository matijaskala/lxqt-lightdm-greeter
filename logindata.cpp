/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXDE-Qt - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org
 *
 * Copyright: 2012 Christian Surlykke
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */

#include <QFile>
#include <QMessageBox>
#include <QDir>
#include <QSettings>

#include <XdgDirs>

#include "logindata.h"
#include "constants.h"



LoginData::LoginData(QLightDM::Greeter *greeter) : 
    QObject(), 
        m_Greeter(greeter),
        m_UsersModel(this), 
        m_SessionsModel(this)
{
    m_StatefilePath = XdgDirs::cacheHome(true) + "/" + DATA_DIR + "/state";
}

LoginData::~LoginData()
{
}

int LoginData::numberOfUsers()
{
    return m_UsersModel.rowCount(QModelIndex());
}

int LoginData::numberOfSessions()
{
    return m_SessionsModel.rowCount(QModelIndex());
}

QString LoginData::userFullName(int user_index)
{
    return data(user_index, m_UsersModel, Qt::DisplayRole);
}

QString LoginData::userName(int user_index)
{
    return data(user_index, m_UsersModel, QLightDM::UsersModel::NameRole);
}

QString LoginData::sessionFullName(int session_index)
{
    return data(session_index, m_SessionsModel, Qt::DisplayRole);
}

QString LoginData::sessionName(int session_index)
{
    return data(session_index, m_SessionsModel, QLightDM::SessionsModel::KeyRole);
}

int LoginData::userSession(QString user_name)
{
    for (int i = 0; i < numberOfUsers(); i++)
    {
        if (userName(i) == user_name)
        {
            return userSession(i);
        }
    }
    return -1;
}

int LoginData::userSession(int user_index)
{
    QString user_session = data(user_index, m_UsersModel, QLightDM::UsersModel::SessionRole);
    for (int i = 0; i < m_SessionsModel.rowCount(QModelIndex()); i++)
    {
        if (sessionName(i) == user_session)
        {
            return i;
        }
    }

    return -1;
}

int LoginData::suggestedUser()
{
    QString user = m_Greeter->selectUserHint();

    if (user.isEmpty()) 
    {
		user = getLastUser(); 
    }

    if (! user.isEmpty()) 
    {
        for (int i = 0; i < m_UsersModel.rowCount(QModelIndex()); i++) 
        {
            QModelIndex modelIndex = m_UsersModel.index(i, 0);
            if (user == m_UsersModel.data(modelIndex, QLightDM::UsersModel::NameRole).toString())
            {
                return i;
            }
        }

    }

    return -1; // Don't know
}

int LoginData::suggestedSession()
{
    int suggestedSessionIndex = 0;

    for (int i = 0; i < numberOfSessions(); i++)
    {
        if (sessionName(i) == "lxqt")
        {
            suggestedSessionIndex = i;
            break;
        }
    }

    return suggestedSessionIndex;
}

QString LoginData::getLastUser()
{
    return QSettings(m_StatefilePath, QSettings::NativeFormat).value(LAST_USER_KEY).toString();
}

void LoginData::setLastUser(QString user)
{
    QSettings(m_StatefilePath, QSettings::NativeFormat).setValue(LAST_USER_KEY, user);
}

QString LoginData::data(int index, QAbstractListModel& model, int role)
{
    if (0 <= index && index < model.rowCount(QModelIndex())) 
    {
        QModelIndex modelIndex = model.index(index, 0);
        return model.data(modelIndex, role).toString();
    }
    else 
    {
        return QString();
    }
}

Msg::Msg()
{
}

void Msg::show(const QString msg)
{
    QMessageBox::information(NULL, "", msg);
}

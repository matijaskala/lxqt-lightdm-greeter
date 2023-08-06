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

#include <QRect>
#include <QApplication>
#include <QDesktopWidget>
#include <QPalette>
#include <QString>
#include <QDebug>

#include <LXQt/Settings>

#include "mainwindow.h"
#include "loginform.h"
#include "constants.h"

using namespace LXQt;

MainWindow::MainWindow(int screen, QWidget *parent)
    : QWidget(parent),
        m_Screen(screen)
{
    setObjectName(QString("MainWindow_%1").arg(screen));
    
    
    QRect screenRect = QApplication::desktop()->screenGeometry(screen);
    setGeometry(screenRect);

    setBackground();

    // display login dialog only in the main screen
    
    if (showLoginForm())
    {
        m_LoginForm = new LoginForm(this);

        int maxX = screenRect.width() - m_LoginForm->width();
        int maxY = screenRect.height() - m_LoginForm->height();
        int defaultX = 10*maxX/100;
        int defaultY = 50*maxY/100;
        int offsetX = getOffset(LOGINFORM_OFFSETX_KEY, maxX, defaultX);
        int offsetY = getOffset(LOGINFORM_OFFSETY_KEY, maxY, defaultY);
        
        m_LoginForm->move(offsetX, offsetY);
        m_LoginForm->show();

        // This hack ensures that the primary screen will have focus
        // if there are more screens (move the mouse cursor in the center
        // of primary screen - not in the center of all X area). It
        // won't affect single-screen environments.
        int centerX = screenRect.width()/2 + screenRect.x();
        int centerY = screenRect.height()/2 + screenRect.y();
        QCursor::setPos(centerX, centerY);
    }
}

MainWindow::~MainWindow()
{
}

bool MainWindow::showLoginForm()
{
    return m_Screen == QApplication::desktop()->primaryScreen();
}

void MainWindow::setFocus(Qt::FocusReason reason)
{
    if (m_LoginForm)
    {
        m_LoginForm->setFocus(reason);
    }
    else 
    {
        QWidget::setFocus(reason);
    }
}

int MainWindow::getOffset(QString key, int maxVal, int defaultVal)
{

    QSettings greeterSettings(CONFIG_FILE, QSettings::IniFormat);
    int offset = defaultVal > maxVal ? maxVal : defaultVal;

    if (greeterSettings.contains(key)) 
    {
        QString offsetAsString = greeterSettings.value(key).toString();
        if (QRegExp("^\\d+px$", Qt::CaseInsensitive).exactMatch(offsetAsString)) 
        {

            offset = offsetAsString.left(offsetAsString.size() - 2).toInt();
            if (offset > maxVal) offset = maxVal;
        }
        else if (QRegExp("^\\d+%$", Qt::CaseInsensitive).exactMatch(offsetAsString))
        {
            int offsetPct = offsetAsString.left(offsetAsString.size() -1).toInt();
            if (offsetPct > 100) offsetPct = 100;
            offset = (maxVal * offsetPct)/100;
        }
        else
        {
            qWarning() << "Could not understand" << key 
                       << "value:" << offsetAsString 
                       << "- must be of form <positivenumber>px or <positivenumber>%, e.g. 35px or 25%" ;
        }
    }

    return offset;
}

void MainWindow::setBackground()
{
    QImage backgroundImage;
    QSettings greeterSettings(CONFIG_FILE, QSettings::IniFormat);
    
    if (greeterSettings.contains(BACKGROUND_IMAGE_KEY))
    {
        QString pathToBackgroundImage = greeterSettings.value(BACKGROUND_IMAGE_KEY).toString();
        
        backgroundImage = QImage(pathToBackgroundImage);
        if (backgroundImage.isNull())
        {
            qWarning() << "Not able to read" << pathToBackgroundImage << "as image";
        }
             
    }
    
    QPalette palette;
    QRect rect = QApplication::desktop()->screenGeometry(m_Screen);
    if (backgroundImage.isNull())
    {
        palette.setColor(QPalette::Background, Qt::black);
    }
    else
    {
        QBrush brush(backgroundImage.scaled(rect.width(), rect.height()));
        palette.setBrush(this->backgroundRole(), brush);
    }
    this->setPalette(palette);
}



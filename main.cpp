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

#include <QApplication>
#include <QDesktopWidget>
#include <QFile>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile styleFile(":/resources/lxqt-lightdm-greeter.qss");
    styleFile.open(QFile::ReadOnly);
    QString styleSheet = styleFile.readAll();
    qDebug() << "Setting styleSheet:" << styleSheet;
    a.setStyleSheet(styleSheet);

    MainWindow *focusWindow = 0;
    for (int i = 0; i < QApplication::desktop()->screenCount(); ++i)
    {
        MainWindow *w = new MainWindow(i);
        w->show();
        if (w->showLoginForm())
            focusWindow = w;
    }

    // Ensure we set the primary screen's widget as active when there
    // are more screens
    if (focusWindow)
    {
        focusWindow->setFocus(Qt::OtherFocusReason);
        focusWindow->activateWindow();
    }

    return a.exec();
}

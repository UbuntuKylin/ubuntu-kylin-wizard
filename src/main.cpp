// -*- Mode: C++; indent-tabs-mode: nil; tab-width: 4 -*-
/*
 * Copyright (C) 2019, National University of Defense Technology(NUDT) & Kylin Ltd
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: handsome_feng <jianfengli@ubuntukylin.com>
 */

#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QStandardPaths>
#include <QtNetwork/QNetworkConfigurationManager>
#include <QMessageBox>

bool isFirstRun()
{
    QString config_dir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    QFile file(config_dir + "/first_run.stamp");
    if (file.exists()) {
        return false;
    } else {
        if (file.open(QIODevice::ReadWrite)) {
            QTextStream stream(&file);
            stream << "hello wizard!";
        } else {
            qDebug() << "Failed to create" << file.fileName();
        }
        return true;
    }
}

bool isLiveCD()
{
    QFile file("/proc/cmdline");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "error";
        return true;
    }
    QTextStream in(&file);
    QString line = in.readAll();
    if (line.contains("casper"))
        return true;
    else
        return false;
}

bool isOnline()
{
    QNetworkConfigurationManager mgr;
    return mgr.isOnline();
}

void checkNetwork()
{
    if (isOnline()) {
        qDebug() << "Is online!";
    } else {
        QMessageBox msgBox;
        msgBox.setText(QMessageBox::tr("You are offline, please connect to the network first and then press 'OK' to run this wizard, or press 'Cancel' to quit."));
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        int ret = msgBox.exec();
        switch (ret) {
          case QMessageBox::Ok:
            if (!isOnline())
                exit(-1);
            break;
          case QMessageBox::Cancel:
            qDebug() << "cancel";
            exit(0);
          default:
            // should never be reached
            break;
        }
    }
}
int main(int argc, char *argv[])
{
    if (isLiveCD() || !isFirstRun()) {
        qDebug() << "Only run when first login!";
        return 0;
    }

    Q_INIT_RESOURCE(wizard);

    QApplication a(argc, argv);

    // i18n
    QTranslator wizardTranslator;
    if (wizardTranslator.load(QLocale::system().name(), ":/i18n/po")) {
        a.installTranslator(&wizardTranslator);
    } else {
        qDebug() << "Failed to load " << QLocale::system().name() << ".qm !";
    }

    // Load qss
    QFile styleFile(":/style/data/wizard.qss");
    styleFile.open(QFile::ReadOnly);
    QString style(styleFile.readAll());
    a.setStyleSheet(style);

    checkNetwork();

    // This should be after i18n
    MainWindow w;
    w.show();

    return a.exec();
}

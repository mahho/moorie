/*
 *   Copyright (C) 2008-2010 by Patryk Połomski
 *   cykuss@gmail.com
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
#include "uploadinstance.h"

uploadInstance::uploadInstance(QString file, QVector<mirrorMailbox*> mirrorMailboxes, QString dpass, QString epass, int msize, unsigned  int fromseg):
        file(file),
        mirrorMailboxes(mirrorMailboxes),
        dpass(dpass),
        epass(epass),
        msize(msize),
        fromseg(fromseg),
        infoString('Brak informacji, spróbuj później.'),
        wyslano(false),
        totalSegments(0)
{
    loadMailboxesFromFile();
}
void uploadInstance::run()
{
    user = uploadMailboxes.first()->username;
    pass = uploadMailboxes.first()->password;
    Instance = new CLibMoor();
    Instance -> splitFile(file.toStdString(), msize);
    if(!Instance -> selectUploadMailBox(user.toStdString(), pass.toStdString(), getToUsernames().toStdString(), dpass.toStdString(), epass.toStdString())){
        totalSegments = Instance->getMyUploadNumOfSeg();
        fileCRC = QString::fromStdString(Instance->getMyUploadFileCRC());
        infoString = generateInfo();
        Instance->generateCleanHashcode();
        Instance->startUpload(fromseg);
    }
}
void uploadInstance::loadMailboxesFromFile()
{
    QDomDocument dokument_xml;
    QFile dokument(Zmienne().configPath+"mailboxes.xml");
    dokument.open( QIODevice::ReadOnly );
    dokument_xml.setContent( &dokument );
    dokument.close();

    QDomNode mailboxes;
    mailboxes = dokument_xml.documentElement();

    QDomNode mailbox, item;
    mailbox = mailboxes.firstChild();

    while(!mailbox.isNull())
    {
        QDomElement login,pass;

        item = mailbox.namedItem("login");
        login = item.toElement();
        item = mailbox.namedItem("password");
        pass = item.toElement();

        uploadMailboxes.append(new mirrorMailbox());

        uploadMailboxes.last()->username = login.text();
        uploadMailboxes.last()->password = pass.text();

        mailbox = mailbox.nextSibling();
    }
}
QString uploadInstance::getToUsernames()
{
    QString usernames = "";
    for(int i = 0; i < mirrorMailboxes.count(); ++i)
    {
        if(i>0) usernames += ",";
        usernames += mirrorMailboxes.at(i)->username;
    }
    return usernames;
}
QString uploadInstance::generateInfo()
{
    QString info = "Plik: " + fileName +"\n\n";
    info = info + "Hasło pobierania: " + dpass + "\n";
    info = info + "Hasło edycji: " + epass + "\n\n";
    info += "Mirrory:\n\n";
    for(int i = 0; i < mirrorMailboxes.count(); ++i)
    {
        info = info + "Login: " + mirrorMailboxes.at(i)->username + "\n";
        info = info + "Hasło: " + mirrorMailboxes.at(i)->password + "\n\n";
    }
    std::string hashcode = Instance->generateCleanHashcode();

    info = info + "Czysty hashcode:\n" + QString::fromStdString(hashcode) + "\n\n";

    info = info + "Hashcode z mirrorami:\n";
    for(int i = 0; i < mirrorMailboxes.count(); ++i)
    {
       user = mirrorMailboxes.at(i)->username;
       pass = mirrorMailboxes.at(i)->password;
       hashcode = Instance->addMirror(epass.toStdString(), hashcode, user.toStdString(), pass.toStdString());
    }
    info = info + QString::fromStdString(hashcode) + "\n\n";

    QTextCodec::setCodecForLocale ( QTextCodec::codecForName ("UTF-8"));
    QFile file(Zmienne().configPath+fileName+".txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << info << "\n";
        file.close();
    }
    return info;
}

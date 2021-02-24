/**********************************************************************************
**                                                                               **
**  Rhode&Schwarz Plugin for LabAnlyser, control&visualize Rhode&Schwarz data.   **
**  Copyright (C) 2015-2021 Andreas Hoffmann                                     **
**                                                                               **
**  Rhode&Schwarz Plugin is free software: you can redistribute it and/or modify **
**  it under the terms of the GNU General Public License as published by         **
**  the Free Software Foundation, either version 3 of the License, or            **
**  (at your option) any later version.                                          **
**                                                                               **
**  This program is distributed in the hope that it will be useful,              **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of               **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                **
**  GNU General Public License for more details.                                 **
**                                                                               **
**  You should have received a copy of the GNU General Public License            **
**  along with this program.  If not, see http://www.gnu.org/licenses/.          **
**                                                                               **
***********************************************************************************
***********************************************************************************/

#include "RhodeSchwarzClass.h"
#include "visa.h"
#include "math.h"
#include <string>
#include <QDebug>


//#include <QVector>

/****************************************************************************************/
// Constructor
/****************************************************************************************/
RhodeSchwarz::RhodeSchwarz()  // std::string name, std::string ip)
{

    this->defaultRM = &defaultRMS;
    int state = viOpenDefaultRM(this->defaultRM);
    this->session = NULL;	

}


int RhodeSchwarz::Connect(QString IP)
{
    CloseConnection();
    QString geraet = "TCPIP::" + IP + "::INSTR";

   status = viOpen(*defaultRM, (ViRsrc)geraet.toStdString().c_str(), VI_NULL, VI_NULL, &session);
   if (status < VI_SUCCESS) {
       session = NULL;
       return status;
   }

   status = viSetAttribute(session, VI_ATTR_TMO_VALUE, 500);
   if (status < VI_SUCCESS) {
       session = NULL;
       return status;
   }
   else {
       return status;
   }
}

int RhodeSchwarz::Calibrate()
{

        status = viSetAttribute(session, VI_ATTR_TMO_VALUE, 15000);
        if (status < VI_SUCCESS) {
            std::cout << "Setting time out failed";
        }

        // command: *CAL?
        // response format: *CAL <diagnostics>
        // <diagnostics> : = 0 or other (0 means calibration successful)
        //                     Comm_HeaDeR

        QString command = "*CAL?";
        QString logMsg = "Calibrating oscilloscope";
        read(command, logMsg);

        std::cout << "Setting time out to 0.5 seconds";
        status = viSetAttribute(session, VI_ATTR_TMO_VALUE, 500);
        if (status < VI_SUCCESS) {
            std::cout << "Setting time out failed";
        }

return 0;
}

bool RhodeSchwarz::CloseConnection()
{
    if (session!=NULL){
        status = viClose(session);
        if (status < VI_SUCCESS){
            return false;
        }
        else{
            return true;
        }
        session = NULL;
    }
    else{
        return  true;
    }

}


/****************************************************************************************/
// Destructor
/****************************************************************************************/
RhodeSchwarz::~RhodeSchwarz()
{

}

QStringList RhodeSchwarz::read(QString command, QString logMsg)
{
    // creating log message for debugging


    status = viWrite(session, (ViBuf)command.toStdString().c_str(), command.length(), &retCount);
    if (status < VI_SUCCESS)
    {
       qDebug() << "Sending command failed";
        qDebug()  << "VISA error code:" << status;
    }


    unsigned char buffer[1024];
    for (int i =0; i< sizeof(buffer); i++) buffer[i] = ' ';

    status = viRead(session, (ViPBuf)buffer, (ViUInt32)sizeof(buffer), &retCount);
    if (status < VI_SUCCESS) {
               qDebug()  << logMsg << "failed";
                qDebug()  << "VISA error code:" << status;
       }
    QString Answer(QString::fromLocal8Bit((const char*) buffer,retCount));
    // remove some strange stuff at the end of buffer


        auto AnswerParts = Answer.simplified().split("\n");
        AnswerParts = AnswerParts.at(0).simplified().split(";");


    return AnswerParts;
}

QStringList RhodeSchwarz::CheckStates(QStringList CommandList)
{
    QString  logMsg = "";
    QStringList ErrorCommands;

    for(auto itt : CommandList)
    {
        QStringList Answers = read(itt,logMsg);
        if(!Answers.size())
        {
            ErrorCommands.push_back(itt);
        }
    }

    return ErrorCommands;
}

QStringList RhodeSchwarz::ReadState(QStringList CommandList)
{
    QString  command;
    for(int i = 0; i < CommandList.size();i++)
    {
        command.append(CommandList[i]);
        if(i < CommandList.size()-1)
            command.append(";:");
        else
            command.append("\n");
    }
    QString  logMsg = "Viewing channel ReadState";
    QStringList Answers = read(command,logMsg);
    return Answers;
}

void RhodeSchwarz::write(QString command, QString logMsg)
{
    // creating log message for debugging

    status = viWrite(session, (ViBuf)command.toStdString().c_str(), command.length(), &retCount);
        if (status < VI_SUCCESS) {
            qDebug()  << logMsg << "failed";
            qDebug()  << "VISA error code:" << status;
        }


}

std::vector<unsigned char> RhodeSchwarz::readbin(QString command, int size)
{
    status = viWrite(session, (ViBuf)command.toStdString().c_str(), command.length(), &retCount);
    if (status < VI_SUCCESS)
    {
       qDebug() << "Sending command failed";
        qDebug()  << "VISA error code:" << status;
    }


    std::vector<unsigned char> buffer;
    buffer.resize(size);
    //unsigned char buffer[1024];
    for (int i =0; i< buffer.size(); i++) buffer[i] = ' ';

    status = viRead(session, (ViPBuf)&(buffer[0]), (ViUInt32)buffer.size(), &retCount);
    if (status < VI_SUCCESS) {

                qDebug()  << "VISA error code:" << status;
       }

    buffer.erase(buffer.begin()+retCount,buffer.end());
   // buffer.erase(buffer.begin(),buffer.begin()+19);





    return buffer;
}

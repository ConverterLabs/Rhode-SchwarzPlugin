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

#include "readoscichannel.h"

ReadOsciChannel::ReadOsciChannel( QString DeviceName_, MessengerClass &messenger_, RhodeSchwarz &Osci_ ,  std::map<QString, DataStorage>& data):
    m_data(data),
    DeviceName(DeviceName_),
    Osci(Osci_),
    messenger(messenger_)
{

}

void ReadOsciChannel::ReadChannel(int Channel)
{
    QString Chan = "C" + QString::number(Channel);
    QString ChanT = "CHAN" + QString::number(Channel);

    if(this->m_data[DeviceName + "::Channel::" + Chan + "::State"].GetBool())
    {
        //Read Header
        //CHANnel<m>[:WAVeform<n>]:DATA:HEADer?
        QStringList Answer = this->Osci.ReadState(QStringList(QString(ChanT+":WAV1:DATA:HEAD?")));
        double xstart;
        double xstop;
        int samples;

        if(Answer.size())
        {
            const double VerticalDivisionCount = 10.0;
            const double NofQuantisationLevels = 253.0 * 256.0;
            QString IDM = DeviceName + "::Channel::" + Chan + "::Scale";
            double VScale = this->m_data[IDM].GetDouble();
            IDM = DeviceName + "::Channel::" + Chan +"::Offset";
            double Offset = this->m_data[IDM].GetDouble();
            QStringList AnswerParts = Answer.at(0).split(",");

            if(AnswerParts.size()<2)
                return;

            xstart = AnswerParts[0].toDouble();
            xstop = AnswerParts[1].toDouble();
            samples = (AnswerParts[2]).toInt();
            std::vector<unsigned char> buffer = this->Osci.readbin(ChanT +":WAV1:DATA?",samples*2+1+1+10);
            //Set new buffered data :)
            QString ID = DeviceName + "::Buffered::"+Chan;
            InterfaceData _Data;
            _Data.SetDataType("vector<double>");
            _Data.SetType("Data");

            std::vector<double> T;
            T.resize(samples);
            std::vector<double> Y;
            Y.resize(samples);

            int j = 2+ QString(buffer[1]).toInt();
            for(int i = 0 ;i<samples ; i++)
            {
                 int16_t tmp2 = *((int16_t*)&buffer[j]);
                 double scale = VerticalDivisionCount * VScale / NofQuantisationLevels;
                 double value = scale * (double)tmp2 + Offset;
                 Y[i] = (value);
                 double tmp1 = xstart + ((double)i)*(xstop - xstart)/((double)samples);
                 T[i] = (tmp1);
                 j += 2;
           }
           _Data.SetData(DataPair( boost::shared_ptr<std::vector<double>>(new std::vector<double>(T)), boost::shared_ptr<std::vector<double>>(new std::vector<double>(Y))));
            messenger.MessageSender("set", ID,  _Data);
        }
    }
}

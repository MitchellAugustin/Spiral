/*
** Copyright (C) 2019-2021 Mitchell Augustin - https://mitchellaugustin.com/
**
** This program is licensed under the GNU General Public License v3
** https://www.gnu.org/licenses/gpl-3.0.en.html
*/

#ifndef SPIRALSTRINGLIST_H
#define SPIRALSTRINGLIST_H

#include <QStringListModel>

/**
 * @brief SpiralStringList - Header for the SpiralStringList adapter used in the section and notebook browsers
 * @author Mitchell Augustin - https://mitchellaugustin.com
 * Adapted from https://stackoverflow.com/questions/5802313/qt4-qstringlistmodel-in-qlistview/5825113#5825113
 * @param parent
 */
class SpiralStringList : public QStringListModel
{
public:
  void append (const QString& string){
    insertRows(rowCount(), 1);
    setData(index(rowCount()-1), string);
  }
  SpiralStringList& operator<<(const QString& string){
    append(string);
    return *this;
  }
};

#endif // SPIRALSTRINGLIST_H

#ifndef SPIRALSTRINGLIST_H
#define SPIRALSTRINGLIST_H

#include <QStringListModel>

//Adapted from https://stackoverflow.com/questions/5802313/qt4-qstringlistmodel-in-qlistview/5825113#5825113
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

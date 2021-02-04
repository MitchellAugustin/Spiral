#ifndef _MTEXTEDIT_H_
#define _MTEXTEDIT_H_

#include <QTextBrowser>
#include <QMimeData>
#include <QImage>

class MTextEdit : public QTextBrowser {
    Q_OBJECT
  public:
    MTextEdit(QWidget *parent);

    void        dropImage(const QImage& image, const QString& format);

  protected:
    bool        canInsertFromMimeData(const QMimeData *source) const;
    void        insertFromMimeData(const QMimeData *source);
    QMimeData  *createMimeDataFromSelection() const;


  private slots:
    void onTextChanged();
    void onCursorPositionChanged();
};

#endif

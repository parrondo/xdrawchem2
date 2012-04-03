#ifndef NETCHOOSEDIALOG_H
#define NETCHOOSEDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QListWidget>

class NetChooseDialog : public QDialog
{
    Q_OBJECT
public:
    NetChooseDialog( QWidget *parent, QStringList );
    QString getFile() { return fn; }

public slots:
    void OK();

private:
    QString fn;
    QStringList results;
    QListWidget *lv; ///TODO
};

#endif

// kate: tab-width 4; indent-width 4; space-indent on; replace-trailing-space-save on;

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDir>
#include <QFileSystemModel>
#include <QModelIndex>
#include <QGridLayout>
#include <QListView>
#include <QImageWriter>
#include <QTableWidget>
#include <QFileDialog>
#include <QHeaderView>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    bool line_changed=false;
    QFileSystemModel *model;
    QGridLayout *backgr;
    QListView *listView;
    QTableWidget *twInfo=nullptr;
     int *sortcolumns = new int[5];
public slots:
    void on_listView_doubleClicked(const QModelIndex &index);
     void twInfoSelected(int logicalIndex);
private slots:
    void dialogClose();
    void on_multiChoice_clicked();
};
#endif // WIDGET_H

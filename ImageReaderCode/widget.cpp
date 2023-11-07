#include "widget.h"
#include "ui_widget.h"
#include <QtCore>



using namespace std;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    model = new QFileSystemModel(this);
    model->setFilter(QDir::QDir::AllEntries);
    model->setRootPath("");
    ui->listView->setModel(model);
    this->setFixedSize(this->width(), this->height());
    ui->lineEdit->setText(" ");
    for(int i = 0; i < 5; i++)
    {
         sortcolumns[i] = 0;
    }
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_listView_doubleClicked(const QModelIndex &index)
{
    QListView* listView = (QListView*)sender();
    QFileInfo fileInfo = model->fileInfo(index);
    ui->lineEdit->setText(model->filePath(index));
    if(fileInfo.fileName() == "..")
    {
        QDir dir = fileInfo.dir();
        dir.cdUp();
        model->index(dir.absolutePath());
        listView->setRootIndex(model->index(dir.absolutePath()));
    }
    else if (fileInfo.fileName() == ".")
    {
        listView->setRootIndex(model->index(""));
    }

    else if(fileInfo.isDir())
    {
        listView->setRootIndex(index);
    }
    else if(!fileInfo.isDir())
    {
        QDir dir = fileInfo.dir();
        QString fileExt = model->fileName(index);
        QImageWriter a(model->filePath(index));
        QString resolution = "";
        QImage img (model->filePath(index));
        for(int i = fileExt.lastIndexOf('.'); i < fileExt.size(); i++)
        {
            resolution.append(fileExt[i]);
        }
        if (resolution == ".jpg"|| resolution == ".JPG"  || resolution == ".gif" || resolution == ".tif" || resolution == ".bmp" ||
                resolution == ".png" || resolution == ".pcx" || resolution == ".BMP")
        {
             ui->FileName->setText(model->fileName(index));
             ui->Size->setText(QString::number(img.size().width() )+ "x" + QString::number(img.size().height() )+"px.");
             ui->Contraction->setText(QString::number(a.compression()));
             ui->Color_depth->setText(QString::number(img.bitPlaneCount())+" bpp");
             ui->Resolution->setText(QString::number(img.physicalDpiY())+" dpi");
        }
    }
}

void Widget::dialogClose()
{
    this->show();
}

void Widget::on_multiChoice_clicked()
{
    QFileDialog *files = new QFileDialog;
    files->setWindowTitle("Open files");
    QList<QUrl> list = files->getOpenFileUrls();

    QDialog *table = new QDialog();
    table->setWindowTitle("Table");
   // table->setWindowIcon(QPixmap(":/img/folder.png"));
    QGridLayout *tableLayout = new QGridLayout(table);
    table->setFixedSize(1800,700);
    twInfo = new QTableWidget(table);
    tableLayout->addWidget(twInfo);
    twInfo->setColumnCount(5);
    twInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
    twInfo->setRowCount(list.size());
    twInfo->setHorizontalHeaderItem(0, new QTableWidgetItem("Name"));
    twInfo->setHorizontalHeaderItem(1, new QTableWidgetItem("Size"));
    twInfo->setHorizontalHeaderItem(2, new QTableWidgetItem("Resolution"));
    twInfo->setHorizontalHeaderItem(3, new QTableWidgetItem("Color depth"));
    twInfo->setHorizontalHeaderItem(4, new QTableWidgetItem("Compression"));
    twInfo->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    for(int i = 0; i < list.size(); i++)
    {
        QFile temp(list[i].toLocalFile());
        QString resolution = "";
        if (temp.fileName().lastIndexOf('.') == -1)
        {
            twInfo->setRowCount(twInfo->rowCount()-1);
            continue;
        }
        for(int i = temp.fileName().lastIndexOf('.'); i < temp.fileName().size(); i++)
        {
            resolution.append(temp.fileName()[i]);
        }
        if (resolution != ".jpg" && resolution != ".gif" && resolution != ".tif" && resolution != ".bmp" &&
                resolution != ".png" && resolution != ".pcx" && resolution != ".BMP")

        {
            twInfo->setRowCount(twInfo->rowCount()-1);
            continue;
        }
         QImage im(list[i].toLocalFile());
        twInfo->setItem(i, 2, new QTableWidgetItem(QString::number(im.physicalDpiY())+" dpi"));
        QString fileName = "";
        for(int i = temp.fileName().lastIndexOf('/') + 1; i < temp.fileName().lastIndexOf('.'); i++)
        {
            fileName.append(temp.fileName()[i]);
        }
        twInfo->setItem(i,0,new QTableWidgetItem(fileName+resolution));

        QImageWriter a(list[i].toLocalFile());
        twInfo->setItem(i, 1, new QTableWidgetItem(QString::number(im.size().width())+"x"+QString::number(im.size().height())+"px."));
        twInfo->setItem(i, 3, new QTableWidgetItem(QString::number(im.bitPlaneCount())+" bpp"));
       // a.setCompression(4);
        twInfo->setItem(i, 4, new QTableWidgetItem(QString::number(a.compression())));
    }
    if (twInfo->rowCount() == 0)
    {
        return;
    }
    table->show();
    connect(table, SIGNAL(rejected()), this, SLOT(dialogClose()));
    connect(twInfo->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(twInfoSelected(int)));
    this->hide();
}


bool column1(QTableWidgetItem* item1, QTableWidgetItem* item2)
{
    return item1->text() > item2->text();
}


bool column2(QTableWidgetItem* item1, QTableWidgetItem* item2)
{
    QStringList string1 = item1->text().split('x');
    QStringList string2 = item2->text().split('x');
    if (string1[0].toInt() > string2[0].toInt())
    {
        return true;
    }
    return false;
}

bool column3(QTableWidgetItem* item1, QTableWidgetItem* item2)
{
    return item1->text() > item2 -> text();
}

bool column4(QTableWidgetItem* item1, QTableWidgetItem* item2)
{
    return item1->text().toInt() > item2->text().toInt();
}

bool column5(QTableWidgetItem* item1, QTableWidgetItem* item2)
{
    return item1->text().toInt() > item2->text().toInt();
}

vector<function<bool(QTableWidgetItem*, QTableWidgetItem*)>> functions = {column1, column2, column3, column4, column5};

vector<int> sortcolumns(5, 0);

void Widget::twInfoSelected(int columnIndex)
{
    if (sortcolumns[columnIndex] == 0)
    {
        for(int i = 0; i < twInfo->rowCount(); i++)
        {
             for(int j = 0; j < twInfo->rowCount() - 1; j++)
             {
                 if (functions[columnIndex](twInfo->item(j, columnIndex), twInfo->item(j + 1, columnIndex)))
                 {
                         for(int k = 0; k < twInfo->columnCount(); k++)
                         {
                             QString temp1 = twInfo->item(j, k)->text();
                             QString temp2 = twInfo->item(j + 1, k) -> text();
                             twInfo->setItem(j+1, k, new QTableWidgetItem(temp1));
                             twInfo->setItem(j, k, new QTableWidgetItem(temp2));
                     }
                 }
             }
         }

        for(int k = 0; k < 5; k++)
        {
            sortcolumns[k] = 0;
        }
        sortcolumns[columnIndex] = 1;
    }
    else
    {
        if (sortcolumns[columnIndex] == 1)
        {
            for(int i = 0; i < twInfo->rowCount(); i++)
            {
                 for(int j = 0; j < twInfo->rowCount() - 1; j++)
                 {
                     if (!functions[columnIndex](twInfo->item(j, columnIndex), twInfo->item(j + 1, columnIndex)))
                     {
                         for(int k = 0; k < twInfo->columnCount(); k++)
                         {
                             QString temp1 = twInfo->item(j, k)->text();
                             QString temp2 = twInfo->item(j + 1, k) -> text();
                             twInfo->setItem(j+1, k, new QTableWidgetItem(temp1));
                             twInfo->setItem(j, k, new QTableWidgetItem(temp2));
                         }
                     }
                 }
             }
            for(int k = 0; k < 5; k++)
            {
                sortcolumns[k] = 0;
            }
            sortcolumns[columnIndex] = 2;
        }
        else
        {
            for(int i = 0; i < twInfo->rowCount(); i++)
            {
                 for(int j = 0; j < twInfo->rowCount() - 1; j++)
                 {
                     if (functions[columnIndex](twInfo->item(j, columnIndex), twInfo->item(j + 1, columnIndex)))
                     {
                             for(int k = 0; k < twInfo->columnCount(); k++)
                             {
                                 QString temp1 = twInfo->item(j, k)->text();
                                 QString temp2 = twInfo->item(j + 1, k) -> text();
                                 twInfo->setItem(j+1, k, new QTableWidgetItem(temp1));
                                 twInfo->setItem(j, k, new QTableWidgetItem(temp2));
                         }
                     }
                 }
             }
            for(int k = 0; k < 5; k++)
            {
                sortcolumns[k] = 0;
            }
            sortcolumns[columnIndex] = 1;
        }
    }
}

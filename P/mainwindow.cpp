#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QSortFilterProxyModel>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QList<int> World_rank;
    QList<QString> Name;
    QList<QString> Country;
    QList<int> Quality_of_ed;


    QFile csv(":/csv/cwurData.csv");
    csv.open(QFile::ReadOnly | QFile::Text);
    QTextStream in(&csv);
    QString s=in.readLine();
    while (!in.atEnd()){
        QString s=in.readLine();
        QStringList strk = s.split('"');
        strk[1] = strk[1].replace("," , ".");
        s = strk.join('"');
        World_rank.append(s.split(",")[0].toInt());
        Name.append(s.split(",")[1]);
        Country.append(s.split(",")[2]);
        Quality_of_ed.append(s.split(",")[3].toInt());


    }
    // Create model:
    TestModel *PhoneBookModel = new TestModel(this);
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(PhoneBookModel);
    proxyModel->setSourceModel(PhoneBookModel);


    // Populate model with data:
    PhoneBookModel->populateData(World_rank,Name,Country,Quality_of_ed);

    // Connect model to table view:
    ui->tableView->setModel(PhoneBookModel);

    // Make table header visible and display table:
    ui->tableView->horizontalHeader()->setVisible(true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->setModel(proxyModel);
    ui->tableView->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

TestModel::TestModel(QObject *parent) : QAbstractTableModel(parent)
{
}

// Create a method to populate the model with data:
void TestModel::populateData(const QList<int> &World_rank,const QList<QString> &Name,const QList<QString> &Country,const QList<int> &Quality_of_ed)
{
    world_rank.clear();
    world_rank = World_rank;
    name.clear();
    name = Name;
    country.clear();
    country = Country;
    quality_of_ed.clear();
    quality_of_ed = Quality_of_ed;
    return;
}

int TestModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return world_rank.length();
}

int TestModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 4;
}

QVariant TestModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole) {
        return QVariant();
    }
    if (index.column() == 0) {
        return world_rank[index.row()];
    } else if (index.column() == 1) {
        return name[index.row()];
    }
    else if (index.column() == 2) {
            return country[index.row()];
        }
    else if (index.column() == 3) {
            return quality_of_ed[index.row()];
        }
    return QVariant();
}

QVariant TestModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if (section == 0) {
            return QString("World Rank");
        } else if (section == 1) {
            return QString("Name");
        } else if (section == 2) {
                    return QString("Country");
        }else if (section == 3) {
                    return QString("Quality of Education");
        }
    }
    return QVariant();
}

void MainWindow::on_qualSort_clicked()
{
    ui->tableView->setSortingEnabled(true);
    ui->tableView->sortByColumn(3,Qt::AscendingOrder);
}

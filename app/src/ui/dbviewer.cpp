#include "dbviewer.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>
#include <QHeaderView>

DbViewer::DbViewer(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("数据库数据查看与修改");
    resize(1200, 600);
    // 使窗口成为顶级窗口（拥有系统标题栏，可自由拖动）
    setWindowFlags(windowFlags() | Qt::Window);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 顶部按钮布局
    QHBoxLayout *topLayout = new QHBoxLayout();
    btnChooseFile = new QPushButton("选择数据库文件", this);
    btnAddRow = new QPushButton("添加新行", this);
    btnDeleteRow = new QPushButton("删除选中行", this);
    btnSave = new QPushButton("保存修改", this);
    btnClose = new QPushButton("关闭", this);

    topLayout->addWidget(btnChooseFile);
    topLayout->addWidget(btnAddRow);
    topLayout->addWidget(btnDeleteRow);
    topLayout->addWidget(btnSave);
    topLayout->addWidget(btnClose);

    mainLayout->addLayout(topLayout);

    // 表格视图
    tableView = new QTableView(this);
    mainLayout->addWidget(tableView);
    setLayout(mainLayout);

    // 窗口关闭时自动删除，避免内存泄漏
    setAttribute(Qt::WA_DeleteOnClose);

    // 信号绑定
    connect(btnChooseFile, &QPushButton::clicked, this, &DbViewer::chooseDatabaseFile);
    connect(btnAddRow, &QPushButton::clicked, this, &DbViewer::addRow);
    connect(btnDeleteRow, &QPushButton::clicked, this, &DbViewer::deleteRow);
    connect(btnSave, &QPushButton::clicked, this, &DbViewer::saveChanges);
    connect(btnClose, &QPushButton::clicked, this, &DbViewer::close);

    model = nullptr;
}

DbViewer::~DbViewer()
{
    if (db.isOpen())
        db.close();
    QSqlDatabase::removeDatabase("db_connection");
}

void DbViewer::chooseDatabaseFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "选择 SQLite 数据库文件", "", "SQLite Files (*.db *.sqlite);;All Files (*)");

    if (fileName.isEmpty())
        return;

    if (db.isOpen()) {
        db.close();
        QSqlDatabase::removeDatabase("db_connection");
    }

    db = QSqlDatabase::addDatabase("QSQLITE", "db_connection");
    db.setDatabaseName(fileName);

    if (!db.open()) {
        QMessageBox::critical(this, "错误", "无法打开数据库: " + db.lastError().text());
        return;
    }
    qDebug() << "数据库打开成功:" << db.databaseName();

    if (model)
        delete model;

    model = new QSqlTableModel(this, db);
    model->setTable("CuttingProcessData");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit); // 手动提交
    if (!model->select()) {
        QMessageBox::critical(this, "错误", "无法加载表 CuttingProcessData: " + model->lastError().text());
        return;
    }

    tableView->setModel(model);
    tableView->resizeColumnsToContents();
    tableView->setSelectionBehavior(QTableView::SelectRows);
    tableView->setEditTriggers(QTableView::DoubleClicked | QTableView::SelectedClicked);
    tableView->horizontalHeader()->setStretchLastSection(true);
}

void DbViewer::addRow()
{
    if (!model) return;
    int row = model->rowCount();
    model->insertRow(row);
    tableView->scrollToBottom();
}

void DbViewer::deleteRow()
{
    if (!model) return;
    QModelIndexList selection = tableView->selectionModel()->selectedRows();
    if (selection.isEmpty()) return;

    for (const QModelIndex &index : selection) {
        model->removeRow(index.row());
    }
}

void DbViewer::saveChanges()
{
    if (!model) return;
    if (model->submitAll()) {
        QMessageBox::information(this, "提示", "修改已保存到数据库");
    } else {
        QMessageBox::critical(this, "错误", "保存失败: " + model->lastError().text());
    }
}

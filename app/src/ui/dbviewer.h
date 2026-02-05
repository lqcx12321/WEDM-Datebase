#pragma once
#include <QWidget>
#include <QTableView>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHBoxLayout>

class DbViewer : public QWidget
{
    Q_OBJECT

public:
    explicit DbViewer(QWidget *parent = nullptr);
    ~DbViewer();

private slots:
    void chooseDatabaseFile(); // 选择数据库
    void addRow();             // 添加新行
    void deleteRow();          // 删除选中行
    void saveChanges();        // 保存修改

private:
    QTableView *tableView;
    QSqlDatabase db;
    QSqlTableModel *model;

    QPushButton *btnChooseFile;
    QPushButton *btnAddRow;
    QPushButton *btnDeleteRow;
    QPushButton *btnSave;
    QPushButton *btnClose;
};

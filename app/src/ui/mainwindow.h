#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>
#include <QGroupBox>

#include "../dao/db.h"
#include "../service/selectionservice.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    // ===== 核心初始化入口 =====
    void buildUI();
    void connectSignals();

    // ===== UI 分区构造函数 =====
    QWidget* createLeftPanel();
    QWidget* createRightPanel();

    // 左侧区域
    QGroupBox* createProcessFilterGroup();
    QGroupBox* createTargetConstraintGroup();
    QGroupBox* createWeightGroup();

    // 右侧区域
    QGroupBox* createPlotGroup();
    QGroupBox* createTableGroup();

    // 表格填充
    void populateTable(const QList<CandidateRow>& rows);

    void resetFilters();

    void exportCSV();


private:
    // ========= 数据库 & 服务 =========
    DB *m_db;
    SelectionService *m_service;

    // ========= 左侧控件：工艺筛选 =========
    QLineEdit *ed_code;

    QDoubleSpinBox *sp_on;
    QDoubleSpinBox *sp_off;
    QDoubleSpinBox *sp_ipm;
    QDoubleSpinBox *sp_hp;
    QDoubleSpinBox *sp_sv;
    QDoubleSpinBox *sp_ws;
    QDoubleSpinBox *sp_wt;

    QLineEdit *cb_wmat;
    QLineEdit *cb_dmat;

    QDoubleSpinBox *sp_wthick_min;
    QDoubleSpinBox *sp_wthick_max;
    QDoubleSpinBox *sp_ddia_min;
    QDoubleSpinBox *sp_ddia_max;

    QPushButton *btn_query;
    QPushButton *btn_reset;
    QPushButton *btn_export;

    // ========= 左侧控件：硬约束 =========
    QDoubleSpinBox *sp_max_ra;
    QDoubleSpinBox *sp_min_speed;
    QDoubleSpinBox *sp_gap_min;
    QDoubleSpinBox *sp_gap_max;
    QDoubleSpinBox *sp_max_lag;

    // ========= 左侧控件：权重 =========
    QDoubleSpinBox *w_speed;
    QDoubleSpinBox *w_ra;
    QDoubleSpinBox *w_lag;
    QDoubleSpinBox *w_gap;
    QDoubleSpinBox *target_gap;

    // ========= 右侧控件 =========
    QComboBox *plot_select;
    QTableWidget *table;
};

#endif // MAINWINDOW_H

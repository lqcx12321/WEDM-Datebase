#include "mainwindow.h"
#include <QVBoxLayout>
#include <QSplitter>
#include <QGroupBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QDebug>
#include <QFileDialog>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("WEDM 智能化数据库 GUI");
    resize(1200, 720);

    m_db = new DB("/home/shj/database/cutting_process_demo.db");
    m_db->init(true);
    m_service = new SelectionService(m_db);

    buildUI();
    connectSignals();
}

void MainWindow::buildUI()
{
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);

    splitter->addWidget(createLeftPanel());
    splitter->addWidget(createRightPanel());
    splitter->setStretchFactor(1, 1);

    setCentralWidget(splitter);
}

// 左侧 UI
QWidget* MainWindow::createLeftPanel()
{
    QWidget *left = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(left);

    layout->addWidget(createProcessFilterGroup());
    layout->addWidget(createTargetConstraintGroup());
    layout->addWidget(createWeightGroup());

    return left;
}

// 工艺/材料筛选
QGroupBox* MainWindow::createProcessFilterGroup()
{
    QGroupBox *g = new QGroupBox("工艺 / 材料筛选（留空表示不限）");
    QFormLayout *f = new QFormLayout(g);

    ed_code = new QLineEdit(); ed_code->setPlaceholderText("CODE");

    sp_on = new QDoubleSpinBox();  sp_on->setRange(0, 100); sp_on->setDecimals(3);
    sp_off = new QDoubleSpinBox(); sp_off->setRange(0, 100); sp_off->setDecimals(3);
    sp_ipm = new QDoubleSpinBox(); sp_ipm->setRange(0, 100); sp_ipm->setDecimals(3);
    sp_hp  = new QDoubleSpinBox(); sp_hp->setRange(0, 100); sp_hp->setDecimals(3);
    sp_sv  = new QDoubleSpinBox(); sp_sv->setRange(0, 200); sp_sv->setDecimals(3);
    sp_ws  = new QDoubleSpinBox(); sp_ws->setRange(0, 20);  sp_ws->setDecimals(3);
    sp_wt  = new QDoubleSpinBox(); sp_wt->setRange(0, 5000); sp_wt->setDecimals(1);

    cb_wmat = new QLineEdit();
    cb_dmat = new QLineEdit();
    sp_wthick_min = new QDoubleSpinBox(); sp_wthick_min->setRange(0, 1000);
    sp_wthick_max = new QDoubleSpinBox(); sp_wthick_max->setRange(0, 1000);
    sp_ddia_min   = new QDoubleSpinBox(); sp_ddia_min->setRange(0, 2);
    sp_ddia_max   = new QDoubleSpinBox(); sp_ddia_max->setRange(0, 2);

    f->addRow("CODE:", ed_code);
    f->addRow("ON (μs):", sp_on);
    f->addRow("OFF (μs):", sp_off);
    f->addRow("IPM (A):", sp_ipm);
    f->addRow("HP:", sp_hp);
    f->addRow("SV:", sp_sv);
    f->addRow("WS (mm/s):", sp_ws);
    f->addRow("WT (g):", sp_wt);
    f->addRow(new QLabel("—— 材料 / 丝 ——"));
    f->addRow("工件材料:", cb_wmat);
    f->addRow("厚度最小:", sp_wthick_min);
    f->addRow("厚度最大:", sp_wthick_max);
    f->addRow("丝径最小:", sp_ddia_min);
    f->addRow("丝径最大:", sp_ddia_max);
    f->addRow("电极丝材料:", cb_dmat);

    // 按钮区
    QHBoxLayout *btn_layout = new QHBoxLayout();
    btn_query  = new QPushButton("查询 / 评分");
    btn_reset  = new QPushButton("重置条件");
    btn_export = new QPushButton("导出 CSV");
    btn_layout->addWidget(btn_query);
    btn_layout->addWidget(btn_reset);
    btn_layout->addWidget(btn_export);

    f->addRow(btn_layout);

    return g;
}

// 指标硬约束
QGroupBox* MainWindow::createTargetConstraintGroup()
{
    QGroupBox *g = new QGroupBox("指标硬约束（0 表示不限）");
    QFormLayout *f = new QFormLayout(g);

    sp_max_ra = new QDoubleSpinBox(); sp_max_ra->setRange(0,10); sp_max_ra->setDecimals(6);
    sp_min_speed = new QDoubleSpinBox(); sp_min_speed->setRange(0,1e6); sp_min_speed->setDecimals(3);
    sp_gap_min = new QDoubleSpinBox(); sp_gap_min->setRange(0,10); sp_gap_min->setDecimals(6);
    sp_gap_max = new QDoubleSpinBox(); sp_gap_max->setRange(0,10); sp_gap_max->setDecimals(6);
    sp_max_lag = new QDoubleSpinBox(); sp_max_lag->setRange(0,10); sp_max_lag->setDecimals(6);

    f->addRow("Ra ≤:", sp_max_ra);
    f->addRow("CutSpeed ≥:", sp_min_speed);
    f->addRow("Gap 最小:", sp_gap_min);
    f->addRow("Gap 最大:", sp_gap_max);
    f->addRow("Lag ≤:", sp_max_lag);

    return g;
}

// 权重区
QGroupBox* MainWindow::createWeightGroup()
{
    QGroupBox *g = new QGroupBox("评分权重（合计 ≤ 1）");
    QFormLayout *f = new QFormLayout(g);

    w_speed = new QDoubleSpinBox(); w_speed->setRange(0,1); w_speed->setValue(0.55);
    w_ra    = new QDoubleSpinBox(); w_ra->setRange(0,1); w_ra->setValue(0.30);
    w_lag   = new QDoubleSpinBox(); w_lag->setRange(0,1); w_lag->setValue(0.10);
    w_gap   = new QDoubleSpinBox(); w_gap->setRange(0,1); w_gap->setValue(0.05);
    target_gap = new QDoubleSpinBox(); target_gap->setRange(0,10); target_gap->setDecimals(6); target_gap->setValue(0.01);

    f->addRow("速度 w:", w_speed);
    f->addRow("Ra w:", w_ra);
    f->addRow("Lag w:", w_lag);
    f->addRow("Gap w:", w_gap);
    f->addRow("目标 Gap:", target_gap);

    return g;
}

// 右侧 UI
QWidget* MainWindow::createRightPanel()
{
    QWidget *right = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(right);

    layout->addWidget(createPlotGroup());
    layout->addWidget(createTableGroup());

    return right;
}

QGroupBox* MainWindow::createPlotGroup()
{
    QGroupBox *g = new QGroupBox("可视化（默认 Ra vs ON）");
    QVBoxLayout *v = new QVBoxLayout(g);

    plot_select = new QComboBox();
    plot_select->addItems({"Ra vs ON1","CutSpeed vs IPM","Ra vs WS","CutSpeed vs WS","LAG vs CUTSPEED"});

    v->addWidget(plot_select);
    v->addStretch();

    return g;
}

QGroupBox* MainWindow::createTableGroup()
{
    QGroupBox *g = new QGroupBox("结果表");
    QVBoxLayout *v = new QVBoxLayout(g);

    table = new QTableWidget(0, 10);
    table->setHorizontalHeaderLabels({"编号","CODE","ON1","OFF1","IPM","HP","MA","SV","V","RA"});
    v->addWidget(table);

    return g;
}

// 信号槽区域，推荐代码入口
void MainWindow::connectSignals()
{
    connect(btn_query, &QPushButton::clicked, this, [=](){
        double ws = w_speed->value();
        double wr = w_ra->value();
        double wl = w_lag->value();
        double wg = w_gap->value();
        double tg = target_gap->value();

        auto rows = m_service->rankedCandidates(ws, wr, wl, wg, tg);
        populateTable(rows);
    });

    connect (btn_reset, &QPushButton::clicked, this, &MainWindow::resetFilters);
    
    connect (btn_export, &QPushButton::clicked, this, &MainWindow::exportCSV);
}

// 填充表格
void MainWindow::populateTable(const QList<CandidateRow>& rows)
{
    table->blockSignals(true);
    table->setRowCount(0);

    for (const auto &r : rows) {
        int row = table->rowCount();
        table->insertRow(row);

        table->setItem(row, 0, new QTableWidgetItem(QString::number(r.id)));
        table->setItem(row, 1, new QTableWidgetItem(r.code));

        table->setItem(row, 2, new QTableWidgetItem(QString::number(r.fields.value("ON1").toDouble())));
        table->setItem(row, 3, new QTableWidgetItem(QString::number(r.fields.value("OFF1").toDouble())));
        table->setItem(row, 4, new QTableWidgetItem(QString::number(r.fields.value("IPM").toDouble())));
        table->setItem(row, 5, new QTableWidgetItem(QString::number(r.fields.value("HP").toDouble())));
        table->setItem(row, 6, new QTableWidgetItem(QString::number(r.fields.value("MA").toDouble())));
        table->setItem(row, 7, new QTableWidgetItem(QString::number(r.fields.value("SV").toDouble())));
        table->setItem(row, 8, new QTableWidgetItem(QString::number(r.fields.value("V").toDouble())));
        table->setItem(row, 9, new QTableWidgetItem(QString::number(r.fields.value("RA").toDouble())));
    }

    table->blockSignals(false);
}

// 清除功能实现
void MainWindow::resetFilters()
{
    ed_code->clear();
    sp_on->clear();
    sp_off->clear();
    sp_ipm->clear();
    sp_hp->clear();
    sp_sv->clear();
    sp_ws->clear();
    sp_wt->clear();
    cb_wmat->clear();
    cb_dmat->clear();
    sp_wthick_min->setValue(0);
    sp_wthick_max->setValue(0);
    sp_ddia_min->setValue(0);
    sp_ddia_max->setValue(0);
    sp_max_ra->setValue(0);
    sp_min_speed->setValue(0);
    sp_gap_min->setValue(0);
    sp_gap_max->setValue(0);
    sp_max_lag->setValue(0);

    qDebug() << "Filters have been reset.";
}

// 导出功能实现
void MainWindow::exportCSV()
{
    QString fileName = QFileDialog::getSaveFileName(
        this, "导出 CSV", "", "CSV 文件 (*.csv)");

    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "无法打开文件用于写入";
        return;
    }

    QTextStream out(&file);

    // 写表头
    QStringList headers;
    for (int c = 0; c < table->columnCount(); ++c)
        headers << table->horizontalHeaderItem(c)->text();

    out << headers.join(",") << "\n";

    // 写数据
    for (int r = 0; r < table->rowCount(); ++r) {
        QStringList row;
        for (int c = 0; c < table->columnCount(); ++c) {
            QTableWidgetItem *item = table->item(r, c);
            row << (item ? item->text() : "");
        }
        out << row.join(",") << "\n";
    }

    file.close();
    qDebug() << "CSV 导出成功:" << fileName;
}






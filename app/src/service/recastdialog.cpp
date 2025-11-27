#include "recastdialog.h"
#include <QFormLayout>
#include <QPushButton>

RecastDialog::RecastDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("重熔层筛选设置");

    // ---- 启用筛选 ----
    cb_enable = new QCheckBox("启用重熔层筛选");

    // ---- 加工等级选择 ----
    cb_grade = new QComboBox();
    cb_grade->addItems({"1级", "2级", "3级", "4级", "5级"});
    connect(cb_grade, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &RecastDialog::onGradeChanged);

    // ---- 自定义最大厚度 ----
    sp_limit = new QDoubleSpinBox();
    sp_limit->setDecimals(4);
    sp_limit->setRange(0, 1e3);
    sp_limit->setValue(0.01);

    // ---- 当前等级标准参数展示 ----
    lb_avg = new QLabel("-");
    lb_local = new QLabel("-");
    lb_crack_num = new QLabel("-");
    lb_crack_len = new QLabel("-");

    // ---- 确定按钮 ----
    QPushButton *btn_ok = new QPushButton("确定");
    connect(btn_ok, &QPushButton::clicked, this, &RecastDialog::accept);

    // ---- 布局 ----
    QFormLayout *f = new QFormLayout(this);
    f->addRow(cb_enable);
    f->addRow("加工等级:", cb_grade);
    f->addRow("等级平均厚度(示例):", lb_avg);
    f->addRow("等级局部最大厚度:", lb_local);
    f->addRow("最大裂纹条数:", lb_crack_num);
    f->addRow("最大裂纹长度:", lb_crack_len);
    f->addRow("自定义最大厚度:", sp_limit);
    f->addRow(btn_ok);

    // ---- 初次加载默认等级 ----
    onGradeChanged(0);
}

void RecastDialog::onGradeChanged(int idx)
{
    // ---- 等级标准表 ----
    static Spec table[5] = {
        {0.012, 0.025, 1, 0.025},                             // 1级
        {0.012, 0.04, 2, 0.035},                               // 2级
        {0.075, 0.50, 4, 0.075, true, true},                  // 3级，avg/local 任意
        {0.075, 1.00, 6, 0.150},                               // 4级
        {0.380, 1.00, 10, 0.200, false, false, false, true}   // 5级，裂纹长度无
    };

    const Spec &s = table[idx];

    lb_avg->setText(s.avg_unlimited ? "任意" : QString::number(s.avg, 'f', 3));
    lb_local->setText(s.local_unlimited ? "任意" : QString::number(s.local, 'f', 3));
    lb_crack_num->setText(s.crack_num_unlimited ? "任意" : QString::number(s.crack_num));
    lb_crack_len->setText(s.crack_len_none ? "无" : QString::number(s.crack_len, 'f', 3));
}
